# QCloneItem
QCloneItem - Show a clone of another window within your QML application (Windows only!)

[![Build status](https://ci.appveyor.com/api/projects/status/wfhbic6prfv4vi21?svg=true)](https://ci.appveyor.com/project/jesperhh/qcloneitem)

* Will clone any window. Top level windows work the best, as they can be tracked automatically when resized and moved. If a child window is selected instead, the software will have to translate this into a subregion of the associated top level window instead.
* Window has to be visible, but can be minimized.
* The clone respects opacity, sizing (and anchors) and visibility.
* It is possible to crop the source window (using source property), and exclude non-client area (sourceCLientAreaOnly property).
* Window to clone is specified by window class and window title.

### Simple Example:
**QML Code:**
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
**Result:**

![Example](/example/example.png "Example")

For a more complex example, see the example in the repository.