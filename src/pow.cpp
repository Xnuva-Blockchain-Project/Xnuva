// Copyright (c) 2009-2010 Satoshi Nakamoto
// Copyright (c) 2009-present The Bitcoin Core developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include <pow.h>

#include <arith_uint256.h>
#include <boost/multiprecision/cpp_int.hpp>
#include <ios>
#include <chain.h>
#include <primitives/block.h>
#include <uint256.h>
#include <util/check.h>

/**
 * XNUVA ASERTI3 fixed-point implementation.
 *
 * Formula:
 *
 * target =
 *   anchor_target *
 *   2^((elapsed - spacing * scheduled_blocks) / half_life)
 *
 * The fractional exponent uses the established cubic integer
 * approximation from the ASERTI3 reference implementation.
 */
arith_uint256 CalculateASERT(
    const arith_uint256& refTarget,
    const int64_t nPowTargetSpacing,
    const int64_t nTimeDiff,
    const int64_t nHeightDiff,
    const arith_uint256& powLimit,
    const int64_t nHalfLife) noexcept
{
    assert(refTarget > 0 && refTarget <= powLimit);
    assert(nHeightDiff >= 0);
    assert(nHalfLife > 0);
    assert(nPowTargetSpacing > 0);

    const int64_t ideal_time =
        nPowTargetSpacing * (nHeightDiff + 1);

    const int64_t schedule_delta =
        nTimeDiff - ideal_time;

    // schedule_delta * 65536 must fit int64_t.
    assert(
        schedule_delta > -(int64_t{1} << 47) &&
        schedule_delta <  (int64_t{1} << 47));

    const int64_t exponent =
        (schedule_delta * 65536) / nHalfLife;

    static_assert(
        int64_t(-1) >> 1 == int64_t(-1),
        "ASERT requires arithmetic right shift");

    int64_t shifts = exponent >> 16;
    const auto frac = uint16_t(exponent);

    assert(
        exponent ==
        (shifts * 65536) + frac);

    const uint32_t factor =
        65536 +
        ((
            195766423245049ULL * frac +
            971821376ULL * frac * frac +
            5127ULL * frac * frac * frac +
            (1ULL << 47)
        ) >> 48);

    const auto to_wide = [](const arith_uint256& value) {
        return boost::multiprecision::uint512_t{
            "0x" + value.GetHex()
        };
    };

    boost::multiprecision::uint512_t wide_target =
        to_wide(refTarget) * factor;

    const boost::multiprecision::uint512_t wide_limit =
        to_wide(powLimit);

    shifts -= 16;

    if (shifts > 0) {
        if (shifts >= 512) {
            return powLimit;
        }

        const auto maximum_before_shift =
            wide_limit >> static_cast<unsigned int>(shifts);

        if (wide_target > maximum_before_shift) {
            return powLimit;
        }

        wide_target <<= static_cast<unsigned int>(shifts);
    } else if (shifts < 0) {
        const uint64_t right_shift =
            static_cast<uint64_t>(-shifts);

        if (right_shift >= 512) {
            wide_target = 0;
        } else {
            wide_target >>= static_cast<unsigned int>(right_shift);
        }
    }

    if (wide_target == 0) {
        wide_target = 1;
    } else if (wide_target > wide_limit) {
        wide_target = wide_limit;
    }

    arith_uint256 nextTarget{0};
    boost::multiprecision::uint512_t remaining =
        wide_target;

    const boost::multiprecision::uint512_t limb_mask{
        0xffffffffffffffffULL
    };

    for (unsigned int limb = 0; limb < 4; ++limb) {
        const boost::multiprecision::uint512_t low =
            remaining & limb_mask;

        const uint64_t value =
            low.convert_to<uint64_t>();

        arith_uint256 part{value};

        if (limb != 0) {
            part <<= limb * 64;
        }

        nextTarget |= part;
        remaining >>= 64;
    }

    assert(remaining == 0);

    return nextTarget;
}


/**
 * XNUVA main-chain difficulty adjustment.
 *
 * The absolute ASERT schedule is anchored to genesis.
 * We use a virtual parent timestamp one target interval
 * before genesis. Therefore a perfectly scheduled chain
 * leaves the target unchanged from the genesis target.
 *
 * Mainnet parameters:
 *   spacing   = 120 seconds
 *   half-life = 43200 seconds (12 hours)
 */
