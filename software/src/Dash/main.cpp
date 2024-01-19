#include <QApplication>
#include <QQmlApplicationEngine>
#include <fmt/core.h>

#include <Full/BMS.hpp>
#include <Full/EnergyMeter.hpp>
#include <Full/MotorController.hpp>
#include <Full/SMU.hpp>
#include <Full/VCU.hpp>

#ifdef DEBUG
#include <runtimeqml.hpp>
#endif

int main(int argc, char* argv[]) {
    QApplication app(argc, argv);
    std::string app_root = app.applicationDirPath().toStdString();
    // Should be before QQml engine
    real::MotorController* motor_controller = new real::MotorController(app_root + "/DBCs/20220510_Gen5_CAN_DB.dbc");
    real::EnergyMeter* energy_meter = new real::EnergyMeter(app_root + "/DBCs/Energy_Meter_CAN_Messages.dbc");
    real::BMS* bms = new real::BMS(app_root + "/DBCs/Orion_CANBUS.dbc");
    real::VCU* vcu = new real::VCU(app_root + "/TorqueMaps");
    real::SMU* smu = new real::SMU(app_root + "/DBCs/SMU_CANBUS.dbc");

    QQmlApplicationEngine engine;

    qmlRegisterSingletonInstance<real::MotorController>("CAN.MotorController", 1, 0, "MotorController", motor_controller);
    qmlRegisterSingletonInstance<real::EnergyMeter>("CAN.EnergyMeter", 1, 0, "EnergyMeter", energy_meter);
    qmlRegisterSingletonInstance<real::BMS>("CAN.BMS", 1, 0, "BMS", bms);
    qmlRegisterSingletonInstance<real::VCU>("CAN.VCU", 1, 0, "VCU", vcu);
    qmlRegisterSingletonInstance<real::SMU>("CAN.SMU", 1, 0, "SMU", smu);
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
