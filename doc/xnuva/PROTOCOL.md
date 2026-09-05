# Xnuva Protocol Constitution

Status: Pre-genesis implementation baseline
Network name: Xnuva
Ticker: XNUV
Native unit: XNUV
Atomic precision: 8 decimal places

This document defines the intended Xnuva blockchain design before
mainnet genesis. Mainnet does not yet exist.

Consensus parameters may only be changed during pre-genesis engineering
when validation proves that a parameter is unsafe or technically
unsuitable. Once the mainnet genesis block is released, consensus
changes require explicit versioned network-upgrade procedures.

## 1. Design principles

Xnuva is an independent Layer-1 blockchain.

The base chain is intended to provide:

- fair proof-of-work issuance
- secure ownership
- permissionless transfer
- transparent monetary policy
- simple and auditable consensus
- long-term maintainability

The base chain will not launch with:

- proof of stake
- masternodes
- founder rewards
- developer taxation
- ICO allocation
- privileged staking allocation
- NFTs as a consensus feature
- smart-contract virtual machines
- on-chain governance
- transaction taxation

## 2. Upstream engineering baseline

Xnuva begins from:

Bitcoin Core v31.1

Preserved upstream commit:

9be056a8a72b624dae9623b2f7bded92c2a21c91

Xnuva modifications must remain identifiable and auditable against this
parent.

## 3. Ledger model

Ledger model:

UTXO

Transaction and script architecture should remain as close to modern
Bitcoin Core behaviour as practical unless Xnuva requires an explicit
consensus difference.

## 4. Proof of work

Consensus:

Pure Proof of Work

Proof-of-work algorithm:

RandomX

Reference implementation target:

RandomX v2.0.1 or a subsequently audited compatible implementation
selected before mainnet genesis.

Block target spacing:

120 seconds

RandomX blockchain key schedule target:

2048 blocks per key

Key-selection delay:

64 blocks

The RandomX implementation must pass deterministic test vectors and
cross-platform verification before genesis is permitted.

## 5. Difficulty adjustment

Difficulty algorithm:

ASERT

Difficulty evaluation:

Every block

Target spacing:

120 seconds

Initial design half-life:

12 hours

The ASERT implementation must use deterministic consensus-safe integer
arithmetic.

Its response must be simulation-tested against:

- rapid hashrate increase
- rapid hashrate decrease
- prolonged low hashrate
- alternating hashrate
- timestamp variation
- boundary and overflow conditions

before mainnet parameters are frozen.

## 6. Monetary policy

Initial block subsidy:

25 XNUV

Subsidy-halving interval:

1,000,000 blocks

Target time between halvings:

approximately 3.8 years at 120-second target spacing

Maximum monetary ceiling:

50,000,000 XNUV

Premine:

0 XNUV

Founder allocation:

0 XNUV

Developer allocation:

0 XNUV

ICO allocation:

0 XNUV

Tail emission:

None

Coinbase maturity target:

100 blocks

Because subsidy values are represented using integer atomic units,
successive halvings eventually truncate fractions below one atomic
unit.

With:

25 XNUV initial subsidy
1,000,000 blocks per subsidy era
100,000,000 atomic units per XNUV

the expected maximum mined issuance from the halving schedule is:

49,999,999.89 XNUV

This remains below the 50,000,000 XNUV monetary ceiling.

Both subsidy calculation and monetary-range checking must independently
prevent issuance above the permitted monetary policy.

## 7. Genesis rules

The Xnuva mainnet genesis block does not yet exist.

Genesis will be created only after:

- consensus implementation is complete
- RandomX validation passes
- difficulty validation passes
- subsidy-boundary tests pass
- regtest mining passes
- multi-node testnet operation passes

Genesis must provide no spendable premine.

Planned genesis statement:

"Xnuva begins: an open chain, fair issuance, no privileged allocation."

The final genesis timestamp, nonce, difficulty, merkle root and block
hash must be generated and independently reproduced at release time.

## 8. Script capabilities

Xnuva is intended to launch with modern script functionality rather than
reproduce historical activation delays.

The implementation target is activation from genesis for the mature
Bitcoin script features required for:

- BIP34-style block heights
- BIP65
- BIP66
- CSV
- SegWit
- Taproot

Exact activation representation must be verified against the Bitcoin
Core 31.1 implementation before consensus parameters are committed.

## 9. Address identity

Primary human-readable network name:

Xnuva

Bech32 human-readable prefix target:

xnuva

Expected native SegWit presentation:

xnuva1q...

Expected Taproot presentation:

xnuva1p...

Legacy Base58 addresses should use a visually identifiable X-prefixed
range where technically possible without ambiguity.

All Base58 version bytes and extended-key prefixes must be calculated
and tested rather than selected solely by appearance.

## 10. Software identity

Target executable names:

xnuvad
xnuva-cli
xnuva-qt
xnuva-wallet
xnuva-tx
xnuva-util

Target configuration file:

xnuva.conf

Target URI scheme:

xnuva:

Target default data directory:

.xnuva on Unix-like systems

Platform-specific paths will follow native operating-system conventions.

## 11. Network identity

Preliminary mainnet operational ports:

P2P: 38444
RPC: 38445

These ports are not consensus parameters and must pass collision and
deployment review before release.

Mainnet, testnet and regtest must each have:

- independent genesis blocks
- independent message-start bytes
- independent address identities where appropriate
- independent ports
- independent chain state

Bitcoin DNS seeds and Bitcoin fixed seeds must never remain active on
an Xnuva network.

No third-party legacy coin-builder infrastructure may be inherited.

## 12. Supply integrity

Every spendable XNUV must ultimately derive from valid proof-of-work
coinbase issuance or a valid transfer of previously issued XNUV.

There will be no hidden issuance mechanism.

Consensus tests must verify:

- first subsidy
- halving boundary minus one block
- every halving boundary
- final non-zero subsidy
- first zero-subsidy block
- cumulative issuance
- invalid overpaying coinbase rejection
- arithmetic overflow protection

## 13. Pre-genesis release rule

Mainnet genesis is forbidden until the following minimum condition has
been demonstrated:

Two independently running Xnuva nodes must successfully:

1. start from clean data directories
2. connect to each other
3. mine RandomX blocks
4. agree on chain tip
5. mature coinbase outputs
6. create wallet addresses
7. transfer XNUV between wallets
8. restart without corruption
9. reindex successfully
10. reject deliberately invalid blocks

Until that gate is passed, every Xnuva chain is disposable development
or test infrastructure.

## 14. Mainnet immutability principle

After public mainnet genesis:

- no re-genesis
- no silent chain reset
- no arbitrary historical rewrite
- no undocumented monetary change

Any future consensus upgrade must be explicit, reviewable, versioned and
deployed with adequate notice.
