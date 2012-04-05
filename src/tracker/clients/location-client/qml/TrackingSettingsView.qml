// import QtQuick 1.0 // to target S60 5th Edition or Maemo 5
import QtQuick 1.1
import com.nokia.symbian 1.1

CommonDialog{
    property int curIndex: 0
    property bool curChecked: true
    signal settingschanged(int interval, bool permission)
    function getInterval(){
        switch(curIndex)
        {
        case 0:
          return 60;
        case 1:
          return 5*60;
        case 2:
            return 15*60;
        case 3:
            return 30*60;
        case 4:
            return 60*60;
        default:
          return 5;
        }
        }


    id: trackSettings
    anchors.fill: parent
    titleText: "Tracking settings"
    content: Column{
    spacing: 10

    CheckBox {
        id: checkbox1
        checked: true
        text: "To allow tracking of me"
           }

    SelectionListItem {
        id: item
        title: "Time interval for connection"
        subTitle: selectionDialog.selectedIndex >= 0
                      ? selectionDialog.model.get(selectionDialog.selectedIndex).name
                      : "Please select"

        onClicked: selectionDialog.open()
        SelectionDialog {
            id: selectionDialog
            titleText: "Select one of the values"
            selectedIndex: 0
            model: ListModel {
                    ListElement { name: "1 min" }
                    ListElement { name: "5 min" }
                    ListElement { name: "15 min" }
                    ListElement { name: "30 min" }
                    ListElement { name: "1 h" }
                }
            }
        }
}
buttons: ToolBar {
    id: buttons
    width: parent.width
    anchors.bottom: parent.bottom
    height: privateStyle.toolBarHeightLandscape + 2 * platformStyle.paddingSmall

    tools: Row {
        anchors.centerIn: parent
        spacing: platformStyle.paddingMedium

        ToolButton {
            text: "Ok"
            width: (buttons.width - 3 * platformStyle.paddingMedium) / 2
            onClicked: {curChecked = checkbox1.checked;
                curIndex = selectionDialog.selectedIndex
                trackSettings.settingschanged(getInterval(),curChecked)
                trackSettings.accept()
            }

        }

        ToolButton {
            text: "Cancel"
            width: (buttons.width - 3 * platformStyle.paddingMedium) / 2
            onClicked: {checkbox1.checked = trackSettings.curChecked; selectionDialog.selectedIndex = curIndex;trackSettings.reject()}
             }
       }
    }

}
