import QtQuick 1.0 // to target S60 5th Edition or Maemo 5
//import com.nokia.meego 1.0
//import org.maemo.fremantle 1.0  // for Core components
import com.nokia.symbian 1.1

CommonDialog {
    id: sheet
    property string acceptButtonText: "Log in"

    signal authrequest(string log, string pass)
    signal signrequest(string email, string log, string pass)
    signal logout()
    function notify(error) {sheet.open(); notifyDialog.notify(error)}
    function logout_handler(){login=""; sheet.open(); sheet.state="Log in"; sheet.logout()}
    state: "Log in"

    property string login: ""
    titleText: state=="Sign up"? "Registration":"Authorization"
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
        anchors.topMargin: 30
        anchors.rightMargin: 10
        anchors.top: parent.top
        contentWidth: col2.width
        contentHeight: col2.height
        anchors.horizontalCenter: parent.horizontalCenter
        flickableDirection: Flickable.VerticalFlick
        Column {
            anchors.fill: parent
            id: col2
            spacing: 25
            Grid{
                id: form
                visible: true
                columns: 2
                spacing: 5
          //      anchors.horizontalCenter: parent.horizontalCenter

                Text{
                    text: "Login:"
                    color: "white"
                }

                TextField{
                     id: loginField
                     width: 150

                 }
                Text{
                     id: emailText
                     text: "e-mail:"
                     color: "white"
                }

                TextField{
                     id: emailField
                     width: 150
                }

                Text{
                     text: "Password:"
                     color: "white"
                }

                TextField{
                     id: passField
                     width: 150
                     echoMode: TextInput.Password
                }

                Text{
                    id: confirmText
                    text: "Password confirm:"
                    visible: false
                    color: "white"
                }

                TextField{
                     id: confirmField
                     echoMode: TextInput.Password
                     visible: false
                     width: 150
                }
            }
            Text {
                id: loginnedText
                visible: false
                text: "You loginned as "+ login
                color: "white"
            }


            Row {
                id: signRow
                //spacing: 10
                visible: true
                //anchors.right: parent.right
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
                    sheet.notify("Not all neccessary data was entered")
                else
                    sheet.authrequest(loginField.text,passField.text)

            }
            else if (state == "Sign up") {
                if (loginField.text.length==0 || passField.text.length==0 || confirmField==0 || emailField.text.length==0)
                    sheet.notify("Not all data was entered")
                else
                    if (emailField.text.indexOf("@")==-1)
                        sheet.notify("The entered e-mail isn't correct")
                else if (passField.text==confirmField.text)
                    sheet.signrequest(emailField,loginField.text,passField.text)
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
                 PropertyChanges { target: emailText; visible: false}
                 PropertyChanges { target: emailField; visible: false}
                 PropertyChanges { target: sheet; acceptButtonText: "Log in"}
                 PropertyChanges { target: form; visible: true}

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
                 PropertyChanges { target: signRow; visible: false}
                 PropertyChanges { target: loginnedText; visible: false}
                 PropertyChanges { target: confirmText; visible: true}
                 PropertyChanges { target: confirmField; visible: true}
                 PropertyChanges { target: emailText; visible: true}
                 PropertyChanges { target: emailField; visible: true}
                 PropertyChanges { target: sheet; acceptButtonText: "Sign up"}
        PropertyChanges { target: form; visible: true}
        }


         ]

}


