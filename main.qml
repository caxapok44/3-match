import QtQuick 2.12
import QtQuick.Window 2.12

Window {
    width: 1200
    height: 600
    visible: true
    minimumHeight: 600
    minimumWidth: 1200
    maximumHeight: 600
    maximumWidth: 1200

    title: qsTr("3-match")
    Image {
        id: root
        anchors.fill: parent
        source: "Resource/bg.png"
        Rectangle
        {
            id: _score
            width: root.width *0.1
            height: _wrapperGameBoard.height * 0.08

            anchors.bottom: _wrapperGameBoard.top
            anchors.horizontalCenter: _wrapperGameBoard.horizontalCenter
            anchors.bottomMargin: 4

            border.color: "red"
            border.width: 4
            radius: height / 2
            color: "white"
            Text {
                id: name
                anchors.centerIn: _score
                font.bold: true
                font.pointSize: parent.height / 2
                color: "red"
                text: _gameEngine.scoring
            }
        }

        Rectangle
        {
            id: _wrapperGameBoard
            opacity: 0.95
            width: root.width * 0.9
            height: root.height * 0.9

            anchors.horizontalCenter: root.horizontalCenter
            anchors.bottom: root.bottom

            border.color: "darkblue"
            border.width: 4
            GameBoard{
                id: _gameEngine
                anchors.fill: _wrapperGameBoard
                anchors.margins: _wrapperGameBoard.border.width
                anchors.bottomMargin: 0
            }
        }
    }

}
