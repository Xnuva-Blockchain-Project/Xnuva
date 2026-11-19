// XNUVA experimental RandomX startup benchmark.
// DO NOT RELEASE. No consensus implementation changes.

#include <bench/bench.h>
#include <randomx.h>
#include <primitives/block.h>
#include <streams.h>
#include <uint256.h>
#include <xnuva/randomx_context.h>

#include <cstddef>
#include <span>
#include <stdexcept>
#include <memory>

static void XnuvaRandomXFreshVM(benchmark::Bench& bench)
{
    const uint256 genesis_id{
        "2d95b3b13732ae491122eaacd12098b2"
        "97ddbb0180f0f6bdfaeee555be491aae"
    };

    const uint256 expected{
        "80b59afe417178f46435a111204c14604"
        "d13971b812588e612597a50f9717085"
    };

    CBlockHeader header{};
    header.nVersion = 1;
    header.hashPrevBlock.SetNull();

    header.hashMerkleRoot = uint256{
        "fb923316be9e4f5db3467d8c2d159bb"
        "3a27fa9ba2c71703c078a0ea7ccb676e8"
    };

    header.nTime = 1788642000;
    header.nBits = 0x207fffff;
    header.nNonce = 0;

    DataStream stream{};
    stream << header;

    if (stream.size() != 80) {
        throw std::runtime_error(
            "XNUVA benchmark header is not 80 bytes");
    }

    const std::span<const std::byte> input{
        stream.data(), stream.size()
    };

    const xnuva::RandomXSeed seed{0, genesis_id};
    const xnuva::RandomXLightCache cache{seed};

    // Correctness must be established before timing.
    if (cache.Hash(input) != expected) {
        throw std::runtime_error(
            "XNUVA baseline RandomX digest mismatch");
    }

    bench.minEpochIterations(1);

    bench.run([&] {
        // Existing validation behaviour: fresh VM per hash.
        const uint256 result{cache.Hash(input)};

        if (result != expected) {
            throw std::runtime_error(
                "XNUVA benchmark hash mismatch");
        }
    });
}

BENCHMARK(XnuvaRandomXFreshVM);


// Experimental reusable VM. Never share this VM concurrently.
// Cache and VM are owned exclusively by this benchmark.
static void XnuvaRandomXReuseVM(benchmark::Bench& bench)
{
    const uint256 genesis_id{
        "2d95b3b13732ae491122eaacd12098b2"
        "97ddbb0180f0f6bdfaeee555be491aae"
    };

    const uint256 expected{
        "80b59afe417178f46435a111204c14604"
        "d13971b812588e612597a50f9717085"
    };

    CBlockHeader header{};
    header.nVersion = 1;
    header.hashPrevBlock.SetNull();

    header.hashMerkleRoot = uint256{
        "fb923316be9e4f5db3467d8c2d159bb"
        "3a27fa9ba2c71703c078a0ea7ccb676e8"
    };

    header.nTime = 1788642000;
    header.nBits = 0x207fffff;
    header.nNonce = 0;

    DataStream stream{};
    stream << header;

    if (stream.size() != 80) {
        throw std::runtime_error(
            "XNUVA reused-VM header is not 80 bytes");
    }

    using CachePtr = std::unique_ptr<
        randomx_cache,
        decltype(&randomx_release_cache)>;

    using VmPtr = std::unique_ptr<
        randomx_vm,
        decltype(&randomx_destroy_vm)>;

    CachePtr cache{
        randomx_alloc_cache(RANDOMX_FLAG_DEFAULT),
        &randomx_release_cache
    };

    if (!cache) {
        throw std::runtime_error(
            "RandomX benchmark cache allocation failed");
    }

    randomx_init_cache(
        cache.get(),
        genesis_id.data(),
        genesis_id.size());

    VmPtr vm{
        randomx_create_vm(
            RANDOMX_FLAG_V2,
            cache.get(),
            nullptr),
        &randomx_destroy_vm
    };

    if (!vm) {
        throw std::runtime_error(
            "RandomX benchmark VM allocation failed");
    }

    const auto check_hash = [&] {
        uint256 result;

        randomx_calculate_hash(
            vm.get(),
            stream.data(),
            stream.size(),
            result.begin());

        if (result != expected) {
            throw std::runtime_error(
                "XNUVA reused-VM digest mismatch");
        }
    };

    // Check the fixed vector before timing.
    check_hash();

    bench.minEpochIterations(1);

    bench.run([&] {
        // Same VM, same seed, same RandomX V2 input.
        check_hash();
    });
}

BENCHMARK(XnuvaRandomXReuseVM);

