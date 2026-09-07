# XNUVA Mainnet Launch

<p align="center">
  <img src="doc/assets/xnuva-coin.png" alt="Xnuva XNUV coin" width="160">
</p>

XNUV mainnet was operationally confirmed on 2026-09-06T21:28:19+01:00.

## Immutable Launch Source

Launch tag:

`xnuva-mainnet-genesis-20260906`

Launch source commit:

`0d669d42e9810e0f5681d8b315a2bb1711265eb2`

The immutable tag remains permanently attached to the exact source that created and validated block 1.

## Permanent Genesis

- Hash: `0000410149e74469ea857100515f786d4bff7fdaf05863ca1b0948cf8f049f9f`
- Merkle root: `fb923316be9e4f5db3467d8c2d159bb3a27fa9ba2c71703c078a0ea7ccb676e8`
- Timestamp: 1788717675
- Nonce: 9105
- nBits: 0x1f07ffff
- Subsidy: 0 XNUV

## Block 1

- Height: 1
- Hash: `c1430034fe105fae719ae896847b26a95b3e896973502ff1cdc5efaac93988a2`
- Mining address: `xnuva1qzq64qkew65lsyq332jxt8emcn87h60prz0z5xl`
- Launch qualification mining time: 56 seconds
- Subsidy: 25 XNUV
- Proof of Work: RandomX v2
- Difficulty adjustment: ASERT
- Target spacing: 120 seconds

## Initial Mainnet Node

- Main P2P port: 29444
- Automatic local onion listener: 29445
- Main RPC port: 29544
- Datadir: `/home/meadcrown/XNUVA-Blockchain/mainnet-live`
- Height at operational confirmation: 1

## Post-Launch Port Correction

The original RPC assignment used P2P+1.

The automatic local onion-service listener reserves P2P+1, so the RPC family was moved to separate 295xx ports in the first post-launch operational commit:

`77be8629432d4d41fa397ab8cf8552ae4e6a3309`

This does not alter consensus, genesis, block 1 or the immutable launch tag.

## Official artwork

The XNUV coin artwork used for Xnuva project branding is stored at [doc/assets/xnuva-coin.png](doc/assets/xnuva-coin.png).

## Development Rule

The genesis and immutable launch tag must never be rewritten.

All later development proceeds through ordinary forward commits on GitHub `main`.
