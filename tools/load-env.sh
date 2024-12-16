#!/bin/sh

get_script_dir() {
    local SCRIPT_PATH=""
    if [ -n "${BASH_SOURCE[0]}" ]; then
        SCRIPT_PATH="${BASH_SOURCE[0]}"
    elif [ -n "${ZSH_VERSION}" ]; then
        SCRIPT_PATH="${(%):-%x}"
    else
        SCRIPT_PATH="$0"
    fi

    local SCRIPT_PATH="$(readlink -f "$SCRIPT_PATH" 2>/dev/null || realpath "$SCRIPT_PATH" 2>/dev/null || echo "$SCRIPT_PATH")"

    local SCRIPT_DIR="$(dirname "$SCRIPT_PATH")"

    echo "$SCRIPT_DIR"
}


add_dir() {
    echo "Adding libraries to path ${1}lib"
    export LD_LIBRARY_PATH="${LD_LIBRARY_PATH}:${1}lib"
}


main() {
    local MODULE_DIR=$(dirname $(get_script_dir))
    echo "Executing at $MODULE_DIR/tools/load-env.sh"

    export LD_LIBRARY_PATH=""

    add_dir "${MODULE_DIR}/external/raylib/"
    add_dir "${MODULE_DIR}/"
}


main
