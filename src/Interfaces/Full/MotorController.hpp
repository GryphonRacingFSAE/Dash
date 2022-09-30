#pragma once

#include <QObject>

#include <Interface.hpp>

class MotorController : public QObject, public CAN::Interface {
    Q_OBJECT
  public:
    MotorController(QObject* parent = nullptr) : QObject(parent) {
        this->CAN::Interface::startReceiving(
            "can0", MotorController::filters, MotorController::timeout_ms);
    }
    ~MotorController() = default;

  signals:
    void newRPM(int16_t rpm);
    void newCoolantTemp(float temp);
    void newOilTemp(float temp);

  private:
    // As of now, newFrame and newError should be extremely fast functions before emitting more
    // signals
    void newFrame(const can_frame&) override;
    void newError(const can_frame&) override;
    void newTimeout() override;

  private:
    static constexpr std::array<can_filter, 2> filters{
        {{
             0x0A0,
             0xFF0 // Grab all messages from 0xA0 to 0xAF
         },
         {
             0x0B0,
             0xFFF // Grab all messages from 0xB0
         }}};

    static constexpr uint32_t timeout_ms = 500;
};