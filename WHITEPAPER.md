# Xnuva Blockchain Project — Technical White Paper

**Version 1.0 — September 2026**

Official website: https://europazeus.org/  
Source organisation: https://github.com/Xnuva-Blockchain-Project

## Abstract

The Xnuva Blockchain Project maintains five independent open-source blockchain projects: Europa (ERA), Zeus (ZUS), Hopium (HOPE), Tao Coin Revival (TAO) and Xnuva (XNUV).

The project has two related purposes. First, it preserves established blockchain history and wallet continuity where that history survives. Second, it modernises software, build systems, node discovery and distribution so the networks can continue to operate on current systems.

The five currencies do not share one ledger or one token. Each network has its own chain history, consensus rules, addresses, ports, wallets and monetary rules. The common element is a preservation-first engineering policy: public source, identifiable releases, conservative consensus change, verifiable network identity and user control of private keys.

## 1. Engineering principles

The project follows five core principles:

1. **Preserve valid history.** An established chain should not be replaced merely because its software needs maintenance.
2. **Modernise conservatively.** Build-system and compatibility work must not silently change network identity or historical consensus.
3. **Make releases verifiable.** Source commits, tags, checksums and technical records should identify what software users are running.
4. **Keep keys with users.** Ordinary upgrades do not require the project to take custody of wallet secrets.
5. **Separate experimental work from production.** A development branch is not a network rule until a compatible release and activation path are qualified.

## 2. Network overview

| Network | Ticker | Role | Consensus / mining | Continuity policy |
| --- | --- | --- | --- | --- |
| Europa | ERA | Established network | Proof of Work | Preserve existing chain and balances |
| Zeus | ZUS | Established network | Hybrid PoW / PoS | Preserve history; prospective compatible changes |
| Hopium | HOPE | Established network | Hybrid PoW / PoS | Preserve history; qualify consensus changes before activation |
| Tao Coin Revival | TAO | Current Revival network | Scrypt PoW | Preserve original genesis and the verified Revival chain |
| Xnuva | XNUV | Independent Layer-1 | RandomX v2 PoW | Preserve immutable September 2026 mainnet launch history |

No balance on one chain automatically represents a balance on another. The project does not merge the five ledgers.

## 3. Preservation and modernisation

### Europa, Zeus and Hopium

For ERA, ZUS and HOPE, maintenance is intended to continue the established network rather than create a replacement chain.

Normal preservation work therefore avoids re-genesis, historical block rewriting, arbitrary balance migration and silent network-identity changes.

If a consensus correction becomes necessary, the preferred approach is prospective activation. Historical blocks remain valid under the rules that produced them, while a new rule begins at a clearly defined future activation point after operators have had time to upgrade.

This approach reduces the risk that a maintenance release unexpectedly divides miners, stakers, nodes, wallets and services into incompatible chains.

### Tao Coin Revival

Tao requires a separate historical explanation. The maintained TaoCoin Revival network preserves the original TaoCoin genesis block unchanged. The historical post-genesis TaoCoin chain is not currently known to survive publicly in a form that can be independently reconstructed and verified.

The project therefore does not manufacture missing blocks or balances. The current Revival network is anchored by the preserved genesis and its published height-1 checkpoint, which defines the verifiable successor chain used by current TAO software.

### Xnuva

Xnuva is a new independent blockchain rather than a revival. XNUV mainnet launched on 6 September 2026 with its own genesis, network parameters and chain history.

The launch genesis, launch tag and launch-source record are permanent historical anchors. Future development may continue, but established mainnet history is not to be rewritten.

## 4. Wallet continuity

A blockchain balance is represented by spendable outputs recorded on the chain. A wallet holds or derives the private keys needed to authorise spending.

For ERA, ZUS and HOPE, preserving the existing chain means a normal software upgrade does not itself require a new token or balance conversion. When an upgraded wallet has the correct keys and has synchronised or rescanned the correct chain, it should identify the outputs controlled by those keys.

Because the clients originate from different generations of Bitcoin-family software, the exact migration procedure may differ between releases. A release may support direct wallet loading, import, rescan or another documented method.

The project therefore follows a simple migration rule:

**Back up first. Upgrade second. Verify the balance third. Delete nothing until verification is complete.**

Wallet files, private keys, seed phrases and passwords must remain private. They should never be posted to GitHub issues, forums or support chats.

