// ======================================================================
// \title  WatchDog.cpp
// \author jjhessing
// \brief  cpp file for WatchDog component implementation class
// ======================================================================

#include "Components/WatchDog/WatchDog.hpp"
#include "FpConfig.hpp"
#include "Components/componentConfig/Constants.hpp"

namespace Components {

// ----------------------------------------------------------------------
// Component construction and destruction
// ----------------------------------------------------------------------

WatchDog ::WatchDog(const char* const compName) : WatchDogComponentBase(compName) {
    this->stroke = true;
}

WatchDog ::~WatchDog() {}

// ----------------------------------------------------------------------
// Handler implementations for user-defined typed input ports
// ----------------------------------------------------------------------

void WatchDog ::healthIn_handler(NATIVE_INT_TYPE portNum, U32 code) {
    if (code == 0x123) {
        stroke = !stroke;
        if(stroke) {
            #ifndef VIRTUAL
            this->heartBeatOut_out(0,Fw::Logic::HIGH);
            #endif
            this->tlmWrite_heartBeat(Fw::Logic::HIGH);
        }
        else {
            #ifndef VIRTUAL
            this->heartBeatOut_out(0,Fw::Logic::LOW);
            #endif
            this->tlmWrite_heartBeat(Fw::Logic::LOW);
        }
    }
    
}

}  // namespace Components
