# Add more folders to ship with the application, here
folder_01.source = src/qml/Apricot
folder_01.target = qml
DEPLOYMENTFOLDERS = folder_01

# Additional import path used to resolve QML modules in Creator's code model
QML_IMPORT_PATH =

# Installation path
# target.path =

include(src/main/main.pri)

# Please do not modify the following two lines. Required for deployment.
include(src/qtquick2applicationviewer/qtquick2applicationviewer.pri)
qtcAddDeployment()
