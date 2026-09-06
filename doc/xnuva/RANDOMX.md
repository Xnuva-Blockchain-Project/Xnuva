# Xnuva RandomX provenance

Xnuva vendors the official RandomX v2.0.1 source.

Upstream repository:

    https://github.com/tevador/RandomX.git

Qualified upstream identity:

- tag: v2.0.1
- commit: aaafe71322df6602c21a5c72937ac284724ae561
- tree: 57752798bc713766b34b487737b8d9258448814e
- license: BSD-3-Clause
- qualified archive SHA256: 0db31a9783479990f625b25d39286f808216424446c32328cfa578c1daee9470
- qualified full-manifest SHA256: 88bfa1663c5abfc83c6e03708526d1bb549cbe4d9d42a2e36e81879d2a39a240

The upstream Git tree contains 171 tracked files.

The reproducible git-archive export contains 165 files.

Six tracked paths are intentionally omitted by upstream
`.gitattributes` export-ignore rules:

- .gitattributes
- .gitignore
- audits/Report-Kudelski.pdf
- audits/Report-Quarkslab.pdf
- audits/Report-TrailOfBits.pdf
- audits/Report-X41.pdf

All 165 vendored files are verified byte-for-byte against the
qualified full tracked-file manifest.

## Xnuva hash architecture

`CBlockHeader::GetHash()` remains SHA256d and remains the Xnuva
block identifier.

RandomX v2 is a separate proof-of-work digest.

Stage 8D supplies a portable light-mode correctness reference only.

Consensus validation does not yet call RandomX.

Mining does not yet call RandomX.

Block storage does not yet call RandomX.
