import QtQuick
import QtQuick.Controls
import MindGraph

TextField {
    id: root

    height: 44
    font.pointSize: 14
    color: Theme.textColor
    placeholderTextColor: Theme.mutedTextColor
    selectedTextColor: Theme.accentTextColor
    selectionColor: Theme.accentColor

    background: Rectangle {
        radius: 12
        color: Theme.inputBackgroundColor
        border.width: 1
        border.color: root.activeFocus ? Theme.accentColor : Theme.borderColor
    }

    leftPadding: 14
    rightPadding: 14
}