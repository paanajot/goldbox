#!/bin/bash

ANDROID_ABIs=("x86" "x86_64" "armv7" "armv8")
CMAKE_BUILD_TYPE=Debug

for ANDROID_ABI in ${ANDROID_ABIs[@]};
do
    echo ${ANDROID_ABI}
    conan build . -pr=android_${ANDROID_ABI} -b=missing -s build_type=${CMAKE_BUILD_TYPE}
    cmake --install ./build/${ANDROID_ABI}/${CMAKE_BUILD_TYPE}
done
