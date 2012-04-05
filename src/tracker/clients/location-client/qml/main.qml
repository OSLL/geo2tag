import QtQuick 1.0 // to target S60 5th Edition or Maemo 5

//import com.nokia.meego 1.0
//import QtMobility.location 1.2
//import org.maemo.fremantle 1.0  // for Core components
import com.nokia.symbian 1.1

Flickable {
    function showLoginView() {login.open(); }
    function showTrackSettings() {trackSettings.open()}
    function auth(log, pass){ console.debug("try auth"); Main.onAuth(log,pass)}
    function reg(log, pass) {console.debug("try sign up"); Main.onReg(log, pass)}
    function incorrect(err){login.notify(err);}
    function entered(name) {login.login=name; login.state="Log out"; notifyDialog.notify("Authorization is successfull");
        if (recbutton.curStatus=="unknown") {
            recbutton.curStatus="Stop";
            Main.trackingOnOff();
        }
            recbutton.prevStatus=recbutton.curStatus}
    function tracking() {Main.trackingOnOff();}
    function getPosition() {map.getPosition()}

    LoginView {id: login;/* width:parent.width; height: parent.height; */anchors.fill: parent}
    TrackingSettingsView {id: trackSettings; width:parent.width; height: parent.height;  }
   //Rectangle {color:"cyan";  width: parent.width; height: parent.height}
    MapViewer {id:map; z:-1; width: parent.width; height: parent.height}
    RecButton {id: recbutton;width: 100; height: 100;  z:5}
    RecButton {id: herebutton;width: 100; height: 100;  z:5; color:"green"; curStatus: "I'm here"; anchors.top: recbutton.bottom; anchors.topMargin: 10}
    NotifyDialog{id: notifyDialog}
    RightPanel{id:rightPanel; z:5}
    Connections { target: login; onAuthrequest: auth(log, pass)}
    Connections {target: login; onSignrequest: reg(log,pass)}
    Connections {target: recbutton; onTrackingRequest: tracking() }
    Connections {target: Main; onTrackingStarted: {recbutton.curStatus="Stop"; recbutton.prevStatus="Stop"} }
    Connections {target: herebutton; onGetPosition: getPosition() }
    Connections {target: trackSettings; onSettingschanged: Main.changeSettings(interval, permission)}

}
