# Shared RandomX Foundation (RX-001)

**STATUS: EXPERIMENTAL / SOURCE-LEVEL CHECKPOINT — DO NOT RELEASE OR DEPLOY**
**Branch:** feature/shared-randomx-engine-20261121
**Created:** 2026-11-21
**Project rule:** Reliability first, release second.

This is an **extraction-ready standalone subproject** within the XNUVA repository.
It is deliberately isolated from XNUVA Core's existing build and consensus.
It can eventually be moved to a dedicated repository without copying any coin-specific
consensus code. GitHub's available integration does not currently support creating a new
repository; creating this isolated branch/subdirectory was the safe immediate step.

## Goal

Provide one reusable, pinned **RandomX v2 hashing library** for future XNUVA,
TAO, ERA, ZUS and HOPE engineering. The first milestone focuses on correct
hash calculation and light-mode cache management, **not** PoW consensus
activation, fast-mode mining, chain security or wallet integration.

The raw RandomX 32-byte result is returned without reversing bytes. Each coin's
adapter must define comparison with its own compact-target representation and
block-ID byte order. This library intentionally does not contain any coin
type, reward, activation, seed-height or difficulty rule.

**Critical algorithm warning:** XNUVA uses a specific `RANDOMX_FLAG_V2`
variant in its vendored RandomX source. Plain upstream RandomX v1 is **not
interchangeable**. Before other coins choose this version for a fork, review
the exact V2 specification/source and get independent test vectors.

## Dependency policy

- Pinned XNUVA source revision: `853a3171ece198f60b5125a16dca83d68a8f53c3`.
- Vendored RandomX source at `src/randomx/` in that commit (including its
  original licensing notices).
- By default CMake downloads that *exact commit*; no floating branch or
  unversioned system `librandomx` is used.
- An explicit `RANDOMX_FOUNDATION_SOURCE_DIR` override can build against a
  local checkout; operators must independently validate its source provenance.
- Compilation and tests have **not** been run by the GitHub write operation.

## Minimal API

```cpp
#include <xnuva_randomx/engine.h>
// Input and seed are caller-defined byte spans.
// xnuva::randomx_foundation::LightHasher hasher(seed_key);
// auto raw_hash = hasher.Hash80ByteHeader(serialized_header_80_bytes);
```

The 256-MiB light-mode cache is initialised once per `LightHasher` and
reused across many hashes for the same seed. Concurrent calls on one instance
are serialised. Construct a separate instance when the seed changes. For
normal efficient mining we will later need a separate full-memory fast-mode
miner, reusable seed-key cache, controls on thread count and performance
measurement; a hashing helper alone is not a mining engine.

## Build in an isolated worktree

From the root of a checkout of this branch, where `src/randomx` is present:

```bash
cmake -S shared-randomx-engine -B /tmp/xnuva-rx-foundation-build \
  -DRANDOMX_FOUNDATION_SOURCE_DIR="$PWD/src/randomx" \
  -DBUILD_TESTING=ON
cmake --build /tmp/xnuva-rx-foundation-build -j2
ctest --test-dir /tmp/xnuva-rx-foundation-build --output-on-failure
```

Without the local override, CMake fetches the pinned XNUVA commit over
GitHub (network access required). Build/test independently on Linux and
Windows before describing compatibility as verified. A locally overridden
checkout is **not automatically proven** to match the pinned revision.

## Integration strategy

Other coins may include this CMake subproject or later consume a dedicated
repository release pinned to a tagged commit:

```cmake
add_subdirectory(path/to/shared-randomx-engine)
target_link_libraries(coin_node PRIVATE xnuva::randomx_foundation)
```

**This is not safe to do in a production daemon yet.** The coin-specific
adapter must choose exactly when to call RandomX and derive the seed from
validated canonical ancestors. No activation height has been set.
Historical PoW must still validate with its original algorithm.
ZUS and HOPE must also preserve and validate their independent PoS blocks
under the existing staking rules.

See [coin integration gates](docs/COIN-INTEGRATION.md).

## Verification

- [x] Shared implementation isolated in a new GitHub development branch.
- [x] Reference vectors and negative test cases committed to source.
- [ ] Actual CMake configuration/compilation and CTest execution.
- [ ] Cross-platform independent reference-vector confirmation.
- [ ] Performance, light/fast parity, memory and resource-failure tests.
- [ ] Source/version/license audit.
- [ ] Independent per-coin chain-context, seed, target, historical replay and
      hard-fork activation tests (separately gated).
- [ ] Release and deployment approval.

Do not equate checked-in test code with a PASS. No live files, wallets,
nodes, miners, blocks or blockchain consensus are modified by this branch.
