#ifndef BITCOIN_XNUVA_RANDOMX_SEED_TRACKER_H
#define BITCOIN_XNUVA_RANDOMX_SEED_TRACKER_H

#include <uint256.h>
#include <xnuva/randomx_context.h>

#include <cstdint>
#include <optional>

class CBlockHeader;
class CBlockIndex;

namespace xnuva {

/**
 * Bounded branch-context tracker for a strictly forward, unindexed sequence
 * of block headers.
 *
 * The indexed anchor is never replaced with active-chain-global state.
 *
 * At most two unindexed RandomX seed identities are retained:
 *  - the currently active unindexed seed;
 *  - the next seed block seen during the 64-block lag window.
 *
 * A separate tracker must be created when a stream is restarted, for example
 * when headers synchronization moves from PRESYNC to REDOWNLOAD.
 */
class RandomXSequentialSeedTracker
{
public:
    explicit RandomXSequentialSeedTracker(
        const CBlockIndex& chain_start);

    /**
     * Return the RandomX seed required by the next candidate header.
     *
     * std::nullopt means the required branch seed is not available.
     */
    std::optional<RandomXSeed> SeedForNext();

    /**
     * Advance the tracker after the caller has accepted the current header.
     *
     * Returns false if the header does not extend the exact tracked branch.
     */
    bool Advance(
        const CBlockHeader& accepted_header);

    int64_t Height() const noexcept
    {
        return m_height;
    }

    const uint256& TipBlockId() const noexcept
    {
        return m_tip_block_id;
    }

private:
    const CBlockIndex& m_chain_start;

    int64_t m_height;
    uint256 m_tip_block_id;

    std::optional<RandomXSeed> m_active_unindexed_seed;
    std::optional<RandomXSeed> m_pending_unindexed_seed;
};

} // namespace xnuva

#endif // BITCOIN_XNUVA_RANDOMX_SEED_TRACKER_H
