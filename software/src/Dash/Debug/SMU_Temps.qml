import QtQuick
import QtQuick.Controls
import QtQuick.Layouts


Rectangle {
    id: root
    color: "black"
    
    readonly property int segments: 5

    Column{
        anchors.centerIn: parent
        spacing: 10
        Text{
            anchors.horizontalCenter: parent.horizontalCenter

            font.family: "Consolas"
            font.pixelSize: 24
            font.bold: true
            color: "white"
            text: "Thermistor Tempuratures"
        }

        Repeater{
            id: repeater
            model: segments

            Column{
                property int num: modelData
                spacing: 0
                Text{
                    font.family: "Consolas"
                    font.pointSize: 10
                    color: "white"
                    text: "Segment #" + `${num}`
                }
                Segment{
                    type: 2 //type 2 = temperatures from smu
                    segment: num
                    max: 100
                    min: 1
                    boxSize: 25
                    rows: 2
                    columns: 28
                }
            }
        }
    }
}