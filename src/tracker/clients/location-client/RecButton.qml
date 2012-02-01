import QtQuick 1.0
import com.nokia.meego 1.0

Item {
    id:recbutton
    anchors.verticalCenter: parent.verticalCenter
    anchors.verticalCenterOffset: 100
    Rectangle {
        id: rec
        width: 50
        height: 50
        color: "red"
        border.color: "black"
        radius: 5
        anchors.left: parent.left
        anchors.leftMargin: 70
        rotation: 45
    }

    Text {
        id: buttonLabel
        text: "REC"
        color: "red"
        anchors.horizontalCenter: rec.horizontalCenter
        font.bold: true

       anchors.bottom: rec.top
       anchors.margins: 5


    }



}
