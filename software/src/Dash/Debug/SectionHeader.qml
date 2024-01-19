import QtQuick

Item {
    id: root

    required property string title;

    width: parent.width
    height: 40

    Column {
        width: parent.width
        height: parent.height

        Rectangle {
            width: parent.width
            height: 12
            color: "black"
        }
        Text {
            font.family: "Consolas"
            text: root.title
            font.pointSize: 12
            font.bold: true
            color: "white"
        }
        Rectangle {
            width: parent.width
            height: 1
            color: "black"
        }
        Rectangle {
            width: parent.width
            height: 1
            color: "white"
        }
        Rectangle {
            width: parent.width
            height: 14
            color: "black"
        }
    }
}