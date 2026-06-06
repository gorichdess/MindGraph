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

    signal openRequested(string noteId, string title, string content)
    signal nodeClicked(string noteId)

    width: 140
    height: 72
    radius: 16
    color: Theme.cardColor
    border.color: root.selectedForLink ? Theme.selectedNodeBorderColor : Theme.borderColor
    border.width: root.selectedForLink ? 3 : 11

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

            onClicked: {
                root.nodeClicked(root.noteId)
            }

            onDoubleClicked: {
                if (!root.linkMode) {
                    root.openRequested(root.noteId, root.title, root.content)
                }
            }

            onReleased: {
                graphController.updateNotePosition(root.noteId, root.x, root.y)
            }
        }
}
