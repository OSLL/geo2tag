import QtQuick 1.0 // to target S60 5th Edition or Maemo 5
//import com.nokia.meego 1.0
//import org.maemo.fremantle 1.0  // for Core components
import com.nokia.symbian 1.1

/*Sheet*/CommonDialog {
    id: sheet
    property string acceptButtonText: "Log in"

    signal authrequest(string log, string pass)
    signal signrequest(string log, string pass)
    function notify(error) {sheet.open(); notifyDialog.notify(error)}
    function logout_handler(){login=""; sheet.open(); sheet.state="Log in"}
    state: "Log in"

    property string login: ""
    titleText: "Authorization"
    buttons: ToolBar {
                     id: buttons
                     width: parent.width
                     height: privateStyle.toolBarHeightLandscape + 2 * platformStyle.paddingSmall

                     tools: Row {
                         anchors.centerIn: parent
                         spacing: platformStyle.paddingMedium

                         ToolButton {

                             text: acceptButtonText
                             width: (buttons.width - 3 * platformStyle.paddingMedium) / 2
                             onClicked: sheet.accept()
                         }

                         ToolButton {

                             text: "Cancel"
                             width: (buttons.width - 3 * platformStyle.paddingMedium) / 2
                             onClicked: sheet.reject()
                         }
                     }
                 }
  /*  acceptButtonText: "Log in"
    rejectButtonText: "Cancel"*/

    content: Flickable{
        anchors.fill: parent

        anchors.leftMargin: 10
        anchors.rightMargin:  10
        anchors.topMargin: 30
        anchors.top: parent.top
        contentWidth: col2.width
        contentHeight: col2.height
        anchors.horizontalCenter: parent.horizontalCenter
        flickableDirection: Flickable.VerticalFlick
        Column {
            id: col2
            spacing: 10
            Grid{
                id: form
                visible: true
                columns: 2
                spacing: 20
                anchors.horizontalCenter: parent.horizontalCenter

                Text{
                    text: "Login:"
                }

                TextField{
                     id: loginField
                 }

                Text{
                     text: "Password:"
                }

                TextField{
                     id: passField
                     echoMode: TextInput.Password
                }

                Text{
                    id: confirmText
                    text: "Password confirm:"
                    visible: false
                }

                TextField{
                     id: confirmField
                     echoMode: TextInput.Password
                     visible: false
                }
            }
            Text {
                id: loginnedText
                visible: false
                text: "You loginned as "+ login
            }


            Row {
                id: signRow
                spacing: 10
                visible: true
                anchors.right: parent.right
                Text {
                    id: regText
                    visible: true
                    color: "blue"
                    text: "Not a member?"
                 }
                 Button {
                     id: regButton
                     visible: true
                     text: "Sign up"
                     width: 120
                     onClicked: sheet.state="Sign up"
                 }
            }
    }
    }
        onAccepted: {if (state == "Log in") {
                if (loginField.text.length==0 || passField.text.length==0)
                    sheet.notify("The login or password isn't entered")
                else  sheet.authrequest(loginField.text,passField.text)
            }
            else if (state == "Sign up") {
                if (loginField.text.length==0 || passField.text.length==0 || confirmField==0)
                    sheet.notify("Not all data is entered")
                else if (passField.text==confirmField.text)
                    sheet.signrequest(loginField.text,passField.text)
                else sheet.notify("Password is not confirm")
            }
            else
                logout_handler()


        }

        onRejected: if (state=="Sign up") state="Log in"
    NotifyDialog {id:notifyDialog}

    states: [
             State {
                 name: "Log in"
                 PropertyChanges { target: signRow; visible: true}
                 PropertyChanges { target: loginnedText; visible: false}
                 PropertyChanges { target: confirmText; visible: false}
                 PropertyChanges { target: confirmField; visible: false}
                 PropertyChanges { target: sheet; acceptButtonText: "Log in"}

             },
             State {
                 name: "Log out"
                 PropertyChanges { target: signRow; visible: false}
                 PropertyChanges { target: loginnedText; visible: true }
                 PropertyChanges { target: form; visible: false}
                 PropertyChanges { target: sheet; acceptButtonText: "Log out"}
             },
             State {
                 name: "Sign up"
                 PropertyChanges { target: signRow; visible: true}
                 PropertyChanges { target: loginnedText; visible: false}
                 PropertyChanges { target: confirmText; visible: true}
                 PropertyChanges { target: confirmField; visible: true}
                 PropertyChanges { target: sheet; acceptButtonText: "Sign up"}}


         ]

}


