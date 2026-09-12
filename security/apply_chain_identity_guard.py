#!/usr/bin/env python3
"""Apply the qualified Xnuva chain-identity guard to a clean security-branch checkout.

Modifies only source files. It does not build, run, mine, submit blocks, touch a
datadir, or update any git ref.
"""

from pathlib import Path
import sys

ROOT = Path(__file__).resolve().parents[1]
INTERFACES = ROOT / "src/node/interfaces.cpp"
MINER = ROOT / "src/node/miner.cpp"
CHAINPARAMS = ROOT / "src/kernel/chainparams.cpp"
POLICY = ROOT / "src/xnuva/chain_identity_security.h"
GUARD = ROOT / "src/xnuva/chain_identity_guard.h"

EXPECTED_ANCHOR = "5e4a861d0237fccca85b9a1673c5429c436ae8f3bc453864d9dc335e9f1070cd"
EXPECTED_WORK = "00000000000000000000000000000000000000000000000000000000000b2000"


def stop(msg: str) -> None:
    print(f"STOP: {msg}", file=sys.stderr)
    raise SystemExit(1)


def once(text: str, old: str, new: str, label: str) -> str:
    n = text.count(old)
    if n != 1:
        stop(f"{label}: expected one insertion point, found {n}")
    return text.replace(old, new, 1)


for p in (INTERFACES, MINER, CHAINPARAMS, POLICY, GUARD):
    if not p.is_file():
        stop(f"missing required file: {p.relative_to(ROOT)}")

policy = POLICY.read_text()
if EXPECTED_ANCHOR not in policy or EXPECTED_WORK not in policy:
    stop("qualified height-88 identity is missing from policy")
if "CHAIN_IDENTITY_QUALIFIED = true" not in policy:
    stop("chain identity policy is not marked qualified")

# Mining/template interface: require sync, peers and canonical identity.
interfaces = INTERFACES.read_text()
interfaces = once(
    interfaces,
    "#include <validationinterface.h>\n",
    "#include <validationinterface.h>\n#include <xnuva/chain_identity_guard.h>\n",
    "guard include",
)
interfaces = once(
    interfaces,
    """    std::unique_ptr<BlockTemplate> createNewBlock(const BlockCreateOptions& options, bool cooldown) override
    {
        // Reject too-small values instead of clamping so callers don't silently
""",
    """    std::unique_ptr<BlockTemplate> createNewBlock(const BlockCreateOptions& options, bool cooldown) override
    {
        xnuva::security::EnforceMainnetBlockProductionGuard(m_node, chainman());

        // Reject too-small values instead of clamping so callers don't silently
""",
    "MinerImpl::createNewBlock guard",
)
interfaces = once(
    interfaces,
    """    std::unique_ptr<BlockTemplate> waitNext(BlockWaitOptions options) override
    {
        auto new_template = WaitAndCreateNewBlock(chainman(), notifications(), m_node.mempool.get(), m_block_template, options, m_assemble_options, m_interrupt_wait);
        if (new_template) return std::make_unique<BlockTemplateImpl>(m_assemble_options, std::move(new_template), m_node);
        return nullptr;
    }
""",
    """    std::unique_ptr<BlockTemplate> waitNext(BlockWaitOptions options) override
    {
        xnuva::security::EnforceMainnetBlockProductionGuard(m_node, chainman());
        auto new_template = WaitAndCreateNewBlock(chainman(), notifications(), m_node.mempool.get(), m_block_template, options, m_assemble_options, m_interrupt_wait);
        if (new_template) {
            xnuva::security::EnforceMainnetBlockProductionGuard(m_node, chainman());
            return std::make_unique<BlockTemplateImpl>(m_assemble_options, std::move(new_template), m_node);
        }
        return nullptr;
    }
""",
    "BlockTemplateImpl::waitNext guard",
)
interfaces = once(
    interfaces,
    """    bool submitSolution(uint32_t version, uint32_t timestamp, uint32_t nonce, CTransactionRef coinbase) override
    {
        AddMerkleRootAndCoinbase(m_block_template->block, std::move(coinbase), version, timestamp, nonce);
""",
    """    bool submitSolution(uint32_t version, uint32_t timestamp, uint32_t nonce, CTransactionRef coinbase) override
    {
        xnuva::security::EnforceMainnetBlockProductionGuard(m_node, chainman());
        AddMerkleRootAndCoinbase(m_block_template->block, std::move(coinbase), version, timestamp, nonce);
""",
    "BlockTemplateImpl::submitSolution guard",
)

# Central BlockAssembler: canonical chain identity/work cannot be bypassed by a
# different local template consumer.
miner = MINER.read_text()
miner = once(
    miner,
    "#include <validation.h>\n",
    "#include <validation.h>\n#include <xnuva/chain_identity_guard.h>\n",
    "central guard include",
)
miner = once(
    miner,
    """    LOCK(::cs_main);
    CBlockIndex* pindexPrev = m_chainstate.m_chain.Tip();
""",
    """    LOCK(::cs_main);
    xnuva::security::EnforceMainnetChainIdentityLocked(m_chainstate.m_chainman);
    CBlockIndex* pindexPrev = m_chainstate.m_chain.Tip();
""",
    "BlockAssembler::CreateNewBlock guard",
)

# Mainnet chain parameters only.
chainparams = CHAINPARAMS.read_text()
main_start = chainparams.find("class CMainParams")
test_start = chainparams.find("class CTestNetParams")
if main_start < 0 or test_start <= main_start:
    stop("could not isolate mainnet chain parameters")

before = chainparams[:main_start]
main = chainparams[main_start:test_start]
after = chainparams[test_start:]
main = once(
    main,
    """        consensus.nMinimumChainWork = uint256{};
        consensus.defaultAssumeValid = uint256{};
""",
    f"""        // Qualified Xnuva launch-chain identity at buried height 88.
        consensus.nMinimumChainWork = uint256{{\"{EXPECTED_WORK}\"}};
        consensus.defaultAssumeValid = uint256{{\"{EXPECTED_ANCHOR}\"}};
""",
    "mainnet minimum-work/assume-valid pair",
)
chainparams = before + main + after

if interfaces.count("EnforceMainnetBlockProductionGuard") != 4:
    stop("unexpected number of production guard references")
if miner.count("EnforceMainnetChainIdentityLocked") != 1:
    stop("central BlockAssembler guard missing or duplicated")
if main.count(EXPECTED_WORK) != 1 or main.count(EXPECTED_ANCHOR) != 1:
    stop("qualified mainnet parameters missing or duplicated")

INTERFACES.write_text(interfaces)
MINER.write_text(miner)
CHAINPARAMS.write_text(chainparams)

print("PASS: Xnuva central BlockAssembler identity guard integrated")
print("PASS: mining/template interface sync-peer-identity guards integrated")
print("PASS: Xnuva mainnet minimum chainwork set to height-88 qualified work")
print("PASS: Xnuva mainnet assume-valid set to height-88 qualified anchor")
print("PASS: test chains left untouched")
print("NEXT: inspect diff, build, then run negative and positive qualification")
