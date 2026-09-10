#!/bin/bash

set +e
set +u
set +o pipefail

BASE="$(cd "$(dirname "$0")" && pwd)"

if [ -n "${XNUVA_CLI:-}" ]; then
    CLI="$XNUVA_CLI"
elif [ -x "$BASE/bin/xnuva-cli" ]; then
    CLI="$BASE/bin/xnuva-cli"
elif [ -x "$BASE/build/bin/xnuva-cli" ]; then
    CLI="$BASE/build/bin/xnuva-cli"
else
    CLI="$(command -v xnuva-cli 2>/dev/null)"
fi

if [ -n "${XNUVA_DATADIR:-}" ]; then
    DATA="$XNUVA_DATADIR"
elif [ -d "$BASE/data" ]; then
    DATA="$BASE/data"
else
    DATA=""
fi

ADDRESS_FILE="${XNUVA_ADDRESS_FILE:-$BASE/mining-address.txt}"

ANCHOR_HEIGHT=88
ANCHOR_HASH="5e4a861d0237fccca85b9a1673c5429c436ae8f3bc453864d9dc335e9f1070cd"
GENESIS_HASH="0000410149e74469ea857100515f786d4bff7fdaf05863ca1b0948cf8f049f9f"

MODE="continuous"
MODE_SET=0
USER_ADDRESS=""
HELP_REQUESTED=0
STOP_REQUESTED=0

CHAIN_NAME=""
BLOCKS=""
HEADERS=""
IBD=""
NETWORKACTIVE=""
CONNECTIONS=""
ANCHOR_SEEN=""
GENESIS_SEEN=""

# ===== XNUVA MINER TERMINAL WINDOW =====
# Continuous mining gets its own visible terminal where GNOME Terminal is
# available. --status, --one and --help remain in the calling terminal.
if [ "${XNUVA_MINER_TERMINAL:-0}" != "1" ]; then
    OPEN_TERMINAL=1

    for arg in "$@"; do
        case "$arg" in
            --status|--one|--help|-h)
                OPEN_TERMINAL=0
                ;;
        esac
    done

    if [ "$OPEN_TERMINAL" -eq 1 ] &&
       command -v gnome-terminal >/dev/null 2>&1 &&
       [ -n "${DISPLAY:-}${WAYLAND_DISPLAY:-}" ]; then

        gnome-terminal \
            --title="Xnuva RandomX Miner" \
            -- bash -lc '
                export XNUVA_MINER_TERMINAL=1
                "$@"
                RC=$?

                echo
                echo "======================================"
                echo "Xnuva miner stopped. Exit code: $RC"
                echo "Press Enter to close this window."
                echo "======================================"
                read

                exit "$RC"
            ' bash "$0" "$@"

        exit $?
    fi
fi
# ===== END XNUVA MINER TERMINAL WINDOW =====

usage()
{
    echo
    echo "Xnuva Core RandomX Miner"
    echo "========================"
    echo
    echo "Usage:"
    echo "  ./miner.sh"
    echo "      Mine continuously in the foreground or a dedicated"
    echo "      GNOME Terminal window when available."
    echo
    echo "  ./miner.sh --one"
    echo "      Mine exactly one block."
    echo
    echo "  ./miner.sh --status"
    echo "      Check mining readiness without mining."
    echo
    echo "  ./miner.sh --address <xnuva-address>"
    echo "      Mine continuously to a specific reward address."
    echo
    echo "  ./miner.sh --one --address <xnuva-address>"
    echo "      Mine exactly one block to a specific reward address."
    echo
    echo "Environment overrides:"
    echo "  XNUVA_CLI=/path/to/xnuva-cli"
    echo "  XNUVA_DATADIR=/path/to/data"
    echo "  XNUVA_WALLET='wallet name'"
    echo
    echo "Safety:"
    echo "  The miner NEVER starts xnuvad or xnuva-qt automatically."
    echo "  The node must already be running with RPC enabled."
    echo "  The node must be on the established Xnuva mainnet chain."
    echo "  Historical checkpoint height $ANCHOR_HEIGHT must equal:"
    echo "    $ANCHOR_HASH"
    echo "  Every mining request is exactly:"
    echo "    generatetoaddress 1 <address>"
    echo
    echo "  Continuous mode rechecks chain identity immediately"
    echo "  before every block. The live tip is not fixed, so other"
    echo "  miners may advance the chain at any time."
    echo
}

rpc()
{
    if [ -n "$DATA" ]; then
        "$CLI" -datadir="$DATA" "$@"
    else
        "$CLI" "$@"
    fi
}

