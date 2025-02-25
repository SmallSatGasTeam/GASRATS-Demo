// ======================================================================
// \title  LinuxInterruptTimer.hpp
// \author xtilloo
// \brief  hpp file for LinuxInterruptTimer component implementation class
// ======================================================================

#ifndef Components_LinuxInterruptTimer_HPP
#define Components_LinuxInterruptTimer_HPP

#include "Components/LinuxInterruptTimer/LinuxInterruptTimerComponentAc.hpp"
#include <Os/Task.hpp>
#include <signal.h>
#include <time.h>

namespace Components {

  class LinuxInterruptTimer :
    public LinuxInterruptTimerComponentBase
  {

    public:

      // ----------------------------------------------------------------------
      // Component construction and destruction
      // ----------------------------------------------------------------------

      //! Construct LinuxInterruptTimer object
      LinuxInterruptTimer(
          const char* const compName //!< The component name
      );
      void init(NATIVE_INT_TYPE queueDepth, NATIVE_INT_TYPE instance);
      void startTimer();
      void stopTimer();

      //! Destroy LinuxInterruptTimer object
      ~LinuxInterruptTimer();

    PRIVATE:

      // ----------------------------------------------------------------------
      // Handler implementations for commands
      // ----------------------------------------------------------------------

      //! Handler implementation for command StartTimer
      void StartTimer_cmdHandler(
          FwOpcodeType opCode, //!< The opcode
          U32 cmdSeq //!< The command sequence number
      ); // removed overrides

      //! Handler implementation for command StopTimer
      void StopTimer_cmdHandler(
          FwOpcodeType opCode, //!< The opcode
          U32 cmdSeq //!< The command sequence number
      ); // removed overrides

      static void signalHandler(
          int sig,
          siginfo_t *si, 
          void *uc
        );

      struct sigevent sev;
      struct sigaction sa;

      time_t timerId;
      

      
      
  };

}

#endif
