#pragma once

#include <Frontend/FakeInterface.hpp>
#include <QObject>

namespace fake {

class EnergyMeter : public QObject, public FakeInterface {
    Q_OBJECT
  public:
    EnergyMeter(const std::string& /* dbc_file_path */ = "") : QObject(nullptr) {
        this->FakeInterface::startReceiving();
    }
    ~EnergyMeter() = default;

  signals:
    void newVoltage(float voltage);
    void newCurrent(float current);

  private:
    void generateValues();
};

} // namespace fake