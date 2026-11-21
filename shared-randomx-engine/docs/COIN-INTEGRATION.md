# Shared RandomX — coin adapter contract and release gates

**Development only. No mainnet hard-fork approval or height is set.**

The foundation computes deterministic RandomX V2 hashes from caller-provided
raw bytes and keys. **It cannot know** whether the caller's blockchain is
at a fork boundary, which ancestor contains the seed, or what constitutes a
valid Proof of Work. Those are consensus rules and must remain coin-specific.

## Required coin adapter

1. Specify the exact RandomX **variant** and pinned source revision (V2 is
   initially implemented here; other variants require separate tests).
2. Serialize each coin's block header and define its exact RandomX input
   length, byte order, block identifier (may remain SHA256d) and target order.
3. Specify a deterministic fork-safe, height-aware seed/key schedule and
   derive it only from validated ancestors of the candidate branch.
4. Define and test separate initial post-activation nBits/target,
   retargeting, chainwork, reorg and long-stall rules; do not assume
   Scrypt difficulty can be translated by a simple hash-rate ratio.
5. In all validation paths, use the original PoW algorithm and difficulty
   for pre-fork blocks and RandomX for post-fork **PoW** blocks only.
6. In hybrid PoW+PoS coins, identify the block type **before** PoW selection.
   Preserve every historic and future PoS kernel hash, stake modifier,
   coinstake signature, stake eligibility, difficulty and reward rule.
7. Independently verify invalid blocks are rejected at H-1, H and H+1;
   test interleaved staking, multiple alternative chains and seed rollover.
8. Pin all miners, daemon and RPC compatibility separately, verify accepted
   blocks on separate nodes and reward maturity/spending, treasury accounting,
   shutdown/restart/reindex and sustained real-world operation.
9. Obtain operator/miner upgrade coordination and explicit separate approval
   for public network activation. Historical funds and genesis are immutable.

## Per-chain matrix

| Chain | Original consensus | Adapter obligation |
|-------|--------------------|--------------------|
| XNUVA | RandomX | Preserve already deployed variant and live chain. No implicit refactor. |
| TAO | Scrypt PoW | Preserve all historic Scrypt and original retarget; new RandomX PoW only after a separately approved activation height. |
| ERA | Legacy PoW | Audit live/historical algorithm and all difficulty rules before implementation. |
| ZUS | Hybrid PoW + PoS | Change only prospective PoW. Preserve all stake paths and hybrid chain-selection semantics. |
| HOPE | Hybrid PoW + PoS | Change only prospective PoW. Preserve all stake paths and hybrid chain-selection semantics. |

**The shared engine must never implement a coin's consensus activation in
the hashing library.** That separation is the key to reusability.