// Hardware-accelerated RandomX V2 experiment.
static void XnuvaRandomXAcceleratedVM(benchmark::Bench& bench)
{
    const uint256 genesis_id{
        "2d95b3b13732ae491122eaacd12098b2"
        "97ddbb0180f0f6bdfaeee555be491aae"
    };

    const uint256 expected{
        "80b59afe417178f46435a111204c14604"
        "d13971b812588e612597a50f9717085"
    };

    CBlockHeader header{};
    header.nVersion = 1;
    header.hashPrevBlock.SetNull();

    header.hashMerkleRoot = uint256{
        "fb923316be9e4f5db3467d8c2d159bb"
        "3a27fa9ba2c71703c078a0ea7ccb676e8"
    };

    header.nTime = 1788642000;
    header.nBits = 0x207fffff;
    header.nNonce = 0;

    DataStream stream{};
    stream << header;

    if (stream.size() != 80) {
        throw std::runtime_error(
            "XNUVA reused-VM header is not 80 bytes");
    }

    using CachePtr = std::unique_ptr<
        randomx_cache,
        decltype(&randomx_release_cache)>;

    using VmPtr = std::unique_ptr<
        randomx_vm,
        decltype(&randomx_destroy_vm)>;

    const randomx_flags recommended{
        randomx_get_flags()
    };

    if (!(recommended & RANDOMX_FLAG_HARD_AES) ||
        !(recommended & RANDOMX_FLAG_JIT)) {
        throw std::runtime_error(
            "Required RandomX acceleration unavailable");
    }

    CachePtr cache{
        randomx_alloc_cache(recommended),
        &randomx_release_cache
    };

    if (!cache) {
        throw std::runtime_error(
            "RandomX benchmark cache allocation failed");
    }

    randomx_init_cache(
        cache.get(),
        genesis_id.data(),
        genesis_id.size());

    VmPtr vm{
        randomx_create_vm(
            static_cast<randomx_flags>(recommended | RANDOMX_FLAG_V2),
            cache.get(),
            nullptr),
        &randomx_destroy_vm
    };

    if (!vm) {
        throw std::runtime_error(
            "RandomX benchmark VM allocation failed");
    }

    const auto check_hash = [&] {
        uint256 result;

        randomx_calculate_hash(
            vm.get(),
            stream.data(),
            stream.size(),
            result.begin());

        if (result != expected) {
            throw std::runtime_error(
                "XNUVA reused-VM digest mismatch");
        }
    };

    // Check the fixed vector before timing.
    check_hash();

    bench.minEpochIterations(1);

    bench.run([&] {
        // Same VM, same seed, same RandomX V2 input.
        check_hash();
    });
}

BENCHMARK(XnuvaRandomXAcceleratedVM);

// Accelerated VM creation per hash.
static void XnuvaRandomXAcceleratedFreshVM(benchmark::Bench& bench)
{
    const uint256 genesis_id{
        "2d95b3b13732ae491122eaacd12098b2"
        "97ddbb0180f0f6bdfaeee555be491aae"
    };

    const uint256 expected{
        "80b59afe417178f46435a111204c14604"
        "d13971b812588e612597a50f9717085"
    };

    CBlockHeader header{};
    header.nVersion = 1;
    header.hashPrevBlock.SetNull();

    header.hashMerkleRoot = uint256{
        "fb923316be9e4f5db3467d8c2d159bb"
        "3a27fa9ba2c71703c078a0ea7ccb676e8"
    };

    header.nTime = 1788642000;
    header.nBits = 0x207fffff;
    header.nNonce = 0;

    DataStream stream{};
    stream << header;

    if (stream.size() != 80) {
        throw std::runtime_error(
            "XNUVA reused-VM header is not 80 bytes");
    }

    using CachePtr = std::unique_ptr<
        randomx_cache,
        decltype(&randomx_release_cache)>;

    using VmPtr = std::unique_ptr<
        randomx_vm,
        decltype(&randomx_destroy_vm)>;

    const randomx_flags recommended{
        randomx_get_flags()
    };

    if (!(recommended & RANDOMX_FLAG_HARD_AES) ||
        !(recommended & RANDOMX_FLAG_JIT)) {
        throw std::runtime_error(
            "Required RandomX acceleration unavailable");
    }

    CachePtr cache{
        randomx_alloc_cache(recommended),
        &randomx_release_cache
    };

    if (!cache) {
        throw std::runtime_error(
            "RandomX benchmark cache allocation failed");
    }

    randomx_init_cache(
        cache.get(),
        genesis_id.data(),
        genesis_id.size());

    const auto check_hash = [&] {
        VmPtr vm{
            randomx_create_vm(
                static_cast<randomx_flags>(recommended | RANDOMX_FLAG_V2),
                cache.get(),
                nullptr),
            &randomx_destroy_vm
        };

        if (!vm) {
            throw std::runtime_error(
                "RandomX benchmark VM allocation failed");
        }

        uint256 result;

        randomx_calculate_hash(
            vm.get(),
            stream.data(),
            stream.size(),
            result.begin());

        if (result != expected) {
            throw std::runtime_error(
                "XNUVA reused-VM digest mismatch");
        }
    };

    // Check the fixed vector before timing.
    check_hash();

    bench.minEpochIterations(1);

    bench.run([&] {
        // Same VM, same seed, same RandomX V2 input.
        check_hash();
    });
}

BENCHMARK(XnuvaRandomXAcceleratedFreshVM);
