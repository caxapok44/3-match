import QtQuick 2.0

Rectangle{
    id: root
    property alias colorCircle: _circle.color

    color: "lightblue"

    border.color: "grey"
    border.width: 1
    Rectangle {
        id: _circle
        width: root.width
        height: root.width
        anchors.fill: parent
        anchors.margins: 5
        radius: Math.max(width/2, height / 2)
        border{
            color: "black"
            width: 2
        }
    }
}


