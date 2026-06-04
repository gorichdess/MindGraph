import QtQuick
import QtQuick.Controls
import MindGraph

TextArea {
    id: root

    font.pointSize: 14
    color: Theme.textColor
    placeholderTextColor: Theme.mutedTextColor
    selectedTextColor: Theme.accentTextColor
    selectionColor: Theme.accentColor
    wrapMode: TextArea.Wrap

    background: Rectangle {
        radius: 12
        color: Theme.inputBackgroundColor
        border.width: 1
        border.color: root.activeFocus ? Theme.accentColor : Theme.borderColor
    }

    leftPadding: 14
    rightPadding: 14
    topPadding: 12
    bottomPadding: 12
}