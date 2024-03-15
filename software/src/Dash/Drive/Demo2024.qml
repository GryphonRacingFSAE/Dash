import QtQuick
import QtQuick.Layouts
import CAN.MotorController
import CAN.BMS
import CAN.VCU
import CAN.EnergyMeter

Item {
    id: root
    property int sideWidth: 200
    property int middleHeight: 450

    Rectangle{
        height: parent.height
        width: parent.width
        color: "black"

        ColumnLayout{
            anchors{
                left: parent.left
                leftMargin: 10
            }
            spacing: 5
            //top row
            RowLayout{
                anchors.horizontalCenter: parent.horizontalCenter
                Battery{
                    id: battery
                    width: 40*9
                    height: 40
                }
            }

            //middle row
            RowLayout{
                anchors.horizontalCenter: parent.horizontalCenter
                
                spacing: 5

                //left panel
                Rectangle{
                    id: leftPanel
                    width: root.sideWidth
                    height: root.middleHeight
                    color: "black"
                    // border{
                    //     width: 2
                    //     color: "white"
                    // }
                    ColumnLayout{
                        width: parent.width
                        //label
                        Text{
                            text: "TEMPS"
                            font.family: "Consolas"
                            font.pointSize: 20
                            font.bold: true
                            color: "white"
                            anchors{
                                horizontalCenter: parent.horizontalCenter
                            }
                        }

                        //layout for boxes
                        GridLayout{
                            anchors.horizontalCenter: parent.horizontalCenter
                            height: parent.height
                            columns: 1
                            rowSpacing: 40

                            DisplayBox{
                                id: motorTempBox
                                width: root.sideWidth/1.3
                                height: root.sideWidth/1.3
                                label: "Motor"
                                
                                high: 90
                            }

                            DisplayBox{
                                id: accTempBox
                                width: root.sideWidth/1.3
                                height: root.sideWidth/1.3
                                label: "Accumul."
                                
                                high: 90
                            }
                        }
                    }
                }

                //middle panel
                Rectangle{
                    id: middlePanel
                    width: 400
                    height: root.middleHeight
                    color: "black"
                    // border{
                    //     width: 2
                    //     color: "white"
                    // }

                    

                    Text{
                        id: speedBox
                        text: "0"
                        font.family: "Consolas"
                        font.pointSize: 125
                        font.bold: true
                        color: "white"

                        anchors{
                            horizontalCenter: parent.horizontalCenter
                            top: parent.top
                            topMargin: 30
                        }

                        Text{
                            text: "kmph"
                            font.family: "Consolas"
                            font.pointSize: 25
                            font.bold: true
                            color: "white"
                            anchors{
                                horizontalCenter: parent.horizontalCenter
                                top: parent.bottom
                            }
                        }
                    }

                    //current lap
                    Rectangle{
                        height: 170
                        width: 350
                        color: "black" 
                        anchors.bottom: parent.bottom
                        anchors.horizontalCenter: parent.horizontalCenter
                        // border{
                        //     color: "white"
                        //     width: 2
                        // }

                        Text{
                            id: lapDiff
                            text: "0.0"
                            color: "green"
                            font.family: "Consolas"
                            font.pointSize: 75
                            anchors{
                                verticalCenter: parent.verticalCenter
                                horizontalCenter: parent.horizontalCenter
                            }
                        }

                        Text{
                            id: currentLapBox
                            text: "00:00:00"
                            color: "white"
                            font.family: "Consolas"
                            font.pointSize: 25
                            font.bold: true
                            anchors{
                                right: parent.right
                                top: parent.top
                                rightMargin: 5
                                topMargin: 3
                            }
                        }

                        Text{
                            text: "Current"
                            color: "white"
                            font.family: "Consolas"
                            font.pointSize: 25
                            font.bold: true
                            anchors{
                                left: parent.left
                                top: parent.top
                                leftMargin: 5
                                topMargin: 3
                            }
                        }

                        Text{
                            id: bestLapBox
                            text: "00:00:00"
                            color: "white"
                            font.family: "Consolas"
                            font.pointSize: 25
                            font.bold: true
                            anchors{
                                right: parent.right
                                bottom: parent.bottom
                                rightMargin: 5
                                bottomMargin: 3
                            }
                        }

                        Text{
                            text: "Best"
                            color: "white"
                            font.family: "Consolas"
                            font.pointSize: 25
                            font.bold: true
                            anchors{
                                left: parent.left
                                bottom: parent.bottom
                                leftMargin: 5
                                bottomMargin: 3
                            }
                        }
                    }
                }

                //right panel
                Rectangle{
                    id: rightPanel
                    width: root.sideWidth
                    height: root.middleHeight
                    color: "black"
                    // pedal position displays
                    RowLayout{
                        spacing: 10
                        anchors.horizontalCenter: parent.horizontalCenter
                        PedalPosBox{
                            id: breakBox
                            width: 60
                            height: root.middleHeight
                            borderWidth:2
                            lineColour: "red"
                            letter: "B"
                        }

                        PedalPosBox{
                            id: acceleratorBox
                            width: 60
                            height: root.middleHeight
                            borderWidth:2
                            lineColour: "green"
                            letter: "A"
                        }
                        //tick marks
                        Rectangle{
                            width: 60
                            height: root.middleHeight
                            color: "black"
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
                                    color: "white"

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
        }
    }

    // Connections {
    //     target: MotorController
    //     function onNewMotorSpeed(speed) {
    //         let gear_ratio = 1/3.48; // 3.48:1 gear ratio
    //         let wheel_rpm = speed * gear_ratio;
    //         let wheel_diameter_inch = 16; // 16" OD
    //         let wheel_circumfrence_m = wheel_diameter_inch * 0.0254 * 3.14; // inch -> m = inch * 0.0254
    //         let wheel_surface_speed_mpm = wheel_circumfrence_m * wheel_rpm;
    //         let wheel_surface_speed_kmph = wheel_surface_speed_mpm / 1000 * 60 // m/min -> km/h = m / 1000 * 60,

    //         speedBox.value = `${10}` //wheel_surface_speed_kmph.toFixed(0)
    //     }
    //     function onNewCoolantTemp(temp) {
    //         coolantTempBox.value = temp
    //     }
    //     function onNewAnalogInput2(temp) { // oil temp
    //         oilTempBox.value = temp
    //     }
    //     function onNewMotorTemp(temp) {
    //         motorTempBox.value = temp
    //     }
    //     function onNewAnalogInput1(voltage) // 12V voltage
    //     {
    //     }

    //     function onNewRequestedTorque(torque)
    //     {
    //         if (torque < 0)
    //         {
    //             torqueContainer.color = "red"
    //         }
    //         else if(torque > 0)
    //         {
    //             torqueContainer.color = "green"
    //         }
    //         else{
    //             torqueContainer.color = "grey"
    //         }

    //         torqueValue.text = `${torque.toFixed(0)}`
    //     }

    //     function onNewOutputTorque(torque) {
    //         if (torque < 0)
    //         {
    //             torqueContainer2.color = "red"
    //         }
    //         else if(torque > 0)
    //         {
    //             torqueContainer2.color = "green"
    //         }
    //         else{
    //             torqueContainer2.color = "grey"
    //         }

    //         torqueValue2.text = `${torque.toFixed(0)}`
    //     }
    // }

    Connections {
        target: MotorController
        function onNewMotorTemp(temp) {
           motorTempBox.value = temp;
        }
    }

    Connections{
        target: BMS
        function onNewHighestTemp(temp) {
            accTempBox.value = temp;
        }
    }
}