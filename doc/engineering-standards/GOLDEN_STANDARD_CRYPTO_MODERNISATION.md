# Golden Standard Cryptocurrency Modernisation Handbook

> **Reliability first, release second.**

> Public engineering baseline; historical observations are not release approvals. Never commit private keys, credentials, unredacted personal data or sensitive infrastructure details.

**Document ID:** GS-CM-001 \| Version: 1.6 (draft) \| Baseline: 18 November 2026

Applies to TAO Revival, XNUVA, ERA, ZUS, HOPE and every future core,
miner, wallet, node, network or game-treasury modernisation.

# Executive engineering covenant

A build, checkpoint, fixture PASS or locally generated block cannot by
itself establish that a cryptocurrency works. Release qualification
requires historical consensus continuity, independent real-network block
acceptance, correct wallet accounting and spendability, dependent-system
integration where applicable, restart/recovery, and sustained monitored
operation. A failure or unknown at a mandatory gate is a release
blocker.

**Production continuity rule:** **Never destabilise a functioning production network merely to accelerate its modernisation.**
A stable legacy node may remain in service while a replacement is developed
and verified separately; a calendar target is never a deployment justification.

Default posture: preserve existing live chains, genesis, balances, keys,
wallet compatibility and mining economics. All consensus changes must be
intentional, documented, activation-height gated where appropriate, and
proven safe. Never quietly reset, re-genesis, alter historic blocks,
bypass consensus, or deploy experimental binaries into production.

# 1. Scope, evidence language and decision authority

## 1.1 Evidence states

| **State**           | **Meaning**                                                                                                                | **Allowed conclusion**                               |
|---------------------|----------------------------------------------------------------------------------------------------------------------------|------------------------------------------------------|
| VERIFIED            | A recorded test with environment, commands, expected/actual outputs and independent observations, within its stated scope. | PASS only for that specific assertion.               |
| REPORTED / OBSERVED | An operator observation or previous checkpoint result without a complete independently reproduced evidence bundle.         | Useful evidence; not expanded into wider guarantees. |
| UNVERIFIED          | No conclusive evidence yet, including a test not run or output not retained.                                               | Do not claim PASS.                                   |
| FAILED / BLOCKED    | An observed failure or a mandatory gate that cannot be satisfied.                                                          | Hold release and open issue.                         |
| NOT APPLICABLE      | Documented reason a control does not apply.                                                                                | Requires explicit approver sign-off.                 |

A historical PASS must identify its exact tested binary, branch, commit
and fixture. A later source change invalidates affected checks until
rerun. When a previous conversation mentions a PASS without its
underlying log, record it as “checkpoint-reported” pending artefact
reconciliation.

## 1.2 Mandatory boundaries

☐ Production chain, genuine genesis, historical blocks and coin supply
are immutable; no re-genesis or database recreation as a shortcut.

☐ Existing funds, wallet keys, legacy addresses and user balances must
remain recoverable and spendable.

☐ Isolated test networks use distinct datadirs, ports, identifiers, keys
and explicit mining guards.

☐ Mainnet production block creation remains disabled until separately
authorised and qualified.

☐ No exchange/custody integration is implied by this handbook; game
treasury is an internal dependency where applicable.

☐ Release decisions are evidence-based and recorded, never inferred from
unit-test green status.

☐ If the current production chain is functioning, retain the proven binary
and operating configuration; modernise only in isolated environments until
all affected release gates and an operator-approved rollback plan pass.

# 2. Baseline inventory and reproducibility

☐ Record authoritative source repository, branch, upstream
version/commit, local commit, exact toolchain and build switches.

☐ Record active deployment machines, OS, systemd units, miner paths,
working directories, arguments and startup order.

☐ Inventory genesis ID, network magic, checkpoints, active chain
tip/height, datadir, block files, chainstate and wallets.

☐ Capture consensus parameters: PoW algorithm, block header layout,
subsidy, halving, maturity, target spacing, retargeting, size/weight,
activation rules, scripts and timestamp rules.

☐ Capture address/version prefixes, Base58/Bech32 support policy,
WIF/key formats, derivation policies, GUI defaults and RPC
compatibility.

☐ Create checksummed, offline-verifiable backups of wallet, chain, key
material, configuration, original binaries, source and service units.
Protect private keys and avoid copying secrets into test reports.

☐ Record RPC endpoints and auth practices; restrict unsafe methods,
monitor open listening ports and avoid publishing credentials.

☐ Record external dependencies: peers, seed nodes, difficulty behaviour,
miner-to-daemon protocol, rotation controller, treasury bridge, database
and website.

# 3. Consensus preservation and historical replay

☐ Pin the authoritative legacy source and audit chainparams rather than
accepting upstream modern defaults.

☐ Recompute/compare genesis hash, merkle root, serialized block/header
bytes, network magic and proof-of-work hashing for known blocks.

☐ Test complete historical replay from a known-good snapshot on a
disposable copy, with verified height, tip hash and per-height consensus
results.

☐ Test independently constructed headers and blocks; distinguish block
ID hash from PoW hash when algorithms differ.

☐ Preserve original difficulty retarget edge cases, timestamp handling,
nBits, coinbase serialization, block-signature legacy fields and
historical exceptions.

☐ Prove subsidy, halvings, fee accounting, coinbase maturity, money
range and any cap at boundary heights, including overflow and negative
tests.

☐ Preserve original script activation and non-activation states (e.g.
SegWit/Taproot must not silently activate on legacy networks).

☐ Prove no silent soft/hard fork: compare old and new validating
implementations on historical fixtures and adversarial blocks.

☐ Any deliberately changed forward rule requires written rationale,
explicit activation height, test vectors, compatibility/upgrade plan and
independent sign-off.

☐ Detect incompatible database/chainstate migrations; support restore or
rollback from preserved backups without assuming reverse schema
compatibility.

# 4. Wallet continuity, addresses and accounting

☐ Import and unlock legacy wallets only on cloned test assets first;
validate all known key formats and encrypted wallet behaviour.

☐ Verify generated/decoded receiving addresses match chain prefixes and
accepted script types; prevent unsupported new default formats.

☐ Verify WIF, HD/non-HD paths where supported, signatures, transaction
serialization, fee calculations and change-address choice.

☐ Reject unsupported output/change types safely with explicit errors;
never silently send to a format the network cannot validate.

☐ Persist keys across multiple graceful shutdowns, cold restarts and
wallet rescans; verify backup/restore of encrypted and unencrypted
wallets.

☐ Fund a wallet via a real accepted block/transaction; check immature
versus mature rewards and balance categories without double counting.

☐ Test outbound spending with a signed and independently accepted
transaction; confirm change returns, fees reconcile and balances survive
rescan and restart.

☐ Test reorg/reversal accounting, double-spend/replaced transaction
handling as supported, and rejected block reward non-credit.

☐ Check UI labels, alerts, warnings, daemon names, transaction types,
currency symbols and user-visible errors for incorrect upstream
branding.

# 5. Mining and real-network acceptance

☐ Trace full job path: wallet/miner address → daemon getblocktemplate or
getwork/Stratum bridge → candidate → PoW → submitblock → peer/network
acceptance → chain tip.

☐ Validate coinbase format, timestamp placement, extra nonce, merkle
construction, serialization (including any legacy empty signature),
difficulty target and PoW encoding.

