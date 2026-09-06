#include <boost/test/unit_test.hpp>

#include <chain.h>
#include <primitives/block.h>
#include <uint256.h>
#include <xnuva/randomx.h>
#include <xnuva/randomx_context.h>

#include <future>
#include <memory>
#include <span>
#include <string>
#include <type_traits>
#include <vector>

namespace {

struct SyntheticChain {
    std::vector<uint256> hashes;
    std::vector<std::unique_ptr<CBlockIndex>> indexes;

    explicit SyntheticChain(int tip_height)
        : hashes(static_cast<std::size_t>(
            tip_height + 1))
    {
        indexes.reserve(
            static_cast<std::size_t>(
                tip_height + 1));

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
            static_cast<std::size_t>(
                height)
            ).get();
    }
};

CBlockHeader ReferenceHeader()
{
    CBlockHeader header{};

    header.nVersion = 1;
    header.hashPrevBlock.SetNull();

    header.hashMerkleRoot =
        uint256{
            "fb923316be9e4f5db3467d8c2d159bb"
            "3a27fa9ba2c71703c078a0ea7ccb676e8"
        };

    header.nTime = 1788642000;
    header.nBits = 0x207fffff;
    header.nNonce = 0;

    return header;
}

const uint256 GENESIS_ID{
    "2d95b3b13732ae491122eaacd12098b2"
    "97ddbb0180f0f6bdfaeee555be491aae"
};

const uint256 SEED_2048_A{
    "00000000000000000000000000000000"
    "00000000000000000000000000002048"
};

const uint256 SEED_2048_B{
    "00000000000000000000000000000000"
    "00000000000000000000000000b02048"
};

const uint256 SEED_4096{
    "00000000000000000000000000000000"
    "00000000000000000000000000004096"
};

const uint256 SEED_6144{
    "00000000000000000000000000000000"
    "00000000000000000000000000006144"
};

} // namespace

BOOST_AUTO_TEST_SUITE(
    xnuva_randomx_context_tests)

BOOST_AUTO_TEST_CASE(
    branch_context_seed_resolution)
{
    SyntheticChain chain{6208};

    chain.hashes[0] = GENESIS_ID;
    chain.hashes[2048] = SEED_2048_A;
    chain.hashes[4096] = SEED_4096;
    chain.hashes[6144] = SEED_6144;

    struct Vector {
        int candidate_height;
        int expected_seed_height;
        uint256 expected_seed_id;
    };

    const std::vector<Vector> vectors{
        {1, 0, GENESIS_ID},
        {2112, 0, GENESIS_ID},
        {2113, 2048, SEED_2048_A},
        {4160, 2048, SEED_2048_A},
        {4161, 4096, SEED_4096},
        {6209, 6144, SEED_6144},
    };

    for (const auto& vector : vectors) {

        const auto seed{
            xnuva::ResolveRandomXSeed(
                chain.At(
                    vector.candidate_height - 1))
        };

        BOOST_REQUIRE(seed.has_value());

        BOOST_CHECK_EQUAL(
            seed->height,
            vector.expected_seed_height);

        BOOST_CHECK(
            seed->block_id ==
            vector.expected_seed_id);
    }

    BOOST_CHECK(
        !xnuva::ResolveRandomXSeed(
            nullptr).has_value());
}

BOOST_AUTO_TEST_CASE(
    same_height_different_fork_seed)
{
    SyntheticChain branch_a{2112};
    SyntheticChain branch_b{2112};

    branch_a.hashes[2048] =
        SEED_2048_A;

    branch_b.hashes[2048] =
        SEED_2048_B;

    const auto seed_a{
        xnuva::ResolveRandomXSeed(
            branch_a.At(2112))
    };

    const auto seed_b{
        xnuva::ResolveRandomXSeed(
            branch_b.At(2112))
    };

    BOOST_REQUIRE(seed_a.has_value());
    BOOST_REQUIRE(seed_b.has_value());

    BOOST_CHECK_EQUAL(
        seed_a->height,
        seed_b->height);

    BOOST_CHECK(
        seed_a->block_id !=
        seed_b->block_id);

    BOOST_CHECK(
        *seed_a < *seed_b ||
        *seed_b < *seed_a);
}

