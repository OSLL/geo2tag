// import QtQuick 1.0 // to target S60 5th Edition or Maemo 5
import QtQuick 1.1
import com.nokia.symbian 1.1

CommonDialog{
    property string customname: "value"

    signal contactchanged(string newName)


    id: contactSettings
    anchors.fill: parent
    titleText: "Contact's information"
    content: Row {
    spacing: 10

        anchors.horizontalCenter: parent.horizontalCenter
        anchors.verticalCenter: parent.verticalCenter

        Text {
            text: "Name"
            color: "white"
            verticalAlignment: Text.AlignBottom

        }

    TextField {
        id: newName;
        text: customname
        width: 150

    }





}
buttons: ToolBar {
    id: buttons
    width: parent.width
    anchors.bottom: parent.bottom
    height: privateStyle.toolBarHeightLandscape + 2 * platformStyle.paddingSmall

    tools: Row {
        anchors.centerIn: parent
        spacing: platformStyle.paddingMedium

        ToolButton {
            text: "Ok"
            width: (buttons.width - 3 * platformStyle.paddingMedium) / 2
            onClicked: {
                contactSettings.contactchanged(newName.text)
                contactSettings.accept()
            }

        }

        ToolButton {
            text: "Cancel"
            width: (buttons.width - 3 * platformStyle.paddingMedium) / 2
            onClicked: {contactSettings.reject()}
             }
       }
    }

}
