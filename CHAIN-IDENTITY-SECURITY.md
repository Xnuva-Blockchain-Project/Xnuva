# Xnuva chain-identity security contract

This document defines the fail-closed mainnet block-production requirement introduced after the Europa chain-loss investigation.

## Purpose

Prevent a node that has lost, reset, replaced, truncated or otherwise loaded the wrong blockchain state from mining a valid-looking alternative Xnuva history from the same genesis.

Xnuva is Proof of Work only. Every mainnet block-production surface must therefore be unable to create a candidate unless the active chain is proven to be the canonical Xnuva chain.

## Already immutable

- Genesis: `0000410149e74469ea857100515f786d4bff7fdaf05863ca1b0948cf8f049f9f`
- Canonical block 1: `c1430034fe105fae719ae896847b26a95b3e896973502ff1cdc5efaac93988a2`

These values come from the permanent launch record. Block 1 is an early fingerprint, not the final buried production anchor.

## Values still to qualify from the live chain

Before this branch can be considered for merge, record and independently verify:

- a well-buried mainnet anchor height;
- the exact block hash at that height;
- the cumulative chainwork at that height.

Do not guess these values and do not derive them from an isolated or freshly rebuilt node.

## Mandatory mainnet production guard

The central Xnuva block-building path must fail closed unless all of the following are true:

1. the active chain contains the canonical block 1;
2. the active chain has reached the qualified buried anchor height;
3. the active-chain block at that height exactly equals the qualified anchor hash;
4. the active tip chainwork is at least the qualified minimum chainwork;
5. the node is not in initial block download / synchronisation state;
6. the node has at least one peer before a public-network mining RPC/template is authorised.

The chain-identity checks must live at the central block-production layer so `getblocktemplate`, built-in generation and any future mining interface cannot bypass them. Peer-state checks may additionally be enforced at the network/RPC boundary where connection state is available.

Testnet/regtest must remain usable for testing and are not to be pinned to Xnuva mainnet history.

## Qualification before merge

Negative test: on a disposable wrong/truncated chain, block-template or built-in mining must be refused and chain height/tip must remain unchanged.

Positive test: on a disposable copy of the genuine Xnuva chain with a peer and IBD false, a valid next-block template must be created on the recognised tip without doing PoW or submitting a block.

Only after both tests pass should the hardening commit be considered for `main`.
