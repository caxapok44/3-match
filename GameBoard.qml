import QtQuick 2.12
import Game 1.0

GridView{
    id: root

    property bool isMoving: false
    property int prevIndex: -4

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
              root.prevIndex = -4
              borColorAnim.complete()
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
            property: "y"; from:0; duration: 300;
        }
    }
    move: Transition{
        id: _moveTrans
        NumberAnimation{
            properties: "x,y"
            duration: 500
        }
    }
    remove: Transition{
        id: _removeTrans

        SequentialAnimation{
            PauseAnimation {
                duration: 500
            }
            ParallelAnimation
            {
                NumberAnimation{
                    id: _numAni
                    property: "opacity"
                    to: 0
                    duration: 500
                }
                NumberAnimation{
                    property: "scale"
                    from:1
                    to: 0
                    duration: 500
                }
            }
        }
    }
    add: Transition{
        id: _addTrans
        SequentialAnimation{
            PauseAnimation {
                duration: 1000
            }
            ParallelAnimation
            {
                NumberAnimation{
                    property: "y"; from:0;  duration: 1000;
                }
                NumberAnimation{
                    property: "opacity"; from:0; to: 1; duration: 1000;
                }
            }


        }
    }
}
