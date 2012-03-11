import QtQuick 1.0 // to target S60 5th Edition or Maemo 5
//import com.nokia.meego 1.0
//import org.maemo.fremantle 1.0  // for Core components
import com.nokia.symbian 1.1
QueryDialog {
    id: notifyDialog
    function notify(error) {err=error; notifyDialog.open()}

    property string err: ""

    width: parent.width
    height: 10
    anchors.verticalCenter: parent.verticalCenter
    opacity: 0.3
    message: err
    acceptButtonText : "Ok"
//        content: Text {
//            text: "The login or password isn't entered"
//            anchors.horizontalCenter: parent.horizontalCenter
//        }
}
