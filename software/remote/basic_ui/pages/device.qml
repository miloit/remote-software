import QtQuick 2.11
import QtQuick.Controls 2.4

Flickable {
    id: itemFlickable
    property string type

    width: parent.width
    height: parent.height
    maximumFlickVelocity: 6000
    flickDeceleration: 1000
    contentHeight: iconFlow.height < 800 - mainNavigation.height - statusBar.height - miniMediaPlayer.height + bottomGradient.height ? iconFlow.height + 40 : iconFlow.height
    boundsBehavior: Flickable.DragAndOvershootBounds
    flickableDirection: Flickable.VerticalFlick

    Behavior on contentY {
        PropertyAnimation {
            duration: 300
            easing.type: Easing.OutExpo
        }
    }

    ScrollBar.vertical: ScrollBar {
        opacity: 0.5
    }

    Column {
        id: iconFlow
        width: parent.width
        anchors.horizontalCenter: parent.horizontalCenter
        spacing: 10

        Component.onCompleted: {
            var e = entities.getByType(type);

            for (var i=0; i<e.length; i++) {
                var comp = Qt.createComponent("qrc:/components/"+ type +"/ui/Button.qml");
                if (comp.status != Component.Ready) {
                    console.debug("Error: " + comp.errorString() );
                }
                var obj = comp.createObject(iconFlow, {obj: e[i]});
            }

        }
    }
}
