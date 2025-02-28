// ======================================================================
// \title  LinuxInterruptTimer.cpp
// \author jjhessing
// \brief  cpp file for LinuxInterruptTimer component implementation class
// ======================================================================

#include "Components/LinuxInterruptTimer/LinuxInterruptTimer.hpp"
#include "FpConfig.hpp"
#include "LinuxInterruptTimer.hpp"

namespace Components {

// ----------------------------------------------------------------------
// Component construction and destruction
// ----------------------------------------------------------------------

LinuxInterruptTimer ::LinuxInterruptTimer(const char* const compName) : LinuxInterruptTimerComponentBase(compName) {}

LinuxInterruptTimer ::~LinuxInterruptTimer() {}

// ----------------------------------------------------------------------
// Handler implementations for commands
// ----------------------------------------------------------------------

void LinuxInterruptTimer ::StartTimer_cmdHandler(FwOpcodeType opCode, U32 cmdSeq) {
    // TODO
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
        //this->log_WARNING_HI_Error(errno, strerror(errno));
        return;
    }
    sa.sa_flags = SA_SIGINFO;
    sa.sa_sigaction = this->signalHandler;
    sigemptyset(&sa.sa_mask);

    if (sigaction(SIGRTMIN, &sa, NULL) == -1) {
      //this->log_WARNING_HI_Error(errno, strerror(errno));
      return;
    }

    if (timer_settime(timerId, 0, &its, NULL) != 0) {
      //this->log_WARNING_HI_Error(errno, strerror(errno));
      return;
    }

    this->log_ACTIVITY_HI_TimerCreated();
    this->cmdResponse_out(opCode, cmdSeq, Fw::CmdResponse::OK);
}

void LinuxInterruptTimer ::StopTimer_cmdHandler(FwOpcodeType opCode, U32 cmdSeq) {
    // TODO
    this->cmdResponse_out(opCode, cmdSeq, Fw::CmdResponse::OK);
}

void LinuxInterruptTimer::signalHandler(int sig, siginfo_t *si, void *uc) {
    //UNUSED(sig);
    //UNUSED(uc);
    struct t_eventData *data = (struct t_eventData *) si->_sifields._rt.si_sigval.sival_ptr;
}

}  // namespace Components
