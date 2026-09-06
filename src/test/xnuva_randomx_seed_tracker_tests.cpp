#include <boost/test/unit_test.hpp>

#include <chain.h>
#include <consensus/params.h>
#include <pow.h>
#include <primitives/block.h>
#include <uint256.h>
#include <xnuva/pow_validation.h>
#include <xnuva/randomx_context.h>
#include <xnuva/randomx_seed_tracker.h>

#include <cstdint>

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

CBlockHeader NextHeader(
    const uint256& previous,
    uint32_t nonce,
    uint32_t branch_marker = 0)
{
    CBlockHeader header{};

    header.nVersion = 1;
    header.hashPrevBlock = previous;

    header.hashMerkleRoot = uint256{
        "fb923316be9e4f5db3467d8c2d159bb"
        "3a27fa9ba2c71703c078a0ea7ccb676e8"
    };

    header.nTime =
        1788642000U +
        nonce +
        branch_marker;

    header.nBits = 0x207fffff;

    header.nNonce =
        nonce ^
        branch_marker;

    return header;
}

struct GenesisAnchor {
    uint256 hash{GENESIS_ID};
    CBlockIndex index{};

    GenesisAnchor()
    {
        index.nHeight = 0;
        index.phashBlock = &hash;
    }
};

xnuva::RandomXSeed RunBranchTo2113(
    uint32_t branch_marker)
{
    GenesisAnchor anchor{};

    xnuva::RandomXSequentialSeedTracker
        tracker{anchor.index};

    uint256 tip{GENESIS_ID};

    for (uint32_t height = 1;
         height <= 2112;
         ++height) {

        const auto seed{
            tracker.SeedForNext()
        };

        BOOST_REQUIRE(seed.has_value());

        CBlockHeader header{
            NextHeader(
                tip,
                height,
                height == 2048
                    ? branch_marker
                    : 0)
        };

        BOOST_REQUIRE(
            tracker.Advance(header));

        tip = header.GetHash();
    }

    const auto seed{
        tracker.SeedForNext()
    };

    BOOST_REQUIRE(seed.has_value());

    return *seed;
}

} // namespace

BOOST_AUTO_TEST_SUITE(xnuva_randomx_seed_tracker_tests)

BOOST_AUTO_TEST_CASE(seed_boundaries_match_frozen_schedule)
{
    GenesisAnchor anchor{};

    xnuva::RandomXSequentialSeedTracker
        tracker{anchor.index};

    uint256 tip{GENESIS_ID};
    uint256 seed_2048{};
    uint256 seed_4096{};

    for (uint32_t height = 1;
         height <= 4160;
         ++height) {

        const auto seed{
            tracker.SeedForNext()
        };

        BOOST_REQUIRE(seed.has_value());

        if (height == 1 ||
            height == 2048 ||
            height == 2112) {

            BOOST_CHECK_EQUAL(
                seed->height,
                0);

            BOOST_CHECK(
                seed->block_id ==
                GENESIS_ID);
        }

        if (height == 2113 ||
            height == 4160) {

            BOOST_CHECK_EQUAL(
                seed->height,
                2048);

            BOOST_CHECK(
                seed->block_id ==
                seed_2048);
        }

        CBlockHeader header{
            NextHeader(
                tip,
                height)
        };

        BOOST_REQUIRE(
            tracker.Advance(header));

        tip = header.GetHash();

        if (height == 2048) {
            seed_2048 = tip;
        }

        if (height == 4096) {
            seed_4096 = tip;
        }
    }

    const auto seed_4161{
        tracker.SeedForNext()
    };

    BOOST_REQUIRE(
        seed_4161.has_value());

    BOOST_CHECK_EQUAL(
        seed_4161->height,
        4096);

    BOOST_CHECK(
        seed_4161->block_id ==
        seed_4096);
}

BOOST_AUTO_TEST_CASE(discontinuous_header_does_not_advance)
{
    GenesisAnchor anchor{};

    xnuva::RandomXSequentialSeedTracker
        tracker{anchor.index};

    const int64_t height_before{
        tracker.Height()
    };

    const uint256 tip_before{
        tracker.TipBlockId()
    };

    CBlockHeader bad{
        NextHeader(
            uint256{
                "ffffffffffffffffffffffffffffffff"
                "ffffffffffffffffffffffffffffffff"
            },
            1)
    };

    BOOST_CHECK(
        !tracker.Advance(bad));

    BOOST_CHECK_EQUAL(
        tracker.Height(),
        height_before);

    BOOST_CHECK(
        tracker.TipBlockId() ==
        tip_before);
}

BOOST_AUTO_TEST_CASE(competing_branch_seed_ids_remain_distinct)
{
    const auto seed_a{
        RunBranchTo2113(
            0x00000000U)
    };

    const auto seed_b{
        RunBranchTo2113(
            0x01000000U)
    };

    BOOST_CHECK_EQUAL(
        seed_a.height,
        2048);

    BOOST_CHECK_EQUAL(
        seed_b.height,
        2048);

    BOOST_CHECK(
        seed_a.block_id !=
        seed_b.block_id);
}

BOOST_AUTO_TEST_CASE(explicit_seed_validation_matches_direct_randomx)
{
    GenesisAnchor anchor{};

    Consensus::Params params{
        TestConsensus()
    };

    CBlockHeader header{
        NextHeader(
            GENESIS_ID,
            1)
    };

    xnuva::RandomXLightContextCache
        contexts{2};

    const xnuva::RandomXSeed seed{
        0,
        GENESIS_ID
    };

    const auto context{
        contexts.Get(seed)
    };

    const uint256 digest{
        context->HashHeader(header)
    };

    const bool expected{
        CheckProofOfWorkImpl(
            digest,
            header.nBits,
            params)
    };

    const auto actual{
        xnuva::ValidateRandomXProofOfWork(
            header,
            seed,
            params,
            contexts)
    };

    BOOST_CHECK_EQUAL(
        actual ==
            xnuva::PoWValidationResult::VALID,
        expected);

    BOOST_CHECK(
        digest !=
        header.GetHash());
}

BOOST_AUTO_TEST_SUITE_END()
