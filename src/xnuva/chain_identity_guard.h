#ifndef XNUVA_CHAIN_IDENTITY_GUARD_H
#define XNUVA_CHAIN_IDENTITY_GUARD_H

#include <arith_uint256.h>
#include <chain.h>
#include <net.h>
#include <node/context.h>
#include <uint256.h>
#include <validation.h>
#include <xnuva/chain_identity_security.h>

#include <stdexcept>
#include <vector>

namespace xnuva::security {

inline void EnforceMainnetBlockProductionGuard(node::NodeContext& node, ChainstateManager& chainman)
{
    if (chainman.GetParams().IsTestChain()) return;

    static_assert(HasQualifiedChainIdentity());

    if (chainman.IsInitialBlockDownload()) {
        throw std::runtime_error("XNUVA MINING SAFETY: node is still synchronising; block production refused");
    }

    if (!node.connman || !node.connman->GetNetworkActive()) {
        throw std::runtime_error("XNUVA MINING SAFETY: P2P networking is inactive; block production refused");
    }

    std::vector<CNodeStats> peer_stats;
    node.connman->GetNodeStats(peer_stats);
    if (peer_stats.empty()) {
        throw std::runtime_error("XNUVA MINING SAFETY: no peer connections; block production refused");
    }

    static constexpr uint256 EXPECTED_BLOCK1{MAINNET_BLOCK1};
    static constexpr uint256 EXPECTED_ANCHOR{MAINNET_BURIED_ANCHOR};
    static constexpr uint256 MINIMUM_CHAINWORK_BLOB{MAINNET_MINIMUM_CHAINWORK};
    const arith_uint256 minimum_chainwork{UintToArith256(MINIMUM_CHAINWORK_BLOB)};

    LOCK(::cs_main);

    const CChain& active_chain{chainman.ActiveChain()};
    const CBlockIndex* tip{active_chain.Tip()};

    if (tip == nullptr) {
        throw std::runtime_error("XNUVA MINING SAFETY: active chain has no tip; block production refused");
    }

    if (active_chain.Height() < MAINNET_BURIED_ANCHOR_HEIGHT) {
        throw std::runtime_error("XNUVA MINING SAFETY: canonical anchor height 88 has not been reached; block production refused");
    }

    const CBlockIndex* block1{active_chain[1]};
    if (block1 == nullptr || block1->GetBlockHash() != EXPECTED_BLOCK1) {
        throw std::runtime_error("XNUVA MINING SAFETY: canonical block-1 fingerprint mismatch; block production refused");
    }

    const CBlockIndex* anchor{active_chain[MAINNET_BURIED_ANCHOR_HEIGHT]};
    if (anchor == nullptr || anchor->GetBlockHash() != EXPECTED_ANCHOR) {
        throw std::runtime_error("XNUVA MINING SAFETY: canonical height-88 anchor mismatch; block production refused");
    }

    if (tip->nChainWork < minimum_chainwork) {
        throw std::runtime_error("XNUVA MINING SAFETY: active chainwork is below the qualified minimum; block production refused");
    }
}

} // namespace xnuva::security

#endif // XNUVA_CHAIN_IDENTITY_GUARD_H
