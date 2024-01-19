#pragma once

#include <QObject>
#include <fmt/core.h>

#include <Frontend/FakeInterface.hpp>

namespace fake {

class MotorController : public QObject, public FakeInterface {
    Q_OBJECT
  public:
    MotorController(const std::string& /* dbc_file_path */ = "") : QObject(nullptr) {
        this->FakeInterface::startReceiving();
    }
    ~MotorController() = default;

    Q_INVOKABLE void clearFaultCodes() {
        fmt::print("Attempt to clear fault codes\n");
    }

  signals:
    // DEBUG A
    void newDCBusCurrent(float current);

    void newDCBusVoltage(float voltage);
    void newOutputVoltage(float voltage);

    void newModuleATemp(float temp);
    void newModuleBTemp(float temp);
    void newModuleCTemp(float temp);
    void newGateDriverBoardTemp(float temp);
    void newControlBoardTemp(float temp);
    void newCoolantTemp(float temp);
    void newHotSpotTemp(float temp);
    void newMotorTemp(float temp);

    // DEBUG B
    void newPWMFrequency(float internal_state);
    void newInverterState(float internal_state);
    void newInverterRunMode(float internal_state);
    void newInverterActiveDischargeState(float internal_state);
    void newInverterEnableLockout(float internal_state);
    void newBMSActive(float internal_state);
    void newBMSLimitingTorque(float internal_state);
    void newLimitMaxSpeed(float internal_state);
    void newLimitHotSpot(float internal_state);
    void newLowSpeedLimiting(float internal_state);
    void newCoolantTempLimiting(float internal_state);

    void newAnalogInput1(float analog_voltage);
    void newAnalogInput2(float analog_voltage);
    void newAnalogInput3(float analog_voltage);
    void newAnalogInput4(float analog_voltage);
    void newAnalogInput5(float analog_voltage);
    void newAnalogInput6(float analog_voltage);

    void newTorqueShudder(float torque);
    void newCommandedTorque(float torque);
    void newTorqueFeedback(float torque);

    void newMotorSpeed(float speed);

    // DEBUG C
    void newPOSTFaultHigh(float post_fault);
    void newPOSTFaultLow(float post_fault);

    // DEBUG D
    void newRUNFaultHigh(float run_fault);
    void newRUNFaultLow(float run_fault);

  private:
    void generateValues();
};

} // namespace fake