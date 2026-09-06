#include <xnuva/randomx_seed_tracker.h>

#include <chain.h>
#include <primitives/block.h>
#include <xnuva/randomx.h>

#include <limits>

namespace xnuva {

RandomXSequentialSeedTracker::RandomXSequentialSeedTracker(
    const CBlockIndex& chain_start)
    : m_chain_start{chain_start},
      m_height{chain_start.nHeight},
      m_tip_block_id{chain_start.GetBlockHash()}
{
}

std::optional<RandomXSeed>
RandomXSequentialSeedTracker::SeedForNext()
{
    if (m_height < 0 ||
        m_height == std::numeric_limits<int64_t>::max()) {
        return std::nullopt;
    }

    const uint64_t candidate_height{
        static_cast<uint64_t>(m_height + 1)
    };

    const uint64_t required_seed_height_u64{
        RandomXSeedHeight(candidate_height)
    };

    if (required_seed_height_u64 >
        static_cast<uint64_t>(
            std::numeric_limits<int>::max())) {
        return std::nullopt;
    }

    const int required_seed_height{
        static_cast<int>(
            required_seed_height_u64)
    };

    /*
     * If the required seed is at or behind the indexed anchor, resolve it
     * exclusively through that branch's ancestry.
     */
    if (required_seed_height <=
        m_chain_start.nHeight) {

        const CBlockIndex* seed_index{
            m_chain_start.GetAncestor(
                required_seed_height)
        };

        if (seed_index == nullptr ||
            seed_index->phashBlock == nullptr) {
            return std::nullopt;
        }

        return RandomXSeed{
            required_seed_height,
            seed_index->GetBlockHash()
        };
    }

    /*
     * Once the seed lies beyond the indexed anchor, it must have been seen
     * earlier in this exact forward stream because RandomX has a 64-block lag.
     */
    if (m_active_unindexed_seed &&
        m_active_unindexed_seed->height ==
            required_seed_height) {
        return m_active_unindexed_seed;
    }

    if (m_pending_unindexed_seed &&
        m_pending_unindexed_seed->height ==
            required_seed_height) {

        m_active_unindexed_seed =
            m_pending_unindexed_seed;

        m_pending_unindexed_seed.reset();

        return m_active_unindexed_seed;
    }

    return std::nullopt;
}

bool RandomXSequentialSeedTracker::Advance(
    const CBlockHeader& accepted_header)
{
    if (accepted_header.hashPrevBlock !=
        m_tip_block_id) {
        return false;
    }

    if (m_height ==
        std::numeric_limits<int64_t>::max()) {
        return false;
    }

    const int64_t next_height{
        m_height + 1
    };

    const uint256 block_id{
        accepted_header.GetHash()
    };

    /*
     * Every seed height produced by RandomXSeedHeight() is an epoch boundary.
     * Retain a newly observed boundary only when it lies beyond our indexed
     * anchor. It becomes active after the frozen 64-block lag.
     */
    if (next_height >
            m_chain_start.nHeight &&
        next_height >= 0 &&
        static_cast<uint64_t>(next_height) %
            RANDOMX_SEED_EPOCH_BLOCKS == 0) {

        /*
         * A pending seed must have become active long before the next 2048
         * block boundary if SeedForNext() has been called for every candidate.
         * Refuse silent state loss if the sequential contract is violated.
         */
        if (m_pending_unindexed_seed) {
            return false;
        }

        m_pending_unindexed_seed =
            RandomXSeed{
                static_cast<int>(next_height),
                block_id
            };
    }

    m_height = next_height;
    m_tip_block_id = block_id;

    return true;
}

} // namespace xnuva