rpc_wallet()
{
    local wallet="$1"
    shift

    if [ -n "$DATA" ]; then
        "$CLI" -datadir="$DATA" -rpcwallet="$wallet" "$@"
    else
        "$CLI" -rpcwallet="$wallet" "$@"
    fi
}

parse_args()
{
    while [ "$#" -gt 0 ]; do
        case "$1" in
            --one)
                if [ "$MODE_SET" -eq 1 ] &&
                   [ "$MODE" != "one" ]; then
                    echo "ERROR: conflicting mining modes."
                    return 2
                fi
                MODE="one"
                MODE_SET=1
                shift
                ;;

            --status)
                if [ "$MODE_SET" -eq 1 ] &&
                   [ "$MODE" != "status" ]; then
                    echo "ERROR: conflicting mining modes."
                    return 2
                fi
                MODE="status"
                MODE_SET=1
                shift
                ;;

            --address)
                shift
                if [ "$#" -lt 1 ]; then
                    echo "ERROR: --address requires a Xnuva address."
                    return 2
                fi
                USER_ADDRESS="$1"
                shift
                ;;

            --help|-h)
                HELP_REQUESTED=1
                shift
                ;;

            *)
                echo "ERROR: unknown argument: $1"
                return 2
                ;;
        esac
    done

    return 0
}

read_state()
{
    local chain_json
    local network_json
    local parsed

    chain_json="$(rpc getblockchaininfo 2>/dev/null)"
    if [ "$?" -ne 0 ] || [ -z "$chain_json" ]; then
        return 1
    fi

    network_json="$(rpc getnetworkinfo 2>/dev/null)"
    if [ "$?" -ne 0 ] || [ -z "$network_json" ]; then
        return 1
    fi

    parsed="$(
        python3 - "$chain_json" "$network_json" <<'PY'
import json
import sys

try:
    chain = json.loads(sys.argv[1])
    net = json.loads(sys.argv[2])

    print("chain=" + str(chain.get("chain", "")))
    print("blocks=" + str(chain.get("blocks", "")))
    print("headers=" + str(chain.get("headers", "")))
    print("ibd=" + ("true" if bool(chain.get("initialblockdownload", True)) else "false"))
    print("networkactive=" + ("true" if bool(net.get("networkactive", False)) else "false"))
    print("connections=" + str(net.get("connections", 0)))

except Exception:
    print("parse_error=true")
PY
    )"

    if printf '%s\n' "$parsed" | grep -q '^parse_error=true$'; then
        return 1
    fi

    CHAIN_NAME="$(printf '%s\n' "$parsed" | awk -F= '$1=="chain"{print $2}')"
    BLOCKS="$(printf '%s\n' "$parsed" | awk -F= '$1=="blocks"{print $2}')"
    HEADERS="$(printf '%s\n' "$parsed" | awk -F= '$1=="headers"{print $2}')"
    IBD="$(printf '%s\n' "$parsed" | awk -F= '$1=="ibd"{print $2}')"
    NETWORKACTIVE="$(printf '%s\n' "$parsed" | awk -F= '$1=="networkactive"{print $2}')"
    CONNECTIONS="$(printf '%s\n' "$parsed" | awk -F= '$1=="connections"{print $2}')"

    if ! [[ "$BLOCKS" =~ ^[0-9]+$ ]] ||
       ! [[ "$HEADERS" =~ ^[0-9]+$ ]] ||
       ! [[ "$CONNECTIONS" =~ ^[0-9]+$ ]]; then
        return 1
    fi

    return 0
}

show_state()
{
    echo "chain=$CHAIN_NAME"
    echo "blocks=$BLOCKS"
    echo "headers=$HEADERS"
    echo "initialblockdownload=$IBD"
    echo "networkactive=$NETWORKACTIVE"
    echo "connections=$CONNECTIONS"

    if [ -n "$GENESIS_SEEN" ]; then
        echo "genesis=$GENESIS_SEEN"
    fi

    if [ -n "$ANCHOR_SEEN" ]; then
        echo "block_${ANCHOR_HEIGHT}=$ANCHOR_SEEN"
    fi
}

