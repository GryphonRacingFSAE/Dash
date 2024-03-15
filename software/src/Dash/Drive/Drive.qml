import QtQuick
import QtQuick.Layouts
import CAN.MotorController
import CAN.BMS
import CAN.EnergyMeter
import CAN.VCU

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
                value: 0
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
                value:43
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
                value: 45
            }
        }

        //Speed
        ColumnLayout {
            Layout.preferredWidth: parent.width/16*6
            anchors.verticalCenter: parent.verticalCenter
            
            Rectangle{
                id: rightPanel
                width: 200
                height: 300
                color: "white"
                anchors.verticalCenter: parent.verticalCenter
                // pedal position displays
                RowLayout{
                    spacing: 10
                    anchors.verticalCenter: parent.verticalCenter
                    PedalPosBox{
                        id: brakeBox
                        pedal: "brake"
                        width: 60
                        height: 300
                        borderWidth:2
                        lineColour: "red"
                        letter: "B"
                        value: 0
                    }

                    PedalPosBox{
                        id: acceleratorBox
                        pedal: "gas"
                        width: 60
                        height: 300
                        borderWidth:2
                        lineColour: "green"
                        letter: "A"
                        value: 0
                    }
                    //tick marks
                    Rectangle{
                        width: 60
                        height: 300
                        color: "white"
                        // border{
                        //     width: 2
                        //     color: "white"
                        // }
                        Repeater{
                            id: ticks
                            model: 11
                            Text{
                                text: `${(index)*10}`+"%"
                                font.family: "Consolas"
                                font.pointSize: 10
                                font.bold: true
                                color: "black"

                                anchors{
                                    left: parent.left
                                    bottom: parent.bottom
                                    bottomMargin: (parent.height) * (index)*10/100 - 7
                                }
                            }
                        }
                    }
                }
            }
        }

        Timer{
            id: loop
            interval: 1 
            running: true
            repeat: true

            property double accTorq: 0
            property double brakeTorq: 0

            property double torque: 0.0;
            property double acceleration: 0.0;
            property double velocity: 0.0;
            property double forceDrag: 0.0;
            property double gearRatio: 3.5;
            property double tireRadius: 0.203;
            property double carMass: 200;
            property double timePeriod: 0.001;

            

            onTriggered: {
                forceDrag = 0.02*carMass*9.81 + 0.5*1.5*1.3*0.3*velocity*velocity;
                acceleration = ((accTorq*gearRatio - brakeTorq)/tireRadius - forceDrag) / carMass;
                velocity += timePeriod*acceleration;

                if(velocity >= 0){
                    speedBox.value = velocity/1000*3600;
                } else {
                    speedBox.value = 0;
                }
            }
        }

    }

    RowLayout {
        id: test
        height: root.height/6
        anchors {
            top: parent.top
            left: parent.left
            right: parent.right
            topMargin: 50
            leftMargin: 10
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

    RowLayout {
        anchors {
            top: parent.top
            horizontalCenter: parent.horizontalCenter
        }

        Rectangle{
            width: 25
            height: 25
            radius: 25
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
        }
        function onNewCoolantTemp(temp) {
            coolantTempBox.value = 45
        }
        function onNewAnalogInput2(temp) { // oil temp
            oilTempBox.value = 43
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
            battery_bar.percent = 87.2
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

    Connections{
        target: VCU
        function onNewAcceleratorPos(pos){
            loop.accTorq = 150*pos/100/2.8
        }

        function onNewBrakePressure(psi){
            loop.brakeTorq = 500*psi/200
        }
    }
}