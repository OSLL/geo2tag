//import Qt 4.7
import QtQuick 1.0

import com.nokia.symbian 1.1
import QtMobility.location 1.2


 Item {
     width: 500
     height: 500
     function getPosition(){
         console.debug(positionSource.position.coordinate.latitude)
         map.center=positionSource.position.coordinate
   //      map.zoomLevel=15

     }

    // focus : true
     function drawPins() {
         var topLeft = map.toCoordinate(Qt.point(0,0))
         var bottomRight = map.toCoordinate(Qt.point(map.width, map.height))
        contactModel.drawPins(topLeft.latitude,topLeft.longitude,bottomRight.latitude,bottomRight.longitude, map.width,map.height);
     }






     Map {
         id: map
         z : 1
         plugin : Plugin {
                             name : "nokia"
                         }
         size.width: parent.width
         size.height: parent.height
         zoomLevel: 7
         mapType: Map.StreetMap
         center: /*positionSource.position.coordinate */Coordinate {
             latitude: 60
             longitude: 30
         }

         Item {
             id: pinpointViewContainer
             anchors.fill:parent
             Repeater {
                 id: landmarkPinpointView
                 model: contactModel
                 delegate: //landmarkMapDelegate

                 Component {
                     id: landmarkMapDelegate
                     Item {
                         id:land
                         width: 20;
                         height: 30;
                         x: X - 10
                         y: Y - 30
                         Image {
                             id: mark
                             source:image
                         }
                         visible: mark.source==""? false:true
                     }
                 }
             }
         }


         PositionSource {
             id: positionSource
             updateInterval: 1000
             active: true

         }

         MapImage {
             id: mylocation
             visible: true
             source: "../images/red_mark.svg"
             coordinate: Coordinate {
                 latitude: positionSource.position.coordinate.latitude
                 longitude: positionSource.position.coordinate.longitude
             }
             offset.x: 10
             offset.y: -30
             z:20
         }



         MapMouseArea {
             property int lastX : -1
             property int lastY : -1

             onPressed : {
                 lastX = mouse.x
                 lastY = mouse.y
             }
             onReleased : {
                 lastX = -1
                 lastY = -1
                 drawPins()
             }
             onPositionChanged: {
                 if (mouse.button == Qt.LeftButton) {
                     if ((lastX != -1) && (lastY != -1)) {
                         var dx = mouse.x - lastX
                         var dy = mouse.y - lastY
                         map.pan(-dx, -dy)
                     }
                     lastX = mouse.x
                     lastY = mouse.y

                 }
                  drawPins()
             }
//             onDoubleClicked: {
//                 map.center = mouse.coordinate
//                 map.zoomLevel += 1
//                 lastX = -1
//                 lastY = -1
//             }
         }

     }

  Button {
      id: plus
      anchors.bottom: parent.bottom
      anchors.left: parent.left
      text: "Zoom +"
      z: 20
      onClicked: map.zoomLevel += 1



    }

  Button {
      id: minus
      anchors.left: plus.right
      anchors.leftMargin: 5
      anchors.bottom: parent.bottom
      text: "Zoom -"
      z: 20
      onClicked: map.zoomLevel -= 1



    }

  Button {

      anchors.left: minus.right
      anchors.bottom: parent.bottom
      text: "I'm here"
      anchors.leftMargin: 5
      z: 20
      onClicked:  {
          drawPins();

          map.center=positionSource.position.coordinate
      }


    }

}


