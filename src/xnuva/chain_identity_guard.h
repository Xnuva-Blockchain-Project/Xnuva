#ifndef XNUVA_CHAIN_IDENTITY_GUARD_H
#define XNUVA_CHAIN_IDENTITY_GUARD_H

#include <arith_uint256.h>
#include <chain.h>
#include <net.h>
#include <node/context.h>
#include <sync.h>
#include <uint256.h>
#include <validation.h>
#include <xnuva/chain_identity_security.h>

#include <stdexcept>
#include <vector>

namespace xnuva::security {

/**
 * Verify the immutable Xnuva mainnet chain identity while cs_main is held.
 *
 * This is deliberately independent of RPC, miner scripts, wallets and P2P
 * state so it can be called from the central BlockAssembler path. Test chains
 * are unaffected.
 */
inline void EnforceMainnetChainIdentityLocked(ChainstateManager& chainman)
{
    if (chainman.GetParams().IsTestChain()) return;

    static_assert(HasQualifiedChainIdentity());
    AssertLockHeld(::cs_main);

    const uint256 expected_block1{MAINNET_BLOCK1};
    const uint256 expected_anchor{MAINNET_BURIED_ANCHOR};
    const uint256 minimum_chainwork_blob{MAINNET_MINIMUM_CHAINWORK};
    const arith_uint256 minimum_chainwork{UintToArith256(minimum_chainwork_blob)};

    const CChain& active_chain{chainman.ActiveChain()};
    const CBlockIndex* tip{active_chain.Tip()};

    if (tip == nullptr) {
        throw std::runtime_error("XNUVA MINING SAFETY: active chain has no tip; block production refused");
    }

    if (active_chain.Height() < MAINNET_BURIED_ANCHOR_HEIGHT) {
        throw std::runtime_error("XNUVA MINING SAFETY: canonical anchor height 88 has not been reached; block production refused");
    }

    const CBlockIndex* block1{active_chain[1]};
    if (block1 == nullptr || block1->GetBlockHash() != expected_block1) {
        throw std::runtime_error("XNUVA MINING SAFETY: canonical block-1 fingerprint mismatch; block production refused");
    }

    const CBlockIndex* anchor{active_chain[MAINNET_BURIED_ANCHOR_HEIGHT]};
    if (anchor == nullptr || anchor->GetBlockHash() != expected_anchor) {
        throw std::runtime_error("XNUVA MINING SAFETY: canonical height-88 anchor mismatch; block production refused");
    }

    if (tip->nChainWork < minimum_chainwork) {
        throw std::runtime_error("XNUVA MINING SAFETY: active chainwork is below the qualified minimum; block production refused");
    }
}

/**
 * Verify chain identity with its own cs_main lock. This is useful from mining
 * interfaces that do not already hold the chain lock.
 */
inline void EnforceMainnetChainIdentity(ChainstateManager& chainman)
{
    if (chainman.GetParams().IsTestChain()) return;
    LOCK(::cs_main);
    EnforceMainnetChainIdentityLocked(chainman);
}

/**
 * Full production guard for the local mining/template interfaces.
 *
 * In addition to the immutable chain identity, mainnet production requires the
 * node to be synchronised, P2P networking to be active, and at least one peer
 * to be connected. Test chains remain unaffected.
 */
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

    EnforceMainnetChainIdentity(chainman);
}

} // namespace xnuva::security

#endif // XNUVA_CHAIN_IDENTITY_GUARD_H
