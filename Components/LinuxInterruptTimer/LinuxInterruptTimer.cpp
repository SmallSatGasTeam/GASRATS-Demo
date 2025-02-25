// ======================================================================
// \title  LinuxInterruptTimer.cpp
// \author xtilloo
// \brief  cpp file for LinuxInterruptTimer component implementation class
// ======================================================================

#include "Components/LinuxInterruptTimer/LinuxInterruptTimer.hpp"
#include "FpConfig.hpp"

// these are the modules from the research code, found in pseudo code tab
#include <errno.h> // error reporting
#include <string.h> //error handling
#include <unistd.h> // gives access to POSIX calles like getpid()


namespace Components {

  // ----------------------------------------------------------------------
  // Component construction and destruction
  // ----------------------------------------------------------------------

  LinuxInterruptTimer ::
    LinuxInterruptTimer(const char* const compName) :
      LinuxInterruptTimerComponentBase(compName)
  {

  }

  LinuxInterruptTimer ::
    ~LinuxInterruptTimer()
  {

  }


  // ----------------------------------------------------------------------
  // Handler implementations for commands
  // ----------------------------------------------------------------------


  // This initializes the component message queue, inherited from LinuxInterruptTimerComponentBase.
  // queueDepth is how many messages can be queued before processing,
  // instance differentiates between multiple instances of the component
  // not sure if this is necessary
  void LinuxInterruptTimer::init(NATIVE_INT_TYPE queueDepth, NATIVE_INT_TYPE instance) 
  {
    LinuxInterruptTimerComponentBase::init(queueDepth, instance);
  } 

  void LinuxInterruptTimer ::
    StartTimer_cmdHandler(
        FwOpcodeType opCode,
        U32 cmdSeq
    )
  {
    // this is where the actual timer code will be placed
    // figure out why the struct cannot be defined.

    // struct itimerspec its = {};
    struct itimerspec its = {};

    its.it_value.tv_sec = 1; // timer starts after 1 second
    its.it_value.tv_nsec = 0;
    its.it_interval.tv_sec = 1; // repeats every second
    its.it_interval.tv_nsec = 0;

    sev.sigev_notify = SIGEV_SIGNAL;
    sev.sigev_signo = SIGRTMIN;
    sev.sigev_value.sival_ptr = &timerId;

    if (timer_create(CLOCK_REALTIME, &sev, &timerId) != 0) {
        this->log_WARNING_HI_Error(errno, strerror(errno));
        return;
    }
    sa.sa_flags = SA_SIGINFO;
    sa.sa_sigaction = TimerComponent::signalHandler;
    sigemptyset(&sa.sa_mask);

    if (sigaction(SIGRTMIN, &sa, NULL) == -1) {
      this->log_WARNING_HI_Error(errno, strerror(errno));
      return;
    }

    if (timer_settime(timerId, 0, &its, NULL) != 0) {
      this->log_WARNING_HI_Error(errno, strerror(errno));
      return;
    }

    this->log_ACTIVITY_HI_TimerCreated();
    this->cmdResponse_out(opCode, cmdSeq, Fw::CmdResponse::OK);
  }

  void LinuxInterruptTimer ::
    StopTimer_cmdHandler(
        FwOpcodeType opCode,
        U32 cmdSeq
    )
  {

    ///////////////
    // TODO HERE //
    ///////////////

    this->cmdResponse_out(opCode, cmdSeq, Fw::CmdResponse::OK);
  }

}
