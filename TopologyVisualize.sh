#!/bin/bash
mkdir visualize
mkdir visualize/converted-txt
mkdir visualize/converted-json

fpl-extract-xml -d visualize build-fprime-automatic-native/FSWDeployment/Top/FSWDeploymentTopologyAppAi.xml

fpl-convert-xml visualize/Command.xml > visualize/converted-txt/Command.txt
fpl-convert-xml visualize/CommandRegistration.xml > visualize/converted-txt/CommandRegistration.txt
fpl-convert-xml visualize/CommandResponse.xml > visualize/converted-txt/CommandResponse.txt
fpl-convert-xml visualize/Downlink.xml > visualize/converted-txt/Downlink.txt
fpl-convert-xml visualize/Events.xml > visualize/converted-txt/Events.txt
fpl-convert-xml visualize/FaultProtection.xml > visualize/converted-txt/FaultProtection.txt
fpl-convert-xml visualize/FSWDeployment.xml > visualize/converted-txt/FSWDeployment.txt
fpl-convert-xml visualize/Health.xml > visualize/converted-txt/Health.txt
fpl-convert-xml visualize/Parameters.xml > visualize/converted-txt/Parameters.txt
fpl-convert-xml visualize/RateGroups.xml > visualize/converted-txt/RateGroups.txt
fpl-convert-xml visualize/Sequencer.xml > visualize/converted-txt/Sequencer.txt
fpl-convert-xml visualize/Telemetry.xml > visualize/converted-txt/Telemetry.txt
fpl-convert-xml visualize/TextEvents.xml > visualize/converted-txt/TextEvents.txt
fpl-convert-xml visualize/Time.xml > visualize/converted-txt/Time.txt
fpl-convert-xml visualize/Uplink.xml > visualize/converted-txt/Uplink.txt

fpl-layout < visualize/converted-txt/Command.txt > visualize/converted-json/Command.json
fpl-layout < visualize/converted-txt/CommandRegistration.txt > visualize/converted-json/CommandRegistration.json
fpl-layout < visualize/converted-txt/CommandResponse.txt > visualize/converted-json/CommandResponse.json
fpl-layout < visualize/converted-txt/Downlink.txt > visualize/converted-json/Downlink.json
fpl-layout < visualize/converted-txt/Events.txt > visualize/converted-json/Events.json
fpl-layout < visualize/converted-txt/FaultProtection.txt > visualize/converted-json/FaultProtection.json
fpl-layout < visualize/converted-txt/FSWDeployment.txt > visualize/converted-json/FSWDeployment.json
fpl-layout < visualize/converted-txt/Health.txt > visualize/converted-json/Health.json
fpl-layout < visualize/converted-txt/Parameters.txt > visualize/converted-json/Parameters.json
fpl-layout < visualize/converted-txt/RateGroups.txt > visualize/converted-json/RateGroups.json
fpl-layout < visualize/converted-txt/Sequencer.txt > visualize/converted-json/Sequencer.json
fpl-layout < visualize/converted-txt/Telemetry.txt > visualize/converted-json/Telemetry.json
fpl-layout < visualize/converted-txt/TextEvents.txt > visualize/converted-json/TextEvents.json
fpl-layout < visualize/converted-txt/Time.txt > visualize/converted-json/Time.json
fpl-layout < visualize/converted-txt/Uplink.txt > visualize/converted-json/Uplink.json

fprime-visual --source-dir visualize/converted-json/