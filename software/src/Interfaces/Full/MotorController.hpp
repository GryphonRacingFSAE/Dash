#pragma once

#include <DBCInterface.hpp>
#include <QObject>

namespace real {

class MotorController : public QObject, public CAN::DBCInterface<MotorController> {
    Q_OBJECT
  public:
    MotorController(const std::string& dbc_file_path = "20220510_Gen5_CAN_DB.dbc") : QObject(nullptr), DBCInterface(dbc_file_path) {
        // DEBUG A
        can_signal_dispatch["INV_DC_Bus_Current"] = &MotorController::newDCBusCurrent;

        can_signal_dispatch["INV_DC_Bus_Voltage"] = &MotorController::newDCBusVoltage;
        can_signal_dispatch["INV_Output_Voltage"] = &MotorController::newOutputVoltage;

        can_signal_dispatch["INV_Module_A_Temp"] = &MotorController::newModuleATemp;
        can_signal_dispatch["VCU_INV_Torque_Command"] = &MotorController::newRequestedTorque;
        can_signal_dispatch["INV_Module_B_Temp"] = &MotorController::newModuleBTemp;
        can_signal_dispatch["INV_Module_C_Temp"] = &MotorController::newModuleCTemp;
        can_signal_dispatch["INV_Gate_Driver_Board_Temp"] = &MotorController::newGateDriverBoardTemp;
        can_signal_dispatch["INV_Control_Board_Temp"] = &MotorController::newControlBoardTemp;
        can_signal_dispatch["INV_Coolant_Temp"] = &MotorController::newCoolantTemp;
        can_signal_dispatch["INV_Hot_Spot_Temp"] = &MotorController::newHotSpotTemp;
        can_signal_dispatch["INV_Motor_Temp"] = &MotorController::newMotorTemp;

        can_signal_dispatch["INV_PWM_Frequency"] = &MotorController::newPWMFrequency;
        can_signal_dispatch["INV_Inverter_State"] = &MotorController::newInverterState;
        can_signal_dispatch["INV_Inverter_Run_Mode"] = &MotorController::newInverterRunMode;
        can_signal_dispatch["INV_Inverter_Discharge_State"] = &MotorController::newInverterActiveDischargeState;
        can_signal_dispatch["INV_Inverter_Enable_Lockout"] = &MotorController::newInverterEnableLockout;
        can_signal_dispatch["INV_BMS_Active"] = &MotorController::newBMSActive;
        can_signal_dispatch["INV_BMS_Limiting_Torque"] = &MotorController::newBMSLimitingTorque;
        can_signal_dispatch["INV_Limit_Max_Speed"] = &MotorController::newLimitMaxSpeed;
        can_signal_dispatch["INV_Limit_Hot_Spot"] = &MotorController::newLimitHotSpot;
        can_signal_dispatch["INV_Low_Speed_Limiting"] = &MotorController::newLowSpeedLimiting;
        can_signal_dispatch["INV_Limit_Coolant_Derating"] = &MotorController::newCoolantTempLimiting;

        // DEBUG B
        can_signal_dispatch["INV_Analog_Input_1"] = &MotorController::newAnalogInput1;
        can_signal_dispatch["INV_Analog_Input_2"] = &MotorController::newAnalogInput2;
        can_signal_dispatch["INV_Analog_Input_3"] = &MotorController::newAnalogInput3;
        can_signal_dispatch["INV_Analog_Input_4"] = &MotorController::newAnalogInput4;
        can_signal_dispatch["INV_Analog_Input_5"] = &MotorController::newAnalogInput5;
        can_signal_dispatch["INV_Analog_Input_6"] = &MotorController::newAnalogInput6;

        can_signal_dispatch["INV_Torque_Shudder"] = &MotorController::newTorqueShudder;
        can_signal_dispatch["INV_Commanded_Torque"] = &MotorController::newCommandedTorque;
        can_signal_dispatch["INV_Torque_Feedback"] = &MotorController::newTorqueFeedback;

        can_signal_dispatch["INV_Motor_Speed"] = &MotorController::newMotorSpeed;

        // DEBUG C
        can_signal_dispatch["INV_Post_Fault_Hi"] = &MotorController::newPOSTFaultHigh;
        can_signal_dispatch["INV_Post_Fault_Lo"] = &MotorController::newPOSTFaultLow;

        // DEBUG D
        can_signal_dispatch["INV_Run_Fault_Hi"] = &MotorController::newRUNFaultHigh;
        can_signal_dispatch["INV_Run_Fault_Lo"] = &MotorController::newRUNFaultLow;
    }

    Q_INVOKABLE void clearFaultCodes() {
        // CM200 CAN Protocol V5.9 Section 2.3.1 and 2.3.3 (Address 20 or 0x14)
        RetCode ans = write(0x0C1, std::array<uint8_t, 8>{0x00, 0x14, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00});

        if (ans != RetCode::Success) {
            fmt::print("Failed to clear fault codes\n");
        }
    }

  signals:
    void newDCBusCurrent(float current);

    void newDCBusVoltage(float voltage);
    void newOutputVoltage(float voltage);

    void newModuleATemp(float temp);
    void newRequestedTorque(float torque);
    void newModuleBTemp(float temp);
    void newModuleCTemp(float temp);
    void newGateDriverBoardTemp(float temp);
    void newControlBoardTemp(float temp);
    void newCoolantTemp(float temp);
    void newHotSpotTemp(float temp);
    void newMotorTemp(float temp);

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
    void newPOSTFaultHigh(float fault);
    void newPOSTFaultLow(float fault);
    void newRUNFaultHigh(float fault);
    void newRUNFaultLow(float fault);

  public:
    static constexpr size_t num_of_filters = 1;
    inline static can_filter filters[num_of_filters] = {{
        0x0A0,
        0x0CF // Grab all messages from 0xA0 to 0xCF (Everything reserved by the motor controller)
    }};

    static constexpr uint32_t timeout_ms = 500;
};

} // namespace real