import QtQuick
import QtQuick.Controls

ApplicationWindow {
    visible: true
    width: 800
    height: 600

    property int curRow
    property int curCol
    title: "JSON Tree Viewer"
    TreeView {
        id: treeView
        model: jsonModel
        anchors.fill: parent


        delegate: TreeViewDelegate{
            font.pixelSize: 20
            implicitWidth: treeView.width
            implicitHeight: 50
            onClicked: {
                if (!hasChildren){
                    textField.text = model.display

                }

            }
        }

        onCurrentColumnChanged: {
            curCol = treeView.currentColumn
        }
        onCurrentRowChanged: {
            curRow = treeView.currentRow
        }

    }
    TextField {
        anchors.bottom: parent.bottom
        anchors.left: parent.left
        anchors.right: parent.right
        id: textField
        font.pixelSize: 30
        placeholderText: "Leaf value will be displayed here"

        onTextEdited: {
            jsonModel.setData(treeView.index(curRow+1, curCol), textField.text);
        }
    }

}