☐ Mine a new genuine block on an isolated multi-node test network; have
an independent node validate and adopt it.

☐ Confirm miner logs, daemon logs, block hash and peer tip agree;
“submitted” or local “found” without acceptance is not PASS.

☐ Verify miner handover, rotation, target coin selection and correct
daemon endpoint during every switch.

☐ Record orphan/stale/rejected paths; distinguish miner computation from
consensus rejection and peer propagation failures.

☐ Prove accepted coinbase reward appears in the intended wallet as
immature, matures after the chain-specific depth, then becomes
spendable.

☐ On networks with an external pool/treasury, validate exact identity
mapping and custody/credit logic, not just hash rate.

# 6. Treasury / game integration

☐ Document coin-to-game identity, miner reward address, daemon RPC,
maturity depth, bridge polling, database ledger and treasury display
paths.

☐ Use a traceable test block/transaction ID and follow each state:
accepted, confirmations, mature, indexed, credited, rendered.

☐ Reconcile daemon balance, wallet entries, bridge events, database
movements and game UI using a common transaction/block ID.

☐ Ensure idempotent bridge replay and exactly-once effective credit;
reject duplicate, stale, wrong-chain and orphaned rewards.

☐ Test rotations and handover between XNUVA, ERA, ZUS, HOPE, TAO as
relevant; no inference that a rotation slot means a coin was mined.

☐ Check treasury totals before and after maturity, poll cycles and
process restarts; record expected delays and retry semantics.

☐ For a failed test, locate the exact broken boundary: no accepted
block, no wallet credit, immature reward, bridge failure, ledger failure
or UI cache/display.

# 7. Security and isolated development

☐ Use disposable worktrees and nonproduction datadirs; explicitly tag
experimental branches and binaries DO NOT RELEASE.

☐ Hash-gate all source changes and original artifacts; retain diffs,
SHA-256 values and reproducible build commands.

☐ Use least privilege for services, segregated users and filesystem
permissions for wallets, datadirs, logs and binaries.

☐ Protect RPC by local binding, credentials/cookie auth, suitable
firewall rules and no passwords in scripts or logs.

☐ Review dependency and build-chain provenance; reject
unsigned/unverified downloads and hardcoded secrets.

☐ Run static analysis, unit and property tests, fuzz/differential tests
for parsers, scripts, headers, address decoding and transaction
serialization.

☐ Negative tests must fail closed: unsafe mainnet mining, wrong
network/datadir, unknown prefixes, malformed blocks, invalid signatures
and unsupported output policies.

☐ Never overwrite production keys or chainstate while debugging; all
destructive migration rehearsals occur on backups.

☐ Record threat model for reorgs, replay, RPC misuse, wallet loss,
overflow, duplicate credits, disk exhaustion and startup stalls.

# 8. Restart, recovery and operations

☐ Measure cold-start time for a fully populated actual datadir; identify
block-index loading, database verification, memory pressure, RandomX
verification and RPC readiness.

☐ Verify clean shutdown and immediate restart twice; confirm same chain
tip, wallet keys, balances and RPC availability.

☐ Simulate abrupt process failure on a disposable copy; validate
database integrity and replay/reindex behaviour.

☐ Test service/daemon startup order, miner reconnection, network peer
rediscovery, backlog processing and treasury bridge resume.

☐ Check degraded conditions: low disk, inaccessible wallet, network
disconnection, RPC timeout, peer disagreement and delayed maturity.

☐ Run a sustained production-like soak (normally 24–48 hours minimum,
longer where coin rotation or maturation requires it).

☐ Record resource metrics, logs, tip advancement, accepted blocks,
wallet/treasury reconciliation, restarts and alerts.

☐ Prove rollback/restore using tested backups; document preconditions,
expected downtime and irreversible migration risks.

# 9. Golden Standard release gates

| **Gate**              | **Objective**                                              | **Mandatory evidence**                                          |
|-----------------------|------------------------------------------------------------|-----------------------------------------------------------------|
| G0: baseline          | Freeze legacy reference and recovery position.             | Hashes, machine inventory, backups restored in a test.          |
| G1: consensus         | Historical chain and monetary rules are compatible.        | Replay/fixtures, vectors, boundary tests, independent checks.   |
| G2: wallet            | Legacy keys, addresses, signing and balances remain valid. | Import/restart/sign/funded spend and balance reconciliation.    |
| G3: network           | New PoW blocks are accepted outside the candidate node.    | Independent node tip/hash agreement, accepted block proof.      |
| G4: economics         | Rewards are correctly credited, mature and spend.          | Coinbase trail, maturity, fee and supply accounting.            |
| G5: dependent systems | Treasury integration works where applicable.               | Block→wallet→bridge→ledger→UI reconciliation.                   |
| G6: recovery          | Restarts, crashes and restore preserve state.              | Recovery logs and wallet/chain checksums.                       |
| G7: operations        | Runs reliably across meaningful natural cycles.            | 24–48h+ soak; rotation/maturity periods covered.                |
| G8: release           | Independent review and operator approval.                  | Signed gate register, known limitations, release/rollback plan. |

Gate policy: all mandatory gates must PASS, or have a narrowly justified
NOT APPLICABLE determination. An UNKNOWN or FAIL blocks release. A fix
reopens every gate affected by its code, data, protocol or deployment
changes.

# 10. Project evidence register — 18 November 2026

This register incorporates previous conversation/checkpoint reports.
“Checkpoint-reported” is not equivalent to a fresh inspection of
original build logs, peer logs, chain databases or binaries. Reconcile
original artefacts before final approval.

## 10.1 TAO Revival

| **Evidence / baseline**   | **Status at handbook baseline**                                                                                                                                    |
|---------------------------|--------------------------------------------------------------------------------------------------------------------------------------------------------------------|
| Modernisation source      | Bitcoin Core 31.1 pinned to upstream commit 9be056a8…; experimental migration/worktree and production-mining guard described in checkpoints.                       |
| Authentic chain           | Genesis 41de2a4b…; legacy revival 637-block fixture and Scrypt PoW/header checks checkpoint-reported PASS.                                                         |
| Economics                 | Reported 50 TAO initial subsidy, 20-block maturity, 210,000 halving interval; sampled subsidy tests PASS; full boundary replay not yet demonstrated.               |
| Wallet and addresses      | Patch 19 legacy address/signing, two graceful shutdown/key persistence, fail-closed change-format policy checkpoint-reported PASS; funded spend not tested.        |
| Current experimental work | 18-patch baseline commit 64e30c9e…; CP71Y-AZ regtest routing work isolated; CP71Y-BA compilation/runtime checks pending.                                           |
| Release status            | BLOCKED: complete historical replay, independent accepted PoW blocks, funded transaction lifecycle, wallet credit/maturity, upgrade/recovery and soak outstanding. |

TAO-specific invariants to audit against authoritative legacy data:
Scrypt PoW rather than SHA256d PoW; SHA256 block identifiers;
legacy-only receive/change formats; historical activation conditions;
5-minute nominal spacing and 2-block retarget described in legacy audit.
Do not treat these historical parameters as validated solely because
written here.

## 10.2 XNUVA Core

