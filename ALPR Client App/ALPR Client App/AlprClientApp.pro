QT       += core gui network networkauth

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += $$files("*.cpp", true) \

HEADERS += $$files("*.h", true) \

INCLUDEPATH +=  $$PWD \
                $$PWD/include \
                $$PWD/include/libopenalpr \
                $$PWD/include/opencv2

DEPENDPATH +=   $$PWD \
                $$PWD/include \
                $$PWD/include/libopenalpr \
                $$PWD/include/opencv2

LIBS += -lws2_32

LIBS += -L$$PWD/libs/release  \
        -lopencv_world455 \

DEFINES += "NDEBUG"
DEFINES += "WINDOWS"
DEFINES += "_CONSOLE"
DEFINES += "NOMIXMAX"
DEFINES += "_UNICODE"
DEFINES += "UNICODE"

FORMS += \
    AlprClientApp.ui \
    LoginWindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

DISTFILES += \
    assets/images/close.png

RESOURCES += \
    Assets.qrc

win32: LIBS += -L$$PWD/libs/release/ -llibopenalpr

INCLUDEPATH += $$PWD/libs/release
DEPENDPATH += $$PWD/libs/release

win32:!win32-g++: PRE_TARGETDEPS += $$PWD/libs/release/libopenalpr.lib
else:win32-g++: PRE_TARGETDEPS += $$PWD/libs/release/liblibopenalpr.a

win32: LIBS += -L$$PWD/libs/release/ -llibtesseract-static

INCLUDEPATH += $$PWD/libs/release
DEPENDPATH += $$PWD/libs/release

win32:!win32-g++: PRE_TARGETDEPS += $$PWD/libs/release/libtesseract-static.lib
else:win32-g++: PRE_TARGETDEPS += $$PWD/libs/release/liblibtesseract-static.a

win32: LIBS += -L$$PWD/libs/release/ -lliblept-DLL

INCLUDEPATH += $$PWD/libs/release
DEPENDPATH += $$PWD/libs/release

win32:!win32-g++: PRE_TARGETDEPS += $$PWD/libs/release/liblept-DLL.lib
else:win32-g++: PRE_TARGETDEPS += $$PWD/libs/release/liblept-DLL.a
