import Qt 4.7
import com.nokia.meego 1.0
Item {
    id: titleBar
    BorderImage { source: ":images/titlebar.sci"; width: parent.width; height: parent.height + 14; y: -7 }

    Item {
        id: container
        width: parent.width ; height: parent.height

        Image {
            id: quitButton
            anchors.right: parent.right//; anchors.leftMargin: 0
            anchors.verticalCenter: parent.verticalCenter
            source: ":images/quit.png"
            MouseArea {
                anchors.fill: parent
                onClicked:
                {
                    console.log("clicked")
                    Qt.quit()


                }
            }
        }
BorderImage {
    id: settingsButton
    source: ":images/titlebar.sci"


    anchors.horizontalCenter: parent.horizontalCenter
    anchors.verticalCenter: parent.verticalCenter
    width: parent.width/3
    height: parent.height+14
    Text {
        text: qsTr("Geo2tag")
        anchors.centerIn: parent
        color: "white"

    }
    MouseArea {
        anchors.fill: parent
        onClicked: console.log("Settings")
    }
}

    }
   /* transitions: Transition {
        NumberAnimation { properties: "x"; easing.type: Easing.InOutQuad }
    }*/
}
