# README

## Purpose

* This folder contains information about what was changed to the fprime folder while creating this deployment.
* These files were used during development to track progress, because git does not track the FPrime folder.
* Some of the information in these files may or may not be used, but are kept in `/doc` for documentation purposes.

## File Changes

#### Svc/FramingProtocol

* Here the files 'EndurosatProtocol.cpp/hpp' were added.
* These are the files that do the actual framing/deframing.
* There was an added variable: 'm_uhfManager' which holds a pointer to a UHFTransceiverManager object.
    * If we initialize this as a nullptr, then it makes it easy to check if the object was successfully passed in.
    * The way it gets passed in is through the setup function, which simply changes 'm_uhfManager' to be a pointer to the UHFTransceiverManager object instance being used in the deployment.
    * The reason for all of this was to allow the frame method to send the actual packet that was framed through the UHFTranceiverManager's transmitData function, which calls the UART_Send_out port.
    * Conclusion: It is a variable that allows us to send framed packets through the UHFTranceiverManagers Uart_Send_out port.

#### Svc/CmakeLists.txt

* Added the line in source files: `"${CMAKE_CURRENT_LIST_DIR}/EndurosatProtocol.cpp"`
    * This just adds the custom protocol that we created.
* Added the line in Module dependencies: `Components/UHFTransceiverManager # Added because for some reason without this line, the UHFTransceiverManager is not found`
    * Added this line to let CMake know that we are using the UHFTransceiverManager component, we depend on it.
    * Also to fix the error described in the comment.

#### TrancieverUartDeployment/Top/TranscieverUartDeploymentTopology.cpp

* Added the line: 'framing.setupComp(&UHFTransceiverManager);'
    * This calls a custom function in EndurosatProtocol.cpp to set up the UHFTransceiverManager to allow the protocol to send data via UHFTransceiverManager

#### Files

* The files that were changed, because they are not tracked by GIT, have been added to this folder.
* You can use these as a reference for how to add a component instance into the fprime folder.
