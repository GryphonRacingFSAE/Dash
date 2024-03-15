import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

Rectangle{
    required property string label;
    property double value;
    property double high;
    property double low;
    property string lineColour;
    color: "black"

    border{
        color: "white"
        width: 2
    }
    //value
    Text{
        id: displayValue
        text: ""
        font.family: "Consolas"
        font.pointSize: 30
        font.bold: true
        color: "white"

        anchors{
            horizontalCenter: parent.horizontalCenter
            verticalCenter: parent.verticalCenter
        }
    }

    //label
    Text{
        text: label
        font.family: "Consolas"
        font.pointSize: 15
        color: "white"

        anchors{
            horizontalCenter: parent.horizontalCenter
            bottom: parent.bottom
        }
    }

    gradient: Gradient{
        GradientStop { position: 0; color: {
            if(value >= high){
                return "red"
            } else {
                return "green"
            }
        }}
        GradientStop{ position: 1; color: "black"}
    }

    onValueChanged: () => {
        displayValue.text = value.toFixed(1)
    }
}