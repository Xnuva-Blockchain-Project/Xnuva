# Prince — XNUV Atomic Unit

**Prince** is the official name of the smallest indivisible denomination of **Xnuva (XNUV)**.

## Definition

- **1 Prince = 0.00000001 XNUV**
- **100,000,000 Prince = 1 XNUV**
- XNUV retains exactly **8 decimal places**.

The unit name is invariant in normal usage: for example, `1 Prince`, `2 Prince`, and `250 Prince`.

## Memorial

The name **Prince** was adopted on **8 September 2026** in memory of **Prince**.

The intention is for Prince to remain a permanent part of Xnuva terminology: the smallest unit of XNUV carries his name.

## Technical status

This naming does **not** create a new asset, alter the blockchain, or change monetary precision.

Internally, XNUV amounts are already represented as integer atomic units. Naming one atomic unit **Prince** therefore leaves all existing technical and consensus properties unchanged:

- no genesis change;
- no block 1 change;
- no chain rewrite or re-genesis;
- no balance conversion or migration;
- no supply change;
- no reward change;
- no `MAX_MONEY` change;
- no serialized amount change;
- no consensus-rule change.

Wallet and user-interface code may display the atomic unit as **Prince** while retaining the existing underlying integer amount representation.
