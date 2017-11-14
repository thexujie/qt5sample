import QtQuick 2.0
import "content" as Content
import com.thexujie 1.0

XxWindow {
	id : root
    title: qsTr("挺好")
    width: 640
    height: 480
    visible: true
	color : Qt.rgba(0.7,0.5,0.7, 0.7)

    /*function timeChanged() {
        var date = new Date;
        var seconds = date.getUTCSeconds();
		root.color = Qt.rgba(0.7 * (seconds % 10),0.5,0.7,0.95)
    }

    Timer {
        interval: 100; running: true; repeat: true;
        onTriggered: root.timeChanged()
    }*/
}
