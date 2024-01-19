#pragma once

#include <QObject>

#include <Frontend/FakeInterface.hpp>

namespace fake {

class BMS : public QObject, public FakeInterface {
    Q_OBJECT
  public:
    BMS(const std::string& /* dbc_file_path */ = "") : QObject(nullptr) {
        this->FakeInterface::startReceiving();
    }
    ~BMS() = default;

  signals:
    void newAvgPackCurrent(float current);
    void newSignedCurrent(float current);
    void newUnsignedCurrent(float current);
    void newCurrentLimitStatus(float current);
    void newChargeCurrentLimitA(float current);
    void newChargeCurrentLimitKW(float current);
    void newDischargeCurrentLimitA(float current);
    void newDischargeCurrentLimitKW(float current);

    void newVoltage(float voltage);
    void newOpenVoltage(float voltage);
    void newSummedVoltage(float voltage);
    void newAvgCellVoltage(float voltage);
    void newLowCellVoltage(float voltage);
    void newLowCellVoltageID(float voltage);
    void newHighCellVoltage(float voltage);
    void newHighCellVoltageID(float voltage);
    void newAvgCellOpenVoltage(float voltage);
    void newLowCellOpenVoltage(float voltage);
    void newHighCellOpenVoltage(float voltage);
    void newMinCellVoltage(float voltage);
    void newMaxCellVoltage(float voltage);

    void newAvgTemp(float temp);
    void newLowestTemp(float temp);
    void newHighestTemp(float temp);
    void newHeatsinkTemp(float temp);
    void newHottestThermistorID(float temp);

    void newStateOfCharge(float value);
    void newAmphours(float value);
    void newResistance(float value);
    void newHealth(float value);
    void newTotalPackCycles(float value);
    void newPackPowerKW(float value);

  private:
    void generateValues();
};

} // namespace fake