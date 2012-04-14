// import QtQuick 1.0 // to target S60 5th Edition or Maemo 5
import QtQuick 1.1
//import com.nokia.meego 1.0
//import org.maemo.fremantle 1.0
import com.nokia.symbian 1.1
Rectangle {

    id:panel
    signal requestToSubscribe(string channel)
    signal requestUnsubscribe(string channel)
    x:parent.width - 50
    width: content.width + 20
    color: "#F0F8FF"
    opacity: 0.7
    height: parent.height


    Row
    {
        id:content
        z:5
        anchors.right: parent.right
        Button {
            id: addButton
            text: "+"
            width: 50
            MouseArea {
                anchors.fill: parent
                onClicked: {
                    if (contact.text.length!=0)
                    panel.requestToSubscribe(contact.text)

                    else
                        notify.notify("Contact's name isn't entered")
                }
            }

        }

    TextField{
        id: contact
        placeholderText: "Enter contact's name"
    }


}


    ContactList {id: contactList; anchors.topMargin:content.height; anchors.leftMargin: border.width; anchors.fill: parent; opacity: parent.opacity; z:15}


    BorderImage {
        id: border
        source: ":../images/strip.png"
        width: 20; height: parent.height




    }
    MouseArea {
        anchors.fill: parent
        drag.target: panel
        drag.axis: Drag.XAxis
        drag.minimumX: panel.parent.width - panel.width
        drag.maximumX: panel.parent.width - 50
    }
  //  RightPanelContent{id:content; z:5; }

    NotifyDialog {id:notify}

    Connections {target: contactList; onUnsubscribe:panel.requestUnsubscribe(channel)}



}

