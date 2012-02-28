// import QtQuick 1.0 // to target S60 5th Edition or Maemo 5
import QtQuick 1.1

Rectangle {
    id: name
    radius: 10

    x:parent.width - 50
    width: 20
    height: parent.height
    opacity: 0.8
    Image {
        source: ":images/strip.png"
        anchors.fill: parent
    }
 /*   border.left: 5; border.top: 5
    border.right: 5; border.bottom: 5*/
}


