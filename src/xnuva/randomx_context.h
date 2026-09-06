#ifndef BITCOIN_XNUVA_RANDOMX_CONTEXT_H
#define BITCOIN_XNUVA_RANDOMX_CONTEXT_H

#include <uint256.h>

#include <cstddef>
#include <cstdint>
#include <map>
#include <memory>
#include <mutex>
#include <optional>
#include <span>
#include <stdexcept>

class CBlockHeader;
class CBlockIndex;

namespace xnuva {

struct RandomXSeed {
    int height;
    uint256 block_id;

    friend bool operator==(const RandomXSeed&, const RandomXSeed&) = default;

    friend bool operator<(const RandomXSeed& lhs, const RandomXSeed& rhs)
    {
        if (lhs.height != rhs.height) {
            return lhs.height < rhs.height;
        }

        return lhs.block_id < rhs.block_id;
    }
};

/**
 * Resolve the RandomX seed for the block which would follow pindex_prev.
 *
 * The lookup is deliberately branch-contextual. It never consults the
 * globally active chain.
 *
 * nullptr represents the genesis candidate, which is exempt from RandomX.
 */
std::optional<RandomXSeed> ResolveRandomXSeed(
    const CBlockIndex* pindex_prev);

class RandomXResourceError : public std::runtime_error
{
public:
    using std::runtime_error::runtime_error;
};

/**
 * Dedicated sequential RandomX v2 mining hasher.
 *
 * Unlike RandomXLightCache::Hash(), this object owns one VM for the
 * complete nonce search. It is deliberately mining-only; ordinary
 * consensus validation keeps its per-call isolated VM behavior.
 */
class RandomXMiningHasher
{
public:
    explicit RandomXMiningHasher(
        const RandomXSeed& seed);

    ~RandomXMiningHasher();

    RandomXMiningHasher(
        const RandomXMiningHasher&) = delete;

    RandomXMiningHasher& operator=(
        const RandomXMiningHasher&) = delete;

    uint256 Hash(
        std::span<const std::byte> input) const;

    uint256 HashHeader(
        const CBlockHeader& header) const;

private:
    struct Impl;
    std::unique_ptr<Impl> m_impl;
};

/**
 * One immutable RandomX light-mode cache for one exact seed block.
 *
 * Multiple separate VMs may use this cache concurrently.
 * Hash() creates an exclusive VM for each invocation.
 */
class RandomXLightCache
{
public:
    explicit RandomXLightCache(RandomXSeed seed);
    ~RandomXLightCache();

    RandomXLightCache(const RandomXLightCache&) = delete;
    RandomXLightCache& operator=(const RandomXLightCache&) = delete;

    const RandomXSeed& Seed() const noexcept;

    uint256 Hash(
        std::span<const std::byte> input) const;

    uint256 HashHeader(
        const CBlockHeader& header) const;

private:
    struct Impl;

    RandomXSeed m_seed;
    std::unique_ptr<Impl> m_impl;
};

/**
 * Small LRU owner for reusable light caches.
 *
 * Eviction changes performance only. A missing cache is deterministically
 * reconstructed from the exact seed block ID.
 */
class RandomXLightContextCache
{
public:
    explicit RandomXLightContextCache(std::size_t capacity);

    std::shared_ptr<const RandomXLightCache> Get(
        const RandomXSeed& seed);

    std::size_t Size() const;

private:
    struct Entry {
        std::shared_ptr<const RandomXLightCache> context;
        uint64_t last_used;
    };

    const std::size_t m_capacity;

    mutable std::mutex m_mutex;
    uint64_t m_clock{0};

    std::map<RandomXSeed, Entry> m_entries;
};

} // namespace xnuva

#endif // BITCOIN_XNUVA_RANDOMX_CONTEXT_H
