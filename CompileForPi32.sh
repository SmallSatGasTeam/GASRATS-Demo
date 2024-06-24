#!/bin/bash

# generates the deployment
fprime-util purge arm-hf-linux
fprime-util generate arm-hf-linux
fprime-util build arm-hf-linux -j $(nproc)

# sends the deployment to pi1
scp -r build-artifacts/arm-hf-linux/FSWDeployment gas@pi0.gas.usu.edu:deployment

# starts the gds on this machine
fprime-gds -n --dictionary build-artifacts/aarch64-linux/FSWDeployment/dict/FSWDeploymentTopologyAppDictionary.xml --ip-client --ip-address pi0.gas.usu.edu