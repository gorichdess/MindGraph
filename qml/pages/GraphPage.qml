import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import MindGraph

Page {
    id: graphPage

    property StackView stackViewRef

    property string graphMode: "none" // none, link, unlink
    property string firstSelectedNoteId: ""

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
                text: graphPage.graphMode === "link" ? "Link: ON" : "Link"
                width: 100
                secondary: graphPage.graphMode !== "link"

                onClicked: {
                    graphPage.graphMode = graphPage.graphMode === "link" ? "none" : "link"
                    graphPage.firstSelectedNoteId = ""
                }
            }

            AppButton {
                text: graphPage.graphMode === "unlink" ? "Unlink: ON" : "Unlink"
                width: 120
                secondary: graphPage.graphMode !== "unlink"
                danger: graphPage.graphMode === "unlink"

                onClicked: {
                    graphPage.graphMode = graphPage.graphMode === "unlink" ? "none" : "unlink"
                    graphPage.firstSelectedNoteId = ""
                }
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
                        model: edgesModel

                        GraphEdge {
                            fromX: model.fromX
                            fromY: model.fromY
                            toX: model.toX
                            toY: model.toY
                        }
                    }

                    Repeater {
                        model: notesModel

                        GraphNode {
                            noteId: model.noteId
                            title: model.title
                            content: model.content
                            x: model.noteX
                            y: model.noteY

                            linkMode: graphPage.graphMode !== "none"
                            selectedForLink: graphPage.firstSelectedNoteId === model.noteId

                            onNodeClicked: function(noteId) {
                                if (graphPage.graphMode === "none") {
                                    return
                                }

                                if (graphPage.firstSelectedNoteId === "") {
                                    graphPage.firstSelectedNoteId = noteId
                                    return
                                }

                                if (graphPage.firstSelectedNoteId === noteId) {
                                    graphPage.firstSelectedNoteId = ""
                                    return
                                }

                                if (graphPage.graphMode === "link") {
                                    graphController.createEdge(graphPage.firstSelectedNoteId, noteId)
                                }

                                if (graphPage.graphMode === "unlink") {
                                    graphController.deleteEdgeBetween(graphPage.firstSelectedNoteId, noteId)
                                }

                                graphPage.firstSelectedNoteId = ""
                            }

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