| **Evidence / baseline** | **Status at handbook baseline**                                                                                                         |
|-------------------------|-----------------------------------------------------------------------------------------------------------------------------------------|
| Local mining            | RandomX RPC/mining setup and single-block local test checkpoint-reported PASS; a previously mined block 5795 / 25 XNUVA reported.       |
| Startup                 | Approximately 23-minute startup reported; RandomX light verification/block-index loading suspected, not established root cause.         |
| Production reliability  | Repeatable mining, independently confirmed acceptance, normal restart and sustained operation NOT VERIFIED.                             |
| Wallet experience       | Residual Bitcoin-branded warnings and UI strings are confirmed operator concern; audit/remediation required, without consensus changes. |
| Release status          | BLOCKED: startup/restart/mining/credit integration and sustained reliability not qualified.                                             |

## 10.3 ERA

| **Evidence / baseline** | **Status at handbook baseline**                                                                             |
|-------------------------|-------------------------------------------------------------------------------------------------------------|
| Mining                  | Development-machine mining activity reported; that observation alone is not proof of network acceptance.    |
| Wallet accounting       | Wallet failed to reflect expected mined reward in operator observations; root cause not isolated.           |
| Network/credit          | Peer acceptance, chain tip, coinbase maturity and address ownership not jointly verified.                   |
| Release status          | BLOCKED: instrument candidate→submit→peer tip→wallet maturity; no assertion that network is the sole fault. |

## 10.4 ZUS and HOPE

| **Evidence / baseline** | **Status at handbook baseline**                                                                                                                                                              |
|-------------------------|----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------|
| Miner patch             | Original miner source SHA-256 baseline verified at CP4; isolated patch for timestamp coinbase and legacy empty block-signature serialization.                                                |
| Deployment              | CP63 checkpoint reported patched binaries installed with backups; TAO slot active at observed rotation handover.                                                                             |
| Live result             | No independently confirmed accepted ZUS/HOPE game mining blocks or new treasury credits as of baseline.                                                                                      |
| Rotation and maturity   | Rotation ~50h/coin and 120 confirmations reported for game pipeline; must verify per-coin rules and actual intervals.                                                                        |
| Supply policy           | Previously flagged prospective cap enforcement gaps: ZUS 40,000,000; HOPE 110,000,000. Verify authoritative current rules and implement only via explicitly gated forward consensus changes. |
| Release status          | BLOCKED: miner interoperability, independent acceptance, credit/maturity, treasury propagation and 24–48h+ meaningful soak unqualified.                                                      |

# 11. Failure investigation decision tree

## A. Miner reports work but no rewards

- Is a candidate hash present? → inspect miner/job creation.

- Does submitblock report acceptance? → inspect consensus rejection
  reason and serialization.

- Does a second peer adopt the hash? → inspect propagation/fork/network
  identity.

- Is the reward in a wallet-owned coinbase? → inspect payout address and
  rescan.

- Has maturity depth elapsed? → test immature/mature categories.

- Is a dependent treasury still unchanged? → trace bridge, database and
  UI.

## B. Wallet startup excessively slow

- Timestamp each phase: process start, datadir lock, LoadBlockIndex,
  validation, wallet load, RPC ready, GUI ready.

- Measure block count/verification cost, RAM/IO and RandomX
  dataset/cache mode on test clone.

- Compare same DB after clean and interrupted shutdown; isolate
  reindex/verifydb behaviour.

- Optimise only after preserving consensus verification; never bypass
  validation for speed.

## C. Modern wallet rejects legacy transactions

- Inspect address prefix/network and scriptPubKey before changing UI
  defaults.

- Inspect signing flag, prevout amount/script, sighash rules and
  serialization.

- Check output/change policy, transaction acceptance and peer script
  rule at real height.

- Reconcile balances and test independently accepted funded spend.

# 12. Standard checkpoint procedure

☐ A. State the ONE hypothesis and immutable production boundary; define
expected PASS/FAIL outcomes.

☐ B. Freeze baseline: git status/commit, binary hashes, backed-up
datadir and chain tip.

☐ C. Create isolated branch/worktree with unique network and datadir;
guard mainnet mining.

☐ D. Execute narrow read-only diagnostics first; retain
stdout/stderr/exit code, command, timestamps and version.

☐ E. Apply smallest patch with reviewed diff; compile with warnings
captured.

☐ F. Run unit, negative, historical and independent multi-node tests as
applicable.

☐ G. Compare hashes, balances, blocks, keys and logs before/after,
including two clean restarts.

☐ H. Mark exactly what is VERIFIED, REPORTED, UNVERIFIED or FAILED;
attach artifacts.

☐ I. Open unresolved issues, blockers and any regression gates; do not
label release ready.

☐ J. Commit only validated source/evidence with reversible steps and an
explicit checkpoint label.

# 13. Required evidence bundle and templates

Every release candidate must have an immutable evidence directory, e.g.
evidence/\<coin\>/\<version\>/\<gate\>/\<UTC-timestamp\>/ (structure
illustrative). No passwords, wallet seeds or signing keys in evidence.

| **Artifact**          | **Required contents**                                                                                                |
|-----------------------|----------------------------------------------------------------------------------------------------------------------|
| manifest.txt          | Coin, test scope, network, host, UTC time, source commit, binary SHA-256, toolchain, datadir class, operator.        |
| baseline.json         | Genesis/tip hash/height, consensus parameter hashes, software version, ports, masked paths, wallet test identifiers. |
| commands.log          | Commands executed, exit code, exact output, timestamps and expected versus actual assertions.                        |
| chain-and-wallet.csv  | Test block IDs, peer tip, confirmations, coinbase ownership, immature/mature, spend TX IDs and reconciled balances.  |
| integration-trace.csv | Miner slot, block, daemon, wallet, bridge event, ledger event, UI observation and timestamps.                        |
| recovery-report.md    | Backup verification, restart/crash test, restore procedure, outcomes and residual risks.                             |
| gate-signoff.md       | PASS/FAIL/UNKNOWN/N/A for G0–G8; reviewer, exceptions, release decision and rollback.                                |

# 14. Release decision form

Coin/build: \_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_ Network:
\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_ Source commit:
\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_

Candidate binary SHA-256:
\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_

Chain reference genesis/tip:
\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_

G0 \_\_\_ G1 \_\_\_ G2 \_\_\_ G3 \_\_\_ G4 \_\_\_ G5 \_\_\_ G6 \_\_\_ G7
\_\_\_ G8 \_\_\_ (PASS / FAIL / UNKNOWN / N/A)

Critical unresolved defects:
\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_

Independent reviewer: \_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_ Operator
approval: \_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_ UTC date:
\_\_\_\_\_\_\_\_\_\_

Decision: ☐ HOLD / DO NOT RELEASE ☐ APPROVED FOR CONTROLLED RELEASE

Release authority may only check APPROVED when all applicable gates are
PASS (or documented N/A) and rollback evidence exists.

# 15. Next actions by engineering priority

☐ TAO: CP71Y-BB and CP71Y-BC runtime safety checks passed within their tested scope; CP71Y-BD through CP71Y-BG generated and checked a separate candidate regtest genesis. Continue source-review and fail-closed regtest implementation; preserve the mainnet production guard; complete historical replay and funded end-to-end multi-node mining/spend before any release.

☐ XNUVA: instrument slow cold-start and repeatable mining; isolate
acceptance, maturity, wallet balance and game bridge separately; clean
remaining Bitcoin strings only in a bounded UI/branding pass.

