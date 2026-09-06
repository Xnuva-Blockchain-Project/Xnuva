# Xnuva RandomX validation context

This layer is deliberately separate from consensus activation.

## Seed resolution

For a candidate block following `pindexPrev`:

    candidate_height = pindexPrev->nHeight + 1
    seed_height = RandomXSeedHeight(candidate_height)
    seed_index = pindexPrev->GetAncestor(seed_height)
    seed_id = seed_index->GetBlockHash()

The lookup is branch-contextual.

The active chain is not consulted to resolve the seed of a competing
branch.

## Cache identity

A reusable light cache is identified by both:

    seed_height
    SHA256d seed block ID

Height alone is not sufficient because competing branches can contain
different blocks at the same height.

## Cache and VM lifetime

The RandomX cache is initialized once and is immutable while being used.

Separate RandomX VMs may reference the same initialized cache.

A VM is never concurrently shared between hashing workers.

The Stage 8G implementation creates an exclusive VM for each Hash()
operation. This is intentionally conservative; a future exclusive VM
pool may improve performance without changing hash semantics.

## Validation mode

Light mode is sufficient for consensus validation.

FULL_MEM/dataset mode remains an optional mining/performance
optimization and is not required for chain validity.

## Eviction and reorgs

Cache retention is performance policy only.

An evicted context can always be reconstructed from the exact seed
block ID.

During a reorg, seed resolution follows the candidate branch ancestry,
not the globally active chain.

## Resource failures

Failure to allocate a RandomX cache or VM is represented as a local
RandomXResourceError.

It is not itself evidence that a candidate block has invalid proof of
work.

## Consensus status

Stage 8G does not change any proof-of-work caller.

CBlockHeader::GetHash() remains SHA256d block identity.

RandomX remains inactive for consensus and mining.
