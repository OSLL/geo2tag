import QtQuick 1.0
import com.nokia.meego 1.0
//import org.maemo.fremantle 1.0  // for Core components
Item {
    id:recbutton
    signal trackingRequest()
    property string curStatus: "Rec"
    property string  prevStatus: "Rec"
   /* signal startTrack();
    signal stopTrack();*/

    anchors.verticalCenter: parent.verticalCenter
    anchors.verticalCenterOffset: 100
    Rectangle {
        id: rec
        width: 50
        height: 50

        color: "red"
        border.color: "black"
        border.width: 3
        radius: 20
        anchors.left: parent.left
        anchors.leftMargin: 70
        rotation: curStatus=="Stop"? 0: 45
        MouseArea
        {
            anchors.fill: parent
            onClicked: {if (recbutton.curStatus=="Rec") recbutton.curStatus="unknown"
            else {
                recbutton.curStatus="Rec"
                recbutton.prevStatus="Rec"
            }
        recbutton.trackingRequest();
        }
        }

    }

    Text {
        id: buttonLabel
        text: curStatus=="unknown" ? prevStatus: curStatus
        color: "red"
        anchors.horizontalCenter: rec.horizontalCenter
        font.bold: true

       anchors.bottom: rec.top
       anchors.margins: 5


    }



}
