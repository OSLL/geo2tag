//import Qt 4.7
import QtQuick 1.0

import com.nokia.symbian 1.1
import QtMobility.location 1.2


 Item {
     property string curName: ""
     id: container
     width: 500
     height: 500
     function setPosition(lat, lon) {
                 var coord = Qt.createQmlObject('import QtMobility.location 1.2; Coordinate{latitude:' + lat + ';longitude:' + lon + ';}', container, "coord");
                 map.center = coord;
         drawPins();
         map.pan(0,0);
             }

     function drawPins() {
        contactModel.drawPins();

     }

     function showContact(contact,lat,lng) {
         curName = contact;
         setPosition(lat,lng);
     }



     Map {
         id: map
         z : 1
         anchors.fill:parent
         plugin : Plugin {
                             name : "nokia"
                         }
         zoomLevel: 10
         mapType: Map.StreetMap
         center: /*positionSource.position.coordinate */Coordinate {
             latitude: 59.945152
             longitude: 30.371842
         }
         onZoomLevelChanged: drawPins()

         MapImage {
             id: mylocation
             visible: true
             source: "../images/red_mark.svg"
             coordinate: Coordinate {
                 latitude: positionSource.position.coordinate.latitude
                 longitude: positionSource.position.coordinate.longitude
             }
            offset.x: -10
            offset.y: -30
         }
//         MapCircle {
//             id:circle
//             radius: 1000
//             center:  Coordinate {
//                 latitude: 60
//                 longitude: 30
//             }
//         }

             Item {
                 id: pinpointViewContainer
                 anchors.fill:parent
                 Repeater {
                     id: landmarkPinpointView
                     model: contactModel
                     delegate:
                     Component {
                         id: landmarkMapDelegate

                         Item {
                             id:land
                             visible: (lng< map.toCoordinate(Qt.point(map.width,map.height)).longitude && lng>map.toCoordinate(Qt.point(0,0)).longitude &&
                                      lat> map.toCoordinate(Qt.point(map.width,map.height)).latitude && lat<map.toCoordinate(Qt.point(0,0)).latitude)? true:false
                             width: 20;
                             height: 30;
                             x: (map.width*(lng- map.toCoordinate(Qt.point(0,0)).longitude)/(map.toCoordinate(Qt.point(map.width,map.height)).longitude - map.toCoordinate(Qt.point(0,0)).longitude)) -10

                            y: (map.height*(lat - map.toCoordinate(Qt.point(0,0)).latitude)/(map.toCoordinate(Qt.point(map.width,map.height)).latitude - map.toCoordinate(Qt.point(0,0)).latitude)) - 30//map.toScreenPosition(lat)-10//
                            z:20
                             Image {
                                 id: mark
                                 source:image
                             }
                             Text {
                                 visible: (curName==name) ? true: false


                                 id: text
                                 font.pointSize: 5
                                 font.bold: true
                                 text: customname
                                 anchors.bottom: mark.top
                             }


                         }
                     }
                 }
             }


}

         PositionSource {
             id: positionSource
             updateInterval: 1000
             active: true

         }



         MouseArea {
             anchors.fill:parent
             property int lastX : -1
             property int lastY : -1
             onPressed :  { lastX = mouse.x; lastY = mouse.y; }
             onReleased : { lastX = -1; lastY = -1; }
             onPositionChanged: {
                 if (lastX>=0) {
                     map.pan(lastX- mouse.x, lastY - mouse.y)
                     lastX = mouse.x
                     lastY = mouse.y
                     drawPins()
                 }

             }
         }







  Button {
      id: plus
      anchors.bottom: parent.bottom
      anchors.left: parent.left
      text: "Zoom +"
      z: 20
      onClicked:
      {



     // focus : true
           map.zoomLevel += 1
          //drawPins();


      }



    }

  Button {
      id: minus
      anchors.left: plus.right
      anchors.leftMargin: 5
      anchors.bottom: parent.bottom
      text: "Zoom -"
      z: 20
      onClicked: {
          if (map.zoomLevel>3) {
               map.zoomLevel -= 1;
              map.pan(0,0);
              //drawPins()
          }


      }



    }

  Button {

      anchors.left: minus.right
      anchors.bottom: parent.bottom
      text: "I'm here"
      anchors.leftMargin: 5
      z: 20
      onClicked:  {
          setPosition(positionSource.position.coordinate.latitude, positionSource.position.coordinate.longitude)
          drawPins();

      }


    }

}


