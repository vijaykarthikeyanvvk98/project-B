import QtQuick 2.15
import QtQuick.Shapes 1.15

import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15
import Qt5Compat.GraphicalEffects
import QtQuick.Effects
import QtQuick.Window 2.15
import QtQuick.Controls.Material 2.15
ApplicationWindow {
    id: root
    visible: true
    color: "black"
    property real pitch:link ? link.pitch_deg: 0.0
    property real yaw:link ? link.yaw_deg :0.0// "0.0\u00B0"
    property real roll:link ?link.roll_deg: 0.0
    property var data_model1: ["Yaw(\u00B0)","Pitch(\u00B0)","Roll(\u00B0)"]
    property var data_model2: [yaw,pitch,roll]
    property var data_model3: ["black","black","black"]
    Rectangle
    {
        id:sensor_panel
        anchors
        {
            right:parent.right
            top:parent.top
            bottom:parent.bottom
            margins:0.005*root.width
        }
        width:0.25*root.width
        color:"#011026"
        //opacity: 0.3
        radius:0.005*root.width
    }

    Rectangle
    {
        id:camera_panel
        anchors
        {
            right:sensor_panel.left
            left:parent.left
            top:parent.top
            bottom:parent.bottom
            margins:0.005*root.width
        }
        //width:0.25*root.width
        color:"#011026"
        //opacity: 0.3
        radius:0.005*root.width
    }

    ColumnLayout
    {
        id:column1
        anchors
        {
            top:sensor_panel.top
            right:sensor_panel.right
            left:sensor_panel.left
            bottom:sensor_panel.bottom
            margins:0.015*root.width
        }
        spacing:0.005*root.width
        clip:true

        Repeater
        {
            model:data_model1
            Item {
                Layout.fillHeight: true
                Layout.fillWidth: true
            Rectangle
            {
                Layout.fillHeight: true
                Layout.fillWidth: true
                color: data_model3[index]
                opacity: 0.5
                radius: 0.005*root.width
            }


            ColumnLayout {
                anchors.centerIn: parent
                spacing: 0.005 * root.width
            Text {
                text:data_model2[index]
                font.bold: true
                style: Text.Sunken
                font.pixelSize: Math.min(root.width / 20, root.height / 15)
                verticalAlignment: Text.AlignVCenter
                horizontalAlignment: Text.AlignHCenter
                Layout.alignment: Qt.AlignHCenter
                color: "white"
            }
            Text {
                text: modelData
                font.bold:true
                font.pixelSize: Math.min(root.width / 70, root.height / 60)
                verticalAlignment: Text.AlignVCenter
                horizontalAlignment: Text.AlignHCenter
                Layout.alignment: Qt.AlignHCenter
                color: "white"
                }
            }
            }
        }
    }

    Rectangle {
        id: imageRect
        anchors
        {
            top:camera_panel.top
            //bottom:camera_panel.bottom
            margins:0.005*root.width
            horizontalCenter:camera_panel.horizontalCenter
        }

        width:0.33*camera_panel.width
        height:0.32*camera_panel.height
        //anchors.left: parent.left
        //anchors.right: parent.right
        //anchors.top: parent.top
        //anchors.bottom: parent.bottom
        //anchors.margins: 0.005 * root.width
        color: "transparent"
        //border.color: "white"
        //border.width: 0.0025*root.width
        visible: true
        Image {
            id: opencvImage
            width: 0.99 * parent.width
            height: 0.99*parent.height // 16:9 aspect ratio
            anchors.horizontalCenter: parent.horizontalCenter
            anchors.centerIn: parent
            clip:true
            //fillMode: Image.PreserveAspectFit
            property bool counter: false
            property int counter2: 0
            visible: true
            source: "qrc:/project-B/images/dummy_template3.jpg"
            asynchronous: false
            cache: false
            function reload() {
                counter = !counter
                counter2++;
                if(counter2 >4 )
                    counter2=1;
                source = "image://live/0?id=" + counter
            }
        }
}

    Rectangle {
        id: imageRect3
        anchors
        {
            top:imageRect.bottom
            right:camera_panel.right
            margins:0.005*root.width
        }

        width:imageRect.width
        height:imageRect.height
        //anchors.left: parent.left
        //anchors.right: parent.right
        //anchors.top: parent.top
        //anchors.bottom: parent.bottom
        //anchors.margins: 0.005 * root.width
        color: "transparent"
        //border.color: "white"
        //border.width: 0.0025*root.width
        visible: true
        Image {
            id: opencvImage3
            width: 0.99 * parent.width
            height: 0.99*parent.height // 16:9 aspect ratio
            anchors.horizontalCenter: parent.horizontalCenter
            anchors.centerIn: parent
            clip:true
            //fillMode: Image.PreserveAspectCrop
            property bool counter: false
            property int counter2: 0
            visible: true
            source: "qrc:/project-B/images/dummy_template3.jpg"
            asynchronous: false
            cache: false
            function reload() {
                counter = !counter;
                source = "image://live/2?id=" + counter
            }
        }
    }

    Rectangle {
        id: imageRect2
        anchors
        {
            top:imageRect.bottom
            left:camera_panel.left
            margins:0.005*root.width
        }

        width:imageRect.width
        height:imageRect.height
        //anchors.left: parent.left
        //anchors.right: parent.right
        //anchors.top: parent.top
        //anchors.bottom: parent.bottom
        //anchors.margins: 0.005 * root.width
        color: "transparent"
        //border.color: "white"
        //border.width: 0.0025*root.width
        visible: true
        Image {
            id: opencvImage2
            width: 0.99 * parent.width
            height: 0.99*parent.height // 16:9 aspect ratio
            anchors.horizontalCenter: parent.horizontalCenter
            anchors.centerIn: parent
            clip:true
            //fillMode: Image.PreserveAspectCrop
            property bool counter: false
            visible: true
            source: "qrc:/project-B/images/dummy_template3.jpg"
            asynchronous: false
            cache: false
            function reload() {
                counter = !counter
                source = "image://live/1?id=" + counter
            }
        }
}

    Rectangle {
        id: imageRect4
        anchors
        {
            top:imageRect3.bottom
            bottom:camera_panel.bottom
            horizontalCenter:camera_panel.horizontalCenter
            margins:0.005*root.width
        }

        width:imageRect.width
        height:imageRect.height
        //anchors.left: parent.left
        //anchors.right: parent.right
        //anchors.top: parent.top
        //anchors.bottom: parent.bottom
        //anchors.margins: 0.005 * root.width
        color: "transparent"
        //border.color: "white"
        //border.width: 0.0025*root.width
        visible: true
        Image {
            id: opencvImage4
            width: 0.99 * parent.width
            height: 0.99*parent.height // 16:9 aspect ratio
            anchors.horizontalCenter: parent.horizontalCenter
            anchors.centerIn: parent
            clip:true
            //fillMode: Image.PreserveAspectCrop
            property bool counter: false
            visible: true
            source: "qrc:/project-B/images/dummy_template3.jpg"
            asynchronous: false
            cache: false
            function reload() {
                counter = !counter
                source = "image://live/3?id=" + counter
            }
        }
}
    /*background: Rectangle
    {
       anchors.fill: parent
       color:"#011026"
    }*/
    /*property real cx: width/2
    property real cy: height/2
    property real radius: 150*/
    /*ColumnLayout
    {
        id:main_column
        width:0.8*root.width
        height:root.height
        anchors
        {
            top:root.top
            bottom:root.bottom
            left:root.left
            right:dummy.left
            margins:0.005*root.width
            //centerIn:parent
        }
        spacing: 0.005*root.width
        clip: true


        RowLayout
        {
            id:row_1
            Layout.fillHeight: true
            width:parent.width
            height: parent.height
            spacing: 0.005*root.width

            Item
            {
                Layout.fillHeight: true
                Layout.fillWidth: true

                Rectangle {
                    id: imageRect
                    Layout.fillHeight: true
                    Layout.fillWidth: true
                    width:parent.width
                    height:parent.height
                    //anchors.left: parent.left
                    //anchors.right: parent.right
                    //anchors.top: parent.top
                    //anchors.bottom: parent.bottom
                    //anchors.margins: 0.005 * root.width
                    color: "transparent"
                    border.color: "white"
                    border.width: 0.0025*root.width
                    visible: true
                    /*Image {
                        id: opencvImage
                        width: 0.99 * parent.width
                        height: 0.99*parent.height // 16:9 aspect ratio
                        anchors.horizontalCenter: parent.horizontalCenter
                        anchors.centerIn: parent
                        clip:true
                        fillMode: Image.PreserveAspectCrop
                        property bool counter: false
                        visible: true
                        source: "qrc:/project-B/images/dummy_template3.jpg"
                        asynchronous: false
                        cache: false
                        function reload() {
                            counter = !counter
                            source = "image://live/image?id=" + counter
                        }
                    }*/
            /*}
            }

            Item
            {
                Layout.fillHeight: true
                Layout.fillWidth: true

                Rectangle {
                    id: imageRect3
                    Layout.fillHeight: true
                    Layout.fillWidth: true
                    width:parent.width
                    height:parent.height
                    //anchors.left: parent.left
                    //anchors.right: parent.right
                    //anchors.top: parent.top
                    //anchors.bottom: parent.bottom
                    //anchors.margins: 0.005 * root.width
                    color: "transparent"
                    border.color: "white"
                    border.width: 0.0025*root.width
                    visible: true
                    Image {
                        id: opencvImage3
                        width: 0.99 * parent.width
                        height: 0.99*parent.height // 16:9 aspect ratio
                        anchors.horizontalCenter: parent.horizontalCenter
                        anchors.centerIn: parent
                        clip:true
                        fillMode: Image.PreserveAspectCrop
                        property bool counter: false
                        visible: true
                        source: "qrc:/project-B/images/dummy_template3.jpg"
                        asynchronous: false
                        cache: false
                        function reload() {
                            counter = !counter
                            source = "image://live/image?id=" + counter
                        }
                    }
            }
            }
        }

        RowLayout
        {
            id:row_2
            Layout.fillHeight: true
            Layout.fillWidth: true
            width:parent.width
            height: parent.height
            spacing: 0.005*root.width

            Item
            {
                Layout.fillHeight: true
                Layout.fillWidth: true

                Rectangle {
                    id: imageRect2
                    Layout.fillHeight: true
                    Layout.fillWidth: true
                    width:parent.width
                    height:parent.height
                    //anchors.left: parent.left
                    //anchors.right: parent.right
                    //anchors.top: parent.top
                    //anchors.bottom: parent.bottom
                    //anchors.margins: 0.005 * root.width
                    color: "transparent"
                    border.color: "white"
                    border.width: 0.0025*root.width
                    visible: true
                    Image {
                        id: opencvImage2
                        width: 0.99 * parent.width
                        height: 0.99*parent.height // 16:9 aspect ratio
                        anchors.horizontalCenter: parent.horizontalCenter
                        anchors.centerIn: parent
                        clip:true
                        fillMode: Image.PreserveAspectCrop
                        property bool counter: false
                        visible: true
                        source: "qrc:/project-B/images/dummy_template3.jpg"
                        asynchronous: false
                        cache: false
                        function reload() {
                            counter = !counter
                            source = "image://live/image?id=" + counter
                        }
                    }
            }
            }

            Item
            {
                Layout.fillHeight: true
                Layout.fillWidth: true

                Rectangle {
                    id: imageRect4
                    Layout.fillHeight: true
                    Layout.fillWidth: true
                    width:parent.width
                    height:parent.height
                    //anchors.left: parent.left
                    //anchors.right: parent.right
                    //anchors.top: parent.top
                    //anchors.bottom: parent.bottom
                    //anchors.margins: 0.005 * root.width
                    color: "transparent"
                    border.color: "white"
                    border.width: 0.0025*root.width
                    visible: true
                    Image {
                        id: opencvImage4
                        width: 0.99 * parent.width
                        height: 0.99*parent.height // 16:9 aspect ratio
                        anchors.horizontalCenter: parent.horizontalCenter
                        anchors.centerIn: parent
                        clip:true
                        fillMode: Image.PreserveAspectCrop
                        property bool counter: false
                        visible: true
                        source: "qrc:/project-B/images/dummy_template3.jpg"
                        asynchronous: false
                        cache: false
                        function reload() {
                            counter = !counter
                            source = "image://live/image?id=" + counter
                        }
                    }
            }
            }
        }
    }*/

    Image {
        id: opencvImage5
        //anchors.horizontalCenter: parent.horizontalCenter
        //anchors.horizontalCenterOffset: -0.2*root.width
        //anchors.verticalCenter:parent.verticalCenter
        anchors.centerIn: camera_panel
        clip:true
        fillMode: Image.PreserveAspectCrop
        property bool counter: false
        visible: true
        source: "qrc:/project-B/images/boat.svg"
        sourceSize.width: 0.075*root.width
        sourceSize.height: 0.075*root.height
        rotation: -90 +yaw
        asynchronous: false
        cache: false
        z:3

    }
    /*Rectangle {
        id: imageRect
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.top: parent.top
        anchors.bottom: parent.bottom
        anchors.margins: 0.005 * root.width
        color: "transparent"
        border.color: "white"
        border.width: 0.0025*root.width
        visible: true
        Image {
            id: opencvImage
            width: 0.99 * parent.width
            height: 0.99*parent.height // 16:9 aspect ratio
            anchors.horizontalCenter: parent.horizontalCenter
            anchors.centerIn: parent
            clip:true
            fillMode: Image.PreserveAspectCrop
            property bool counter: false
            visible: true
            source: "qrc:/project-B/images/dummy_template3.jpg"
            asynchronous: false
            cache: false
            function reload() {
                counter = !counter
                source = "image://live/image?id=" + counter
            }
        }
}*/

    Button
    {
        id:dummy
        width: 0.01*root.width
        height:0.01*root.height
        anchors.right: parent.right
        anchors.rightMargin: 0.175*root.width
        anchors.verticalCenter: parent.verticalCenter
    }


    //--------------------------------------------------
    // MASK SHAPE
    //--------------------------------------------------

    /*OpacityMask {

        anchors.fill: parent

        source: Grid {

            anchors.fill: parent
            columns: 2

            Image { source: "qrc:/project-B/images/dummy_template3.jpg"; fillMode: Image.PreserveAspectCrop }
            Image { source: "qrc:/project-B/images/dummy_template3.jpg"; fillMode: Image.PreserveAspectCrop }
            Image { source: "qrc:/project-B/images/dummy_template3.jpg"; fillMode: Image.PreserveAspectCrop }
            Image { source: "qrc:/project-B/images/dummy_template3.jpg"; fillMode: Image.PreserveAspectCrop }
        }

        maskSource: Item {

            anchors.fill: parent

            Shape {

                id: pizzaMask

                anchors.fill: parent

                property real cx: width/2
                property real cy: height/2
                property real radius: width/2

                Repeater {

                    model: 4

                    Shape {

                        anchors.fill: parent

                        ShapePath {
                            id: slicePath   // ✅ ADD THIS LINE

                            fillColor: "white"
                            strokeWidth: 0

                            property real startAngle: 45 + index*90
                            property real endAngle: startAngle + 90

                            property real x1: pizzaMask.cx + pizzaMask.radius * Math.cos(startAngle*Math.PI/180)
                            property real y1: pizzaMask.cy + pizzaMask.radius * Math.sin(startAngle*Math.PI/180)

                            property real x2: pizzaMask.cx + pizzaMask.radius * Math.cos(endAngle*Math.PI/180)
                            property real y2: pizzaMask.cy + pizzaMask.radius * Math.sin(endAngle*Math.PI/180)

                            startX: pizzaMask.cx
                            startY: pizzaMask.cy

                            PathLine {
                                x: slicePath.x1
                                y: slicePath.y1
                            }

                            PathArc {
                                x: slicePath.x2
                                y: slicePath.y2
                                radiusX: pizzaMask.radius
                                radiusY: pizzaMask.radius
                            }

                            PathLine {
                                x: pizzaMask.cx
                                y: pizzaMask.cy
                            }

                        }
                    }
                }
            }
        }
    }*/



    /*Item {
        width: 0.5*root.width
        height: 0.5*root.height
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.horizontalCenterOffset: -0.15*root.width
        anchors.top: parent.top
        anchors.topMargin: 0.005*root.width
        // 1. The Image you want to display
        Image {
            id: pizzaImage
            source:"qrc:/project-B/images/dummy_template3.jpg" // Replace with your image
            anchors.fill: maskShape
            fillMode:Image.PreserveAspectFit
            visible: false // Hidden because it's used as a source for the mask
        }

        // 2. Your Shape (used as the mask)
        Shape {
            id: maskShape
            anchors.fill: parent
            visible: false // Hidden because it's used as the mask source
            layer.enabled: true // Required for masking smooth edges
            antialiasing: true

            ShapePath {
                fillColor: "black" // Color doesn't matter, only the alpha channel
                // 1. Start at the "tip" (bottom center)
                startX: maskShape.width / 2; startY: maskShape.height * 0.9
                PathLine {
                    // Center + radius * cos(startAngle)
                    x: (maskShape.width / 2) + (maskShape.height * 0.9) * Math.cos(227 * Math.PI / 180)
                    y: (maskShape.height * 0.9) + (maskShape.height * 0.9) * Math.sin(227 * Math.PI / 180)
                }
                PathAngleArc {
                    centerX: maskShape.width / 2; centerY: maskShape.height * 0.9
                    radiusX: Math.min(maskShape.width, maskShape.height) * 0.9; radiusY: radiusX
                    startAngle: 227
                    sweepAngle: 86
                    moveToStart: false
                }
                PathLine { x: maskShape.width / 2; y: maskShape.height * 0.9 }
            }
        }

        // 3. The actual OpacityMask that combines them
        OpacityMask {
            anchors.fill: parent
            source: pizzaImage
            maskSource: maskShape
        }
    }

    Item {
        width: 0.4*root.width
        height: 0.85*root.height
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.horizontalCenterOffset: -0.0375*root.width
        anchors.verticalCenter:  parent.verticalCenter
        anchors.verticalCenterOffset:-0.045*root.width
        //anchors.verticalCenterOffset:0.3*root.width

        //anchors.topMargin: 0.005*root.width
        // 1. The Image you want to display
        Image {
            id: pizzaImage2
            source:"qrc:/project-B/images/dummy_template3.jpg" // Replace with your image
            anchors.fill: parent
            fillMode:Image.PreserveAspectFit
            rotation: 90
            visible: true//false // Hidden because it's used as a source for the mask
        }

        // 2. Your Shape (used as the mask)
        /*Shape {
            id: maskShape2
            anchors.fill: parent
            visible: false // Hidden because it's used as the mask source
            layer.enabled: true // Required for masking smooth edges
            antialiasing: true

            ShapePath {
                fillColor: "black" // Color doesn't matter, only the alpha channel
                // 1. Start at the "tip" (bottom center)
                startX: maskShape2.width *0.15; startY: maskShape2.height/2
                PathLine {
                    // Center + radius * cos(startAngle)
                    x: (maskShape2.width *0.15) + (maskShape2.height*0.9) * Math.cos(340 * Math.PI / 180)
                    y: (maskShape2.height/2) + (maskShape2.height*0.9) * Math.sin(340 * Math.PI / 180)
                }
                PathAngleArc {
                    centerX: maskShape2.width *0.15; centerY: maskShape2.height/2
                    radiusX: Math.min(maskShape2.width, maskShape2.height) * 0.9; radiusY: radiusX
                    startAngle: 340
                    sweepAngle: 86
                    moveToStart: false
                }
                PathLine { x: maskShape2.width *0.15;y: maskShape2.height/2 }
            }
        }

        // 3. The actual OpacityMask that combines them
        OpacityMask {
            anchors.fill: parent
            source: pizzaImage2
            maskSource: maskShape2
        }*/
    //}

    Connections{
        target: liveImageProvider

        function onImageChanged()
        {
            opencvImage.reload()
        }

        function onImageChanged2()
        {
            opencvImage2.reload()
            //console.log("2")
        }


        function onImageChanged3()
        {
            opencvImage3.reload()
        }

        function onImageChanged4()
        {
            opencvImage4.reload()
            //console.log("Hai")
        }


    }


    Component.onCompleted: {
        VideoStreamer.openVideoCamera(0)
        opencvImage.visible = true
        opencvImage2.visible = true
        opencvImage3.visible = true
        opencvImage4.visible = true

    }
}
