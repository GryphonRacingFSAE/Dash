import QtQuick
import QtQuick.Layouts
import CAN.MotorController
import CAN.BMS
import CAN.EnergyMeter

Item {
    id: root
    RowLayout {
        id: main
        anchors {
            top: test.bottom
            bottom: root.bottom
            left: parent.left
            right: parent.right
            bottomMargin: 30
        }



        //Motor and Inverter
        ColumnLayout {
            Layout.preferredWidth: parent.width/16*5

            DataBox {
                Layout.fillWidth: true
                Layout.fillHeight: true
                id: motorTempBox
                title: "Motor Temp"
                fontSize: root.height/20
                precision: 0;
                low: 30
                high: 75
            }

            DataBox {
                Layout.fillWidth: true
                Layout.fillHeight: true
                id: inverterTempBox
                title: "Inverter Temp"
                fontSize: root.height/20
                precision: 0;
                low: 20
                high: 40
            }
        }


        //Speed and Accum Temp
        ColumnLayout {
            Layout.preferredWidth: parent.width/16*5

            DataBox {
                Layout.fillWidth: true
                Layout.fillHeight: true
                id: speedBox
                title: "Speed"
                fontSize: root.height/20
                precision: 0
            }

            DataBox {
                Layout.fillWidth: true
                Layout.fillHeight: true
                id: accumTempBox
                title: "Accum Temp"
                fontSize: root.height/20
                precision: 0;
                low: 25
                high: 60
            }
        }

        //Oil and Coolant
        ColumnLayout {
            Layout.preferredWidth: parent.width/16*5

            DataBox {
                Layout.fillWidth: true
                Layout.fillHeight: true
                id: oilTempBox
                title: "Oil Temp"
                fontSize: root.height/20
                precision: 0;
                low: 30
                high: 70
            }
            DataBox {
                Layout.fillHeight: true
                Layout.fillWidth: true
                id: coolantTempBox
                title: "Coolant Temp"
                fontSize: root.height/20
                precision: 0;
                low: 25
                high: 60
            }
        }

        //Speed
        ColumnLayout {
            Layout.preferredWidth: parent.width/16*6

            Rectangle {
                id: torqueContainer
                color: "grey"
                radius: height/3
                Layout.fillWidth: true;
                implicitHeight: main.height/10

                Text{
                    id: torqueValue
                    text: ""
                    font.bold: true
                    color:"black"
                    font.pointSize: main.height/15
                    anchors{
                        horizontalCenter: parent.horizontalCenter
                        verticalCenter: parent.verticalCenter
                    }
                }
            }

            Rectangle {
                id: torqueContainer2
                color: "grey"
                radius: height/3
                Layout.fillWidth: true;
                implicitHeight: main.height/10

                Text{
                    id: torqueValue2
                    text: ""
                    font.bold: true
                    color:"black"
                    font.pointSize: main.height/15
                    anchors{
                        horizontalCenter: parent.horizontalCenter
                        verticalCenter: parent.verticalCenter
                    }
                }
            }
            Item {
                Layout.fillWidth: true
                implicitHeight: 160
                Text {
                    id: speedValue
                    font.pointSize: main.height/3.5
                    opacity: 0.9
                    font.bold: true
                    color:"black"
                    text: ""
                    anchors{
                        top: parent.top
                        topMargin: main.height/10
                        horizontalCenter: parent.horizontalCenter
                    }
                }

                Text {
                    font.family: "Consolas"
                    text: "kmph"
                    font.bold: true
                    color:"black"
                    font.pointSize: main.height/15
                    anchors{
                        top:speedValue.bottom
                        horizontalCenter: parent.horizontalCenter
                        topMargin: - main.height/15
                    }
                }
            }
        }

    }

    RowLayout {
        id: test
        height: root.height/5
        anchors {
            top: parent.top
            left: parent.left
            right: parent.right
            margins: 10
        }
        Battery {
            id: battery_bar
            Layout.fillHeight: true
            Layout.fillWidth: true
        }
        //Logo
        Image {
            id: logo
            source:"qrc:/images/Logo"
            Layout.fillHeight: true
            Layout.preferredWidth: sourceSize.width/sourceSize.height * height
        }
    }

    Connections {
        target: MotorController
        function onNewMotorSpeed(speed) {
            let gear_ratio = 1/3.48; // 3.48:1 gear ratio
            let wheel_rpm = speed * gear_ratio;
            let wheel_diameter_inch = 16; // 16" OD
            let wheel_circumfrence_m = wheel_diameter_inch * 0.0254 * 3.14; // inch -> m = inch * 0.0254
            let wheel_surface_speed_mpm = wheel_circumfrence_m * wheel_rpm;
            let wheel_surface_speed_kmph = wheel_surface_speed_mpm / 1000 * 60 // m/min -> km/h = m / 1000 * 60,

            speedBox.value = `${wheel_surface_speed_kmph.toFixed(0)}`
        }
        function onNewCoolantTemp(temp) {
            coolantTempBox.value = temp
        }
        function onNewAnalogInput2(temp) { // oil temp
            oilTempBox.value = temp
        }
        function onNewMotorTemp(temp) {
            motorTempBox.value = temp
        }
        function onNewAnalogInput1(voltage) // 12V voltage
        {
        }

        function onNewRequestedTorque(torque)
        {
            if (torque < 0)
            {
                torqueContainer.color = "red"
            }
            else if(torque > 0)
            {
                torqueContainer.color = "green"
            }
            else{
                torqueContainer.color = "grey"
            }

            torqueValue.text = `${torque.toFixed(0)}`
        }

        function onNewOutputTorque(torque) {
            if (torque < 0)
            {
                torqueContainer2.color = "red"
            }
            else if(torque > 0)
            {
                torqueContainer2.color = "green"
            }
            else{
                torqueContainer2.color = "grey"
            }

            torqueValue2.text = `${torque.toFixed(0)}`
        }
    }

    Connections {
        target: BMS
        function onNewStateOfCharge(percent) {
            battery_bar.percent = percent
        }
        function onNewAvgTemp(temp) {}
        function onNewHighestTemp(temp) {
            inverterTempBox.value = temp
            accumTempBox.value = temp
        }
        function onNewAvgPackCurrent(current) {}
        function onNewVoltage(voltage) {}
        function onNewOpenVoltage(voltage){}
    }
}