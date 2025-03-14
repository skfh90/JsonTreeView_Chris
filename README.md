- JsonItem

Represent Every node of TreeView. Contains QVariant value and parent, chlidItems of node

- JsonModel

Inherits QAbstractItemModel, heart of this project
loadJson from given path of file, parse that to rootItem pointer.
It must override the necessary virtual functions of QAbstractItemModel such as index, parent, rowCount, columnCount, data, setData and hasChildren