check_ready()
{
    local show="${1:-yes}"

    ANCHOR_SEEN=""
    GENESIS_SEEN=""

    if [ -z "$CLI" ] || [ ! -x "$CLI" ]; then
        echo "STOP: xnuva-cli was not found."
        echo "Set XNUVA_CLI=/path/to/xnuva-cli if needed."
        echo "MINING_READY=NO"
        return 1
    fi

    if ! read_state; then
        echo "STOP: Xnuva node RPC is unavailable."
        echo "The miner will NOT start a daemon automatically."
        echo "Start/synchronize the intended node with RPC enabled first."
        echo "MINING_READY=NO"
        return 1
    fi

    GENESIS_SEEN="$(rpc getblockhash 0 2>/dev/null)"
    if [ "$GENESIS_SEEN" != "$GENESIS_HASH" ]; then
        [ "$show" = "yes" ] && show_state
        echo "STOP: node genesis does not match Xnuva mainnet."
        echo "expected=$GENESIS_HASH"
        echo "actual=$GENESIS_SEEN"
        echo "MINING_READY=NO"
        return 1
    fi

    if [ "$CHAIN_NAME" != "main" ]; then
        [ "$show" = "yes" ] && show_state
        echo "STOP: node is not on Xnuva mainnet."
        echo "MINING_READY=NO"
        return 1
    fi

    if [ "$BLOCKS" -lt "$ANCHOR_HEIGHT" ]; then
        [ "$show" = "yes" ] && show_state
        echo "STOP: node is below the qualified launch-chain checkpoint."
        echo "blocks=$BLOCKS required_at_least=$ANCHOR_HEIGHT"
        echo "MINING_READY=NO"
        return 1
    fi

    ANCHOR_SEEN="$(rpc getblockhash "$ANCHOR_HEIGHT" 2>/dev/null)"
    if [ "$ANCHOR_SEEN" != "$ANCHOR_HASH" ]; then
        [ "$show" = "yes" ] && show_state
        echo "STOP: node does not match the established Xnuva launch chain."
        echo "height=$ANCHOR_HEIGHT"
        echo "expected=$ANCHOR_HASH"
        echo "actual=$ANCHOR_SEEN"
        echo "MINING_READY=NO"
        return 1
    fi

    if [ "$HEADERS" -lt "$BLOCKS" ]; then
        [ "$show" = "yes" ] && show_state
        echo "STOP: invalid local chain/header state."
        echo "MINING_READY=NO"
        return 1
    fi

    if [ "$IBD" != "false" ]; then
        [ "$show" = "yes" ] && show_state
        echo "STOP: node reports initial block download."
        echo "MINING_READY=NO"
        return 1
    fi

    if [ "$NETWORKACTIVE" != "true" ]; then
        [ "$show" = "yes" ] && show_state
        echo "STOP: network activity is disabled."
        echo "MINING_READY=NO"
        return 1
    fi

    if [ "$CONNECTIONS" -lt 1 ]; then
        [ "$show" = "yes" ] && show_state
        echo "STOP: node has no peer connections."
        echo "MINING_READY=NO"
        return 1
    fi

    if [ "$show" = "yes" ]; then
        show_state
        echo "MINING_READY=YES"
    fi

    return 0
}

validate_address()
{
    local address="$1"
    local json

    json="$(rpc validateaddress "$address" 2>/dev/null)"
    if [ "$?" -ne 0 ] || [ -z "$json" ]; then
        return 1
    fi

    python3 - "$json" <<'PY'
import json
import sys

try:
    obj = json.loads(sys.argv[1])
    raise SystemExit(0 if bool(obj.get("isvalid", False)) else 1)
except Exception:
    raise SystemExit(1)
PY
}

choose_address()
{
    local address=""
    local wallets_json
    local wallet_count
    local wallet_name

    if [ -n "$USER_ADDRESS" ]; then
        address="$USER_ADDRESS"
    elif [ -s "$ADDRESS_FILE" ]; then
        address="$(head -n 1 "$ADDRESS_FILE" | tr -d '\r\n')"
    fi

    if [ -n "$address" ]; then
        if ! validate_address "$address"; then
            echo "STOP: invalid Xnuva mining address: $address"
            return 1
        fi

        MINING_ADDRESS="$address"
        return 0
    fi

    wallets_json="$(rpc listwallets 2>/dev/null)"
    if [ "$?" -ne 0 ] || [ -z "$wallets_json" ]; then
        echo "STOP: no mining address is configured and loaded wallets could not be queried."
        echo "Use --address <xnuva-address>."
        return 1
    fi

    read -r wallet_count wallet_name < <(
        python3 - "$wallets_json" <<'PY'
import json
import sys

try:
    wallets = json.loads(sys.argv[1])
    print(len(wallets), wallets[0] if len(wallets) == 1 else "")
except Exception:
    print(-1, "")
PY
    )

    if [ "${XNUVA_WALLET+x}" = "x" ] && [ -n "$XNUVA_WALLET" ]; then
        wallet_name="$XNUVA_WALLET"
        wallet_count=1
    fi

    if [ "$wallet_count" != "1" ] || [ -z "$wallet_name" ]; then
        echo "STOP: no mining address is configured."
        echo "Load exactly one wallet, set XNUVA_WALLET, or use:"
        echo "  ./miner.sh --address <xnuva-address>"
        return 1
    fi

    address="$(rpc_wallet "$wallet_name" getnewaddress "Xnuva Mining" bech32 2>/dev/null)"
    if [ "$?" -ne 0 ] || [ -z "$address" ]; then
        echo "STOP: could not obtain a mining address from wallet: $wallet_name"
        return 1
    fi

    if ! validate_address "$address"; then
        echo "STOP: wallet returned an invalid mining address."
        return 1
    fi

    printf '%s\n' "$address" > "$ADDRESS_FILE"
    chmod 600 "$ADDRESS_FILE" 2>/dev/null

    echo "Created mining address in wallet: $wallet_name"
    echo "Saved address to: $ADDRESS_FILE"

    MINING_ADDRESS="$address"
    return 0
}

