import QtQuick 1.1
import com.nokia.symbian 1.1

Rectangle {
    id: contactContainer
    property variant contact: contactModel
   signal unsubscribe(string channel)
//    width: 500
//    height: 300
    ListView {

        id: list
        delegate: delegate
        anchors.fill: parent
        model: contact
        highlight: Rectangle { color: "lightsteelblue"; radius: 5 }
        focus: true


    Component {
        id: delegate
        Rectangle {
            //color: ListView.isCurrentItem ? "blue" : "#F0F8FF"
            color: "#F0F8FF"
            width: parent.width
            height: nameText.height + loginText.height + 12


            Image {
                visible: image==""? false: true
                id:status
                source: image
                anchors.verticalCenter: parent.verticalCenter
            }


            Text {
                anchors.left: status.right
                id: nameText
                text: customname
         //       font.pointSize: 10
                font.bold: true
            }
            Text {
                anchors.left: status.right
                id: loginText
                anchors.top: nameText.bottom
                anchors.margins: 3
                text: "<" + name +">"
           //     font.pointSize: 10
                color: "blue"
            }

            Button {
                anchors.right: parent.right
                text: "Del"
                z:20
                onClicked: {
                    console.debug("del!")
                    contactContainer.unsubscribe(name)
                }
            }



//            Rectangle {

//                anchors.top: loginText.bottom
//                anchors.margins: 2
//                width: parent.width
//                height: 2
//                color: "black"
//            }
        }
    }
}
}
