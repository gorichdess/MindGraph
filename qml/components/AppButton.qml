import QtQuick
import QtQuick.Controls
import MindGraph

Button {
    id: root
    height: 44

    property bool danger: false
    property bool secondary: false

    contentItem: Text {
        text: root.text
        color: {
            if (root.danger) return Theme.accentTextColor
            if (root.secondary) return Theme.buttonSecondaryTextColor
            return Theme.accentTextColor
        }

        font.pointSize: 14
        font.bold: true
        horizontalAlignment: Text.AlignHCenter
        verticalAlignment: Text.AlignVCenter
    }

    background: Rectangle {
        radius: 12

        border.width: root.secondary && !root.danger ? 1 : 0
        border.color: Theme.borderColor

        color: {
            if (root.danger)
                return root.pressed ? Theme.dangerPressedColor : Theme.dangerColor

            if (root.secondary)
                return Theme.buttonSecondaryColor

            return root.pressed ? Theme.accentPressedColor : Theme.accentColor
        }
    }
}