# Xnuva chain-identity security contract

This document defines the fail-closed mainnet block-production requirement introduced after the Europa chain-loss investigation.

## Purpose

Prevent a node that has lost, reset, replaced, truncated or otherwise loaded the wrong blockchain state from mining a valid-looking alternative Xnuva history from the same genesis.

Xnuva is Proof of Work only. Every mainnet block-production surface must therefore be unable to create a candidate unless the active chain is proven to be the canonical Xnuva chain.

## Qualified mainnet identity

The workstation live node independently qualified the following recognised Xnuva chain values on 12 September 2026:

- Genesis: `0000410149e74469ea857100515f786d4bff7fdaf05863ca1b0948cf8f049f9f`
- Canonical block 1: `c1430034fe105fae719ae896847b26a95b3e896973502ff1cdc5efaac93988a2`
- Buried anchor height: **88**
- Buried anchor hash: `5e4a861d0237fccca85b9a1673c5429c436ae8f3bc453864d9dc335e9f1070cd`
- Minimum chainwork at height 88: `00000000000000000000000000000000000000000000000000000000000b2000`

The qualification node was on mainnet height 175, had the exact permanent genesis and block 1, exact height-88 anchor, IBD false, active networking and three peer connections. Its tip chainwork was `0000000000000000000000000000000000000000000000000000000000160000`.

## Mandatory mainnet production guard

The central Xnuva block-building path must fail closed unless all of the following are true:

1. the active chain contains the canonical block 1;
2. the active chain has reached height 88;
3. the active-chain block at height 88 exactly equals the qualified anchor hash;
4. the active tip chainwork is at least the qualified minimum chainwork;
5. the node is not in initial block download / synchronisation state;
6. P2P networking is active and the local production interface has at least one peer.

The immutable identity/work checks belong in the central `BlockAssembler` path so a different local template consumer cannot bypass them. The mining/template interface additionally enforces sync and peer state and rechecks long-lived template/submission paths.

Testnet/regtest remain usable for testing and are not pinned to Xnuva mainnet history.

## Mainnet chain parameters

The hardening must set:

- `nMinimumChainWork` to the qualified height-88 chainwork;
- `defaultAssumeValid` to the qualified height-88 anchor.

These values strengthen normal modern chain selection/validation but do not replace the explicit block-production identity guard.

## Qualification before merge

Negative test: on a disposable wrong/truncated chain, block-template or built-in mining must be refused and chain height/tip must remain unchanged.

Positive test: on a disposable copy of the genuine Xnuva chain with a peer and IBD false, a valid next-block template must be created on the recognised tip without doing PoW or submitting a block.

Only after both tests pass should the hardening commit be considered for `main`.
