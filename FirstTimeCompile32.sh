#!/bin/bash
# IMPORTANT: before running this check the fprime website to see if your dependencies are all good
# https://nasa.github.io/fprime/Tutorials/CrossCompilationSetup/CrossCompilationSetupTutorial.html#1-introduction

sudo mkdir -p /opt/toolchains
sudo chown $USER /opt/toolchains

# only use this if you haven't before
curl -Ls https://developer.arm.com/-/media/Files/downloads/gnu-a/10.2-2020.11/binrel/gcc-arm-10.2-2020.11-x86_64-arm-none-linux-gnueabihf.tar.xz | tar -JC /opt/toolchains --strip-components=1 -x

# as long as this outputs anything other than a command or file not found error it's good
/opt/toolchains/bin/arm-none-linux-gnueabihf-gcc -v

export ARM_TOOLS_PATH=/opt/toolchains
# this is to check if environment vairable is correctly set
# it should return `/opt/toolchains` if it's all good
echo $ARM_TOOLS_PATH

# generates the deployment
fprime-util generate arm-hf-linux
fprime-util build arm-hf-linux -j $(nproc)

# sends the deployment to pi1
scp -r build-artifacts/arm-hf-linux/FSWDeployment gas@pi0.gas.usu.edu:deployment

# starts the gds on this machine
fprime-gds -n --dictionary build-artifacts/arm-hf-linux/FSWDeployment/dict/FSWDeploymentTopologyAppDictionary.xml --ip-client --ip-address pi0.gas.usu.edu