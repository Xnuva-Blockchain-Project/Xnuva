#include <boost/test/unit_test.hpp>

#include <primitives/block.h>
#include <uint256.h>
#include <xnuva/randomx.h>

#include <span>
#include <string>

BOOST_AUTO_TEST_SUITE(xnuva_randomx_tests)

BOOST_AUTO_TEST_CASE(seed_height_contract)
{
    struct Vector {
        uint64_t height;
        uint64_t seed;
    };

    constexpr Vector vectors[] = {
        {0, 0},
        {1, 0},
        {64, 0},
        {2048, 0},
        {2112, 0},
        {2113, 2048},
        {4096, 2048},
        {4160, 2048},
        {4161, 4096},
        {6208, 4096},
        {6209, 6144},
    };

    for (const auto& vector : vectors) {
        BOOST_CHECK_EQUAL(
            xnuva::RandomXSeedHeight(vector.height),
            vector.seed);
    }
}

BOOST_AUTO_TEST_CASE(upstream_v2_reference_vector)
{
    const std::string key{"test key 000"};
    const std::string input{"This is a test"};

    const std::span<const char> key_chars{
        key.data(),
        key.size()
    };

    const std::span<const char> input_chars{
        input.data(),
        input.size()
    };

    const uint256 hash{
        xnuva::RandomXHashV2(
            std::as_bytes(input_chars),
            std::as_bytes(key_chars))
    };

    /*
     * Upstream raw RandomX v2 output:
     *
     * 22ec6b861b3eb23686b2efbad69513c9
     * 67ecfce80983df66c9c5b4fbfb4cdb6f
     *
     * uint256::GetHex() reverses internal byte display.
     */
    BOOST_CHECK_EQUAL(
        hash.GetHex(),
        "6fdb4cfbfbb4c5c966df8309e8fcec67"
        "c91395d6baefb28636b23e1b866bec22");
}

BOOST_AUTO_TEST_CASE(xnuva_genesis_header_reference_vector)
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

    const uint256 seed_block_id{
        "2d95b3b13732ae491122eaacd12098b2"
        "97ddbb0180f0f6bdfaeee555be491aae"
    };

    BOOST_CHECK_EQUAL(
        header.GetHash().GetHex(),
        "2d95b3b13732ae491122eaacd12098b2"
        "97ddbb0180f0f6bdfaeee555be491aae");

    BOOST_CHECK_EQUAL(
        seed_block_id.data()[0],
        0xae);

    BOOST_CHECK_EQUAL(
        seed_block_id.data()[31],
        0x2d);

    const uint256 pow_hash{
        xnuva::RandomXHeaderHashV2(
            header,
            seed_block_id)
    };

    const uint256 repeated{
        xnuva::RandomXHeaderHashV2(
            header,
            seed_block_id)
    };

    BOOST_CHECK(pow_hash == repeated);
    BOOST_CHECK(pow_hash != header.GetHash());

    BOOST_TEST_MESSAGE(
        "XNUVA_RANDOMX_REFERENCE_HEADER_VECTOR="
        << pow_hash.GetHex());
}

BOOST_AUTO_TEST_SUITE_END()
