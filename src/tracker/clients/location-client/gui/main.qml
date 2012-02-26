import QtQuick 1.0 // to target S60 5th Edition or Maemo 5
import com.nokia.meego 1.0
//import QtMobility.location 1.2
//import org.maemo.fremantle 1.0  // for Core components


Flickable {
    function showLoginView() {login.open(); }
    function auth(log, pass){ console.debug("try auth"); Main.onAuth(log,pass)}
    function incorrect(err){login.notify(err);}
    function entered(name) {login.login=name; notifyDialog.notify("Authorization is successfull");
        if (recbutton.curStatus=="unknown") {
            recbutton.curStatus="Stop";
            Main.trackingOnOff();
        }
            recbutton.prevStatus=recbutton.curStatus}
    function tracking() {Main.trackingOnOff();}
    width: 500
    height: 500
    LoginView {id: login; width: parent.width; height: parent.height}
   // Rectangle {color:"cyan";  width: parent.width; height: parent.height}
    MapViewer {id:map; z:-1; width: parent.width; height: parent.height}
    RecButton {id: recbutton;width: 100; height: 100;  z:5}
    NotifyDialog{id: notifyDialog}
    RightPanel{id:rightPanel}
    Connections { target: login; onAuthrequest: auth(log, pass)}
    Connections {target: recbutton; onTrackingRequest: tracking() }
    Connections {target: Main; onTrackingStarted: {recbutton.curStatus="Stop"; recbutton.prevStatus="Stop"} }

}
