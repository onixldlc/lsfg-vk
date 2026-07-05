import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import QtQuick.Window
import lsfgvk 1.0
import "dialogs"
import "panes"
import "widgets"

ApplicationWindow {
    title: "lsfg-vk Configuration Window"
    width: 900
    height: 550
    minimumWidth: 700
    minimumHeight: 400
    visible: true

    // Qt Quick Controls Fusion in Qt 6.4 doesn't reuse palette.windowText for
    // Button/Text roles when the platform theme leaves them unset — it falls
    // back to its own defaults, which look wrong on our platform-theme window
    // colour. Alias the roles that Labels use correctly (windowText, window)
    // into the Button/Text/Base roles so all controls share the same source.
    palette.buttonText: palette.windowText
    palette.text: palette.windowText
    palette.button: palette.window
    palette.base: palette.window
    palette.alternateBase: palette.window

    CenteredDialog {
        id: create_dialog
        name: "Create New Profile"
        onConfirm: Backend.createProfile(create_name.text)

        TextField {
            Layout.fillWidth: true
            id: create_name
            placeholderText: "Choose a profile name"
            focus: true
        }
    }

    CenteredDialog {
        id: rename_dialog
        name: "Rename Profile"
        onConfirm: Backend.renameProfile(rename_name.text)

        TextField {
            Layout.fillWidth: true
            id: rename_name
            placeholderText: "Choose a profile name"
            focus: true
        }
    }

    CenteredDialog {
        id: delete_dialog
        name: "Confirm Deletion"
        onConfirm: Backend.deleteProfile()

        Label {
            Layout.fillWidth: true
            text: "Are you sure you want to delete the selected profile?"
            horizontalAlignment: Text.AlignHCenter
        }
    }

    LargeDialog {
        id: active_in_dialog

        List {
            Layout.fillWidth: true
            Layout.fillHeight: true

            model: Backend.active_in
            selected: Backend.active_in_index
            onSelect: (index) => {
                Backend.active_in_index = index
                var idx = Backend.active_in.index(index, 0);
                active_in_name.text = Backend.active_in.data(idx);
            }
        }

        RowLayout {
            spacing: 8

            TextField {
                Layout.fillWidth: true
                id: active_in_name
                placeholderText: "Specify linux binary / exe file / process name"
                focus: true
            }
            Button {
                text: "Add"
                icon.name: "list-add"
                onClicked: Backend.addActiveIn(active_in_name.text)
            }
            Button {
                text: "Remove"
                icon.name: "list-remove"
                onClicked: Backend.removeActiveIn()
            }
        }
    }

    SplitView {
        anchors.fill: parent
        orientation: Qt.Horizontal

        Pane {
            SplitView.minimumWidth: 200
            SplitView.preferredWidth: 250
            SplitView.maximumWidth: 300

            Label {
                text: "Profiles"
                Layout.fillWidth: true
                font.bold: true
                horizontalAlignment: Text.AlignHCenter
            }

            List {
                model: Backend.profiles
                selected: Backend.profile_index
                onSelect: (index) => Backend.profile_index = index
            }

            Button {
                Layout.fillWidth: true
                text: "Create New Profile"
                onClicked: {
                    create_name.text = ""
                    create_dialog.open()
                }
            }
            Button {
                Layout.fillWidth: true
                text: "Rename Profile"
                onClicked: {
                    var idx = Backend.profiles.index(Backend.profile_index, 0);
                    rename_name.text = Backend.profiles.data(idx);
                    rename_dialog.open()
                }
            }
            Button {
                Layout.fillWidth: true
                text: "Delete Profile"
                onClicked: {
                    delete_dialog.open()
                }
            }
        }

        Pane {
            SplitView.fillWidth: true

            Group {
                name: "Global Settings"

                GroupEntry {
                    title: "Path to Lossless Scaling"
                    description: "Change the location of Lossless.dll"

                    FileEdit {
                        Layout.fillWidth: true

                        title: "Select Lossless.dll"
                        filter: "Dynamic Link Library Files (*.dll)"

                        text: Backend.dll
                        onUpdate: (text) => Backend.dll = text
                    }
                }

                GroupEntry {
                    title: "Allow half-precision"
                    description: "Allow acceleration through half-precision"

                    CheckBox {
                        Layout.alignment: Qt.AlignRight

                        checked: Backend.allow_fp16
                        onToggled: Backend.allow_fp16 = checked
                    }
                }
            }

            Group {
                name: "Profile Settings"
                enabled: Backend.available

                GroupEntry {
                    title: "Active In"
                    description: "Specify which applications this profile is active in"

                    Button {
                        Layout.alignment: Qt.AlignRight

                        text: "Edit..."
                        onClicked: active_in_dialog.open()
                    }
                }

                GroupEntry {
                    title: "Multiplier"
                    description: "Control the amount of generated frames"

                    SpinBox {
                        Layout.alignment: Qt.AlignRight

                        from: 2
                        to: 100

                        value: Backend.multiplier
                        onValueModified: Backend.multiplier = value
                    }
                }

                GroupEntry {
                    title: "Flow Scale"
                    description: "Lower the internal motion estimation resolution"

                    FlowSlider {
                        Layout.fillWidth: true

                        from: 0.25
                        to: 1.00

                        value: Backend.flow_scale
                        onUpdate: (value) => Backend.flow_scale = value
                    }
                }

                GroupEntry {
                    title: "Performance Mode"
                    description: "Use a significantly lighter frame generation model"

                    CheckBox {
                        Layout.alignment: Qt.AlignRight

                        checked: Backend.performance_mode
                        onToggled: Backend.performance_mode = checked
                    }
                }

                GroupEntry {
                    title: "Pacing Mode"
                    description: "Change how frames are presented to the display"

                    ComboBox {
                        Layout.fillWidth: true

                        model: ["None"]
                        currentIndex: Backend.pacing_mode
                        onActivated: (index) => Backend.pacing_mode = index
                    }
                }

                GroupEntry {
                    title: "GPU"
                    description: "Select which GPU to use for frame generation"

                    ComboBox {
                        Layout.fillWidth: true

                        model: Backend.gpus
                        currentIndex: Backend.gpu
                        onActivated: (index) => Backend.gpu = index
                    }
                }
            }

            Item {
                Layout.fillHeight: true
            }
        }
    }
}
