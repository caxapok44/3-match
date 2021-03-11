import QtQuick 2.12
import QtQuick.Window 2.12
import QtQuick.Controls 2.12


ApplicationWindow {
    id: root

    height: 600
    minimumHeight: 600
    maximumHeight: 600
    width: 1200
    minimumWidth: 1200
    maximumWidth: 1200

    visible: true
    title: qsTr("3-match")

    background: Image{
        anchors.fill: parent

        source: "Resource/bg.png"
    }

    Score {
        id: _score

        anchors.horizontalCenter: _gameEngine.horizontalCenter

        width: root.width *0.1
        height: root.height * 0.08

        score: _gameEngine.model.score
    }

    GameBoard{
        id: _gameEngine

        width: root.width * 0.9
        height: root.height * 0.9

        anchors.top: _score.bottom

        anchors.horizontalCenter:  parent.horizontalCenter

    }


}
