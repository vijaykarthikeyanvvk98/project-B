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
    signal map_image_Captured
    property real pitch:link ? link.pitch_deg: 0.0
    property real yaw:link ? link.yaw_deg :0.0// "0.0\u00B0"
    property real roll:link ?link.roll_deg: 0.0
    property string logFileDir: link ? link.logFileDir : ""
    property var data_model1: ["Yaw(\u00B0)","Pitch(\u00B0)","Roll(\u00B0)"]
    property var data_model2: [yaw,pitch,roll]
    property var data_model3: ["black","black","black"]
    property var data_model4: ["SnapShot","  Record  ","  Process "]
    property var data_model5: [
        { type: "image", source: "qrc:/icons/screenshot3.png" },
        { type: "canvas" },
        { type: "image2", source: "qrc:/icons/processing.png" }
    ]
    property string tooltip_text: ""
    property color neonblue: "#00FFFF"
    property color neonGreen: "#39FF14"
    property bool record_status: false
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

    Rectangle
    {
        anchors
        {
            //top:sensor_panel.top
            verticalCenter:parent.verticalCenter
            verticalCenterOffset:0.14*root.width
            right:sensor_panel.right
            left:sensor_panel.left
            //bottom:sensor_panel.bottom
            margins:0.015*root.width
        }
        height:root.width/4
        color: "grey"//data_model3[index]
        opacity: 0.5
        radius: 0.005*root.width
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
        spacing:0.01*root.width
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

        Repeater
        {
            model:data_model5
            Item {
                Layout.fillHeight: true
                Layout.fillWidth: true

                Button
                {
                    //Layout.fillHeight: true
                    //Layout.fillWidth: true
                    width:0.025*root.width
                    height: width
                    anchors.centerIn: parent
                    /*contentItem: Text
                    {
                        text:modelData
                        color: "white"
                        //font.family: font_family
                        font.bold: true
                        font.italic: false
                        font.underline: false
                        font.strikeout: false
                        //font.pixelSize: font_size // "Palatino Linotype"
                        //font.bold: true
                        font.pixelSize: Math.min(root.width / 90, root.height / 70)
                        anchors.horizontalCenter: parent.horizontalCenter
                        anchors.verticalCenter: parent.verticalCenter
                        style: Text.Sunken
                    }*/

                    contentItem: Loader {

                        anchors.fill: parent
                        //anchors.centerIn: parent
                        //width:0.75*parent.width
                        //height:0.8*parent.height
                        sourceComponent: {
                            switch(modelData.type)
                            {
                            case "image":
                                return imageComponent2
                            case "canvas":
                                return canvasComponent
                            case "image2":
                                return imageComponent
                            default:
                                break
                        }
                        }
                    }

                    background: Rectangle
                    {
                        anchors.fill: parent
                        //width:0.5*parent.width
                        //height:0.5*parent.height
                        //anchors.centerIn:parent
                        color:"grey"//"#011026"
                        opacity:1.0
                        radius:width/2//0.005*root.width
                    }

                    MouseArea
                    {
                        id: mouseArea
                        anchors.fill: parent
                        hoverEnabled: true
                        cursorShape: Qt.PointingHandCursor

                        onClicked:
                        {
                            //console.log(index)
                            switch(index)
                            {
                            case 0:
                                tooltip_text= "Capture the\n Frame"
                                capture_map_image()
                                break;
                            case 1:
                                if(!record_status)
                                {
                                 tooltip_text= "Stop and\nSave Recording"
                                 statusindicator2.color="darkgreen";
                                VideoStreamer.start_recording();
                                statusindicator2.visible=true
                                start_Timer.start()
                                //pause_recording.visible=true
                                }
                                else
                                {
                                tooltip_text= "Start\nVideo Recording"
                                statusindicator2.color="red"
                                VideoStreamer.stop_recording()
                                start_Timer.elapsedTime=0
                                timerText.text="00:00:00"
                                statusindicator2.visible=false
                                //pause_recording.visible=false
                                start_Timer.stop()
                                }
                                record_status = !record_status
                                break;
                            case 2:
                                tooltip_text= "Image Stitching"
                                VideoStreamer.image_stitching()
                                break;
                            default:
                                break;
                            }

                            //

                        }

                        onEntered:
                                {
                            switch(index)
                            {
                            case 0:
                                tooltip_text= "Capture the\n Frame"
                                break;
                            case 1:
                                if(record_status)
                                    tooltip_text= "Stop and\nSave Recording"
                                else
                                    tooltip_text= "Start\nVideo Recording"
                                break;
                            case 2:
                                tooltip_text= "Image Stitching"
                                break;
                            default:
                                break;
                            }

                                    tooltip_set(column1.x+column1.width/1.5,column1.y+column1.height/1.8,tooltip_text)
                                }

                        onExited:
                                {
                                    tooltip_reset()
                                }

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

    /*Image {
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

    }*/
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
        }


    }


    Component.onCompleted: {
        VideoStreamer.openVideoCamera(0)
        opencvImage.visible = true
        opencvImage2.visible = true
        opencvImage3.visible = true
        opencvImage4.visible = true

    }

    Connections
    {
        target: VideoStreamer

        function onRecording_stop()
        {
            //clear_status()
            template_text2.text="Alert!!!"
            template_content2.text="Video has been saved successfully!!!"
            message_template2.open()

        }

        function onImage_stitched()
        {
            template_text2.text="Alert!!!"
            template_content2.text="Images has been processed successfully!!!"
            message_template2.open()
        }
    }


    function capture_map_image()
                   {
                        /*var saveDirectory;
                            if (Qt.platform.os === "windows") {
                                saveDirectory = StandardPaths.writableLocation(StandardPaths.PicturesLocation);
                            } else {
                               saveDirectory = StandardPaths.writableLocation(StandardPaths.HomeLocation);
                            }*/
                            //var logFileDir; = link.logFileDir //saveDirectory + "/PipeLine Screenshots";
                            var date1 = new Date();
                            var formattedDate1 = date1.toLocaleString(Qt.locale("en_IN"), "dd.MM.yyyy-hh.mm.ss");
                            var logFileDir2= logFileDir +"/Screenshots "+ formattedDate1
                            if(link.create_dynamic_library(logFileDir2))
                                ;//console.log(true)
                           // Capture and save the image
                            var rectangles = [imageRect,imageRect2,imageRect3,imageRect4]
                            var rect,count_ss= 0;
                            for (var i = 0; i < rectangles.length; i++) {
                                (function (rect) {
                                    var date = new Date();
                                    var formattedDate = date.toLocaleString(Qt.locale("en_IN"), "dd.MM.yyyy-hh.mm.ss");
                                    var fileName = "/screenshot_" + formattedDate + "_" + i;
                                    rect.grabToImage(function (result) {
                                        var filePath = logFileDir2 + fileName;
                                        //console.log(filePath)
                                        filePath = filePath.replace("file:///", "");
                                        //console.log(filePath)
                                        if (result.saveToFile(filePath + ".jpeg")) {
                                            //console.log("Success: " + filePath);
                                            count_ss++
                                        } else {
                                            console.log("Failure");
                                        }
                                    }, Qt.size(opencvImage.sourceSize.width, opencvImage.sourceSize.height));
                                })(rectangles[i]);
                            }
                               map_image_Captured()

               }

    function tooltip_reset()
    {
        tooltip_template.visible=false
        /*tooltip_template.x=0
        tooltip_template.y=0
        tooltip_template.text=""*/
   }

    function tooltip_set(x,y,text)
    {
        tooltip_template.x= x
        tooltip_template.y= y
        tooltip_template.text=text
        tooltip_template.visible=true
        start_Timer3.start()
    }

    ToolTip
         {
             id:tooltip_template
             x:0
             y:0
             z:1
             text:""
             contentItem: Text
                            {
                                color:neonGreen
                                text:tooltip_template.text
                            }
             background: Rectangle
                             {    color:"black"
                                 opacity:0.5
                                 border.color: "#21be2b"
                             }
         }

    Timer {
        id: start_Timer3
        interval: 1500 // Timer interval in milliseconds
        running: false // Start the timer when the application starts

        repeat: false

        onTriggered: {
            tooltip_reset()
        }
    }

    onMap_image_Captured: {
        template_content2.text = "Frame has been\n Captured Successfully!!!"
        message_template2.open()
    }

    Dialog {
           id: message_template2
           anchors.centerIn: parent
           padding:0.005*root.width
           //z: 2
           //x: root.width / 2.25
           //y: root.height / 2.25

           contentItem: Item {
                   // We use an Item wrapper so we can set a specific width/height
                   implicitWidth: 0.15 * root.width
                   implicitHeight: 0.15 * root.height

                   Column {
                       anchors.fill: parent
                       spacing:0.0075*root.width // This prevents the overlap

                       Text {
                           id: template_text2
                           text: "Alert!!!"
                           color: "white"
                           font.bold: true
                           font.pointSize:Math.min(root.width/75,root.height/60)
                           width: parent.width
                           horizontalAlignment: Text.AlignHCenter
                       }

                       Text {
                           id: template_content2
                           text: "Video has been saved successfully!!!"
                           color: "white"
                           width: parent.width
                           wrapMode: Text.WordWrap
                           horizontalAlignment: Text.AlignHCenter
                           font.pointSize:Math.min(root.width/75,root.height/60)
                       }
                   }
               }
           footer: DialogButtonBox {
               alignment: Qt.AlignCenter
               //buttonLayout: DialogButtonBox.WinLayout
               background: Rectangle {
                   anchors.fill: parent
                   color: "transparent"
               }

               Button {
                   //text: qsTr("Save")
                   DialogButtonBox.buttonRole: DialogButtonBox.AcceptRole
                   background: Rectangle {
                       color: "#1a3154"
                   }
                   contentItem: Text {
                       id: response_button4
                       text: "Close"
                       font.pixelSize: Math.min(root.width / 90, root.height / 70)
                       style: Text.Sunken
                       color: "White"
                       horizontalAlignment: Text.AlignHCenter
                       verticalAlignment: Text.AlignVCenter
                   }
               }
           }

           background: Rectangle {
               color: "black"
               opacity: 0.75
               radius: 0.01 * parent.width
               border.color: "#21be2b"
           }

           onAccepted: {
               message_template2.close()
           }
       }

    Component {
        id: imageComponent

        Image {
            anchors.centerIn: parent
            source:"qrc:/project-B/images/processing.png"
            width: 0.5*parent.width
            height: width

            fillMode: Image.PreserveAspectFit
        }
    }

    Component {
        id: imageComponent2

        Image {
            anchors.centerIn: parent
            source:"qrc:/project-B/images/screenshot3.png"
            width: parent.width
            height: width
            fillMode: Image.PreserveAspectFit

        }
    }

    Component {
        id: canvasComponent

        Canvas {
            id: sta_sto
            //anchors.fill: parent
            anchors.centerIn: parent

                width: parent.width
                height: width

            contextType: "2d"

            onPaint: {
                var ctx = getContext("2d");
                ctx.clearRect(0, 0, width, height);

                if (!record_status) {
                    // ▶ Play icon
                    ctx.fillStyle = "#eadeda";
                    ctx.beginPath();
                    ctx.moveTo(0.3 * width, 0.2 * height);
                    ctx.lineTo(0.3 * width, 0.8 * height);
                    ctx.lineTo(0.8 * width, 0.5 * height);
                    ctx.closePath();
                    ctx.fill();
                } else {
                    // ■ Stop icon
                    ctx.fillStyle = "red";
                    ctx.fillRect(0.25* width, 0.25 * height,
                                 0.5 * width, 0.5 * height);
                }
            }

            Connections {
                target: root
                function onRecord_statusChanged() {
                    sta_sto.requestPaint()
                }
            }
        }
    }

    Rectangle {
          id: statusindicator2
          width: 0.01 * root.width
          height: width
          radius: 100
          //anchors.centerIn: parent
          color: "red"
          anchors.bottom: imageRect4.bottom
          anchors.left: parent.left
          anchors.leftMargin: 0.01 * root.width
          anchors.bottomMargin: 0.007 * parent.width
          opacity: 1
          property bool isBlink: false
          onIsBlinkChanged: {
              if (isBlink)
                  startBlinkAnimation()
              else
                  stopBlinkAnimation()
          }function startBlinkAnimation() {
              blinkAnimation.running = true
          }
          function stopBlinkAnimation() {
              blinkAnimation.running = false
              opacity = 1
          }
          SequentialAnimation {
              id: blinkAnimation
              loops: Animation.Infinite
              PropertyAnimation {
                  target: statusindicator2
                  property: "opacity"
                  from: 1
                  to: 0
                  duration: 500
              }
              PropertyAnimation {
                  target: statusindicator2
                  property: "opacity"
                  from: 0
                  to: 1
                  duration: 300
              }
          }
          Timer {
              id: start_Timer2
              interval: 500
              running: false // Start the timer when the application starts
              repeat: true
              property int elapsedTime: 1 // Elapsed time in seconds
              onTriggered: {
                  statusindicator2.isBlink = !statusindicator2.isBlink // Toggle blinking
              }
          }
      }


    Timer {
           id: start_Timer
           interval: 1000 // Timer interval in milliseconds (1 second in this case)
           running: false // Start the timer when the application starts
           repeat: true
           property int elapsedTime: 0
           onTriggered: {
               updateTimer()
           }
       }
       function updateTimer() {
           var hours = Math.floor(start_Timer.elapsedTime / 3600)
           var minutes = Math.floor((start_Timer.elapsedTime % 3600) / 60)
           var seconds = start_Timer.elapsedTime % 60
           timerText.text = padNumber(hours, 2) + ":" + padNumber(
                       minutes, 2) + ":" + padNumber(seconds, 2)
           start_Timer.elapsedTime++
       }

       function padNumber(num, size) {
           var s = num.toString()
           while (s.length < size) {
               s = "0" + s
           }
           return s
       }

       Text {
                         id: timerText
                         color: "White"
                         visible: statusindicator2.visible
                         anchors.left: statusindicator2.right
                         anchors.leftMargin:0.005*root.width
                         anchors.bottom: statusindicator2.bottom
                         //anchors.bottomMargin: 0.005*parent.width
                         font.pixelSize:Math.min(root.width/47,root.height/37)
                         opacity: statusindicator2.opacity
                     }
}
