import QtQuick 2.0
import Apricot 1.0
import Apricot.Tools 1.0

Rectangle {
    width: 800
    height: 600

    ToolActionArea  {
        anchors.fill: parent
        canvas: project.canvas
        tool: dragTool
    }

    DragTool {
        id: dragTool
    }
}
