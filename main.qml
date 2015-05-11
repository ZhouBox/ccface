import QtQuick 2.4
import QtQuick.Controls 1.3
import QtQuick.Window 2.2
import QtQuick.Dialogs 1.2
import OpenCV 1.0

ApplicationWindow {
    title: qsTr("Hello World")
    width: 640
    height: 480
    visible: true

    menuBar: MenuBar {
        Menu {
            title: qsTr("&File")
            MenuItem {
                text: qsTr("&Open")
                onTriggered: {

                    //messageDialog.show(qsTr("Open OpenCV triggered"));
                    opencvCamera.m_run = true
                }

            }
            MenuItem {
                text: qsTr("&Close")
                onTriggered: {

                    //messageDialog.show(qsTr("Open OpenCV triggered"));
                    opencvCamera.m_run = false
                }

            }
            MenuItem {
                text: qsTr("E&xit")
                onTriggered: Qt.quit();
            }
        }
    }

    OpenCVcamera {
        id:opencvCamera
        m_cameraId: 0
        m_run: false

        //width: 320
        //height: 240
    }

    OpenCVshowFrame {
        //anchors.centerIn: parent
        anchors {left:parent.left; top:parent.top}
        id:opencvShowFrame
        m_capture: opencvCamera
        m_frameRate: 33
        m_run: true
        width: 640
        height: 380
        onWho:{
            console.log(i);
            messageDialog.show(qsTr("ID:") + i);
        }
    }

    Label {
        id:label
        anchors {left: parent.left;top:opencvShowFrame.bottom;topMargin: 10}
        text: qsTr("")
    }


    TextField {
        id:employeeId
        y: 0
        anchors {left: parent.left;top:label.bottom; topMargin: 5}
        width: 80
        height: 30
        anchors.leftMargin: 0
        validator: RegExpValidator {
            regExp: /[0-9]+/
        }
        placeholderText:qsTr("ID")
    }

    Button {
        id:button

        text: qsTr("save")

        anchors {left: employeeId.right;top:employeeId.top}


        onClicked: {
            if (employeeId.text.length !== 0 && opencvCamera.m_run) {
                opencvShowFrame.setLabel(parseInt(employeeId.text))
                console.log(parseInt(employeeId.text))
                opencvShowFrame.startTrain()
                messageDialog.show(qsTr("OK!"))
            }
        }
    }

    Button {
        id:button2

        text: qsTr("select")

        anchors {right: parent.right;top:button.top}


        onClicked: {
            if (opencvCamera.m_run) {

                opencvShowFrame.startPredict();
            }

        }
    }

    MessageDialog {
        id: messageDialog
        title: qsTr("May I have your attention, please?")

        function show(caption) {
            messageDialog.text = caption;
            messageDialog.open();
        }
    }


}
