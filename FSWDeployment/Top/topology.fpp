module FSWDeployment {

  # ----------------------------------------------------------------------
  # Symbolic constants for port numbers
  # ----------------------------------------------------------------------

    enum Ports_RateGroups {
      rateGroup1
      rateGroup2
      rateGroup3
      rateGroup4
    }

  topology FSWDeployment {

    # ----------------------------------------------------------------------
    # Instances used in the topology
    # ----------------------------------------------------------------------

    instance $health
    instance blockDrv
    instance tlmSend
    instance cmdDisp
    instance cmdSeq
    instance comDriver
    instance comQueue
    instance comStub
    instance deframer
    instance eventLogger
    instance fatalAdapter
    instance fatalHandler
    instance fileDownlink
    instance fileManager
    instance fileUplink
    instance bufferManager
    instance framer
    instance posixTime
    instance prmDb
    instance rateGroup1
    instance rateGroup2
    instance rateGroup3
    instance rateGroup4
    instance rateGroupDriver
    instance textLogger
    instance systemResources
    instance antennaDeploy
    instance flightLogic
    instance cameraManager
    instance transmissionManager
    instance epsManager
    instance dataCollector
    instance imuInterface
    instance i2cDriver
    instance dummyTranceiverDriver

    # ----------------------------------------------------------------------
    # Pattern graph specifiers
    # ----------------------------------------------------------------------

    command connections instance cmdDisp

    event connections instance eventLogger

    param connections instance prmDb

    telemetry connections instance tlmSend

    text event connections instance textLogger

    time connections instance posixTime

    health connections instance $health

    # ----------------------------------------------------------------------
    # Direct graph specifiers
    # ----------------------------------------------------------------------

    connections Downlink {

      eventLogger.PktSend -> comQueue.comQueueIn[0]
      tlmSend.PktSend -> comQueue.comQueueIn[1]
      fileDownlink.bufferSendOut -> comQueue.buffQueueIn[0]

      comQueue.comQueueSend -> framer.comIn
      comQueue.buffQueueSend -> framer.bufferIn

      framer.framedAllocate -> bufferManager.bufferGetCallee
      framer.framedOut -> comStub.comDataIn
      framer.bufferDeallocate -> fileDownlink.bufferReturn

      comDriver.deallocate -> bufferManager.bufferSendIn
      comDriver.ready -> comStub.drvConnected

      comStub.comStatus -> framer.comStatusIn
      framer.comStatusOut -> comQueue.comStatusIn
      comStub.drvDataOut -> comDriver.$send

    }

    connections FaultProtection {
      eventLogger.FatalAnnounce -> fatalHandler.FatalReceive
    }

    connections RateGroups {
      # Block driver
      blockDrv.CycleOut -> rateGroupDriver.CycleIn

      # Rate group 1
      rateGroupDriver.CycleOut[Ports_RateGroups.rateGroup1] -> rateGroup1.CycleIn
      rateGroup1.RateGroupMemberOut[0] -> tlmSend.Run
      rateGroup1.RateGroupMemberOut[1] -> fileDownlink.Run
      rateGroup1.RateGroupMemberOut[2] -> systemResources.run
      rateGroup1.RateGroupMemberOut[3] -> flightLogic.startup
      rateGroup1.RateGroupMemberOut[4] -> dataCollector.run
      # rateGroup1.RateGroupMemberOut[4] -> transmissionManager.beacon

      # Rate group 2
      rateGroupDriver.CycleOut[Ports_RateGroups.rateGroup2] -> rateGroup2.CycleIn
      rateGroup2.RateGroupMemberOut[0] -> cmdSeq.schedIn

      # Rate group 3
      rateGroupDriver.CycleOut[Ports_RateGroups.rateGroup3] -> rateGroup3.CycleIn
      rateGroup3.RateGroupMemberOut[0] -> $health.Run
      rateGroup3.RateGroupMemberOut[1] -> blockDrv.Sched
      rateGroup3.RateGroupMemberOut[2] -> bufferManager.schedIn

      # Rate group 4
      rateGroupDriver.CycleOut[Ports_RateGroups.rateGroup4] -> rateGroup4.CycleIn
      rateGroup4.RateGroupMemberOut[0] -> transmissionManager.beacon
    }

    connections Sequencer {
      cmdSeq.comCmdOut -> cmdDisp.seqCmdBuff
      cmdDisp.seqCmdStatus -> cmdSeq.cmdResponseIn
    }

    connections Uplink {

      comDriver.allocate -> bufferManager.bufferGetCallee
      comDriver.$recv -> comStub.drvDataIn
      comStub.comDataOut -> deframer.framedIn

      deframer.framedDeallocate -> bufferManager.bufferSendIn
      deframer.comOut -> cmdDisp.seqCmdBuff

      cmdDisp.seqCmdStatus -> deframer.cmdResponseIn

      deframer.bufferAllocate -> bufferManager.bufferGetCallee
      deframer.bufferOut -> fileUplink.bufferSendIn
      deframer.bufferDeallocate -> bufferManager.bufferSendIn
      fileUplink.bufferSendOut -> bufferManager.bufferSendIn
    }

    connections FSWDeployment {
      # Add here connections to user-defined components
      flightLogic.deployAntenna -> antennaDeploy.deploy
      flightLogic.deployCamera -> cameraManager.deploy
      flightLogic.takePic -> cameraManager.takePic
      flightLogic.fakeData -> dataCollector.aggregate
      flightLogic.epsHealth -> epsManager.returnHealth
      transmissionManager.beaconState -> flightLogic.beaconState
      transmissionManager.sendData -> dummyTranceiverDriver.sendTransToGround
      dummyTranceiverDriver.recvTransFromGround -> transmissionManager.recvData
      imuInterface.collector -> dataCollector.aggregate
      imuInterface.requestI2CData -> i2cDriver.read
      imuInterface.i2cWrite -> i2cDriver.write
      dataCollector.ping[0] -> flightLogic.dataRequest
      dataCollector.ping[1] -> imuInterface.dataRequest
      imuInterface.gyroData -> dataCollector.imuIncoming
      imuInterface.allocate -> bufferManager.bufferGetCallee
      imuInterface.deallocate -> bufferManager.bufferSendIn
    }

  }

}
