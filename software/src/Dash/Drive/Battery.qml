import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import CAN.BMS

Item {
    id: root
    property int circle: 25
    property int tickHeight: 20
    Rectangle{
        width: root.width
        height: root.height
        color: "black"


        RowLayout{
            anchors.verticalCenter: parent.verticalCenter
            anchors.horizontalCenter: parent.horizontalCenter
            spacing: 0

            //battery icon
            Image {
                source: "qrc:/images/BatterySymbol"
                fillMode: Image.PreserveAspectFit 
                Layout.preferredWidth: circle
                Layout.preferredHeight: circle
            }

            Repeater{
                id: batteryBar
                model: 20
                property int percent: 0
                Rectangle{
                    width: circle
                    height: circle
                    radius: 180

                    gradient: Gradient{
                        GradientStop{ position: 0.01; color: "white"}
                        GradientStop { position: 1; color: {
                                if(20 * batteryBar.percent/100 < index){
                                    return "red";
                                } else {
                                    return "yellow";
                                }
                            }
                        }
                    }
                }
            }

            //battery icon
            Image {
                source: "qrc:/images/BatterySymbol"
                fillMode: Image.PreserveAspectFit 
                Layout.preferredWidth: circle
                Layout.preferredHeight: circle
            }
        }

        //tick
        Rectangle{
            width: 2
            height: tickHeight
            radius: 90

            anchors{
                horizontalCenter: parent.horizontalCenter
                bottom: parent.bottom
                bottomMargin: 10
            }

            Rectangle{
                width: 2
                height: tickHeight
                radius: 90
                anchors{
                    right: parent.right
                    rightMargin: circle*5
                }
            }

            Rectangle{
                width: 2
                height: tickHeight
                radius: 90
                anchors{
                    right: parent.right
                    rightMargin: circle*10
                }
            }

            Rectangle{
                width: 2
                height: tickHeight
                radius: 90

                anchors{
                    left: parent.left
                    leftMargin: circle*5
                }
            }

            Rectangle{
                width: 2
                height: tickHeight
                radius: 90

                anchors{
                    left: parent.left
                    leftMargin: circle*10
                }
            }

        }
    }
    

    Connections {
        target: BMS
        function onNewStateOfCharge(percent) {
            batteryBar.percent = percent;
        }
    }
}