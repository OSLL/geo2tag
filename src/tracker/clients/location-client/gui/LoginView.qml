import QtQuick 1.0 // to target S60 5th Edition or Maemo 5
import com.nokia.meego 1.0
//import org.maemo.fremantle 1.0  // for Core components

Sheet {
    id: sheet

    signal authrequest(string log, string pass)
    function notify(error) {sheet.open(); notifyDialog.notify(error)}
    //function login_handler(){}
    function logout_handler(){login=""; sheet.open()}




    property string login: ""


    acceptButtonText: login.length==0 ? "Log in": "Log out"
    rejectButtonText: "Cancel"

    content: Flickable {

        anchors.fill: parent
        anchors.leftMargin: 10
        anchors.rightMargin:  10
        anchors.topMargin: 10


        contentWidth: col2.width
       contentHeight: col2.height
        anchors.horizontalCenter: parent.horizontalCenter
        flickableDirection: Flickable.VerticalFlick
        Column {
            id: col2
           anchors.top: parent.top
           anchors.leftMargin: 10
           anchors.rightMargin:  10
           anchors.topMargin: 10

            spacing: 10
                    Grid{
                        visible: login.length==0 ? true: false
                        columns: 2
                        spacing: 20
                        anchors.horizontalCenter: parent.horizontalCenter

                        Text{
                            text: "Login:"
                        }
                            TextField
                            {
                                id: loginField
                            }

                        Text{
                            text: "Password:"
                        }

                        TextField
                        {
                            id: passField
                            echoMode: TextInput.Password

                        }

                }
                    Text {
                        visible: login.length==0 ? false: true
                        text: "You loginned as "+ login
                    }

    }
    }
    onAccepted: login.length==0 ? ((loginField.text.length==0 || passField.text.length==0)? sheet.notify("The login or password isn't entered"):
                                                                                           sheet.authrequest(loginField.text,passField.text)): logout_handler()
    onRejected: console.debug("Reject!")
    NotifyDialog {id:notifyDialog}



}