BOOST_AUTO_TEST_CASE(
    reusable_cache_frozen_vector)
{
    const CBlockHeader header{
        ReferenceHeader()
    };

    const xnuva::RandomXSeed seed{
        0,
        GENESIS_ID
    };

    const uint256 reference{
        xnuva::RandomXHeaderHashV2(
            header,
            GENESIS_ID)
    };

    xnuva::RandomXLightContextCache
        manager{1};

    const auto first{
        manager.Get(seed)
    };

    const auto second{
        manager.Get(seed)
    };

    BOOST_CHECK_EQUAL(
        manager.Size(),
        1U);

    BOOST_CHECK_EQUAL(
        first.get(),
        second.get());

    const uint256 contextual{
        first->HashHeader(header)
    };

    BOOST_CHECK(
        contextual == reference);

    BOOST_CHECK_EQUAL(
        contextual.GetHex(),
        "80b59afe417178f46435a111204c14604"
        "d13971b812588e612597a50f9717085");

    BOOST_TEST_MESSAGE(
        "XNUVA_RANDOMX_CONTEXT_HEADER_VECTOR="
        << contextual.GetHex());
}

BOOST_AUTO_TEST_CASE(
    eviction_and_rebuild_are_deterministic)
{
    const std::string input{
        "Xnuva RandomX context eviction test"
    };

    const std::span<const char>
        input_chars{
            input.data(),
            input.size()
        };

    const auto input_bytes{
        std::as_bytes(input_chars)
    };

    const xnuva::RandomXSeed seed_a{
        2048,
        SEED_2048_A
    };

    const xnuva::RandomXSeed seed_b{
        2048,
        SEED_2048_B
    };

    xnuva::RandomXLightContextCache
        manager{1};

    uint256 hash_a_first;
    std::weak_ptr<
        const xnuva::RandomXLightCache
    > weak_a;

    {
        const auto context_a{
            manager.Get(seed_a)
        };

        weak_a = context_a;

        hash_a_first =
            context_a->Hash(
                input_bytes);
    }

    {
        const auto context_b{
            manager.Get(seed_b)
        };

        const uint256 hash_b{
            context_b->Hash(
                input_bytes)
        };

        BOOST_CHECK(
            hash_b != hash_a_first);
    }

    BOOST_CHECK(
        weak_a.expired());

    const auto context_a_rebuilt{
        manager.Get(seed_a)
    };

    const uint256 hash_a_rebuilt{
        context_a_rebuilt->Hash(
            input_bytes)
    };

    BOOST_CHECK(
        hash_a_rebuilt ==
        hash_a_first);

    BOOST_CHECK_EQUAL(
        manager.Size(),
        1U);
}

BOOST_AUTO_TEST_CASE(
    separate_vms_share_immutable_cache)
{
    const std::string input{
        "Xnuva RandomX concurrent VM test"
    };

    const std::span<const char>
        input_chars{
            input.data(),
            input.size()
        };

    const auto input_bytes{
        std::as_bytes(input_chars)
    };

    const xnuva::RandomXSeed seed{
        0,
        GENESIS_ID
    };

    xnuva::RandomXLightContextCache
        manager{1};

    const auto context{
        manager.Get(seed)
    };

    const uint256 expected{
        context->Hash(input_bytes)
    };

    auto future_a{
        std::async(
            std::launch::async,
            [context, input_bytes] {
                return context->Hash(
                    input_bytes);
            })
    };

    auto future_b{
        std::async(
            std::launch::async,
            [context, input_bytes] {
                return context->Hash(
                    input_bytes);
            })
    };

    const uint256 hash_a{
        future_a.get()
    };

    const uint256 hash_b{
        future_b.get()
    };

    BOOST_CHECK(hash_a == expected);
    BOOST_CHECK(hash_b == expected);
    BOOST_CHECK(hash_a == hash_b);
}

BOOST_AUTO_TEST_CASE(
    resource_error_is_distinct_type)
{
    BOOST_CHECK((
        std::is_base_of_v<
            std::runtime_error,
            xnuva::RandomXResourceError>
    ));

    BOOST_CHECK_THROW(
        xnuva::RandomXLightContextCache{0},
        std::invalid_argument);
}

BOOST_AUTO_TEST_SUITE_END()
