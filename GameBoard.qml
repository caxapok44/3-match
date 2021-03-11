import QtQuick 2.0

import Game 1.0

GridView{
    id: root

    property bool isMoving: false
    property int prevIndex: -1

    interactive: false

    model: GameBoardModel {
    }
    cellHeight: height / root.model.row
    cellWidth: width / root.model.col

    delegate:Sphere{
        id: _sphere

        width: root.cellWidth
        height: root.cellHeight

        visible: y >= 0
        colorCircle: colorBall

        onBallClicked: {
            root.model.changeMovePhase(index)
            isMoving = !isMoving
            if (isMoving){
              root.prevIndex = index
            }
            else{
              root.prevIndex = -1
            }
        }
        SequentialAnimation on color{
            id: borColorAnim

            running: index === prevIndex ? true : false
            alwaysRunToEnd: true
            loops: Animation.Infinite

            ColorAnimation { from: "lightblue"; to: "red"; duration: 300;}
            ColorAnimation { from: "red"; to: "lightblue"; duration: 150;}
            PauseAnimation { duration: 250 }
        }
}
    populate: Transition {
        NumberAnimation{
            properties: "y"; from:0; duration: 300;
        }
    }

}
