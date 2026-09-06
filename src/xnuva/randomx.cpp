#include <xnuva/randomx.h>

#include <streams.h>

#include <randomx.h>

#include <memory>
#include <stdexcept>

namespace {

struct RandomXCacheDeleter {
    void operator()(randomx_cache* cache) const
    {
        randomx_release_cache(cache);
    }
};

struct RandomXVMDeleter {
    void operator()(randomx_vm* vm) const
    {
        randomx_destroy_vm(vm);
    }
};

using RandomXCachePtr =
    std::unique_ptr<randomx_cache, RandomXCacheDeleter>;

using RandomXVMPtr =
    std::unique_ptr<randomx_vm, RandomXVMDeleter>;

} // namespace

namespace xnuva {

uint64_t RandomXSeedHeight(uint64_t height)
{
    if (height <= RANDOMX_SEED_EPOCH_BLOCKS +
                      RANDOMX_SEED_EPOCH_LAG) {
        return 0;
    }

    return (height - RANDOMX_SEED_EPOCH_LAG - 1) &
           ~(RANDOMX_SEED_EPOCH_BLOCKS - 1);
}

uint256 RandomXHashV2(
    std::span<const std::byte> input,
    std::span<const std::byte> key)
{
    RandomXCachePtr cache{
        randomx_alloc_cache(RANDOMX_FLAG_DEFAULT)
    };

    if (!cache) {
        throw std::runtime_error(
            "RandomX cache allocation failed");
    }

    randomx_init_cache(
        cache.get(),
        key.data(),
        key.size());

    RandomXVMPtr vm{
        randomx_create_vm(
            RANDOMX_FLAG_V2,
            cache.get(),
            nullptr)
    };

    if (!vm) {
        throw std::runtime_error(
            "RandomX v2 VM allocation failed");
    }

    uint256 result;

    randomx_calculate_hash(
        vm.get(),
        input.data(),
        input.size(),
        result.begin());

    return result;
}

uint256 RandomXHeaderHashV2(
    const CBlockHeader& header,
    const uint256& seed_block_id)
{
    DataStream stream{};
    stream << header;

    if (stream.size() != 80) {
        throw std::runtime_error(
            "Xnuva RandomX header serialization is not 80 bytes");
    }

    const std::span<const std::byte> header_bytes{
        stream.data(),
        stream.size()
    };

    const std::span<const unsigned char> seed_bytes{
        seed_block_id.data(),
        seed_block_id.size()
    };

    return RandomXHashV2(
        header_bytes,
        std::as_bytes(seed_bytes));
}

} // namespace xnuva
