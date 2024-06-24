// ======================================================================
// \title  FlightLogicTestMain.cpp
// \author jjhessing
// \brief  cpp file for FlightLogic component test main function
// ======================================================================

#include "FlightLogicTester.hpp"

TEST(Nominal, testTakePic) {
  Components::FlightLogicTester tester;
  tester.testTakePic();
}

TEST(Nominal, testResetFlags) {
  Components::FlightLogicTester tester;
  tester.testResetFlags();
}

TEST(Nominal, testSendTransmission) {
  Components::FlightLogicTester tester;
  tester.testSendTransmission();
}

TEST(Nominal, testDetumble) {
  Components::FlightLogicTester tester;
  tester.testDetumble();
}

TEST(Nominal, testStartupDelay) {
  Components::FlightLogicTester tester;
  tester.testStartup();
}

TEST(Nominal, testLowPower) {
  Components::FlightLogicTester tester;
  tester.testLowPower();
}

TEST(Nominal, testAntennaFail) {
  Components::FlightLogicTester tester;
  tester.testAntennaFailure();
}

TEST(Nominal, testCameraFail) {
  Components::FlightLogicTester tester;
  tester.testCameraFailure();
}

TEST(Nominal, testHealthPing) {
  Components::FlightLogicTester tester;
  tester.testHealthPing();
}

TEST(Nominal, testDataRequest) {
  Components::FlightLogicTester tester;
  tester.testDataRequest();
}

TEST(Nominal, testRecvTransmission) {
  Components::FlightLogicTester tester;
  tester.testRecvTransmission();
}

TEST(Nominal, testSendBeaconState) {
  Components::FlightLogicTester tester;
  tester.testSendBeaconState();
}

int main(int argc, char** argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
