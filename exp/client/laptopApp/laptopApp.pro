QT       += core gui network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    alpr/AlprAdapter.cpp \
    generator/OpenCvAdapter.cpp \
    generator/FrameGenerator.cpp \
    login/LoginWindow.cpp \
    main.cpp \
    LaptopApp.cpp \
    model/DebugInfoModel.cpp \
    model/FrameModel.cpp \
    model/RecentPlatesModel.cpp \
    model/VehicleInfoModel.cpp \
    handler/MsgHandlerManager.cpp \
    handler/DebugInfoMsgHandler.cpp \
    handler/FrameMsgHandler.cpp \
    handler/RecentPlatesMsgHandler.cpp \
    handler/VehicleInfoMsgHandler.cpp

HEADERS += \
    LaptopApp.h \
    alpr/AlprAdapter.h \
    generator/OpenCvAdapter.h \
    generator/FrameGenerator.h \
    handler/DebugInfoMsgHandler.h \
    handler/FrameMsgHandler.h \
    handler/IMsgHandler.h \
    handler/MsgHandlerManager.h \
    handler/RecentPlatesMsgHandler.h \
    handler/VehicleInfoMsgHandler.h \
    login/LoginWindow.h \
    model/DebugInfoModel.h \
    model/FrameModel.h \
    model/RecentPlatesModel.h \
    model/VehicleInfoModel.h \
    MsgHandler/DebugInfoMsgHandler.h \
    MsgHandler/IMsgHandler.h \
    MsgHandler/PlaybackMsgHandler.h \
    MsgHandler/RecentPlatesMsgHandler.h \
    MsgHandler/MsgHandlerManager.h \
    MsgHandler/VehicleInfoMsgHandler.h

INCLUDEPATH +=  $$PWD \
                $$PWD/include \
                $$PWD/libs/dependencies/include/tesseract \
                $$PWD/lib/libopenalpr \
                $$PWD/lib/precomplied

DEPENDPATH +=   $$PWD \
                $$PWD/include \
                $$PWD/libs/dependencies/include/tesseract \
                $$PWD/lib/libopenalpr \
                $$PWD/lib/precomplied

LIBS += -lws2_32

LIBS += -L$$PWD/lib  \
        -lopencv_world455 \

DEFINES += "NDEBUG"
DEFINES += "WINDOWS"
DEFINES += "_CONSOLE"
DEFINES += "NOMIXMAX"
DEFINES += "_UNICODE"
DEFINES += "UNICODE"

FORMS += \
    LaptopApp.ui \
    LoginWindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

DISTFILES += \
    assets/images/close.png

RESOURCES += \
    Assets.qrc

win32: LIBS += -L$$PWD/lib/precomplied/ -llibopenalpr

INCLUDEPATH += $$PWD/lib/precomplied
DEPENDPATH += $$PWD/lib/precomplied

win32:!win32-g++: PRE_TARGETDEPS += $$PWD/lib/precomplied/libopenalpr.lib
else:win32-g++: PRE_TARGETDEPS += $$PWD/lib/precomplied/liblibopenalpr.a

win32: LIBS += -L$$PWD/lib/precomplied/ -llibtesseract-static

INCLUDEPATH += $$PWD/lib/precomplied
DEPENDPATH += $$PWD/lib/precomplied

win32:!win32-g++: PRE_TARGETDEPS += $$PWD/lib/precomplied/libtesseract-static.lib
else:win32-g++: PRE_TARGETDEPS += $$PWD/lib/precomplied/liblibtesseract-static.a

win32: LIBS += -L$$PWD/lib/precomplied/ -lliblept-DLL

INCLUDEPATH += $$PWD/lib/precomplied
DEPENDPATH += $$PWD/lib/precomplied

win32:!win32-g++: PRE_TARGETDEPS += $$PWD/lib/precomplied/liblept-DLL.lib
else:win32-g++: PRE_TARGETDEPS += $$PWD/lib/precomplied/libliblept-DLL.a
