import QtQuick

Item {
    required property string title;
    required property double fontSize;
    required property double low;
    required property double high;
    required property int precision;
    property double value;

    onValueChanged: () => {
        display_value.text = value.toFixed(precision)
        if (title == "Speed") {
            display_value.color = "black"
        } else if (value < low) {
            display_value.color = "blue"
        } else if (value > high) {
            display_value.color = "red"
        } else {
            display_value.color = "green"
        }
    }

    //display text
    Text {
        font.family: "Consolas"
        
        font.pointSize: parent.fontSize * 2/3
        text: parent.title
        color: "black"
        font.bold: true
        anchors {
            bottom: parent.verticalCenter
            horizontalCenter: parent.horizontalCenter
        }
    }

    //value text
    Text {
        id: display_value
        font.family: "Consolas"
        font.pointSize: parent.fontSize * 20 / 8
        text: ""
        color: "black"
        font.bold: true
        anchors {
            top: parent.verticalCenter
            horizontalCenter: parent.horizontalCenter
        }
    }
}