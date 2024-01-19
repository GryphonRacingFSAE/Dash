#pragma once

#include <DBCInterface.hpp>
#include <Frontend/FakeInterface.hpp>
#include <QObject>

namespace demo {

class MotorController : public QObject, public CAN::DBCInterface<MotorController>, fake::FakeInterface {
    Q_OBJECT
  public:
    MotorController(const std::string& dbc_file_path = "20220510_Gen5_CAN_DB.dbc") : QObject(nullptr), DBCInterface(dbc_file_path) {
        this->delay = std::chrono::milliseconds(20);
        can_signal_dispatch["INV_Motor_Speed"] = &MotorController::newMotorSpeed;
        can_signal_dispatch["INV_Motor_Temp"] = &MotorController::newMotorTemp;
        can_signal_dispatch["INV_Coolant_Temp"] = &MotorController::newCoolantTemp;
        can_signal_dispatch["INV_Analog_Input_1"] = &MotorController::new12VVoltage;
        can_signal_dispatch["INV_Analog_Input_2"] = &MotorController::newOilTemp;
        can_signal_dispatch["INV_Module_A_Temp"] = &MotorController::newModuleATemp;
        can_signal_dispatch["VCU_INV_Torque_Command"] = &MotorController::newRequestedTorqueHandler;
        this->FakeInterface::startReceiving();
    }

    Q_INVOKABLE void clearFaultCodes() {
        // CM200DZ CAN Protocol V5.9 Section 2.3.1 and 2.3.3 (Address 20 or 0x14)
        RetCode ans = CAN::Interface::write(0x0C1, std::array<uint8_t, 8>{0x00, 0x14, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00});

        if (ans != RetCode::Success) {
            fmt::print("Failed to clear fault codes\n");
        }
    }

  signals:
    void new12VVoltage(float voltage);
    void newOilTemp(float temp);
    void newRequestedTorque(float torque);
    void newOutputTorque(float torque);

  public:
    static constexpr size_t num_of_filters = 3;
    inline static can_filter filters[num_of_filters] = {{
                                                            0x0A0,
                                                            0x7F0 // Grab all messages from 0xA0 to 0xAF
                                                        },
                                                        {
                                                            0x0B0,
                                                            0x7F0 // Grab all messages from 0xB0 to 0xBF
                                                        },
                                                        {
                                                            0x0C0,
                                                            0x7F0 // Grab all messages from 0xC0 to 0xCF
                                                        }};

    static constexpr uint32_t timeout_ms = 500;
    static constexpr uint32_t torque_slew_rate = 60; // 60 N.m/s
    std::atomic<float> requestedTorque = 0;
    std::atomic<float> outputTorque = 0;

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

    void newMotorSpeed(float rpm);

  private:
    void generateValues() {
        static float current = 0.0f;
        static float voltage = 0.0f;
        static float temp = 0.0f;
        static float temp2 = 0.0f;
        static float internal_state = 0.0f;
        static float analog_voltage = 0.0f;
        static float torque = 0.0f;

        // DEBUG A
        emit newDCBusCurrent(current);

        emit newDCBusVoltage(voltage);
        emit newOutputVoltage(voltage);

        emit newModuleATemp(temp);
        emit newModuleBTemp(temp);
        emit newModuleCTemp(temp);
        emit newGateDriverBoardTemp(temp);
        emit newControlBoardTemp(temp);
        emit newCoolantTemp(temp2);
        emit newHotSpotTemp(temp);
        emit newMotorTemp(temp);

        // DEBUG B
        emit newPWMFrequency(internal_state);
        emit newInverterState(internal_state);
        emit newInverterRunMode(internal_state);
        emit newInverterActiveDischargeState(internal_state);
        emit newInverterEnableLockout(internal_state);
        emit newBMSActive(internal_state);
        emit newBMSLimitingTorque(internal_state);
        emit newLimitMaxSpeed(internal_state);
        emit newLimitHotSpot(internal_state);
        emit newLowSpeedLimiting(internal_state);
        emit newCoolantTempLimiting(internal_state);

        emit newAnalogInput1(analog_voltage);
        emit newAnalogInput2(analog_voltage);
        emit newAnalogInput3(analog_voltage);
        emit newAnalogInput4(analog_voltage);
        emit newAnalogInput5(analog_voltage);
        emit newAnalogInput6(analog_voltage);

        emit newTorqueShudder(torque);
        emit newCommandedTorque(torque);
        emit newTorqueFeedback(torque);

        current += 0.1f;
        voltage += 0.1f;
        temp += 0.293f;
        temp2 += 0.192f;
        internal_state += 0.1f;
        analog_voltage += 0.215f;
        torque += 0.1f;

        if (current >= 100) {
            current = 0.0f;
        }
        if (voltage >= 100) {
            voltage = 0.0f;
        }
        if (temp >= 73) {
            temp = 0.0f;
        }
        if (temp2 >= 63) {
            temp2 = 8.1;
        }
        if (internal_state >= 100) {
            internal_state = 0.0f;
        }
        if (analog_voltage >= 82) {
            analog_voltage = 9.2f;
        }
        if (torque >= 100) {
            torque = 0.0f;
        }

        fmt::print("{}, {}\n", requestedTorque, outputTorque);
        if (outputTorque < requestedTorque) {
            outputTorque += torque_slew_rate * 0.02;
            outputTorque = outputTorque > requestedTorque ? requestedTorque.load() : outputTorque.load();
        } else if (outputTorque > requestedTorque) {
            outputTorque -= torque_slew_rate * 0.02;
            outputTorque = outputTorque < requestedTorque ? requestedTorque.load() : outputTorque.load();
        }

        emit newOutputTorque(outputTorque);

        static float velocity = 0.0f;
        float gearRatio = 3.5;
        float tireRadius = 0.203;
        float carMass = 306;
        float area = 1.2; // m^2
        float rollResistance = 0.04;
        float dragCoefficient = 0.75;
        float rho = 1.2; // 1.2kg/m^3

        /*Equation to calculate Fd*/
        /*Fd = Cr * Mg + 1/2 * P * Cd * A * V^2*/
        float forceDrag = rollResistance * carMass * 9.81 + 0.5 * rho * dragCoefficient * area * velocity * velocity;
        float acceleration = ((outputTorque * gearRatio) / tireRadius - forceDrag) / carMass;
        velocity += acceleration * 0.020;         // 20 milliseconds
        velocity = velocity < 0 ? 0.0 : velocity; // m/s

        float kmph = velocity / 1000 * 60 * 60;
        float rpm = kmph * 45.4;
        emit newMotorSpeed(rpm);
        int16_t rpm2 = (int16_t)rpm;
        uint16_t rpm3 = *reinterpret_cast<uint16_t*>(&rpm2);

        // CM200DZ CAN Protocol V6.1
        auto ans =
            CAN::Interface::write(0x0A5, std::array<uint8_t, 8>{0x00, 0x00, (uint8_t)(rpm3 & 0xFF), (uint8_t)(rpm3 >> 8), 0x00, 0x00, 0x00, 0x00});

        if (ans != RetCode::Success) {
            fmt::print("Failed to send new rpm\n");
        }
    }

  private:
    void newRequestedTorqueHandler(float torque) {
        requestedTorque = torque;
        emit newRequestedTorque(requestedTorque);
    }
};

} // namespace demo
