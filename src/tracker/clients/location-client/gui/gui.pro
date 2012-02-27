# Add more folders to ship with the application, here
folder_01.source = qml/gui
folder_01.target = qml
DEPLOYMENTFOLDERS = folder_01

# Additional import path used to resolve QML modules in Creator's code model
QML_IMPORT_PATH =

symbian:TARGET.UID3 = 0xEBD4C177

# Smart Installer package's UID
# This UID is from the protected range and therefore the package will
# fail to install if self-signed. By default qmake uses the unprotected
# range value if unprotected UID is defined for the application and
# 0x2002CCCF value if protected UID is given to the application
#symbian:DEPLOYMENT.installer_header = 0x2002CCCF

# Allow network access on Symbian
symbian:TARGET.CAPABILITY += NetworkServices

# If your application uses the Qt Mobility libraries, uncomment the following
# lines and add the respective components to the MOBILITY variable.
# CONFIG += mobility
# MOBILITY +=

# Speed up launching on MeeGo/Harmattan when using applauncherd daemon
CONFIG += qdeclarative-boostable

# Add dependency to Symbian components
# CONFIG += qt-components

DEPENDPATH += .
INCLUDEPATH += . ../lib-client \
               ../../../../http_requests/inc \
               ../../../../json/inc \
               ../../../../../3rdparty/qjson-0.7.1/src/ \
               ../../../../common/inc
QT+=network
CONFIG+=network
QMAKE_LFLAGS += -L../lib-client
LIBS +=-lclient

# The .cpp file which was generated for your project. Feel free to hack it.
SOURCES += \
    main.cpp \
    MainWindow.cpp

HEADERS += \
    MainWindow.h

QT += declarative network
TARGET = geo2tag
TEMPLATE=app
CONFIG += mobility12 network debug
MOBILITY += location
#LIBS+= -lQtLocation


RESOURCES += \
     mapviewer.qrc
OTHER_FILES += \
    RecButton.qml\
    images/quit.png \
    images/titlebar.png \
    images/titlebar.sci \
    LoginView.qml \
    MapViewer.qml \
    main.qml \
    NotifyDialog.qml

maemo5 {
    target.path = /opt/gui/bin
    INSTALLS += target
}

contains(MEEGO_EDITION,harmattan) {
    target.path = /opt/gui/bin
    INSTALLS += target
}

