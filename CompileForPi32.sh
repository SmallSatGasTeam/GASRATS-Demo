#!/bin/bash

#Options
while getopts ":hp:" opt; do
    case $opt in
        #help
        h)
            echo "Usage: $0 [-h help] [-pi pi #]"
            exit 0
            ;;
        #pi number
        p)
            pi_num=$OPTARG
            ;;
        \?)
            echo "Option -$OPTARG is invalid"
            exit 1
            ;;
    esac
done

#If no optional argument provided, set pi_num to 0
if [[ $OPTIND -eq 1 ]]; then
    pi_num=0
fi

# generates the deployment
fprime-util build arm-hf-linux -j $(nproc)

# sends the deployment to pi1
scp -r build-artifacts/arm-hf-linux/FSWDeployment gas@pi$pi_num.gas.usu.edu:

# starts the gds on this machine
fprime-gds -n --dictionary build-artifacts/arm-hf-linux/FSWDeployment/dict/FSWDeploymentTopologyAppDictionary.xml --ip-client --ip-address pi$pi_num.gas.usu.edu