For TAO, this continuity statement applies to balances recorded on the current Tao Revival chain. The project does not claim to recreate unknown balances from unavailable historical post-genesis blocks.

## 5. Europa (ERA)

Europa is an established Proof-of-Work network maintained under a preservation-first policy.

The maintained source retains the identity and continuity of the ERA blockchain while keeping the client buildable and usable on current systems. The mainnet configuration uses Proof of Work with a five-minute target spacing.

The Europa maintenance policy explicitly rejects re-genesis, blockchain reset, historical rewrite and balance migration as ordinary maintenance methods.

Source: https://github.com/Xnuva-Blockchain-Project/Europa

## 6. Zeus (ZUS)

Zeus is an established Bitcoin-family cryptocurrency with hybrid Proof-of-Work and Proof-of-Stake operation.

Maintenance is intended to preserve the existing chain while supporting continued wallet use, mining, staking and node operation.

The maintained source includes prospective enforcement of the published **40,000,000 ZUS** monetary ceiling. The enforcement is prospective so that already accepted historical blocks are not rewritten merely to apply the ceiling retroactively.

Consensus-sensitive releases must be introduced through a clear upgrade and activation process.

Source: https://github.com/Xnuva-Blockchain-Project/Zeus

## 7. Hopium (HOPE)

Hopium is an established Bitcoin-family hybrid network with Proof-of-Work and Proof-of-Stake components and a strong staking heritage.

The maintenance objective is to preserve the live chain and wallet continuity while restoring reliable builds, tests, node discovery and current-platform usability.

Historical project material identifies **110,000,000 HOPE** as the published monetary ceiling. Any stronger consensus enforcement of that ceiling must be implemented prospectively and qualified before activation. Until a specific activation is published, the rules enforced by the software actually adopted by the network remain authoritative.

Source: https://github.com/Xnuva-Blockchain-Project/Hopium

## 8. Tao Coin Revival (TAO)

Tao Coin Revival is the current maintained TAO network. It uses scrypt Proof of Work and preserves the original TaoCoin genesis block.

Legacy source is retained for historical reference, but normal users should use the Revival network. Revival and legacy network identities are deliberately different and do not peer with one another.

The current Revival chain is defined by the preserved genesis and the published Revival height-1 checkpoint.

Source: https://github.com/Xnuva-Blockchain-Project/Tao

## 9. Xnuva (XNUV)

Xnuva is an independent Bitcoin-style UTXO Layer-1 blockchain.

| Parameter | Xnuva mainnet |
| --- | --- |
| Ticker | XNUV |
| Ledger model | Bitcoin-style UTXO |
| Proof of Work | RandomX v2 |
| Block identity | SHA256d |
| Target block time | 120 seconds |
| Difficulty adjustment | ASERT |
| ASERT half-life | 43,200 seconds |
| Genesis subsidy | 0 XNUV |
| Initial block subsidy | 25 XNUV |
| Halving interval | 1,000,000 reward-bearing blocks |
| Coinbase maturity | 100 blocks |
| Precision | 8 decimal places |
| Maximum money constant | 50,000,000 XNUV |
| Premine | None |
| Founder/developer allocation | None |
| ICO | None |
| Tail emission | None |
| SegWit | Enabled |
| Taproot | Enabled |

The smallest denomination is **Prince**:

- 1 Prince = 0.00000001 XNUV
- 100,000,000 Prince = 1 XNUV

The denomination is named in memory of Prince the American Akita. It is a naming convention only and does not alter precision, balances or consensus arithmetic.

The Xnuva launch statement is:

> Xnuva begins: an open chain, fair issuance, no privileged allocation.

Source: https://github.com/Xnuva-Blockchain-Project/Xnuva

## 10. Consensus-change policy

Consensus-sensitive changes can divide a network if nodes disagree about validity. An established-chain consensus change should therefore normally meet the following conditions before production activation:

1. existing chain identity and valid history are preserved;
2. the reason for the change is documented;
3. pre-activation behaviour remains compatible with the established network;
4. activation is explicit and deterministic;
5. source is identifiable by commit and release tag;
6. available tests and manual validation have been completed;
7. operators receive a clear upgrade path;
8. miners, stakers, wallets and services can identify the intended chain.

Experimental modernisation branches are not production consensus. Branch names that identify work as experimental or not for release should be treated literally until a qualified release is published.

## 11. Source provenance and release qualification

Maintained source is published through the Xnuva Blockchain Project GitHub organisation.

