#include <xnuva/pow_validation.h>

#include <arith_uint256.h>
#include <consensus/params.h>
#include <pow.h>
#include <primitives/block.h>
#include <xnuva/randomx_context.h>

namespace xnuva {

PoWValidationResult ValidateProofOfWork(
    const CBlockHeader& header,
    const CBlockIndex* pindex_prev,
    const Consensus::Params& params,
    RandomXLightContextCache& randomx_contexts)
{
    /*
     * Genesis is the one deliberate exception to RandomX.
     *
     * nullptr is only the genesis-candidate context. Keep the existing
     * SHA256d proof check byte-for-byte in meaning while block identity
     * remains CBlockHeader::GetHash().
     */
    if (pindex_prev == nullptr) {
        return CheckProofOfWorkImpl(
                   header.GetHash(),
                   header.nBits,
                   params)
            ? PoWValidationResult::VALID
            : PoWValidationResult::INVALID;
    }

    /*
     * Resolve the seed only through the supplied previous block index.
     * ResolveRandomXSeed() deliberately performs branch-contextual
     * GetAncestor() lookup and never consults the globally active chain.
     */
    const auto seed{
        ResolveRandomXSeed(pindex_prev)
    };

    if (!seed) {
        return PoWValidationResult::CONTEXT_UNAVAILABLE;
    }

    return ValidateRandomXProofOfWork(
        header,
        *seed,
        params,
        randomx_contexts);
}

PoWValidationResult ValidateRandomXProofOfWork(
    const CBlockHeader& header,
    const RandomXSeed& seed,
    const Consensus::Params& params,
    RandomXLightContextCache& randomx_contexts)
{
    /*
     * Reject malformed/out-of-range compact targets before doing any
     * expensive RandomX work.
     */
    const auto target{
        DeriveTarget(
            header.nBits,
            params.powLimit)
    };

    if (!target) {
        return PoWValidationResult::INVALID;
    }

    /*
     * RandomXResourceError intentionally propagates from Get()/HashHeader().
     * Local allocation/cache/VM failure is not consensus-invalid proof.
     */
    const auto context{
        randomx_contexts.Get(seed)
    };

    const uint256 pow_hash{
        context->HashHeader(header)
    };

    return UintToArith256(pow_hash) <= *target
        ? PoWValidationResult::VALID
        : PoWValidationResult::INVALID;
}

} // namespace xnuva