☐ ERA: collect the mined candidate/block hash and independent peer tip,
then trace coinbase ownership, confirmation depth and wallet
rescan/accounting.

☐ ZUS/HOPE: validate deployed miner hashes, observe a real eligible
rotation and accepted block, await documented maturity, and reconcile
ledger/treasury; audit monetary cap activation separately.

☐ Across all: deploy only one risk-bounded change at a time, preserve
chain history, re-run affected gates and keep all five release-blocked
until proved otherwise.

# 16. Handbook governance

Version changes to this handbook require a change-log entry with date,
reason, affected checkpoints, evidence and sign-off. Engineering lessons
must be generalised into reusable tests, not converted into assumptions
that other coins share identical consensus rules. A stronger future gate
supersedes a weaker historic checkpoint. This standard applies to core
nodes, miners, wallets, bridge services, game treasuries and production
packaging.

## 16.1 TAO regtest engineering lessons — 18 November 2026 (draft addendum)

**Evidence provenance:** checkpoint output supplied by the operator in the TAO modernisation session. This addendum records narrow checkpoint-reported observations; it does not independently inspect the development host or qualify a release.

| Checkpoint | Observed result | Reusable engineering requirement |
|---|---|---|
| CP71Y-BA | Experimental daemon/CLI compiled; an attempted `-regtest` startup failed at argument parsing rather than reaching the intended guard. | Confirm actual registered CLI network selectors; do not mistake an unrelated early failure for the desired fail-closed condition. |
| CP71Y-BB | `-chain=regtest` reached the undefined-regtest constructor and exited with the intended error. | Test guards by asserting the *specific* rejection path, not simply a nonzero exit code. |
| CP71Y-BC | An isolated mainnet-format daemon identified the original TAO genesis, explicitly rejected an RPC block-creation request, remained at height zero and shut down cleanly. | Run real binary/RPC guard tests with disposable datadirs and isolated networking; record exact tested entry points. This does **not** prove all mining paths are blocked. |
| CP71Y-BD / BE | Separate regtest genesis candidate generated and reparsed in Python; 80-byte header, transaction serialization, Merkle root, double-SHA256 block ID, Scrypt 1024/1/1 PoW and target were checked. | Separate the block-identity hash from the PoW hash and verify raw bytes and compact target independently. |
| CP71Y-BF / BG | TAO cryptographic interfaces and built libraries identified; standalone C++ program linked against TAO's libraries reproduced candidate block ID, Merkle root and Scrypt PoW, with mutation controls. | Cross-check test vectors in distinct implementations and perform negative mutation tests; library checks do not substitute for complete node consensus validation. |
| CP71Y-BH | First constructor patch-generator attempt stopped with a mainnet-class pattern mismatch before applying code. | Hash-gate patches and require exact source preconditions; stop rather than force a mismatched patch. |
| CP71Y-BI / BJ | Read-only source audit identified the actual mainnet class, genesis API, difficulty functions, and still-disabled mining guard; corrected class matcher confirmed. | Inspect real source interfaces before generating consensus-sensitive changes; preserve mainnet constructor byte-for-byte. |
| CP71Y-BK | Initial BK generator stopped on duplicate `CMainParams` matches; BL later corrected it and generated a hash-verified constructor patch. | Target full unique declarations rather than ambiguous class names; retain failure logs and precondition checks. |

**Candidate only (not activated):** proposed separate genesis block ID `d99e3c5c076af8ba636d3d284382e2c9dab0f0f8916f30c0ce87d6c8b7d3041e`. Native cryptographic matching is not a substitute for acceptance by the full node or by independent peers.

**Still release-blocked:** historical chain replay, complete consensus verification, real-network block acceptance, funded wallet credit/maturity/spend, recovery and sustained operation. The draft regtest must have separate genesis, datadir, network magic, ports, and address/key identity. Never reuse Bitcoin regtest magic or silently enable TAO mainnet mining.

**Handbook rule:** every new verified lesson should generate a dated, evidence-scoped addendum and, when technically relevant, a regression test. Discussion alone does not update GitHub; a traceable Git commit and review are required. The PR remains draft until reviewed.

### TAO CP71Y-BL–BN compact-target arithmetic — 18 November 2026

- **CP71Y-BL (checkpoint-reported PASS):** the corrected generator produced a 132-line regtest constructor patch (`SHA-256 1bf803db2e0b7a9ef553a8a2fc8c99dc69ea5ba8183b266975ca0b27a43f0a21`), proved the existing mainnet class was byte-for-byte unchanged, and verified patch applicability. The candidate remained unapplied.
- **CP71Y-BM (review blocked by test defect):** the disposable static review passed structural and network-identity assertions, then failed because the checker incorrectly required the decoded genesis compact target to *equal* the full-precision PoW limit. This failure did not establish a consensus defect.
- **CP71Y-BN (checkpoint-reported PASS):** independently checked `0x207fffff` decoding, compact round-trip, positive target at/below the full PoW limit, and the saved Scrypt proof against that decoded target. Verified `target = 7fffff0000...0000` and `limit = 7fffff...ffff`; truncation of low-order precision makes strict equality wrong. Production code, authoritative Git and historical fixture remained unchanged.
- **Required regression tests:** separately verify compact encoding, compact decoding, rounding, sign and overflow flags, target `> 0`, target `<= powLimit`, hash `<= target`, and difficulty transition boundaries; exercise actual C++ `DeriveTarget`/`CheckProofOfWork` and negative vectors before full validation. Never conflate a static-review PASS with real-node acceptance.
- **Current state:** CP71Y-BO corrected static review and CP71Y-BP experimental application are checkpoint-reported PASS; CP71Y-BQ daemon/CLI compiled successfully. The regtest constructor has not yet been runtime-tested. Regtest mining remains disabled, and release gates remain blocked.

### TAO CP71Y-BO–BQ constructor and build evidence — 18 November 2026

**Scope and provenance:** operator-supplied terminal output; checkpoint-reported, not independently reproduced on the development machine.

- **BO — corrected static review PASS:** reviewed the BL patch on a disposable copy; compared mainnet class byte-for-byte; checked proposed network identity and compact target arithmetic. Inherited activation and economic rules, global collisions and full validation remain unqualified.
- **BP — isolated source application PASS:** reviewed patch applied only in the separate experimental source workspace; backup SHA-256 `370981a719218c826c469ff78bdc53a19656729715b42e934e71454c326483c5`; modified `src/kernel/chainparams.cpp` SHA-256 `0b21d025bfbef3e795fb75f87448d6ef75c537b4b2beff60b8050c83e9f003cc`. Authoritative Git, historical chain fixture and mainnet block-production guard checked unchanged.
- **BQ — isolated build PASS:** compiled experimental daemon SHA-256 `aa6dec6396ff48770f9961fc9f2a93d5197fb0fd6b9d55422439d18e7ab4c69e` and CLI SHA-256 `817195d1459a96a553fcd296889477c2bab68b12314d1d688444c6d3ebdd8a9e`. Compilation did not start a daemon, prove genesis acceptance, or enable any mining.
- **Next runtime gate BR (unverified):** start only within disposable isolated regtest datadir, confirm genesis/hash, C++ serialized transaction and network identity, ensure test mining stays disabled, independently repeat the mainnet mining-denial test with the BQ build, and verify clean restart before further modifications.
- **Regression requirement:** preserve a separately hash-checked rollback copy before applying experimental consensus patches. Treat binary SHA fingerprints and source tree state as part of every runtime evidence bundle. A compile PASS has no implication of release readiness.

