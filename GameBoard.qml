import QtQuick 2.0
import Game 1.0
GridView{
    id: root
    property bool isMoving: false
    property real movingItemX: 0
    property real movingItemY: 0
    property int scoring: 0
    property int ind
    model: GameBoardModel {
    }
    cellHeight: height / root.model.raw
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
                MouseArea{
                    anchors.fill: parent
                    onClicked:{
                        root.model.changeMovePhase(index)                        
                        scoring = root.model.score
                    }
                }
            }

            NumberAnimation {
                target: _sphereWrapper
                property: "name"
                duration: 200
                easing.type: Easing.InOutQuad
            }
        }
}
