import QtQuick 2.0

Item {
    id: root
    property int squareSize: 70
    property alias iconPath: tile.source
    signal pressed(real x, real y)
    signal released(real x, real y)

    height: squareSize
    width : squareSize

    MouseArea {
        id: mouseArea
        anchors.fill: parent
        drag.target: tile

        onPressed: {
            root.pressed(parent.x / squareSize, parent.y / squareSize)
        }

        onReleased: {
            parent = tile.Drag.target !== null ? tile.Drag.target : root
            root.released((parent.x + mouseX) / squareSize, (parent.y + mouseY) / squareSize)
        }

        Image {
            id: tile

            height: squareSize
            width : squareSize

            anchors.verticalCenter: parent.verticalCenter
            anchors.horizontalCenter: parent.horizontalCenter

            Drag.active: mouseArea.drag.active
            Drag.hotSpot.x: squareSize / 2
            Drag.hotSpot.y: squareSize / 2
            states: State {
                when: mouseArea.drag.active
                ParentChange { target: tile; parent: root }
                AnchorChanges { target: tile; anchors.verticalCenter: undefined; anchors.horizontalCenter: undefined }
            }
        }

        Rectangle {
            anchors.fill: parent
            color: "gray"
            opacity: root.enabled ? 0 : 0.25
        }
    }
}
