# Xnuva Core Documentation

This directory contains build, operation, protocol and developer documentation for **Xnuva Core (XNUV)**.

Xnuva Core is derived from the Bitcoin Core codebase, so a number of lower-level documents retain upstream terminology where the implementation remains inherited. Those documents should be read as technical implementation references for Xnuva Core; they do **not** mean that Xnuva is the Bitcoin network.

## Mainnet quick reference

- Proof of Work: RandomX v2 from block 1
- Block identity: SHA256d
- Target spacing: 120 seconds
- Difficulty adjustment: ASERT
- ASERT half-life: 43,200 seconds
- P2P port: 29444
- RPC port: 29544
- Bech32 HRP: `xnuva`
- Configuration file: `xnuva.conf`
- Default data directory: `.xnuva`

At launch, Xnuva has no DNS or fixed seed nodes. A fresh node therefore needs a known peer, for example:

```ini
addnode=<node-ip>:29444
```

## Running Xnuva Core

Depending on how Xnuva Core was built, the principal executables are:

- `xnuva` — graphical client where GUI support is enabled
- `xnuvad` — headless full node
- `xnuva-cli` — RPC command-line client
- `xnuva-wallet`
- `xnuva-tx`
- `xnuva-util`

## Building

The following build notes originate from the upstream codebase and remain useful while Xnuva-specific packaging is being completed:

- [Dependencies](dependencies.md)
- [macOS Build Notes](build-osx.md)
- [Unix Build Notes](build-unix.md)
- [Windows Build Notes](build-windows-msvc.md)
- [FreeBSD Build Notes](build-freebsd.md)
- [OpenBSD Build Notes](build-openbsd.md)
- [NetBSD Build Notes](build-netbsd.md)

Some commands, binary names or external links inside individual inherited documents may still reference Bitcoin Core and are being reviewed progressively. The root [Xnuva README](/README.md) and Xnuva launch documents are authoritative for Xnuva-specific identity and network parameters.

## Development references

- [Developer Notes](developer-notes.md)
- [Productivity Notes](productivity.md)
- [Release Process](release-process.md)
- [Translation Process](translation_process.md)
- [Translation Strings Policy](translation_strings_policy.md)
- [JSON-RPC Interface](JSON-RPC-interface.md)
- [Unauthenticated REST Interface](REST-interface.md)
- [BIPs inherited by the codebase](bips.md)
- [DNS seed policy](dnsseed-policy.md)
- [Benchmarking](benchmarking.md)
- [Internal Design Docs](design/)

## Node and wallet references

- [Configuration File Reference](bitcoin-conf.md)
- [Files](files.md)
- [Managing Wallets](managing-wallets.md)
- [Multisig Tutorial](multisig-tutorial.md)
- [Offline Signing Tutorial](offline-signing-tutorial.md)
- [PSBT Support](psbt.md)
- [Reduce Memory](reduce-memory.md)
- [Reduce Traffic](reduce-traffic.md)
- [Tor Support](tor.md)
- [Transaction Relay Policy](policy/README.md)
- [ZMQ](zmq.md)

The file name `bitcoin-conf.md` is inherited upstream; Xnuva's actual configuration file is `xnuva.conf`.

## Mainnet launch records

- [Launch State](/XNUVA-LAUNCH-STATE.md)
- [Mainnet Launch](/XNUVA-MAINNET-LAUNCH.md)
- [Post-Launch Development](/XNUVA-POST-LAUNCH.md)

## License

Distributed under the [MIT software license](/COPYING), retaining applicable upstream copyright and attribution notices.
