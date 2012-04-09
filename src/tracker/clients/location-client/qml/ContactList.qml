import QtQuick 1.1
//import com.mycompany.qmlcomponents 1.0

Rectangle {
    property variant contact: contactModel

//    width: 500
//    height: 300
    ListView {
        id: list
        delegate: delegate
        anchors.fill: parent
        model: contact
    }
    Component {
        id: delegate
        Rectangle {
            color: "#F0F8FF"
            width: parent.width
            height: nameText.height + genreText.height + 12
            Text {
                id: nameText
                text: customname
         //       font.pointSize: 10
                font.bold: true
            }
            Text {
                id: genreText
                anchors.top: nameText.bottom
                anchors.margins: 3
                text: "<" + name +">"
           //     font.pointSize: 10
                color: "blue"
            }
            Rectangle {

                anchors.top: genreText.bottom
                anchors.margins: 3
                width: parent.width
                height: 5
                color: "black"
            }
        }
    }
}
