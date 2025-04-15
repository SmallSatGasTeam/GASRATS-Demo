# HOW TO ADD THE CUSTOM FRAMER TO THE PROJECT
## Add the framer to the FPrime repo
- You'll need to run the bash file located in this folder by running `bash copyToFprime.sh`
    - This will add the `.cpp` and `.hpp` files into `fprime/Svc/FramingProtocol/`
- After this is done, you'll need to edit the `CMakeLists.txt` in `fprime/Svc/FramingProtocol/` to have the following instead of what it already has:
```cmake
####
# F prime CMakeLists.txt:
#
# SOURCE_FILES: combined list of source and autocoding files
# MOD_DEPS: (optional) module dependencies
#
# Note: using PROJECT_NAME as EXECUTABLE_NAME
####
set(SOURCE_FILES
  "${CMAKE_CURRENT_LIST_DIR}/DeframingProtocol.cpp"
  "${CMAKE_CURRENT_LIST_DIR}/FramingProtocol.cpp"
  "${CMAKE_CURRENT_LIST_DIR}/FprimeProtocol.cpp"
  "${CMAKE_CURRENT_LIST_DIR}/EndurosatProtocol.cpp"
)

set(MOD_DEPS
    Fw/Buffer
    Fw/Time
    Utils/Hash
    Utils/Types
)

register_fprime_module()

set(UT_SOURCE_FILES
  "${CMAKE_CURRENT_LIST_DIR}/test/ut/DeframingTester.cpp"
  "${CMAKE_CURRENT_LIST_DIR}/test/ut/FramingTester.cpp"
  "${CMAKE_CURRENT_LIST_DIR}/test/ut/main.cpp"
)
set(UT_MOD_DEPS STest)
register_fprime_ut()

```
- Once this is done, the framer has been added to FPrime.

## Add the framer to your FPrime project
- Create a new deployment using UART, so run the following command in your root directory `fprime-util new --deployment`, and when the options are shown choose `UART`
- Once this is created, go to the deployment folder.
    - Once here, open the file that's named something like `BlahBlahDeploymentToplogy.cpp`
- Once that `DeploymentTopology.cpp` file is open, edit the line `#include <Svc/FramingProtocol/FprimeProtocol.hpp>` to be `#include <Svc/FramingProtocol/EndurosatProtocol.hpp> // using our own protocol!!`.
- Then, change the lines:
```cpp
Svc::FprimeFraming framing;
Svc::FprimeDeframing deframing;
```
- to be these lines:
```cpp
Svc::EndurosatFraming framing; // using our own custom implementation!
Svc::EndurosatDeframing deframing; // using our own custom implementation!
```
- And you're done! To run the deployment just go into the deployment folder in your terminal and run `fprime-util gds`

## GDS
- So the GDS is the thing that actually has the ground station/computer communicate with the Raspberry pi that's running the FPrime deployment (aka the code).
- The GDS will be ran over the antenna, which means that it'll need to be ran over a transciever that'll actually transmit the stuff through the antenna.
- This transciever uses the communication protocol UART, just over a wire from the pi to the transciever.
- This means that, in order to actually test/use this deployment, you need to have a transciever correctly wired to the pi, the transicever on the pi connected to an antenna, and your computer also connected to a transciever that's connected to an antenna to actually be able to use the custom framer. 
- This is the whole point of the custom framer, it formats the data to be sent to the satellite over the transcievers. 
### Testing the framer
- Unfortunetly, I haven't been able to test this part before leaving for the summer, so idk if it actually works. 
### What I do know about using the deployment
- When you run the gds via the deployment, you'll need to set a baud rate for the UART driver so it communicates at the rate the transicever expects it to.
    - I do not know exactly how to set the baud rate, but I do know that it's just some flag that you supply to the `fprime-gds` command.

## Helpful resources
- The Transciever's [datasheet](https://usu.sharepoint.com/sites/GA/Shared%20Documents/Projects/GASRATS/EnduroSat%20Spec%20Sheets/UHF%20Transceiver%20II%20User%20Manual.pdf?CT=1741310232865&OR=ItemsView)
    - This contains all the specific info you need in order to communicate with the transicever
- The offical 'guide' on how to make the [custom framer](https://fprime.jpl.nasa.gov/latest/docs/reference/communication-adapter-interface)
    - This is esentially a tutorial of how to make the custom framer, this is what I followed to create the framer
    - The only thing I actually did was implement the `FramingProtocol` class, because fprime hooks up everything else correctly as far as I can tell, as long as you [add the custom framer to fprime](#add-the-framer-to-the-fprime-repo) and then implement it in [your deployment](#add-the-framer-to-your-fprime-project)
- The document under components called [WhatIveLearned.md](../Components/WhatIveLearned.md) is where I documented my journey making this custom framer.
    - Idk if it'll be helpful, but it shows my thought process and may help if you're confused on why I did something a certain way.