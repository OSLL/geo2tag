import QtQuick 1.0 // to target S60 5th Edition or Maemo 5
import com.nokia.meego 1.0
//import QtMobility.location 1.2
//import org.maemo.fremantle 1.0  // for Core components


Flickable {
    function showLoginView() {login.open(); }
    function auth(log, pass){ console.debug("try auth"); Client.auth(log,pass)}
    function incorrect(err){login.notify(err);}
    function entered(name) {login.login=name; notifyDialog.notify("Authorization is successfull")}
    width: 500
    height: 500
    LoginView {id: login; width: parent.width; height: parent.height}
   // Rectangle {color:"cyan";  width: parent.width; height: parent.height}
    MapViewer {id:map; z:-1; width: parent.width; height: parent.height}
    NotifyDialog{id: notifyDialog}
    Connections { target: login; onAuthrequest: auth(log, pass)}

}
