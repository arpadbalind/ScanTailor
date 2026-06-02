#!/bin/sh

set -e

help() {
    cat <<EOF >&2
Usage: ${0} <path>

Options:
    <path> A path where a compile_commands.json is located
EOF
}

if [ $# -lt 1 ]; then
    help
    exit 1
fi

path="${1}"

if [ ! -d "${path}" ]; then
    echo "Error: ${path} is not a directory" >&2
    exit 1
fi

shift
files=$*

if [ -z "${files}" ]; then
    files=$(git ls-files -co --exclude-standard | grep '\.\(c\|cpp\)$')
fi

output=$(mktemp -tu tidy.XXXXXX)
trap 'rm -f "${output}"' EXIT
mkfifo "${output}"

cat< "${output}" | sed -E '/^clang-tidy/d' | sed 's/clang-tidy.*//' &
echo "${files}" | xargs run-clang-tidy -quiet -p "${path}" > "${output}"
