#pragma once

#include <DBCInterface.hpp>
#include <QObject>

namespace real {

class BMS : public QObject, public CAN::DBCInterface<BMS> {
    Q_OBJECT
  public:
    BMS(const std::string& dbc_file_path = "Orion_CANBUS.dbc") : QObject(nullptr), DBCInterface(dbc_file_path) {
        can_signal_dispatch["Pack_Open_Voltage"] = &BMS::newAccumulatorOpenVoltage;
        can_signal_dispatch["Pack_SOC"] = &BMS::newAccumulatorSOC;
        can_signal_dispatch["Pack_Inst_Voltage"] = &BMS::newAccumulatorInstVoltage;
        can_signal_dispatch["Pack_Current"] = &BMS::newAccumulatorCurrent;
        can_signal_dispatch["High_Temperature"] = &BMS::newAccumulatorMaxTemp;
        can_signal_dispatch["Internal_Temperature"] = &BMS::newBMSTemp;
    }

  signals:
    void newBMSTemp(float temp);
    void newAccumulatorMaxTemp(float temp);
    void newAccumulatorCurrent(float current);
    void newAccumulatorInstVoltage(float voltage);
    void newAccumulatorOpenVoltage(float voltage);
    void newAccumulatorSOC(float percent);

  public:
    static constexpr size_t num_of_filters = 3;
    inline static can_filter filters[num_of_filters] = {{
        0x0E0,
        0x7F0 // Grab 0x0E0 to 0x0EF for broadcast messages
    }};

    static constexpr uint32_t timeout_ms = 500;
};

} // namespace real
