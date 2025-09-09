#!/bin/bash

if [ -d ./build-fprime-automatic-native ]; then
    echo "Already generated."
else
    fprime-util generate
fi

fprime-util build -j $(nproc)

cd FSWDeployment/Top

fprime-gds