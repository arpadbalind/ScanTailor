#!/bin/sh

set -e

help() {
    cat <<EOF >&2
Usage: ${0} <path>
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

scope="${SRC}"

echo "scope=<${scope}>"

if [ -n "${scope}" ]; then
    if [ -d "${scope}" ]; then
        files=$(find "${scope}" -type f \( -name '*.c' -o -name '*.cpp' \))
    elif [ -f "${scope}" ]; then
        files="${scope}"
    else
        echo "Error: ${scope} does not exist" >&2
        exit 1
    fi
else
    files=$(git ls-files -co --exclude-standard | grep '\.\(c\|cpp\)$')
fi

echo "ARPY_FILES:"
printf '%s\n' ${files}

output=$(mktemp -tu tidy.XXXXXX)
trap 'rm -f "${output}"' EXIT
mkfifo "${output}"

cat < "${output}" | sed -E '/^clang-tidy/d' | sed 's/clang-tidy.*//' &
echo "${files}" | xargs run-clang-tidy -quiet -p "${path}" > "${output}"
