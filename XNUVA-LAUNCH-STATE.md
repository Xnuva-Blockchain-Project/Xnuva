# XNUVA / XNUV Mainnet Launch State

## Consensus

- Proof of Work: RandomX v2 from block 1
- Block identity: SHA256d
- Target spacing: 120 seconds
- Difficulty adjustment: ASERT
- ASERT half-life: 43,200 seconds
- Genesis subsidy: 0 XNUV
- Initial reward-bearing subsidy: 25 XNUV
- Halving interval: 1,000,000 reward-bearing blocks
- Coinbase maturity: 100
- Premine: none
- Founder/dev allocation: none
- ICO: none
- Tail emission: none
- SegWit: enabled
- Taproot: enabled

## Permanent Genesis

Statement:

> Xnuva begins: an open chain, fair issuance, no privileged allocation.

- Time: 1788717675
- Nonce: 9105
- nBits: 0x1f07ffff
- Hash: 0000410149e74469ea857100515f786d4bff7fdaf05863ca1b0948cf8f049f9f
- Merkle root: fb923316be9e4f5db3467d8c2d159bb3a27fa9ba2c71703c078a0ea7ccb676e8
- Reward: 0 XNUV
- powLimit: 0007ffff00000000000000000000000000000000000000000000000000000000

## Block 1

- Hash: c1430034fe105fae719ae896847b26a95b3e896973502ff1cdc5efaac93988a2
- Mining address: xnuva1qzq64qkew65lsyq332jxt8emcn87h60prz0z5xl
- Mining duration during launch qualification: 56 seconds
- Subsidy policy: 25 XNUV

## Mining implementation

The built-in RPC miner reuses one RandomX v2 VM for a complete nonce search.
Consensus validation remains independent and unchanged.

## Mainnet Identity

- P2P magic: e3 fc a7 92
- P2P port: 29444
- bech32 HRP: xnuva
- PUBKEY_ADDRESS: 75
- SCRIPT_ADDRESS: 76
- SECRET_KEY: 203
- DNS/fixed seeds at launch: none

## Immutable Source

Launch tag:

`xnuva-mainnet-genesis-20260906`

Never amend, move, rebase, delete or rewrite this tag.

Future development proceeds using new Git commits.