## 16.2 Production continuity and maintenance-first policy — 18 November 2026

> **Never destabilise a functioning production network merely to accelerate its modernisation.**

This rule governs decisions to modernise a live cryptocurrency, node, miner,
wallet, or treasury dependency. New software is not necessarily a safer
production choice merely because its underlying upstream version is newer.

**Mandatory operational controls:**

- **Keep functioning networks online.** Establish and record the live
  baseline (binary hash, chain tip, peer agreement, wallet/treasury state,
  mining and restart health). Treat an absence of reported faults as an
  observation, not as proof that every component is healthy.
- **Separate maintenance from modernisation.** Repair verified faults and
  address material security risks with the smallest justified changes.
  Do not introduce consensus, database, wallet or network upgrades simply
  to meet a modernisation schedule.
- **Isolate experimental work.** Use separate branches, binaries, datadirs,
  networks and keys. Never deploy a test build onto a functioning production
  network to speed up a migration.
- **Make cutover evidence-based.** Require affected G0–G8 release gates,
  independent peer/block acceptance, wallet balances and spendability,
  treasury reconciliation where applicable, tested backup/rollback and
  sustained operation. An unqualified candidate stays off production.
- **Prefer reversible, staged rollout.** Document an operator-approved
  change window, monitoring thresholds, stopping conditions and recovery
  procedure. Keep a working legacy baseline until the replacement is
  proven and the operator authorises migration.
- **Protect against avoidable disruption.** If modernisation introduces
  uncertainty or threatens a functioning network, pause the migration
  without pausing routine maintenance or urgently needed security fixes.

**Current planning application (operator direction, not a production
qualification):** TAO's legacy Revival network is reported operational;
continue its Bitcoin Core modernisation gradually and in isolation,
currently one dedicated day per week, without setting an artificial
cutover deadline. Maintain ZUS and HOPE while separately verifying the
unresolved live-mining/game-treasury credit path. Prioritise confirmed
ERA and XNUVA defects as targeted maintenance; do not infer that all
five networks need wholesale upgrades.

**Evidence and approval:** This addendum records the engineering policy
agreed on 18 November 2026. It is **not** evidence that a particular
network or build passed an operational gate. The handbook remains in a
draft pull request pending review and sign-off; no coin software,
configuration, wallet, blockchain, or running node is changed by this
documentation update.

## 16.3 Optional build dependencies and reproducible build qualification — 19 November 2026

**Cross-project lesson (TAO Revival / XNUVA Core):** Modern Bitcoin Core-derived projects may enable optional multiprocess/interprocess communication (IPC) facilities requiring Cap'n Proto. A missing optional development dependency is a **configuration blocker**, not evidence that PoW, historical chain data or consensus code has failed.

**XNUVA checkpoint 47V — operator-supplied evidence:** An isolated experimental CMake configuration, with `BUILD_BENCH=ON`, `BUILD_TESTS=ON` and `RelWithDebInfo`, stopped at `src/ipc/libmultiprocess/CMakeLists.txt:18` reporting that Cap'n Proto was not found. The CMake diagnostic explicitly suggested `-DENABLE_IPC=OFF` as an alternative. This configuration has **not yet been shown to complete successfully**; no benchmark compilation, RandomX performance improvement or release qualification is implied. A similar dependency obstacle was reported during TAO modernisation, but this entry does not assert an independently reproduced TAO build result.

**Mandatory build-qualification checklist:**

- ☐ Identify whether each missing dependency serves consensus verification, wallet safety, networking, operational requirements or an optional build feature. Do not classify a missing library as a consensus defect without evidence.
- ☐ Where the feature is genuinely unnecessary for the scoped **isolated experiment**, prefer an explicit documented feature toggle over opportunistic package installation or consensus-code modification. For the current XNUVA benchmark candidate, investigate `-DENABLE_IPC=OFF`; do not assume that this choice suits every production deployment.
- ☐ Never disable chain validation, signature checking, PoW, wallet security, or other required functionality as a shortcut to satisfy a build.
- ☐ Record the exact CMake command, source commit/worktree, compiler and library versions, feature flags, configuration output, executable hashes and subsequent test outcomes. Treat `BUILD_BENCH` and `BUILD_TESTS` as explicit, reproducible settings.
- ☐ If configuration fails, preserve the diagnostic and inspect/reconfigure **only the isolated build directory**. Do not delete, overwrite or repurpose a qualified historical build as a workaround.
- ☐ After a configuration change, verify that expected targets exist, compile, and pass the affected tests; confirm the disabled feature is not required for intended RPC, wallet, network or deployment functions. Recheck all affected release gates before any production change.

**Evidence state:** XNUVA missing-dependency diagnostic OBSERVED; IPC-disabled reconfiguration UNVERIFIED pending output. This is a handbook/process update only. No XNUVA or TAO binaries, blockchains, wallets, consensus rules or production deployments are changed by this entry.

## 16.4 Emergency repair and accelerated implementation — 19 November 2026

> **Reliability first. Emergency repairs without unnecessary delay. Release as soon as essential safety is demonstrated.**

**Purpose and authority.** A confirmed defect harming end users must not wait for unrelated modernisation milestones or every unrelated test. Emergency classification expedites repair, focused verification, approval and deployment; it does not convert a FAIL or UNVERIFIED into a PASS. The responsible operator records severity, accepts the scoped residual risk and authorises deployment or containment.

| Priority | Triggers | Response |
|---|---|---|
| **P0 — Critical** | Active risk of funds/keys loss or theft, exploited security flaw, chain corruption or network-wide consensus failure | Immediate containment and user warning as needed; urgent minimal repair; essential safety tests; operator-approved emergency rollout. If no safe binary exists, containment comes first. |
| **P1 — Urgent** | Wallet fails to start/recover, mining or block-processing failure, major operational/user-facing defect | Prioritise targeted repair, fast focused regression testing, backups and reversible monitored deployment ahead of routine roadmap work. |
| **P2 — Routine** | Cosmetic branding, elective optimisation, ordinary UI improvement or planned modernisation without material harm | Standard review and qualification; reclassify if user impact warrants. |

**Mandatory emergency minimum-safety gate (record PASS / FAIL / UNVERIFIED / N/A and evidence):**

