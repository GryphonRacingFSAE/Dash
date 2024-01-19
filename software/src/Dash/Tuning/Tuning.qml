import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import CAN.VCU

Rectangle {
    id: root
    color: "black"
    

    readonly property int rows: 11
    readonly property int columns: 14
    readonly property int boxSize: 35

    property int selectedRow: 0
    property int selectedColumn: 0
    readonly property int selectedIndex: selectedRow*columns + selectedColumn // index counts down columns for whatever reason
    Keys.onLeftPressed: () => {
        if (selectedColumn == 0){
            selectedColumn = columns - 1;
        } else {
            selectedColumn--;
        }
    }
    Keys.onRightPressed: () => {
        if (selectedColumn == columns - 1){
            selectedColumn = 0;
        } else {
            selectedColumn++;
        }
    }
    Keys.onUpPressed: () => {
        if (selectedRow == 0){
            selectedRow = rows - 1;
        } else {
            selectedRow--;
        }
    }
    Keys.onDownPressed: () => {
        if (selectedRow == rows - 1){
            selectedRow = 0;
        } else {
            selectedRow++;
        }
    }
    Keys.onDigit1Pressed: () => {
        repeater.itemAt(selectedIndex).torque = Math.max(repeater.itemAt(selectedIndex).torque - 1, VCU.minTorque)
    }
    Keys.onDigit2Pressed: () => {
        repeater.itemAt(selectedIndex).torque = Math.min(repeater.itemAt(selectedIndex).torque + 1, VCU.maxTorque)
    }
    Keys.onPressed: (event) => {
        switch (event.key) {
            case Qt.Key_S: {
                // Array must be in row-major order to work as expected (each row is pedal position)
                let torque_map = [];
                for (let i = 0; i < rows * columns; i++) {
                    torque_map.push(repeater.itemAt(i).torque);
                }
                VCU.saveTorqueMapCSV(torque_map); 
                break;
            }
            case Qt.Key_Plus: VCU.profileId = Math.min(VCU.profileId + 1, 3); break;
            case Qt.Key_Minus: VCU.profileId = Math.max(VCU.profileId - 1, 0); break;
        }
    }
    Keys.onReturnPressed: () => {
        // Array must be in row-major order to work as expected (each row is pedal position)
        let torque_map = [];
        for (let i = 0; i < rows * columns; i++) {
            torque_map.push(repeater.itemAt(i).torque);
        }
        VCU.sendTorqueMap(torque_map);
    }

    Rectangle {
        id: box_matrix

        color: "white"
        anchors.centerIn: parent
        width: root.columns * boxSize
        height: root.rows * boxSize
        GridLayout {
            anchors.centerIn: parent
            width: root.columns * boxSize
            height: root.rows * boxSize

            rows: root.rows
            columns: root.columns

            columnSpacing: 0
            rowSpacing: 0
            Repeater {
                id: repeater
                model: VCU.currentTorqueMap
                Rectangle {
                    property int torque: modelData

                    property bool isSelected: index == root.selectedIndex
                    border {
                        color: "blue"
                        width: isSelected ? 3 : 0
                    }

                    Text {
                        font.family: "Consolas"
                        width: parent.width
                        height: parent.height

                        horizontalAlignment: Text.AlignHCenter
                        verticalAlignment: Text.AlignVCenter

                        text: repeater.itemAt(index).torque ?? "NULL"
                        font.pointSize: 10
                    }

                    Layout.fillWidth: true
                    Layout.fillHeight: true
                    color: {
                        if (torque < 0) {
                            return Qt.rgba(1, 0, 0, torque/VCU.minTorque)
                        } else {
                            return Qt.rgba(0, 1, 0, torque/VCU.maxTorque)
                        }
                    }
                }
            }
        }
    }

    // motor speed axis
    Row {
        width: box_matrix.width
        bottomPadding: 2

        anchors {
            bottom: box_matrix.top
            horizontalCenter: box_matrix.horizontalCenter
        }

        Repeater {
            model: 6500/500 + 1

            Text {
                font.family: "Consolas"
                required property int index

                width: parent.width/14
                horizontalAlignment: Text.AlignHCenter

                text: (index * 500) + ""
                color: "white"
            }
        }
    }

    // pedal travel axis
    Column {
        height: box_matrix.height
        rightPadding: 6

        anchors {
            right: box_matrix.left
            verticalCenter: box_matrix.verticalCenter
        }

        Repeater {
            model: 100/10 + 1

            Text {
                font.family: "Consolas"
                required property int index

                width: 20
                height: parent.height/11
                horizontalAlignment: Text.AlignRight
                verticalAlignment: Text.AlignVCenter

                text: (index*10) + ""
                color: "white"
            }
        }
    }

    // motor speed label
    Text {
        font.family: "Consolas"
        width: box_matrix.width
        height: 46

        anchors {
            bottom: box_matrix.top
            horizontalCenter: box_matrix.horizontalCenter
        }

        horizontalAlignment: Text.AlignHCenter

        text: "Motor Speed  [RPM]"
        font.pixelSize: 16
        font.bold: true
        color: "white"
    }

    // pedal travel label
    Text {
        font.family: "Consolas"
        height: box_matrix.height

        anchors {
            right: box_matrix.left
            verticalCenter: box_matrix.verticalCenter
        }

        verticalAlignment: Text.AlignVCenter

        text: "Pedal Travel  [%]"
        font.pixelSize: 16
        font.bold: true
        color: "white"

        rotation: -90
        transform: Translate {x: 26}
    }

    // profile indicators
    Text {
        font.family: "Consolas"
        width: parent.width

        anchors {
            top: box_matrix.bottom
            bottom: parent.bottom
        }

        horizontalAlignment: Text.AlignHCenter
        verticalAlignment: Text.AlignVCenter

        text: "Profile: #" + `${VCU.profileId}`
        color: "white"
        font.pixelSize: 24
        font.bold: true
    }
}