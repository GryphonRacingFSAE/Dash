#include <QApplication>
#include <QQmlApplicationEngine>
#include <fmt/core.h>

#include <Frontend/BMS.hpp>
#include <Frontend/EnergyMeter.hpp>
#include <Frontend/MotorController.hpp>
#include <Frontend/SMU.hpp>
#include <Frontend/VCU.hpp>

#ifdef DEBUG
#include <runtimeqml.hpp>
#endif

int main(int argc, char* argv[]) {
    QApplication app(argc, argv);
    std::string app_root = app.applicationDirPath().toStdString();
    // Should be before QQml engine
    fake::MotorController* motor_controller = new fake::MotorController(app_root + "/DBCs/20220510_Gen5_CAN_DB.dbc");
    fake::EnergyMeter* energy_meter = new fake::EnergyMeter(app_root + "/DBCs/Energy_Meter_CAN_Messages.dbc");
    fake::BMS* bms = new fake::BMS(app_root + "/DBCs/Orion_CANBUS.dbc");
    fake::VCU* vcu = new fake::VCU(app_root + "/TorqueMaps");
    fake::SMU* smu = new fake::SMU(app_root + "/DBCs/SMU_CANBUS.dbc");

    QQmlApplicationEngine engine;

    qmlRegisterSingletonInstance<fake::MotorController>("CAN.MotorController", 1, 0, "MotorController", motor_controller);
    qmlRegisterSingletonInstance<fake::EnergyMeter>("CAN.EnergyMeter", 1, 0, "EnergyMeter", energy_meter);
    qmlRegisterSingletonInstance<fake::BMS>("CAN.BMS", 1, 0, "BMS", bms);
    qmlRegisterSingletonInstance<fake::VCU>("CAN.VCU", 1, 0, "VCU", vcu);
    qmlRegisterSingletonInstance<fake::SMU>("CAN.SMU", 1, 0, "SMU", smu);
#ifdef DEBUG
    RuntimeQml* rt = new RuntimeQml(&engine);
    rt->parseQrc(ROOT_SOURCE_PATH "/qml.qrc");
    rt->setAutoReload(true); // Reload automatically on file update

    rt->load(QStringLiteral("qrc:/main.qml"));
#else
    engine.load(QStringLiteral("qrc:/main.qml"));
#endif
    auto retcode = app.exec();
    return retcode;
}