1. **Triage/contain.** Identify affected users, versions, chains and failure mode. Distinguish observation from verified root cause; for P0, disable affected operations or warn users if safer than distributing unverified code.
2. **Preserve recovery.** Record running binary/hash, chain height, wallet state and relevant config. Before any destructive reinstall or datadir change, independently verify recoverable, secure backups of all wallet databases/files (including descriptor wallets where used), keys and recovery-critical settings. Never commit secrets.
3. **Minimise the change.** Use a traceable hotfix commit and reproducible toolchain. Keep unrelated branding, performance, wallet, database and consensus changes separate unless a combined change is justified and separately tested.
4. **Test affected invariants.** Compile and run focused tests for the repair plus every safety-critical area it touches: chain and historical-block compatibility, PoW/consensus and signature checks, wallet loading and balances, signing/spendability, peer/block acceptance, mining/reward maturity and treasury integration as applicable. Never turn off required security/consensus checks to make a test pass. A failing *unrelated* test may be deferred only with documented evidence that it is unrelated; do not silently ignore it.
5. **Deploy with containment.** Use smallest practical rollout (isolated environment, test machine or limited cohort before public distribution), clear stopping conditions and operator approval. Preserve prior binary and data. Prove rollback is feasible; database migrations and already-accepted consensus changes may make binary rollback impossible. If so, document an alternate recovery strategy before deployment.
6. **Verify the repair in operation.** Check actual startup, wallet access, chain tip/peer agreement, balances and critical functions, clean shutdown/restart, independent network acceptance where applicable, and relevant user reports. Monitor and pause/roll back if regressions appear.
7. **Finish the evidence.** Log incident priority, symptoms, root-cause confidence, commit/build hashes, scoped test results, deferred tests, decision authority, rollout size, outcome and monitoring. Complete remaining regression, recovery and sustained-operation checks before claiming *fully qualified general release*.

**Explicit emergency exception and limit.** P0/P1 may receive a narrowly scoped **provisional emergency deployment** based on the essential applicable evidence, without waiting an arbitrary 24–48 hours before protecting users. This is not full release qualification. A failed safety-critical gate blocks deployment of that candidate; use containment and a safer repair instead. Distinguish an operator-approved development-machine deployment from an end-user release. Where unresolved issues remain, communicate them plainly.

**XNUVA context (operator-supplied evidence, not an approved release):** experimental security-warning branding branch `ui/xnuva-core-rebranding-20261119` was pushed at commit `6d1cc05a2a`; GUI compilation and standalone translation lookup passed. CP51X GUI testing reported an RPC assertion failure and a wallet-test timeout. RandomX startup acceleration is independently experimental. The combined build, relevance of those failures, backup recovery and end-user release are not yet qualified. Accelerated work may continue without treating these open gates as passed.

**Governance.** Keep incident-level decisions and verification in traceable issues, PRs or checkpoint logs. Update the living handbook with new verified lessons. This draft policy changes no production binary, configuration, wallet, blockchain or network.

## 16.5 Safe wallet discovery, upgrade and recovery protocol — 19 November 2026

> **An installation or upgrade must never make an existing wallet appear lost merely because the application opened a different or empty wallet. Protect the user's keys and funds before changing files.**

**Purpose.** A wallet database can be present and valid on disk without being loaded by the GUI. A newly created empty wallet, an undiscovered wallet directory, or opening the wrong named wallet can display a zero balance even though the user's original wallet remains intact. This is a serious end-user safety and trust failure. Wallet-file existence, successful database opening, transaction history, active-chain recognition and spendable balance are separate facts and must not be conflated.

**Mandatory safe-upgrade requirements:**

1. **Discover before creating.** At startup and during installation/upgrade, inspect the configured wallet directory and supported wallet locations. Detect existing wallet databases and named wallet directories before offering or creating a new wallet. If existing wallet data is found but cannot be loaded or enumerated, stop the automatic first-run wallet-creation path and explain the recovery/open-existing-wallet action. Never silently substitute a new empty wallet.
2. **Offer the existing wallet explicitly.** Present discovered wallet names/locations in the GUI or installer and let the user select the intended wallet. Distinguish the chain data directory from the wallet directory and show the effective paths. Support the project's documented wallet-directory layout; do not assume that copying a `wallet.dat` file into an arbitrary folder automatically registers or loads it.
3. **Preserve before changing.** Before replacing binaries, changing wallet paths, migrating wallet formats, rescanning, reindexing, restoring, or removing an installation, create a separate backup of every existing wallet database and required recovery material. Do not overwrite the only copy. Prefer a cleanly shut down wallet or the application's supported backup mechanism so the copy is transactionally consistent; record the source path, timestamp and SHA-256. Protect encrypted wallet files and recovery material as secrets.
4. **Verify backups and restores.** Check that each backup exists, has a plausible nonzero size, matches its recorded hash after copying, and passes the wallet/database integrity procedure supported by that wallet implementation. A hash proves byte identity, not semantic validity or recoverability. Periodically rehearse restoration on a separate disposable datadir and confirm the wallet opens.
5. **Never overwrite on name collision.** If a target wallet name or `wallet.dat` already exists, abort and ask the user to choose a distinct recovery location or explicitly follow a documented replacement procedure after verified backup. No installer, migration or GUI convenience action may silently overwrite, delete, rename or reset wallet data.
6. **Make recovery non-destructive.** Provide a clear “Open/Restore existing wallet” path. Restore to a new, separate wallet directory by default; preserve the source and all backups. Explain that a raw wallet database is generally opened/restored from its supported directory layout, not imported as a single key or transaction file. Never ask users to create a replacement wallet as a remedy for a missing balance.
7. **Verify the selected wallet and funds.** After opening/restoring, show the selected wallet name and path. Reconcile known addresses, transaction count/history and available, pending, immature and total balances against a trusted pre-upgrade record where available. Check that relevant transactions' blocks belong to the active chain; a wallet record alone does not prove an accepted or spendable coin. Do not label a balance “lost” or “restored” without evidence.
8. **Fail safely and explain next steps.** If discovery, opening, migration or integrity checks fail, preserve all files, report the exact wallet path and actionable error, and provide recovery instructions. Do not automatically delete, recreate, rescan destructively, or overwrite the wallet. Keep the previous executable and data available for rollback, while recognising that a wallet schema migration may require a separately tested recovery path.
9. **Separate application and user data.** Install/upgrade executables independently of wallet databases and blockchain data. Package managers and uninstallers must not remove user data by default. Document data-directory and wallet-directory paths for each supported operating system, and make backup retention and restore procedures understandable to non-experts.
10. **Test the end-user journey.** On a clean test machine or disposable profile, test upgrade over an existing installation with one or more named wallets; discovery of nested wallet directories; first launch when an existing wallet is present; explicit selection of the correct wallet; recovery from a separately verified backup; name collision; inaccessible/corrupt wallet; interrupted upgrade; rollback; and preservation of balances/history after restart. Include a regression test proving that an existing wallet is never silently replaced by an empty one.

**Release gate (wallet safety):** A fresh install, upgrade or recovery release is BLOCKED unless the supported end-user path reliably discovers/offers existing wallets, preserves the original files, opens the intended wallet, and demonstrates balance/history continuity on a representative test wallet. Any inability to discover or open an existing wallet must fail closed with recovery guidance; it must not fall through to creating an empty wallet. Record PASS / FAIL / UNVERIFIED and the tested OS, source commit, binary hash, data paths and evidence. This gate supplements G2 (wallet) and G6 (recovery); it does not replace signing, spendability, active-chain, maturity or independent block-acceptance checks.

**XNUVA incident lesson — 19 November 2026 (operator-supplied evidence):** During a fresh GitHub `main` installation, the GUI first opened `Dev Wallet`, which displayed a much smaller balance than expected. A separately preserved backup was then opened as `Recovered Wallet`, and the GUI displayed 128,650 XNUV total (126,300 available; 2,350 immature) in the supplied screenshot. The debug log showed `Dev Wallet` loading and 5,145 wallet transactions, but did not show `Recovered Wallet` being loaded at that earlier point. This demonstrates the practical risk of opening the wrong wallet and the importance of discovery, explicit wallet selection and independent backup preservation. It does **not** establish that the earlier wallet was corrupt, that any coins were lost, or that the displayed balances/immature rewards have passed active-chain and spendability qualification. Preserve both wallet databases; verify the chain and reward status separately.