unsigned int GetNextWorkRequired(
    const CBlockIndex* pindexLast,
    const CBlockHeader* pblock,
    const Consensus::Params& params)
{
    assert(pindexLast != nullptr);

    if (params.fPowNoRetargeting) {
        return pindexLast->nBits;
    }

    const arith_uint256 powLimit =
        UintToArith256(params.powLimit);

    if (
        params.fPowAllowMinDifficultyBlocks &&
        pblock->GetBlockTime() >
            pindexLast->GetBlockTime() +
                2 * params.nPowTargetSpacing
    ) {
        return powLimit.GetCompact();
    }

    const CBlockIndex* genesis =
        pindexLast->GetAncestor(0);

    assert(genesis != nullptr);
    assert(genesis->nHeight == 0);

    arith_uint256 anchorTarget;
    anchorTarget.SetCompact(genesis->nBits);

    assert(anchorTarget > 0);
    assert(anchorTarget <= powLimit);

    constexpr int64_t XNUVA_ASERT_HALF_LIFE =
        12 * 60 * 60;

    const int64_t virtual_anchor_parent_time =
        genesis->GetBlockTime() -
        params.nPowTargetSpacing;

    const int64_t time_diff =
        pindexLast->GetBlockTime() -
        virtual_anchor_parent_time;

    const int64_t height_diff =
        pindexLast->nHeight;

    return CalculateASERT(
        anchorTarget,
        params.nPowTargetSpacing,
        time_diff,
        height_diff,
        powLimit,
        XNUVA_ASERT_HALF_LIFE
    ).GetCompact();
}

unsigned int CalculateNextWorkRequired(const CBlockIndex* pindexLast, int64_t nFirstBlockTime, const Consensus::Params& params)
{
    if (params.fPowNoRetargeting)
        return pindexLast->nBits;

    // Limit adjustment step
    int64_t nActualTimespan = pindexLast->GetBlockTime() - nFirstBlockTime;
    if (nActualTimespan < params.nPowTargetTimespan/4)
        nActualTimespan = params.nPowTargetTimespan/4;
    if (nActualTimespan > params.nPowTargetTimespan*4)
        nActualTimespan = params.nPowTargetTimespan*4;

    // Retarget
    const arith_uint256 bnPowLimit = UintToArith256(params.powLimit);
    arith_uint256 bnNew;

    // Special difficulty rule for Testnet4
    if (params.enforce_BIP94) {
        // Here we use the first block of the difficulty period. This way
        // the real difficulty is always preserved in the first block as
        // it is not allowed to use the min-difficulty exception.
        int nHeightFirst = pindexLast->nHeight - (params.DifficultyAdjustmentInterval()-1);
        const CBlockIndex* pindexFirst = pindexLast->GetAncestor(nHeightFirst);
        bnNew.SetCompact(pindexFirst->nBits);
    } else {
        bnNew.SetCompact(pindexLast->nBits);
    }

    bnNew *= nActualTimespan;
    bnNew /= params.nPowTargetTimespan;

    if (bnNew > bnPowLimit)
        bnNew = bnPowLimit;

    return bnNew.GetCompact();
}

// Check that on difficulty adjustments, the new difficulty does not increase
// or decrease beyond the permitted limits.
bool PermittedDifficultyTransition(
    const Consensus::Params& params,
    int64_t height,
    uint32_t old_nbits,
    uint32_t new_nbits)
{
    (void)height;
    (void)old_nbits;

    if (params.fPowAllowMinDifficultyBlocks) {
        return true;
    }

    bool negative{false};
    bool overflow{false};

    arith_uint256 target;
    target.SetCompact(
        new_nbits,
        &negative,
        &overflow);

    return
        !negative &&
        !overflow &&
        target != 0 &&
        target <= UintToArith256(params.powLimit);
}

// Bypasses the actual proof of work check during fuzz testing with a simplified validation checking whether
// the most significant bit of the last byte of the hash is set.
bool CheckProofOfWork(uint256 hash, unsigned int nBits, const Consensus::Params& params)
{
    if (EnableFuzzDeterminism()) return (hash.data()[31] & 0x80) == 0;
    return CheckProofOfWorkImpl(hash, nBits, params);
}

std::optional<arith_uint256> DeriveTarget(unsigned int nBits, const uint256 pow_limit)
{
    bool fNegative;
    bool fOverflow;
    arith_uint256 bnTarget;

    bnTarget.SetCompact(nBits, &fNegative, &fOverflow);

    // Check range
    if (fNegative || bnTarget == 0 || fOverflow || bnTarget > UintToArith256(pow_limit))
        return {};

    return bnTarget;
}

bool CheckProofOfWorkImpl(uint256 hash, unsigned int nBits, const Consensus::Params& params)
{
    auto bnTarget{DeriveTarget(nBits, params.powLimit)};
    if (!bnTarget) return false;

    // Check proof of work matches claimed amount
    if (UintToArith256(hash) > bnTarget)
        return false;

    return true;
}