extract_hash()
{
    python3 - "$1" <<'PY'
import json
import sys

try:
    value = json.loads(sys.argv[1])
    if isinstance(value, list) and len(value) == 1 and isinstance(value[0], str):
        print(value[0])
        raise SystemExit(0)
except Exception:
    pass

raise SystemExit(1)
PY
}

classify_result()
{
    local hash="$1"
    local header_json
    local parsed

    header_json="$(rpc getblockheader "$hash" true 2>/dev/null)"
    if [ "$?" -ne 0 ] || [ -z "$header_json" ]; then
        echo "accepted=unknown hash=$hash"
        return 1
    fi

    parsed="$(
        python3 - "$header_json" <<'PY'
import json
import sys

try:
    h = json.loads(sys.argv[1])
    print(str(h.get("height", "")))
    print(str(h.get("confirmations", "")))
except Exception:
    raise SystemExit(1)
PY
    )" || return 1

    local height
    local confirmations

    height="$(printf '%s\n' "$parsed" | sed -n '1p')"
    confirmations="$(printf '%s\n' "$parsed" | sed -n '2p')"

    if [[ "$confirmations" =~ ^-?[0-9]+$ ]] && [ "$confirmations" -gt 0 ]; then
        echo "accepted=active height=$height confirmations=$confirmations hash=$hash"
        return 0
    fi

    if [ "$confirmations" = "-1" ]; then
        echo "accepted=stale-side-chain height=$height confirmations=-1 hash=$hash"
        return 0
    fi

    echo "accepted=unknown height=$height confirmations=$confirmations hash=$hash"
    return 1
}

mine_one()
{
    local before
    local result
    local found_hash
    local after

    if ! check_ready no; then
        return 1
    fi

    before="$BLOCKS"

    result="$(rpc generatetoaddress 1 "$MINING_ADDRESS" 2>&1)"
    if [ "$?" -ne 0 ]; then
        echo "STOP: mining RPC failed."
        printf '%s\n' "$result"
        return 1
    fi

    found_hash="$(extract_hash "$result")"
    if [ "$?" -ne 0 ] || [ -z "$found_hash" ]; then
        echo "STOP: mining RPC returned an unexpected result."
        printf '%s\n' "$result"
        return 1
    fi

    after="$(rpc getblockcount 2>/dev/null)"
    if ! [[ "$after" =~ ^[0-9]+$ ]]; then
        echo "STOP: unable to verify chain height after mining."
        return 1
    fi

    printf '%s  tip_before=%s tip_after=%s  ' "$(date '+%F %T')" "$before" "$after"
    classify_result "$found_hash"
    return $?
}

trap 'STOP_REQUESTED=1' INT TERM HUP

if ! parse_args "$@"; then
    usage
    exit 2
fi

if [ "$HELP_REQUESTED" -eq 1 ]; then
    usage
    exit 0
fi

if ! check_ready yes; then
    exit 1
fi

if [ "$MODE" = "status" ]; then
    exit 0
fi

if ! choose_address; then
    exit 1
fi

echo
echo "Xnuva RandomX mining address:"
echo "  $MINING_ADDRESS"
echo

if [ "$MODE" = "one" ]; then
    mine_one
    exit $?
fi

echo "XNUVA MINER RUNNING"
echo "Press Ctrl-C to stop safely."
echo

while [ "$STOP_REQUESTED" -eq 0 ]; do
    if ! mine_one; then
        echo
        echo "MINER STOPPED: safety check or mining request failed."
        exit 1
    fi
done

echo
echo "Xnuva miner stopped."
exit 0
