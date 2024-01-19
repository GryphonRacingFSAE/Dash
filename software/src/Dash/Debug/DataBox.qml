import QtQuick

Item {
    required property string title;
    property double value;
    property int precision;
    property bool is_fault: false;

    width: parent.width
    height: 18

    onValueChanged: () => {
        display_value.text = value.toFixed(1) // TODO: update individual precision values
        if (is_fault) {
            if (value == 0.0) {
                display_value.color = "green"
            } else {
                display_value.color = "red"
            }
        }
    }

    Text {
        font.family: "Consolas"
        text: parent.title + ":  "
        font.pointSize: 10
        color: "white"
    }

    Text {
        id: display_value
        font.family: "Consolas"
        text: ""
        font.pointSize: 10
        font.bold: true
        color: "green"
        
        anchors {
            right: parent.right
        }
    }
}