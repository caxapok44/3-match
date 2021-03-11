import QtQuick 2.12
import QtQuick.Controls 2.12
import QtQuick.Window 2.12


Rectangle{
    id: root
    property alias score: _name.text

    border.color: "red"
    border.width: 4
    radius: height / 2

    color: "white"

    Text {
        id: _name

        anchors.centerIn: root

        font.bold: true
        font.pointSize: parent.height / 2
        color: "red"
    }
}
