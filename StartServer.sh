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


fprime-gds -n --dictionary build-artifacts/arm-hf-linux/FSWDeployment/dict/FSWDeploymentTopologyAppDictionary.xml --ip-client --ip-address pi$pi_num.gas.usu.edu