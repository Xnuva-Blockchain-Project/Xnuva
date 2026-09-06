#include <boost/test/unit_test.hpp>

#include <chain.h>
#include <consensus/params.h>
#include <pow.h>
#include <primitives/block.h>
#include <uint256.h>
#include <xnuva/pow_validation.h>
#include <xnuva/randomx_context.h>

#include <cstdint>
#include <memory>
#include <vector>

namespace {

const uint256 GENESIS_ID{
    "2d95b3b13732ae491122eaacd12098b2"
    "97ddbb0180f0f6bdfaeee555be491aae"
};

Consensus::Params TestConsensus()
{
    Consensus::Params params{};

    params.powLimit = uint256{
        "ffffffffffffffffffffffffffffffff"
        "ffffffffffffffffffffffffffffffff"
    };

    return params;
}

CBlockHeader TestHeader()
{
    CBlockHeader header{};

    header.nVersion = 1;
    header.hashPrevBlock = GENESIS_ID;

    header.hashMerkleRoot = uint256{
        "fb923316be9e4f5db3467d8c2d159bb"
        "3a27fa9ba2c71703c078a0ea7ccb676e8"
    };

    header.nTime = 1788642000;
    header.nBits = 0x207fffff;
    header.nNonce = 0;

    return header;
}

struct SyntheticChain {
    std::vector<uint256> hashes;
    std::vector<std::unique_ptr<CBlockIndex>> indexes;

    explicit SyntheticChain(int tip_height)
        : hashes(static_cast<std::size_t>(tip_height + 1))
    {
        indexes.reserve(
            static_cast<std::size_t>(tip_height + 1));

        for (int height = 0;
             height <= tip_height;
             ++height) {

            auto index{
                std::make_unique<CBlockIndex>()
            };

            index->nHeight = height;

            if (height > 0) {
                index->pprev =
                    indexes[
                        static_cast<std::size_t>(
                            height - 1)
                    ].get();
            }

            index->phashBlock =
                &hashes[
                    static_cast<std::size_t>(
                        height)
                ];

            indexes.push_back(
                std::move(index));
        }
    }

    CBlockIndex* At(int height)
    {
        return indexes.at(
            static_cast<std::size_t>(height)
        ).get();
    }
};

} // namespace

BOOST_AUTO_TEST_SUITE(xnuva_pow_validation_tests)

BOOST_AUTO_TEST_CASE(genesis_preserves_sha256d_pow)
{
    Consensus::Params params{
        TestConsensus()
    };

    CBlockHeader header{
        TestHeader()
    };

    xnuva::RandomXLightContextCache
        contexts{2};

    const bool legacy_result{
        CheckProofOfWorkImpl(
            header.GetHash(),
            header.nBits,
            params)
    };

    const auto result{
        xnuva::ValidateProofOfWork(
            header,
            nullptr,
            params,
            contexts)
    };

    BOOST_CHECK_EQUAL(
        result == xnuva::PoWValidationResult::VALID,
        legacy_result);

    BOOST_CHECK(
        result !=
        xnuva::PoWValidationResult::CONTEXT_UNAVAILABLE);
}

BOOST_AUTO_TEST_CASE(missing_branch_seed_is_not_invalid_pow)
{
    Consensus::Params params{
        TestConsensus()
    };

    CBlockHeader header{
        TestHeader()
    };

    /*
     * Height 0 with no block-id pointer deliberately makes the seed
     * unavailable for candidate height 1.
     */
    CBlockIndex previous{};
    previous.nHeight = 0;
    previous.phashBlock = nullptr;

    xnuva::RandomXLightContextCache
        contexts{2};

    BOOST_CHECK(
        xnuva::ValidateProofOfWork(
            header,
            &previous,
            params,
            contexts) ==
        xnuva::PoWValidationResult::CONTEXT_UNAVAILABLE);
}

BOOST_AUTO_TEST_CASE(randomx_result_matches_direct_reference)
{
    Consensus::Params params{
        TestConsensus()
    };

    SyntheticChain chain{0};
    chain.hashes[0] = GENESIS_ID;

    CBlockHeader header{
        TestHeader()
    };

    xnuva::RandomXLightContextCache
        contexts{2};

    const auto seed{
        xnuva::ResolveRandomXSeed(
            chain.At(0))
    };

    BOOST_REQUIRE(seed.has_value());

    const auto context{
        contexts.Get(*seed)
    };

    const uint256 randomx_hash{
        context->HashHeader(header)
    };

    const bool expected{
        CheckProofOfWorkImpl(
            randomx_hash,
            header.nBits,
            params)
    };

    const auto actual{
        xnuva::ValidateProofOfWork(
            header,
            chain.At(0),
            params,
            contexts)
    };

    BOOST_CHECK_EQUAL(
        actual == xnuva::PoWValidationResult::VALID,
        expected);

    BOOST_CHECK(
        randomx_hash !=
        header.GetHash());
}

BOOST_AUTO_TEST_CASE(randomx_accept_and_reject_vectors)
{
    Consensus::Params params{
        TestConsensus()
    };

    SyntheticChain chain{0};
    chain.hashes[0] = GENESIS_ID;

    xnuva::RandomXLightContextCache
        contexts{2};

    const auto seed{
        xnuva::ResolveRandomXSeed(
            chain.At(0))
    };

    BOOST_REQUIRE(seed.has_value());

    const auto context{
        contexts.Get(*seed)
    };

    bool found_valid{false};
    bool found_invalid{false};

    CBlockHeader valid_header{};
    CBlockHeader invalid_header{};

    /*
     * At regtest-style 0x207fffff approximately half of uniformly
     * distributed hashes lie on each side of the target.
     *
     * The deterministic RandomX sequence below is bounded. We first use the
     * direct RandomX result to locate one member of each class, then require
     * ValidateProofOfWork() to reproduce both decisions.
     */
    for (uint32_t nonce = 0;
         nonce < 32 &&
         (!found_valid || !found_invalid);
         ++nonce) {

        CBlockHeader candidate{
            TestHeader()
        };

        candidate.nNonce = nonce;

        const uint256 digest{
            context->HashHeader(candidate)
        };

        const bool valid{
            CheckProofOfWorkImpl(
                digest,
                candidate.nBits,
                params)
        };

        if (valid && !found_valid) {
            valid_header = candidate;
            found_valid = true;
        }

        if (!valid && !found_invalid) {
            invalid_header = candidate;
            found_invalid = true;
        }
    }

    BOOST_REQUIRE_MESSAGE(
        found_valid,
        "deterministic RandomX nonce window did not contain a valid proof");

    BOOST_REQUIRE_MESSAGE(
        found_invalid,
        "deterministic RandomX nonce window did not contain an invalid proof");

    BOOST_CHECK(
        xnuva::ValidateProofOfWork(
            valid_header,
            chain.At(0),
            params,
            contexts) ==
        xnuva::PoWValidationResult::VALID);

    BOOST_CHECK(
        xnuva::ValidateProofOfWork(
            invalid_header,
            chain.At(0),
            params,
            contexts) ==
        xnuva::PoWValidationResult::INVALID);
}

BOOST_AUTO_TEST_SUITE_END()
