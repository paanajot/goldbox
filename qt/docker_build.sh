#!/bin/bash

if [ $# -ne 1 ]
then
    printf "docker_build.sh requires exactly 1 argument [fedora, tools]\n"
    exit 1
fi

export DOCKER_BUILDKIT=1

docker build \
    --target final-qt \
    --build-arg USERNAME=${USER} \
    --build-arg UID=$(id -u) \
    -t ${USER}/qul_${1}:latest \
    --file Dockerfile_${1} \
    .
