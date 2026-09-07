# Xnuva Core Transaction Relay Policy

This directory documents **Xnuva Core** mempool and transaction-relay policy.

Policy rules are local node rules applied in addition to consensus when evaluating unconfirmed transactions before they enter the mempool or are relayed to peers. They are not consensus rules and are not applied to transactions already contained in blocks.

Much of this policy architecture is inherited from the upstream Bitcoin Core codebase and remains applicable to Xnuva Core unless Xnuva-specific code or documentation states otherwise.

This documentation is not an exhaustive list of all policy rules.

- [Mempool Design and Limits](mempool-design.md)
- [Mempool Replacements](mempool-replacements.md)
- [Packages](packages.md)
