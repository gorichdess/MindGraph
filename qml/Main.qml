import QtQuick
import QtQuick.Controls
import MindGraph

ApplicationWindow {
    id: window
    width: 800
    height: 760
    visible: true
    title: "Mind Grap"
    color: Theme.backgroundColor

    StackView {
        id: stackView
        anchors.fill: parent
        initialItem: GraphPage {
            stackViewRef: stackView
        }
    }
}
