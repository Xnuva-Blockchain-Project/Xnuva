# Golden Standard Cryptocurrency Modernisation Handbook

> **Reliability first, release second.**

> Public engineering baseline; historical observations are not release approvals. Never commit private keys, credentials, unredacted personal data or sensitive infrastructure details.

**Document ID:** GS-CM-001 \| Version: 1.0 \| Baseline: 18 November 2026

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
| CP71Y-BK | Corrected regtest constructor patch proposed, **not yet evidenced as generated, applied or compiled**. | Review all network identity choices, magic collisions, genesis assertions, activation rules, address prefixes and regtest mining boundaries before implementation. |

**Candidate only (not activated):** proposed separate genesis block ID `d99e3c5c076af8ba636d3d284382e2c9dab0f0f8916f30c0ce87d6c8b7d3041e`. Native cryptographic matching is not a substitute for acceptance by the full node or by independent peers.

**Still release-blocked:** historical chain replay, complete consensus verification, real-network block acceptance, funded wallet credit/maturity/spend, recovery and sustained operation. The draft regtest must have separate genesis, datadir, network magic, ports, and address/key identity. Never reuse Bitcoin regtest magic or silently enable TAO mainnet mining.

**Handbook rule:** every new verified lesson should generate a dated, evidence-scoped addendum and, when technically relevant, a regression test. Discussion alone does not update GitHub; a traceable Git commit and review are required. The PR remains draft until reviewed.

## Standing rule: NO PASS without scope. NO RELEASE without evidence. NO LOSS OF HISTORY, KEYS OR FUNDS.

Document ends — v1.0 baseline. Operational proof must be supplied by
live logs and independent nodes; a handbook cannot itself validate a
running cryptocurrency.