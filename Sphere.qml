import QtQuick 2.0

Rectangle{
    id: root
    property alias colorCircle: _circle.color

    signal ballClicked()

    color: "lightblue"
    border.color: "grey"
    border.width: 1

    Rectangle {
        id: _circle
        anchors.fill: parent
        anchors.margins: 5

        radius: Math.max(width/2, height / 2)
        border{
            color: "black"
            width: 2
        }        
    }
    MouseArea{
        anchors.fill: parent

        onClicked: root.ballClicked()
    }
}


