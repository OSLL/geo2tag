import QtQuick 1.0
import com.nokia.meego 1.0
//import org.maemo.fremantle 1.0  // for Core components
Item {
    id:recbutton
    anchors.verticalCenter: parent.verticalCenter
    anchors.verticalCenterOffset: 100
    Button {
        id: rec
        width: 50
        height: 50
        platformStyle: ButtonStyle{
            inverted: true}
      /*  color: "red"
        border.color: "black"
        radius: 5*/
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
