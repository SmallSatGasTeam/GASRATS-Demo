#!/bin/bash

fprime-cli command-send flightLogic.takePic --dictionary build-artifacts/arm-hf-linux/FSWDeployment/dict/FSWDeploymentTopologyAppDictionary.xml

# TODO: have the camera component take photos with a different name every time

fprime-cli command-send fileDownlink.SendFile --arguments "photos/fprimeTest.jpg" "fprimeTest.jpg" --dictionary build-artifacts/arm-hf-linux/FSWDeployment/dict/FSWDeploymentTopologyAppDictionary.xml
