import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import CAN.SMU


Rectangle {  
    id: root 
    readonly property int boxSize: 25
    readonly property int rows: 2
    readonly property int columns: 28

    color: "white"
    width: columns * boxSize
    height: rows * boxSize

    required property int seg;

    GridLayout{
        width: root.columns * boxSize
        height: root.rows * boxSize
        anchors.centerIn: parent

        rows: root.rows
        columns: root.columns

        columnSpacing: 0
        rowSpacing: 0

        Repeater {
            id: repeater
            model: 56

            Rectangle{
                width: boxSize
                height: boxSize

                property int temp: SMU.temperatures[index + seg*56]
                Text{
                    font.family: "Consolas"
                    width: parent.width
                    height: parent.height
                    horizontalAlignment: Text.AlignHCenter
                    verticalAlignment: Text.AlignVCenter

                    text: repeater.itemAt(index).temp
                    font.pointSize: 10
                }


                color: {
                    if (repeater.itemAt(index).temp < 0) {
                        return Qt.rgba(0, 0, 1, repeater.itemAt(index).temp/-40)
                    } else {
                        return Qt.rgba(1, 0, 0, repeater.itemAt(index).temp/80)
                    }
                }
            }
        }
    }
}