import QtQuick 2.0
import Game 1.0
GridView{
    id: root
    property bool isMoving: false
    property real movingItemX: 0
    property real movingItemY: 0
    property int scoring: 0
    property int ind
    property int ind2
    model: GameBoardModel {
    }
    cellHeight: height / root.model.row
    cellWidth: width / root.model.col
    objectName: "gridview"


    delegate:
        Item{        
        id: _sphereWrapper
        width: root.cellWidth
        height: root.cellHeight

            Sphere{
                id: _sphere
                anchors.fill: _sphereWrapper
                colorCircle: colorBall
                objectName: index
                MouseArea{
                    anchors.fill: parent
                    onClicked:{
                        root.model.changeMovePhase(index)                        
                        scoring = root.model.score
                        isMoving = !isMoving
                        if (isMoving)
                        {
                          root.ind = index
                        }
                        else
                        {
                          root.ind = -1
                        }
                    }
                }
                SequentialAnimation on color{
                    id: borColorAnim
                    running: index === ind ? true : false
                    alwaysRunToEnd: true
                    loops: Animation.Infinite // The animation is set to loop indefinitely
                    ColorAnimation { from: "lightblue"; to: "red"; duration: 500;}
                    ColorAnimation { from: "red"; to: "lightblue"; duration: 500;}
                   // PauseAnimation { duration: 250 } // This puts a bit of time between the loop
                }
            }            
        }
    populate: Transition {
        NumberAnimation{
            properties: "y"; from:0; duration: 300;
        }
    }
}
