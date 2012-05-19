import QtQuick 1.0 // to target S60 5th Edition or Maemo 5

//import com.nokia.meego 1.0
//import QtMobility.location 1.2
//import org.maemo.fremantle 1.0  // for Core components
import com.nokia.symbian 1.1
//import com.mycompany.qmlcomponents 1.0

Flickable {
    function showLoginView() {login.open(); }
    function showTrackSettings() {trackSettings.open()}
    function auth(log, pass){ console.debug("try auth"); Main.onAuth(log,pass)}
    function reg(email, log, pass) {console.debug("try sign up"); Main.onReg(email, log, pass)}
    function incorrect(err){
        if (err=="Incorrect login or password" || err=="User with that name already exist")
        login.notify(err);
    else
    notifyDialog.notify(err);}
    function entered(name) {login.login=name; login.state="Log out"; notifyDialog.notify("Authorization is successfull");
   /*     if (recbutton.curStatus=="unknown") {
            recbutton.curStatus="Stop";
            Main.trackingOnOff();
        }
            recbutton.prevStatus=recbutton.curStatus*/}
    function tracking() {Main.trackingOnOff();}
    function getPosition() {map.getPosition()}

    function showNotify(){
        notifyDialog.notify("Please, confirm registration from your email")}

    LoginView {id: login;/* width:parent.width; height: parent.height; */anchors.fill: parent}
    TrackingSettingsView {id: trackSettings; width:parent.width; height: parent.height;  }
   //Rectangle {color:"cyan";  width: parent.width; height: parent.height}
    MapViewer {id:map; z:-1; width: parent.width; height: parent.height}

    NotifyDialog{id: notifyDialog}
    RightPanel{id:rightPanel; z:5}
    Connections { target: login; onAuthrequest: Main.onAuth(log,pass)/*auth(log, pass)*/}
    Connections {target: login; onSignrequest: reg(email,log,pass)}
    Connections {target: login; onLogout: Main.logout()}
    //Connections {target: recbutton; onTrackingRequest: tracking() }
    Connections {target: Main; onTrackingStarted: {recbutton.curStatus="Stop"; recbutton.prevStatus="Stop"} }
    //Connections {target: herebutton; onGetPosition: getPosition() }
    Connections {target: trackSettings; onSettingschanged: Main.changeSettings(interval, permission)}
    Connections {target: rightPanel; onRequestToSubscribe:Main.onSubscribe(channel)}
    Connections {target: rightPanel; onRequestUnsubscribe: Main.unsubscribeChannel(channel)}
    Connections {target: rightPanel; onShow: map.showContact(contact,lat,lng)}


}