Release engineering priorities include preserving upstream licence and copyright notices, retaining useful historical source for provenance, identifying releases by commit and tag, publishing checksums where available, testing wallet and chain compatibility, and keeping experimental code separate from qualified binaries.

For Xnuva, launch-state documents additionally preserve the genesis identity and immutable launch source.

A distributed binary is a convenience representation of a specific source state. Public source remains central to independent verification.

## 12. Node discovery and infrastructure

Older networks can lose peers and seed infrastructure even while their blockchain data remains valid. The project therefore operates public bootstrap nodes and is adding fixed-seed support where appropriate.

Bootstrap nodes help peers find one another. They are not consensus authorities and do not decide which blocks are valid.

Wallet RPC interfaces should not be exposed directly to the public Internet. Public P2P connectivity and private authenticated RPC access are separate security domains.

The long-term infrastructure objective is greater seed and peer diversity so that ordinary network discovery does not depend on one project-operated host.

## 13. Mining and staking

The networks intentionally retain different consensus models:

- Europa — Proof of Work;
- Zeus — hybrid Proof of Work / Proof of Stake;
- Hopium — hybrid Proof of Work / Proof of Stake;
- Tao Revival — scrypt Proof of Work;
- Xnuva — RandomX v2 Proof of Work with ASERT.

For Xnuva, the published Linux mining helper communicates with a running node through RPC and verifies chain identity before mining. It is a convenience tool rather than a separate consensus implementation.

## 14. Distribution and usability

The project aims to support both source-first users and ordinary wallet users.

As of September 2026, Windows x64 releases are published for ERA, ZUS, HOPE and XNUV. TAO Windows packaging and broader macOS / simplified Linux packaging remain part of continuing release work.

Because release status changes, current downloads should always be checked at https://europazeus.org/.

## 15. Development model

The Xnuva Blockchain Project maintains software and infrastructure but does not hold users’ private keys.

The project can publish source, binaries, documentation and recommended upgrades. Network-wide consensus still depends on the compatible software voluntarily run by participating nodes, miners, stakers and services.

The preferred development model is therefore transparent and conservative: public repositories, identifiable commits and tags, documented material changes, preservation of chain history, explicit separation of experimental work, coordinated consensus activation and release-specific wallet guidance.

## 16. Roadmap

Current engineering priorities are:

1. continue qualification and maintenance of ERA, ZUS and HOPE clients without rewriting established histories;
2. strengthen fixed-seed and public-node diversity;
3. complete and qualify Tao Revival packaging for ordinary users;
4. expand cross-platform distribution where practical;
5. keep major modernisation work isolated until compatibility evidence supports release;
6. complete remaining prospective monetary-policy enforcement work through documented activation rather than retroactive history changes;
7. improve wallet-migration testing and documentation;
8. improve release reproducibility, provenance records and checksums;
9. maintain Xnuva’s public launch records while expanding its peer infrastructure;
10. expand documentation that lets users independently verify network identity.

The roadmap is validation-led rather than date-led. Experimental work may be changed or abandoned if it cannot preserve compatibility safely.

## 17. Security considerations

Blockchain software is security-critical. Relevant risks include private-key loss, damaged backups, software defects, dependency vulnerabilities, incompatible forks, low-participation network attacks, malicious third-party binaries, compromised infrastructure and incorrect operator configuration.

Users should verify release sources and checksums, maintain multiple secure wallet backups, keep RPC private and verify a migration before deleting older wallet data.

This document is technical project documentation. It makes no claim about market price, liquidity or future value of any currency described here.

## 18. Authoritative links

- Project website: https://europazeus.org/
- GitHub organisation: https://github.com/Xnuva-Blockchain-Project
- Europa: https://github.com/Xnuva-Blockchain-Project/Europa
- Zeus: https://github.com/Xnuva-Blockchain-Project/Zeus
- Hopium: https://github.com/Xnuva-Blockchain-Project/Hopium
- Tao: https://github.com/Xnuva-Blockchain-Project/Tao
- Xnuva: https://github.com/Xnuva-Blockchain-Project/Xnuva
- Wallet upgrade guide: https://europazeus.org/wallet-upgrade.html

## 19. Document status

This is Version 1.0, published in September 2026.

If this document conflicts with a chain’s actually adopted consensus rules, immutable chain history or a later qualified release notice, the network consensus and qualified release documentation take precedence.

Material revisions to this white paper should be committed publicly so that changes remain visible in Git history.
