import QtQuick
import MindGraph

Item {
    id: root

    property real fromX: 0
    property real fromY: 0
    property real toX: 0
    property real toY: 0

    x: Math.min(fromX, toX)
    y: Math.min(fromY, toY)
    width: Math.max(Math.abs(toX - fromX), 1)
    height: Math.max(Math.abs(toY - fromY), 1)

    Canvas {
        id: canvas
        anchors.fill: parent

        onPaint: {
            var ctx = getContext("2d")
            ctx.clearRect(0, 0, width, height)

            ctx.strokeStyle = Theme.edgeColor
            ctx.lineWidth = 2
            ctx.beginPath()
            ctx.moveTo(root.fromX - root.x, root.fromY - root.y)
            ctx.lineTo(root.toX - root.x, root.toY - root.y)
            ctx.stroke()
        }
    }

    onFromXChanged: canvas.requestPaint()
    onFromYChanged: canvas.requestPaint()
    onToXChanged: canvas.requestPaint()
    onToYChanged: canvas.requestPaint()
}