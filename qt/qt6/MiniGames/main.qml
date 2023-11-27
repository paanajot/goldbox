import QtQuick

Window {
   color: "#272822"
   width: 320
   height: 480
   visible: true

//   z:10
   Rectangle {
       y: 64
       z: 0
       width: 256
       height: 256
       color: "green"

       Rectangle {
           x: 192
           y: 64
           z: 0
           width: 108
           height: 158
           color: "red"
       }
   }
   Rectangle {
       x: 64
       y: 192
       z: 1
       width: 256
       height: 256
       color: "blue"
   }
}
