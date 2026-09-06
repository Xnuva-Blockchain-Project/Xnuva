#ifndef BITCOIN_XNUVA_RANDOMX_H
#define BITCOIN_XNUVA_RANDOMX_H

#include <primitives/block.h>
#include <uint256.h>

#include <cstddef>
#include <cstdint>
#include <span>

namespace xnuva {

static constexpr uint64_t RANDOMX_SEED_EPOCH_BLOCKS{2048};
static constexpr uint64_t RANDOMX_SEED_EPOCH_LAG{64};

/**
 * Return the SHA256d block height whose block ID supplies
 * the RandomX seed key for candidate block `height`.
 */
uint64_t RandomXSeedHeight(uint64_t height);

/**
 * Portable RandomX v2 light-mode reference implementation.
 *
 * This deliberately prioritizes deterministic correctness over
 * hashing throughput. It is not yet the optimized validation or
 * mining cache path.
 */
uint256 RandomXHashV2(
    std::span<const std::byte> input,
    std::span<const std::byte> key);

/**
 * Hash the canonical serialized 80-byte Xnuva block header using
 * the internal/serialized bytes of the SHA256d seed-block ID.
 *
 * This is a PoW digest.
 * It is NOT the Xnuva block identifier.
 */
uint256 RandomXHeaderHashV2(
    const CBlockHeader& header,
    const uint256& seed_block_id);

} // namespace xnuva

#endif // BITCOIN_XNUVA_RANDOMX_H
