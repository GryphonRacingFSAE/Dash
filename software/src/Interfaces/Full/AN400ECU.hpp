#pragma once

#include <DBCInterface.hpp>
#include <QObject>

namespace real {

class AN400ECU : public QObject, public CAN::DBCInterface<AN400ECU> {
    Q_OBJECT
  public:
    AN400ECU(const std::string& dbc_file_path = "AN400ECU.dbc") : QObject(nullptr), DBCInterface(dbc_file_path) {
        can_signal_dispatch["RPM"] = &AN400ECU::newRPM;
        can_signal_dispatch["TPS"] = &AN400ECU::newTPS;
        can_signal_dispatch["MAP"] = &AN400ECU::newMAP;
        can_signal_dispatch["Lambda"] = &AN400ECU::newLambda;
        can_signal_dispatch["Battery_Voltage"] = &AN400ECU::newBatteryVoltage;
        can_signal_dispatch["Coolant_Temp"] = &AN400ECU::newCoolantTemp;
        can_signal_dispatch["Air_Temp"] = &AN400ECU::newAirTemp;
    }

  signals:
    void newRPM(float rpm);
    void newTPS(float tps);
    void newMAP(float map);
    void newLambda(float lambda);
    void newBatteryVoltage(float voltage);
    void newCoolantTemp(float temp);
    void newAirTemp(float temp);

  public:
    static constexpr size_t num_of_filters = 5;
    inline static can_filter filters[num_of_filters] = {{
                                                            0x0CFFF048,
                                                            0x1FFFF8FF // Grab all messages from 0CFFF048 to 0CFFF748
                                                        },
                                                        {
                                                            0x0CFFF848,
                                                            0x1FFFFCFF // Grab all messages from 0CFFF848 to 0CFFFB48
                                                        },
                                                        {
                                                            0x0CFFFC48,
                                                            0x1FFFFEFF // Grab all messages from 0CFFFC48 to 0CFFFD48
                                                        },
                                                        {
                                                            0x0CFFFE48,
                                                            0x1FFFFFFF // Grab all messages from 0CFFFE48
                                                        },
                                                        {
                                                            0x0CFFD048,
                                                            0x1FFFFFFF // Grab all messages from 0CFFD048
                                                        }};

    static constexpr uint32_t timeout_ms = 500;
};

} // namespace real
