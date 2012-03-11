import QtQuick 1.0
//import com.nokia.meego 1.0
//import org.maemo.fremantle 1.0  // for Core components
import com.nokia.symbian 1.1
Item {
    id:recbutton
    signal trackingRequest()
    signal getPosition()
    property string curStatus: "Rec"
    property string  prevStatus: "Rec"
    property string color: "red"
   /* signal startTrack();
    signal stopTrack();*/

    anchors.verticalCenter: parent.verticalCenter
    anchors.verticalCenterOffset: 30
    Rectangle {
        id: rec
        width: 50
        height: 50

        color: recbutton.color=="red" ? "red" : "green"
        border.color: "black"
        border.width: 3
        radius: 20
        anchors.left: parent.left
        anchors.leftMargin: 70
        rotation: curStatus=="Stop"? 0: 45
        MouseArea
        {
            anchors.fill: parent
            onClicked: {
                if (recbutton.color == "red") {
                   if (recbutton.curStatus=="Rec") recbutton.curStatus="unknown"
                   else {
                       recbutton.curStatus="Rec"
                       recbutton.prevStatus="Rec"
                   }
                   recbutton.trackingRequest()
                }
                else
                    recbutton.getPosition();
            }

        }

    }

    Text {
        id: buttonLabel
        text: curStatus=="unknown" ? prevStatus: curStatus
        color: recbutton.color=="red" ? "red" : "green"
        anchors.horizontalCenter: rec.horizontalCenter
        font.bold: true

       anchors.bottom: rec.top
       anchors.margins: 5


    }



}
