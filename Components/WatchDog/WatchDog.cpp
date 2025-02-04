// ======================================================================
// \title  WatchDog.cpp
// \author jjhessing
// \brief  cpp file for WatchDog component implementation class
// ======================================================================

#include "Components/WatchDog/WatchDog.hpp"
#include "FpConfig.hpp"
#include "Components/componentConfig/Constants.hpp"
#include "Components/WatchDog/WatchDogComponentAc.hpp"

#if FW_ASSERT_LEVEL == FW_FILEID_ASSERT
#define fileIdFs "Assert: 0x%08" PRIx32 ":%" PRI_PlatformUIntType
#else
#define fileIdFs "Assert: \"%s:%" PRI_PlatformUIntType "\""
#endif

namespace Components {

bool healthy = true;

int pings = 0;

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
    // for testing
    FW_ASSERT(numPings < 5);

    if (code == 0x123)
    {
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
        // for testing
        pings++;
    }
}

void GaspacsAssertHook ::reportAssert(FILE_NAME_ARG file, NATIVE_UINT_TYPE lineNo, NATIVE_UINT_TYPE numArgs, FwAssertArgType arg1,
                                      FwAssertArgType arg2, FwAssertArgType arg3, FwAssertArgType arg4, FwAssertArgType arg5,
                                      FwAssertArgType arg6) {
    CHAR destBuffer[FW_ASSERT_DFL_MSG_LEN];
    this->defaultReportAssert(file, lineNo, numArgs, arg1, arg2, arg3, arg4, arg5, arg6, destBuffer, sizeof(destBuffer));
    this->printAssert(destBuffer);
    this->doAssert();
}

void GaspacsAssertHook ::doAssert() {
    healthy = false;
}

void GaspacsAssertHook::defaultReportAssert
            (
            FILE_NAME_ARG file,
            NATIVE_UINT_TYPE lineNo,
            NATIVE_UINT_TYPE numArgs,
            FwAssertArgType arg1,
            FwAssertArgType arg2,
            FwAssertArgType arg3,
            FwAssertArgType arg4,
            FwAssertArgType arg5,
            FwAssertArgType arg6,
            CHAR* destBuffer,
            NATIVE_INT_TYPE buffSize
            ) {

        switch (numArgs) {
            case 0:
                (void) snprintf(destBuffer, buffSize, fileIdFs, file, lineNo);
                break;
            case 1:
                (void) snprintf(
                    destBuffer,
                    buffSize,
                    fileIdFs " %" PRI_FwAssertArgType,
                    file,
                    lineNo,
                    arg1
                );
                break;
            case 2:
                (void) snprintf(
                    destBuffer,
                    buffSize,
                    fileIdFs " %" PRI_FwAssertArgType " %" PRI_FwAssertArgType,
                    file,
                    lineNo,
                    arg1, arg2
                );
                break;
            case 3:
                (void) snprintf(
                    destBuffer,
                    buffSize,
                    fileIdFs " %" PRI_FwAssertArgType " %" PRI_FwAssertArgType
                      " %" PRI_FwAssertArgType,
                    file,
                    lineNo,
                    arg1, arg2, arg3
                );
                break;
            case 4:
                (void) snprintf(
                    destBuffer,
                    buffSize,
                    fileIdFs " %" PRI_FwAssertArgType " %" PRI_FwAssertArgType
                      " %" PRI_FwAssertArgType " %" PRI_FwAssertArgType,
                    file,
                    lineNo,
                    arg1, arg2, arg3, arg4);
                break;
            case 5:
                (void) snprintf(
                    destBuffer,
                    buffSize,
                    fileIdFs " %" PRI_FwAssertArgType " %" PRI_FwAssertArgType
                      " %" PRI_FwAssertArgType " %" PRI_FwAssertArgType
                      " %" PRI_FwAssertArgType,
                    file,
                    lineNo,
                    arg1, arg2, arg3, arg4, arg5
                );
                break;
            case 6:
                (void) snprintf(
                    destBuffer,
                    buffSize,
                    fileIdFs " %" PRI_FwAssertArgType " %" PRI_FwAssertArgType
                      " %" PRI_FwAssertArgType " %" PRI_FwAssertArgType
                      " %" PRI_FwAssertArgType " %" PRI_FwAssertArgType,
                    file,
                    lineNo,
                    arg1, arg2, arg3, arg4, arg5, arg6
                );
                break;
            default: // in an assert already, what can we do?
                break;
        }

        // null terminate
        destBuffer[buffSize-1] = 0;

    }

}  // namespace Components
