#!/bin/bash

if [ $# -ne 1 ]
then
    printf "docker_build.sh requires exactly 1 argument [cpp, py, set_finder, qt, all]\n"
    exit 1
fi

# Build image with BuildKit, new, more powerful build engine
export DOCKER_BUILDKIT=1

declare -A folders=(["cpp"]="cpp" ["set_finder"]="set_finder" ["set_finder_poc"]="set_finder/poc" ["qt"]="qt")

build () {
    echo "!!! Building $1"
    docker build \
        --target $1 \
        --build-arg USERNAME=$USER \
        --build-arg UID=$(id -u) \
        -t panajot/$1:latest \
        "${folders[$1]}"
}

if [ "$1" == "all" ]
then
    for value in "${!folders[@]}"
    do
        build "$value"
    done
else
    build "$1"
fi
