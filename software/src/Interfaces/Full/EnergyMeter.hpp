#pragma once

#include <DBCInterface.hpp>
#include <QObject>

namespace real {

class EnergyMeter : public QObject, public CAN::DBCInterface<EnergyMeter> {
    Q_OBJECT
  public:
    EnergyMeter(const std::string& dbc_file_path = "Energy_Meter_CAN_Messages.dbc") : QObject(nullptr), DBCInterface(dbc_file_path) {
        can_signal_dispatch["Current"] = &EnergyMeter::newCurrent;
        can_signal_dispatch["Voltage"] = &EnergyMeter::newVoltage;
    }

  signals:
    void newVoltage(float voltage);
    void newCurrent(float current);

  public:
    static constexpr size_t num_of_filters = 2;
    inline static can_filter filters[num_of_filters] = {{
                                                            0x100,
                                                            0x7FF // Grab all messages from 0x100
                                                        },
                                                        {
                                                            0x400,
                                                            0x7FF // Grab all messages from 0x400
                                                        }};

    static constexpr uint32_t timeout_ms = 500;
};

} // namespace real