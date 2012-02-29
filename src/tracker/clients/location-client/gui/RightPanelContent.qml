// import QtQuick 1.0 // to target S60 5th Edition or Maemo 5
import QtQuick 1.1
import com.nokia.meego 1.0
Rectangle {
    color: "#F0F8FF"
//    width: parent.width-20
    Row
    {
        Button {
            id: addButton
            text: "+"
            width: 50

        }

    TextField{
        id: contact
   //     anchors.right: parent.right
        placeholderText: "All channels"
    }
    }
}
