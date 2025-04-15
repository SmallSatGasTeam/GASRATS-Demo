#!/bin/bash

#Options
while getopts ":hp:d:" opt; do
    case $opt in
        #help
        h)
            echo "Usage: $0 [-h help] [-pi pi #] [-d deployment]"
            exit 0
            ;;
        #pi number
        p)
            pi_num=$OPTARG
            ;;
        d)
            deployment=$OPTARG
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
    deployment="FSWDeployment"
fi

echo $deployment

# generates the deployment
fprime-util build arm-hf-linux -j $(nproc)

# sends the deployment to pi1
scp -r build-artifacts/arm-hf-linux/${deployment}/bin/${deployment} gas@pi$pi_num.gas.usu.edu:

# starts the gds on this machine
fprime-gds -n --dictionary build-artifacts/arm-hf-linux/${deployment}/dict/${deployment}TopologyAppDictionary.xml --ip-client --ip-address pi${pi_num}.gas.usu.edu