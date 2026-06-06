import QtQuick
import QtQuick.Controls
import MindGraph

Rectangle {
    id: root

    property string noteId: ""
    property string title: ""
    property string content: ""

    property bool linkMode: false
    property bool selectedForLink: false

    property real pressX: 0
    property real pressY: 0
    property bool wasDragged: false

    signal openRequested(string noteId, string title, string content)
    signal nodeClicked(string noteId)

    width: 140
    height: 72
    radius: 16

    color: root.selectedForLink ? Theme.selectedNodeBackgroundColor : Theme.cardColor

    border.color: {
        if (root.selectedForLink)
            return Theme.selectedNodeBorderColor

        if (root.linkMode)
            return Theme.linkModeBorderColor

        return Theme.borderColor
    }

    border.width: root.selectedForLink ? 4 : (root.linkMode ? 2 : 1)

    AppText {
        id: titleText
        text: root.title
        anchors.centerIn: parent
        width: parent.width - 20
        horizontalAlignment: Text.AlignHCenter
        elide: Text.ElideRight
        font.bold: true
    }

    MouseArea {
        anchors.fill: parent
        drag.target: root
        drag.minimumX: 0
        drag.minimumY: 0
        drag.maximumX: root.parent ? root.parent.width - root.width : 1000
        drag.maximumY: root.parent ? root.parent.height - root.height : 1000

        onPressed: {
            root.pressX = root.x
            root.pressY = root.y
            root.wasDragged = false
        }

        onPositionChanged: {
            if (Math.abs(root.x - root.pressX) > 3 || Math.abs(root.y - root.pressY) > 3) {
                root.wasDragged = true
            }
        }

        onClicked: {
            if (!root.wasDragged) {
                root.nodeClicked(root.noteId)
            }
        }

        onDoubleClicked: {
            if (!root.linkMode && !root.wasDragged) {
                root.openRequested(root.noteId, root.title, root.content)
            }
        }

        onReleased: {
            if (root.wasDragged) {
                graphController.updateNotePosition(root.noteId, root.x, root.y)
            }
        }
    }
}