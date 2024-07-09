# GASRATS-FSW

This is the prototype for the GASRATS-FSW. Besides the Flight Logic component, all other components are shells used to show off Flight Logic functionality. At the moment, Flight Logic is configured with these capabilities: 

- Check EPS data to verify health deploy conditions.
- Delay deployment for x minutes as specified in the [config file](./Components/componentConfig/Constants.hpp). This is to meet Nanoracks'.
- Prompt the [Antenna Manager](./Components/AntennaDeploy/) to deploy the antenna.
- Prompt the [Camera Manager](./Components/CameraManager/) to deploy the camera.
- Use the beaconState flag to transition the beacon between OFF, INITIAL, and STANDARD states.
- Prompt the [Camera Manager](./Components/CameraManager/) to take a photo
- Reboot the system if the antenna or the camera fails to deploy.
- Use the lowPower flag to send the system into lowPower mode.

Additionally, if run on GAS pi0 a picture can be taken and downlinked using the GDS and existing fprime infrastructure.

The ability to save and read system flags between reboots and bootup health checks still needs to be added to the Flight Logic component.

For more information on Flight Logic see the [Flight Logic Docs](./Components/FlightLogic/docs/sdd.md).

For more information about the beacon see the [Transmission Manager Docs](./Components/TransmissionManager/docs/sdd.md).

## Running on a Laptop

To run this code on a laptop, make sure VIRTUAL is defined in the [config file](./Components/componentConfig/Constants.hpp) uncommenting `#Define VIRTUAL` if it wasn't previously uncommented. Next, ensure line 127 in [instances.fpp](./FSWDeployment/Top/instances.fpp) reads `instance comDriver: Drv.TcpClient base id 0x4000`.


Then generate the program, build the program, and run fprime-gds from the [FSWDeployment folder](./FSWDeployment/Top/).

## Running on a pi

To run this code on a pi, comment out VIRTUAL in the [config file](./Components/componentConfig/Constants.hpp). Next change line 127 in instances.fpp to `instance comDriver: Drv.TcpServer base id 0x4000`.

Generate and build the program using arm-hf-linux cross-compiler and upload the executable file to a pi of your choice. (GAS pi 0 is best if you have access to it.) After the executable is uploaded, find it on the pi and run it using `sudo ./FSWDeployment -a 0.0.0.0 -p 50000`. Finally, on your own computer run `fprime-gds -n --dictionary build-artifacts/arm-hf-linux/FSWDeployment/dict/FSWDeploymentTopologyAppDictionary.xml --ip-client --ip-address <address of your pi>`.

Alternatively, the [FirstTimeCompile32.sh](./FirstTimeCompile32.sh) will install the build tools for the cross-compiler and then automatically generate, build, and upload the executable to pi0.

If you already have the arm-hf-linux, generate for arm-hf-linux and then run [CompileForPi32.sh](./CompileForPi32.sh) to automatically build the project, upload it to pi0, and start the gds.

Both scripts still require manually running the executable file on the pi.


## Component Connection Diagram

![FSWDeployment Diagram](./FSWDeployment%20Visualization.png)

