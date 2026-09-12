#ifndef XNUVA_CHAIN_IDENTITY_SECURITY_H
#define XNUVA_CHAIN_IDENTITY_SECURITY_H

#include <string_view>

namespace xnuva::security {

inline constexpr std::string_view MAINNET_GENESIS =
    "0000410149e74469ea857100515f786d4bff7fdaf05863ca1b0948cf8f049f9f";

inline constexpr std::string_view MAINNET_BLOCK1 =
    "c1430034fe105fae719ae896847b26a95b3e896973502ff1cdc5efaac93988a2";

// Populate only from an independently verified, well-buried block on the
// genuine live Xnuva chain. Deliberately unset until qualification.
inline constexpr int MAINNET_BURIED_ANCHOR_HEIGHT = -1;
inline constexpr std::string_view MAINNET_BURIED_ANCHOR = "";
inline constexpr std::string_view MAINNET_MINIMUM_CHAINWORK = "";

inline constexpr bool REQUIRE_NOT_IBD = true;
inline constexpr bool REQUIRE_PEER = true;
inline constexpr bool PROTECT_POW = true;
inline constexpr bool PROTECT_POS = false;
inline constexpr bool CHAIN_IDENTITY_QUALIFIED = false;

constexpr bool HasQualifiedChainIdentity()
{
    return CHAIN_IDENTITY_QUALIFIED &&
           MAINNET_GENESIS.size() == 64 &&
           MAINNET_BLOCK1.size() == 64 &&
           MAINNET_BURIED_ANCHOR_HEIGHT >= 1 &&
           MAINNET_BURIED_ANCHOR.size() == 64 &&
           MAINNET_MINIMUM_CHAINWORK.size() == 64;
}

} // namespace xnuva::security

#endif // XNUVA_CHAIN_IDENTITY_SECURITY_H
