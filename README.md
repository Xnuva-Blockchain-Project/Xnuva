# Xnuva Core

<p align="center">
  <img src="doc/assets/xnuva-coin.png" alt="Xnuva XNUV coin" width="160">
</p>

**Xnuva (XNUV)** is an independent Layer-1, Bitcoin-style UTXO blockchain using **RandomX v2 Proof of Work** and **ASERT difficulty adjustment**.

Xnuva Core is derived from the Bitcoin Core codebase, but Xnuva is a separate network with its own genesis block, consensus parameters, monetary policy, addresses, ports and chain history.

## Mainnet status

XNUV mainnet launched on **6 September 2026**.

- Genesis hash: `0000410149e74469ea857100515f786d4bff7fdaf05863ca1b0948cf8f049f9f`
- Block 1 hash: `c1430034fe105fae719ae896847b26a95b3e896973502ff1cdc5efaac93988a2`
- Immutable launch tag: `xnuva-mainnet-genesis-20260906`
- Immutable launch-source commit: `0d669d42e9810e0f5681d8b315a2bb1711265eb2`

The launch tag is permanent and must not be moved or rewritten.

## Consensus and monetary policy

| Parameter | Xnuva |
| --- | --- |
| Ticker | XNUV |
| Model | Bitcoin-style UTXO |
| Proof of Work | RandomX v2 |
| Block identity | SHA256d |
| Target block time | 120 seconds |
| Difficulty adjustment | ASERT |
| ASERT half-life | 43,200 seconds |
| Genesis subsidy | 0 XNUV |
| Initial block subsidy | 25 XNUV |
| Halving interval | 1,000,000 reward-bearing blocks |
| Coinbase maturity | 100 blocks |
| Amount precision | 8 decimals |
| Smallest denomination | **Prince** |
| 1 Prince | 0.00000001 XNUV |
| `MAX_MONEY` | 50,000,000 XNUV |
| Premine | None |
| Founder/developer allocation | None |
| ICO | None |
| Tail emission | None |
| SegWit | Enabled |
| Taproot | Enabled |

Genesis statement:

> Xnuva begins: an open chain, fair issuance, no privileged allocation.

## Denominations

The smallest indivisible denomination of XNUV is officially named **Prince**.

- **1 Prince = 0.00000001 XNUV**
- **100,000,000 Prince = 1 XNUV**
- The unit name is invariant in normal usage: for example, `1 Prince` and `250 Prince`.

Prince is named in memory of **Prince the American Akita**. The naming is a terminology and display convention only; it does not change XNUV precision, monetary policy, serialized amounts, balances, or consensus rules.

See [Prince — XNUV Atomic Unit](doc/prince-denomination.md).

## Mainnet network identity

- P2P port: **29444**
- RPC port: **29544**
- Local automatic onion-service listener: **29445**
- Bech32 HRP: **xnuva**
- URI scheme: **xnuva:**
- P2P message magic: `e3 fc a7 92`

At launch there are **no DNS or fixed seed nodes**. Until seed infrastructure is added, additional nodes must connect using a known peer:

```ini
addnode=<node-ip>:29444
```

## Binaries

A normal build produces the Xnuva executables:

- `xnuvad`
- `xnuva-cli`
- `xnuva` (GUI where GUI support is enabled)
- `xnuva-wallet`
- `xnuva-tx`
- `xnuva-util`

The default data directory is `.xnuva` and the primary configuration file is `xnuva.conf`.

## Building from source

Xnuva Core is currently intended primarily for developers and experienced node operators.

Platform-specific build documentation is available in the [doc directory](doc/).

The project is in early mainnet development. Test cleanup, public seed infrastructure, release packaging and easy installers continue as forward Git commits.

## Launch documentation

The exact launch state and immutable chain identity are recorded in:

- [XNUVA-LAUNCH-STATE.md](XNUVA-LAUNCH-STATE.md)
- [XNUVA-MAINNET-LAUNCH.md](XNUVA-MAINNET-LAUNCH.md)
- [XNUVA-POST-LAUNCH.md](XNUVA-POST-LAUNCH.md)

## Branding

The XNUV coin artwork used by the project is stored at [doc/assets/xnuva-coin.png](doc/assets/xnuva-coin.png).

## Development policy

Mainnet genesis and the immutable launch tag are historical network anchors and must never be rewritten.

Normal fixes, documentation changes, packaging work and other development continue as new commits on `main`.

Any future consensus change must be introduced as an explicit coordinated network upgrade rather than by rewriting existing chain history.

## License and upstream attribution

Xnuva Core retains the applicable Bitcoin Core copyright notices and is distributed under the terms of the MIT license. See [COPYING](COPYING).

Bitcoin Core provided the upstream codebase from which Xnuva Core was developed. Xnuva is an independent blockchain and is not the Bitcoin network.
