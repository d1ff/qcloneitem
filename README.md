# QCloneItem
QCloneItem - Show a clone of another window within your QML application (Windows only!)

* Will clone any *top-level* window.
* Window has to be visible, but can be minimized.
* The clone respects opacity, sizing (and anchors) and visibility.
* It is possible to crop the source window (using source property), and exclude non-client area (sourceCLientAreaOnly property).
* Window to clone is specified by window class and window title. If window is created after the Clone, call updateTargetWindow to establish connection.

### Simple Example:
QML Code:
```
import QtQuick 2.0
import Clone 1.0

Rectangle {
    id: root
    width: 640
    height: 480

        Clone {
            id: clone
            anchors.fill: parent
            anchors.margins: 50
            windowTitle: "Calculator"
            windowClass: "CalcFrame"
        }
}

```
Result:
![Example](/example/example.png "Example")
For a more complex example, see the example in the repository.

More to come