**Governance:** This is a mandatory product and release-engineering requirement for XNUVA, ERA, TAO, ZUS, HOPE and future projects using this handbook. Implementation may differ by wallet architecture, but no project may silently create an empty wallet in place of an existing one. Documentation alone does not implement the control; the GUI/installer behaviour and regression evidence must be supplied before the relevant release gate passes.

## 16.6 Legacy node build compatibility and VPS deployment evidence — 20 November 2026

**Scope:** operator-supplied Contabo VPS 2 checkpoints 28–52 for TAO Revival, Zeus (ZUS), and Hopium (HOPE), on Ubuntu 24.04 (GCC 13, modern Boost). This entry distinguishes confirmed build/runtime observations from unresolved end-to-end qualification. **Infrastructure deployment is not coin modernisation or a general software release.** Do not modify live production chains or wallet data to resolve an isolated VPS build failure.

### Observed builds and minimally scoped local compatibility repairs

| Project and source | Observed failure or outcome | Local response and evidence state | Deferred upstream work |
|---|---|---|---|
| **TAO Revival**, official legacy source commit `4c0049cc4d4937fd69a09bb06bf68e4757fa0aaf` | Headless legacy binary built without source patches; P2P connected to Dell T620. Contabo accepted blocks through height **1,233**, canonical Revival block 1 `717fc5ef97ded9f7d514fcb1ed558bc76596ce85f09890efa6e668d228c4a57f`. | Observed network handshake, block-acceptance logs and auto-start `tao-revival` enabled/active. Not evidence of reboot recovery, sustained operation or a later matching tip. | Continue routine monitoring and qualification. Do not substitute experimental Bitcoin Core 31.1 TAO modernisation binaries for the existing Revival chain. |
| **Zeus**, `main` commit `a3a541bc30866610723395e83d6f7cff79391fbf` | Boost Signals2 in `src/init.cpp`: `NotifyBlockTip.disconnect(BlockNotifyGenesisWait)` caused an invalid `static_cast` on Ubuntu 24.04 Boost. First attempt put connection handle inside an `if` block and failed variable-scope compilation. | Locally retained a `boost::signals2::connection` in the enclosing scope, assigned the result of `connect()`, and called `genesisWaitConnection.disconnect()`; **wallet-enabled headless build succeeded**. Additional local `src/rpc/rawtransaction.cpp` `#include "util.h"` resolved undeclared `GetArg`. Installed `zeusd` SHA-256 `ea28bed4fff0c4a00eaba292501f4b5539e043d1ebe5bd0ae15df87189ec0897`. | Review/verify permanent code changes independently, with focused tests. Do not promote the unreviewed VPS diff to a source release. |
| **Zeus wallet-disabled trial** | `--disable-wallet` exposed unguarded `pwalletMain` and `CReserveKey` usage in `src/rpc/mining.cpp`; after locally guarding the RPC paths, linking still failed on `CWallet::CreateCoinStake`, `HaveAvailableCoinsForStaking`, and `CReserveKey` methods referenced by consensus/miner-related objects. | Wallet-disabled approach was **abandoned**, not declared fixed. Restored `src/rpc/mining.cpp`, reconfigured `--with-incompatible-bdb` with available Berkeley DB **5.3**, and built wallet-enabled; runtime config uses `disablewallet=1` and `staking=0`. | Investigate true wallet-free node support separately; never strip staking/consensus validation to obtain a green build. Berkeley DB 5.3 build is not evidence of compatibility with production wallets that expect 6.2. |
| **Hopium**, branch `hope-repair-final-20260904` commit `7b97b7a1b56107dbbde44e68feb1b9b9586b2e93` | Same Boost Signals2 disconnect error in `src/init.cpp`. Autotools preparation changed generated tracked files (`configure`, `aclocal.m4`, `Makefile.in`); this is distinct from hand-edited consensus code. | Same *outer-scope connection-handle* local correction compiled successfully with wallet support. Installed `hopiumd` SHA-256 `8869e04db31980e5956bdba0b8ffd48148ff8c7b01fa1e4d4411663f95e673ce`; `hopium-cli` SHA-256 `4f1d29a0df3ea3a21d5c26112796b2b00ee7483f5c6f732bcc5c423c1a72f1af`. | Review and test a separate permanent Boost compatibility repair later; avoid committing regenerated build-system files accidentally. |

**Other observed warnings:** Zeus compilation warned about potentially dangling `find_value(...)` references in `rpc/mining.cpp` and possible uninitialised `prevector` data under GCC 13. These warnings were **not** the demonstrated build blockers and were **not** resolved. Triage them later; do not mark them harmless without analysis.

### Operational evidence and unresolved gates

- **TAO Revival:** `tao-revival` enabled and active via systemd, with a successful established P2P handshake and acceptance of the canonical history through block 1,233 in the supplied logs. Source-genesis and block-1 validation are observed. No actual reboot/recovery test or extended continuity evidence was supplied.
- **ZUS:** `zeus-node` enabled and active; RPC bound to `127.0.0.1:22395`, P2P listening on `22396` (IPv4/IPv6). Source-expected genesis `000007a830306520fa2efc02258706ec6bac5514f6396b9313df676ac498e8ee` confirmed locally. At supplied checkpoints, validated chain rose from **6,704** to **16,145**, with **22,955** headers reported earlier. **Still syncing;** no independently verified historical-height hash comparison with the T620 or sustained-operation qualification.
- **HOPE:** `hopium-node` enabled and active; RPC bound to `127.0.0.1:22535`, P2P listening on `22536` (IPv4/IPv6). Source-expected genesis `00000270333dc42277a321c31bf0107115d8d6b551279e4c13c187a36ec6e133` confirmed locally. Connected to peer `81.130.208.151:22536`; verified inbound block messages and in-flight block requests. At checkpoint 52 **6,064** validated blocks versus **145,157** headers: **synchronisation underway, not complete**. `verificationprogress: 1` does not supersede the header/block-height discrepancy.
- **All three:** systemd returned **enabled** and **active** for `tao-revival`, `zeus-node` and `hopium-node` (checkpoint 51). This demonstrates startup configuration and process state, **not** tested reboot recovery or guaranteed external reachability; confirm firewall/provider controls and peer availability separately. No production wallets or private keys were imported; ZUS/HOPE disabled wallet loading and staking at runtime.

**Maintenance/release rule:** Preserve exact source commits, local patch/diff and backup, build flags, linker/compiler versions, executable hashes, P2P/RPC bindings, relevant logs, chain anchors and outstanding gates. Keep RPC localhost-only and run daemon services unprivileged. Any future permanent fix to Zeus or Hopium belongs on its own reviewable maintenance branch, with reproducible build and regression verification covering startup, P2P, historical-chain compatibility, PoS/PoW validation, wallet operations where applicable, and restart recovery. **Do not infer full release readiness from a passing compile, active service, local genesis, or incomplete sync.**

