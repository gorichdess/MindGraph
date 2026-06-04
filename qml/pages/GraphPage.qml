import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import MindGraph

Page {
    id: graphPage

    property StackView stackViewRef

    background: Rectangle {
        color: Theme.backgroundColor
    }

    ColumnLayout {
        anchors.fill: parent
        anchors.margins: 16
        spacing: 12

        RowLayout {
            Layout.fillWidth: true
            spacing: 8

            AppTextField {
                id: noteTitleInput
                Layout.fillWidth: true
                height: 44
                placeholderText: "New note title"
                font.pointSize: 14
            }

            AppButton {
                text: "+"
                width: 56
                onClicked: {
                    graphController.createNote(noteTitleInput.text)
                    noteTitleInput.text = ""
                }
            }
        }

        Rectangle {
            Layout.fillWidth: true
            Layout.fillHeight: true
            radius: 20
            color: Theme.backgroundNodeColor
            border.color: Theme.borderColor

            Flickable {
                id: flickable
                anchors.fill: parent
                contentWidth: 1200
                contentHeight: 1200
                clip: true

                Item {
                    id: graphCanvas
                    width: flickable.contentWidth
                    height: flickable.contentHeight

                    Repeater {
                        model: notesModel

                        GraphNode {
                            noteId: model.noteId
                            title: model.title
                            content: model.content
                            x: model.noteX
                            y: model.noteY

                            onOpenRequested: function(noteId, title, content) {
                                graphPage.stackViewRef.push("NoteEditorPage.qml", {
                                    noteId: noteId,
                                    noteTitle: title,
                                    noteContent: content,
                                    stackViewRef: graphPage.stackViewRef
                                })
                            }
                        }
                    }
                }
            }
        }
    }
}
