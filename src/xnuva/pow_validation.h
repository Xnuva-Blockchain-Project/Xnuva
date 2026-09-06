#ifndef BITCOIN_XNUVA_POW_VALIDATION_H
#define BITCOIN_XNUVA_POW_VALIDATION_H

class CBlockHeader;
class CBlockIndex;

namespace Consensus {
struct Params;
}

namespace xnuva {

class RandomXLightContextCache;
struct RandomXSeed;

/**
 * Result of validating one Xnuva block header's proof of work.
 *
 * VALID
 *   The appropriate proof for this candidate satisfies header.nBits.
 *
 * INVALID
 *   The target encoding is invalid or the proof exceeds the target.
 *
 * CONTEXT_UNAVAILABLE
 *   A non-genesis candidate could not resolve its required branch-contextual
 *   RandomX seed. This is not proof of invalid work.
 *
 * RandomXResourceError is deliberately not converted into one of these
 * values. Allocation/cache/VM failure is a local resource failure and
 * propagates to the caller.
 */
enum class PoWValidationResult {
    VALID,
    INVALID,
    CONTEXT_UNAVAILABLE,
};

/**
 * Validate the proof of work for the block which would follow pindex_prev.
 *
 * pindex_prev == nullptr represents the genesis candidate. Genesis retains
 * the legacy SHA256d proof-of-work check.
 *
 * Every post-genesis candidate uses RandomX v2 over the canonical serialized
 * 80-byte CBlockHeader. Its seed is resolved from pindex_prev, so competing
 * branches are validated against their own ancestry.
 *
 * CBlockHeader::GetHash() remains SHA256d block identity and is never replaced
 * by the RandomX digest.
 */
PoWValidationResult ValidateProofOfWork(
    const CBlockHeader& header,
    const CBlockIndex* pindex_prev,
    const Consensus::Params& params,
    RandomXLightContextCache& randomx_contexts);

/**
 * Validate post-genesis RandomX proof of work using an already resolved,
 * exact branch seed.
 *
 * This entry point exists for strictly sequential unindexed header streams,
 * where no CBlockIndex exists yet for the candidate ancestry.
 *
 * It never changes CBlockHeader::GetHash() and never treats the supplied seed
 * block ID as display-order hex.
 *
 * RandomXResourceError deliberately propagates.
 */
PoWValidationResult ValidateRandomXProofOfWork(
    const CBlockHeader& header,
    const RandomXSeed& seed,
    const Consensus::Params& params,
    RandomXLightContextCache& randomx_contexts);

} // namespace xnuva

#endif // BITCOIN_XNUVA_POW_VALIDATION_H
