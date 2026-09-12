#!/usr/bin/env python3
"""Integrate the qualified Xnuva mainnet chain-identity guard.

This script is intentionally narrow and fail-closed. It modifies only:
  * src/node/interfaces.cpp
  * src/kernel/chainparams.cpp

It does not build, run, mine, submit blocks, modify a datadir, or update git refs.
"""

from pathlib import Path
import sys

ROOT = Path(__file__).resolve().parents[1]
INTERFACES = ROOT / "src/node/interfaces.cpp"
CHAINPARAMS = ROOT / "src/kernel/chainparams.cpp"
POLICY = ROOT / "src/xnuva/chain_identity_security.h"
GUARD = ROOT / "src/xnuva/chain_identity_guard.h"

EXPECTED_ANCHOR = "5e4a861d0237fccca85b9a1673c5429c436ae8f3bc453864d9dc335e9f1070cd"
EXPECTED_WORK = "00000000000000000000000000000000000000000000000000000000000b2000"


def stop(message: str) -> None:
    print(f"STOP: {message}", file=sys.stderr)
    raise SystemExit(1)


for path in (INTERFACES, CHAINPARAMS, POLICY, GUARD):
    if not path.is_file():
        stop(f"required file missing: {path.relative_to(ROOT)}")

policy_text = POLICY.read_text()
if EXPECTED_ANCHOR not in policy_text or EXPECTED_WORK not in policy_text:
    stop("qualified height-88 Xnuva identity is not present in the policy header")
if "CHAIN_IDENTITY_QUALIFIED = true" not in policy_text:
    stop("Xnuva chain identity policy is not marked qualified")

# ---- Central mining/template interface ----
interfaces = INTERFACES.read_text()

include_old = "#include <validationinterface.h>\n"
include_new = "#include <validationinterface.h>\n#include <xnuva/chain_identity_guard.h>\n"

if interfaces.count("#include <xnuva/chain_identity_guard.h>") == 0:
    if interfaces.count(include_old) != 1:
        stop("expected exactly one validationinterface include insertion point")
    interfaces = interfaces.replace(include_old, include_new, 1)
elif interfaces.count("#include <xnuva/chain_identity_guard.h>") != 1:
    stop("unexpected number of Xnuva guard includes")

method_old = """    std::unique_ptr<BlockTemplate> createNewBlock(const BlockCreateOptions& options, bool cooldown) override
    {
        // Reject too-small values instead of clamping so callers don't silently
"""

method_new = """    std::unique_ptr<BlockTemplate> createNewBlock(const BlockCreateOptions& options, bool cooldown) override
    {
        xnuva::security::EnforceMainnetBlockProductionGuard(m_node, chainman());

        // Reject too-small values instead of clamping so callers don't silently
"""

if interfaces.count("EnforceMainnetBlockProductionGuard(m_node, chainman())") == 0:
    if interfaces.count(method_old) != 1:
        stop("expected exactly one MinerImpl::createNewBlock insertion point")
    interfaces = interfaces.replace(method_old, method_new, 1)
elif interfaces.count("EnforceMainnetBlockProductionGuard(m_node, chainman())") != 1:
    stop("unexpected number of Xnuva block-production guard calls")

# ---- Mainnet chain parameters only ----
chainparams = CHAINPARAMS.read_text()
main_start = chainparams.find("class CMainParams")
test_start = chainparams.find("class CTestNetParams")
if main_start < 0 or test_start < 0 or test_start <= main_start:
    stop("could not isolate Xnuva mainnet chain parameters")

before = chainparams[:main_start]
main = chainparams[main_start:test_start]
after = chainparams[test_start:]

old_pair = """        consensus.nMinimumChainWork = uint256{};
        consensus.defaultAssumeValid = uint256{};
"""
new_pair = f"""        // Qualified Xnuva launch-chain identity at buried height 88.
        consensus.nMinimumChainWork = uint256{{\"{EXPECTED_WORK}\"}};
        consensus.defaultAssumeValid = uint256{{\"{EXPECTED_ANCHOR}\"}};
"""

if EXPECTED_WORK not in main:
    if main.count(old_pair) != 1:
        stop("expected exactly one empty mainnet minimum-work/assumevalid pair")
    main = main.replace(old_pair, new_pair, 1)
else:
    if main.count(EXPECTED_WORK) != 1 or main.count(EXPECTED_ANCHOR) != 1:
        stop("mainnet security values are present in an unexpected form")

chainparams = before + main + after

INTERFACES.write_text(interfaces)
CHAINPARAMS.write_text(chainparams)

print("PASS: Xnuva central block-production guard integrated")
print("PASS: Xnuva mainnet nMinimumChainWork set to qualified height-88 work")
print("PASS: Xnuva mainnet defaultAssumeValid set to qualified height-88 anchor")
print("PASS: test chains left untouched")
print("NEXT: inspect git diff, build, and run negative/positive qualification tests")
