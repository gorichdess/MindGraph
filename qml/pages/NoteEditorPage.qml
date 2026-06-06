import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import MindGraph

Page {
    id: noteEditorPage

    property StackView stackViewRef
    property string noteId: ""
    property string noteTitle: ""
    property string noteContent: ""

    background: Rectangle {
        color: Theme.backgroundColor
    }

    ColumnLayout {
        anchors.fill: parent
        anchors.margins: 16
        spacing: 12

        RowLayout {
            Layout.fillWidth: true

            AppButton {
                text: "Back"
                secondary: true
                width: 90
                onClicked: noteEditorPage.stackViewRef.pop()
            }

            Item {
                Layout.fillWidth: true
            }

            AppButton {
                text: "Delete"
                danger: true
                onClicked: {
                    graphController.deleteNote(noteEditorPage.noteId)
                    noteEditorPage.stackViewRef.pop()
                }
            }
        }

        AppTextField {
            id: titleField
            Layout.fillWidth: true
            height: 48
            text: noteEditorPage.noteTitle
            placeholderText: "Title"
            font.pointSize: 16
        }

        AppTextArea {
            id: contentArea
            Layout.fillWidth: true
            Layout.fillHeight: true
            text: noteEditorPage.noteContent
            placeholderText: "Write your note here..."
            wrapMode: TextArea.Wrap
            font.pointSize: 14
        }

        RowLayout {
            Layout.fillWidth: true
            spacing: 8

            AppTextField {
                id: linkedNoteTitleInput
                Layout.fillWidth: true
                placeholderText: "Linked note title"
            }

            AppButton {
                text: "Create linked note"
                width: 170

                onClicked: {
                    graphController.createLinkedNote(
                        noteEditorPage.noteId,
                        linkedNoteTitleInput.text
                    )

                    linkedNoteTitleInput.text = ""
                }
            }
        }

        AppButton {
            text: "Save"
            Layout.fillWidth: true
            onClicked: {
                graphController.updateNote(noteEditorPage.noteId, titleField.text, contentArea.text)
                noteEditorPage.stackViewRef.pop()
            }
        }
    }
}