**Status:** Contabo node build/installation/startup observations **VERIFIED FROM OPERATOR-SUPPLIED LOGS**. Permanent upstream source fixes **DEFERRED / NOT COMMITTED**. Full synchronization, independent historical-chain comparison, reboot recovery and sustained operation **UNVERIFIED**. This handbook entry changes documentation only; it does not alter running nodes, coin source repositories, wallets, balances or chain state.

## 16.6A RandomX-first modular Proof-of-Work policy (21 November 2026)

**Policy decision / engineering direction — NOT a consensus activation or release approval.** Before resuming TAO Revival's broader modernisation and CP66 networking qualification, establish and integrate a reusable, separately versioned RandomX Proof-of-Work foundation **in an isolated experimental build**. Resume staged TAO acceptance testing after the new integration is independently verified. Never deploy the experimental build on live nodes or bypass outstanding P2P checks.

**Authoritative shared engineering reference:** [Xnuva-RandomX-Engine](https://github.com/Xnuva-Blockchain-Project/Xnuva-RandomX-Engine), especially [modular architecture](https://github.com/Xnuva-Blockchain-Project/Xnuva-RandomX-Engine/blob/main/docs/MODULAR-RANDOMX-ARCHITECTURE.md), [coin integration gates](https://github.com/Xnuva-Blockchain-Project/Xnuva-RandomX-Engine/blob/main/docs/COIN-INTEGRATION.md), and the README. This handbook adds governance and sequencing; it **does not supersede** those detailed technical specifications.

### Mandatory design and transition controls

- [ ] Develop **one pinned, build-time C++ RandomX engine dependency** with clearly defined hash interfaces and independently pinned source version/commit; do not use floating upstream branches, unversioned libraries or live-swappable executable plugins. Preserve licence and source provenance.
- [ ] Implement **five isolated coin consensus adapters**: TAO Revival, ERA, ZUS, HOPE and XNUVA. Each adapter controls its own historical algorithm, block-header serialization, block ID versus PoW hash byte ordering, nBits/target comparison, difficulty, seed derivation, chain-context validation and future activation logic. A shared hash library must **not** decide coin-specific consensus.
- [ ] Recognise that existing XNUVA uses a **RandomX v2** variant with `RANDOMX_FLAG_V2`; generic upstream RandomX v1 must not silently replace it. XNUVA's work is a separate **bit-for-bit, no-fork compatibility refactor** until independently proven equivalent; no second algorithm transition should be inferred.
- [ ] **TAO first:** review the existing experimental Bitcoin Core 31.1 worktree and original Scrypt rules; introduce RandomX only behind explicit future-height activation controls on disposable test networks. Preserve and replay all pre-activation Scrypt blocks **unchanged**, including genesis, block IDs and historical difficulty.
- [ ] Apply the same forward-only historical-preservation requirements to ERA, ZUS and HOPE. For ZUS and HOPE, retain independently validated **PoS blocks and staking consensus**, not just their PoW path; do not force legacy PoS history through RandomX.
- [ ] No activation height, live hard fork, upgraded-node cutover or economic/retargeting change is approved by this policy. A future production transition needs a documented network-upgrade plan, sufficient participant readiness, interoperability tests, risk/rollback limits and explicit operator sign-off. Historical funds, legacy addresses, wallets and treasury balances must remain valid.
- [ ] Use independent reference vectors for the **exact pinned RandomX variant**, canonical seed/epoch boundary tests, invalid target/header tests, light-versus-fast-mode parity, memory exhaustion/resource constraints, multithreading, deterministic cross-platform hashes and performance on representative hardware.
- [ ] Qualify each adapter independently: clean compile; isolated historical replay across the activation boundary; old/new validation behaviour; real independent peer/block acceptance; mining and miner-to-node interoperability; coinbase/stake rewards, maturity, accounting and spendability; treasury reconciliation where applicable; reorg; restart/recovery; and sustained monitored operation. Retest all affected gates when the shared module version changes.
- [ ] Treat a successful CMake configure, source commit, fixture PASS or local block hash as **narrow evidence only**. The shared repository currently describes itself as **EXPERIMENTAL / DO NOT RELEASE OR DEPLOY** and identifies compilation, CTest, independent vector validation and per-coin integration as outstanding. Do not mark these completed without evidence.
- [ ] Preserve T620 and Contabo live infrastructure and current legacy chains, wallets, miners and balances. All initial RandomX changes are documentation/development only. Continue any remaining emergency-repair verification independently; it is not waived by a changed modernisation priority.

**Implementation order:** shared RandomX engine source/test qualification → experimental TAO adapter and pre-/post-activation replay → resume TAO CP66 networking and end-to-end qualification → independent ERA/ZUS/HOPE adoption → XNUVA bit-for-bit modular refactor, each gated separately. The precise sequence after TAO may be reprioritised for maintenance needs but no release gates may be skipped.

**Evidence status (21 November 2026):** policy **APPROVED BY OPERATOR**; shared foundation repository and design documents **OBSERVED**; complete independent shared-engine testing, TAO integration, per-coin fork specification and any live activation **UNVERIFIED / NOT AUTHORISED**.

## 16.7 Revision history

| Version | Date | Reason and scope | Evidence / approval |
|---|---|---|---|
| 1.0 | 18 November 2026 | Initial comprehensive Golden Standard baseline and TAO checkpoint lessons | Draft handbook, PR #1; release evidence as marked in section 10 |
| 1.1 (draft) | 18 November 2026 | Add mandatory production-continuity rule, maintenance-first decision controls, and isolation/cutover safeguards | Operator-directed policy update; Git commit in PR #1; review/sign-off pending |
| 1.2 (draft) | 19 November 2026 | Document optional Cap'n Proto / IPC dependency triage, reproducible feature flags and isolated build-config recovery | XNUVA CP47V observed configuration blocker; IPC-disabled recovery not yet verified; draft PR #1 |
| 1.3 (draft) | 19 November 2026 | Add P0/P1 emergency repair classification, fast-track testing, provisional rollout, recovery and rollback controls | Operator-agreed policy; CP51X failures unresolved; draft PR #1 pending review |
| 1.4 (draft) | 19 November 2026 | Add mandatory Safe Wallet Discovery, Upgrade and Recovery Protocol; prevent silent empty-wallet substitution; require verified backups, explicit wallet selection and balance/history continuity | XNUVA fresh-install incident and operator-supplied recovery screenshot/log; wallet-discovery and release regression requirements added; implementation qualification remains outstanding |
| 1.5 (draft) | 20 November 2026 | Record Contabo VPS 2 TAO/ZUS/HOPE operational evidence, legacy Boost build compatibility, wallet-disabled Zeus build limitations and deferred source-repair qualification | Operator-supplied checkpoints 28–52; nodes enabled/active, ZUS/HOPE still synchronising; upstream repairs and full qualification outstanding |
| 1.6 (draft) | 21 November 2026 | RandomX-first modular PoW policy for TAO, ERA, ZUS, HOPE and XNUVA; forward-only consensus controls and shared-engine verification gates | Operator-approved sequencing; experimental shared engine exists, integration and deployment not qualified |

## Standing rule: NO PASS without scope. NO RELEASE without evidence. NO LOSS OF HISTORY, KEYS OR FUNDS.

Document ends — v1.6 draft, preserving the earlier baselines. Operational proof must be supplied by
live logs and independent nodes; a handbook cannot itself validate a
running cryptocurrency.