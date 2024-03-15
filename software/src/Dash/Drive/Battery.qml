import QtQuick

Rectangle {
    id: battery
    radius:height/10
    property double percent: 0

    onPercentChanged: () => {
        if (percent <= 20) {
            color = "red"
        } else if (percent <= 40) {
            color = "yellow"
        } else {
            color = "green"
        }
    }

    //Battery inside border
    Rectangle {
        color: "white"
        radius:height/12
        anchors {
            fill: parent
            margins:5
        }
    }

    //Responsive battery display (changes width & color based on battery percentage)
    Rectangle {
        color: battery.color
        width: (battery.width-20)/100*percent
        radius:height/14
        anchors {
            top: parent.top
            bottom: parent.bottom
            left: parent.left
            margins:10
        }
    }

    //Battery icon
    Image {
        id: batteryIcon
        source: "qrc:/images/BatterySymbol"
        width: battery.height/3
        height: 2*width
        anchors {
            bottom: parent.bottom
            left: parent.left
            margins:15
        }
    }

    //Battery text
    Text {
        font.pointSize: battery.height/3
        color: "black"
        font.bold: true
        text: `${percent.toFixed(1)}%`

        anchors {
            verticalCenter: batteryIcon.verticalCenter
            left: batteryIcon.right
            margins: 15
        }
    }
}