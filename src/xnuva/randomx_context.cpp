#include <xnuva/randomx_context.h>

#include <chain.h>
#include <primitives/block.h>
#include <streams.h>
#include <xnuva/randomx.h>

#include <randomx.h>

#include <algorithm>
#include <memory>
#include <utility>

namespace xnuva {

std::optional<RandomXSeed> ResolveRandomXSeed(
    const CBlockIndex* pindex_prev)
{
    if (pindex_prev == nullptr) {
        return std::nullopt;
    }

    const uint64_t candidate_height{
        static_cast<uint64_t>(pindex_prev->nHeight) + 1
    };

    const uint64_t seed_height_u64{
        RandomXSeedHeight(candidate_height)
    };

    if (seed_height_u64 >
        static_cast<uint64_t>(pindex_prev->nHeight)) {
        return std::nullopt;
    }

    const int seed_height{
        static_cast<int>(seed_height_u64)
    };

    const CBlockIndex* seed_index{
        pindex_prev->GetAncestor(seed_height)
    };

    if (seed_index == nullptr ||
        seed_index->phashBlock == nullptr) {
        return std::nullopt;
    }

    return RandomXSeed{
        seed_height,
        seed_index->GetBlockHash()
    };
}

struct RandomXLightCache::Impl {
    randomx_cache* cache{nullptr};

    explicit Impl(const uint256& seed_block_id)
    {
        cache = randomx_alloc_cache(
            RANDOMX_FLAG_DEFAULT);

        if (cache == nullptr) {
            throw RandomXResourceError(
                "RandomX light cache allocation failed");
        }

        randomx_init_cache(
            cache,
            seed_block_id.data(),
            seed_block_id.size());
    }

    ~Impl()
    {
        if (cache != nullptr) {
            randomx_release_cache(cache);
        }
    }
};

RandomXLightCache::RandomXLightCache(
    RandomXSeed seed)
    : m_seed{std::move(seed)},
      m_impl{std::make_unique<Impl>(
          m_seed.block_id)}
{
}

RandomXLightCache::~RandomXLightCache() = default;

const RandomXSeed&
RandomXLightCache::Seed() const noexcept
{
    return m_seed;
}

uint256 RandomXLightCache::Hash(
    std::span<const std::byte> input) const
{
    std::unique_ptr<
        randomx_vm,
        decltype(&randomx_destroy_vm)
    > vm{
        randomx_create_vm(
            RANDOMX_FLAG_V2,
            m_impl->cache,
            nullptr),
        &randomx_destroy_vm
    };

    if (!vm) {
        throw RandomXResourceError(
            "RandomX v2 light VM allocation failed");
    }

    uint256 result;

    randomx_calculate_hash(
        vm.get(),
        input.data(),
        input.size(),
        result.begin());

    return result;
}

uint256 RandomXLightCache::HashHeader(
    const CBlockHeader& header) const
{
    DataStream stream{};
    stream << header;

    if (stream.size() != 80) {
        throw std::runtime_error(
            "Xnuva RandomX header serialization is not 80 bytes");
    }

    return Hash(
        std::span<const std::byte>{
            stream.data(),
            stream.size()
        });
}

RandomXLightContextCache::RandomXLightContextCache(
    std::size_t capacity)
    : m_capacity{capacity}
{
    if (m_capacity == 0) {
        throw std::invalid_argument(
            "RandomX cache capacity must be nonzero");
    }
}

std::shared_ptr<const RandomXLightCache>
RandomXLightContextCache::Get(
    const RandomXSeed& seed)
{
    std::lock_guard<std::mutex> lock{
        m_mutex
    };

    const auto existing{
        m_entries.find(seed)
    };

    if (existing != m_entries.end()) {
        existing->second.last_used = ++m_clock;
        return existing->second.context;
    }

    if (m_entries.size() >= m_capacity) {
        const auto victim{
            std::min_element(
                m_entries.begin(),
                m_entries.end(),
                [](const auto& lhs,
                   const auto& rhs) {
                    return lhs.second.last_used <
                           rhs.second.last_used;
                })
        };

        if (victim != m_entries.end()) {
            m_entries.erase(victim);
        }
    }

    auto context{
        std::make_shared<RandomXLightCache>(
            seed)
    };

    m_entries.emplace(
        seed,
        Entry{
            context,
            ++m_clock
        });

    return context;
}

std::size_t
RandomXLightContextCache::Size() const
{
    std::lock_guard<std::mutex> lock{
        m_mutex
    };

    return m_entries.size();
}

} // namespace xnuva
