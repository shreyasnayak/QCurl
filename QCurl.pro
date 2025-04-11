QT -= gui
QT += core network

CONFIG += c++11 console
CONFIG -= app_bundle

#OS Based config
win32:{

    # Use G729 codec  : USE_BCG729_CODEC
    # DEFINES += USE_BCG729_CODEC

    !contains(QMAKE_TARGET.arch, x86_64) {
        ## TxLibs
        INCLUDEPATH += "C:\Program Files\TxLib\include"
        LIBS += "-LC:\Program Files\TxLib\lib"

        #PjSIP
        INCLUDEPATH += C:\OpenSource\PjSIP_2.10\include
        LIBS += -LC:\OpenSource\PjSIP_2.10\win32\lib

        # OpenSSL
        INCLUDEPATH += C:/OpenSource/OpenSSL/Win32/include
        LIBS += -LC:\OpenSource\OpenSSL\Win32\lib

         CONFIG(release, debug|release) {
            ## Windows x86 (32bit) specific build here
            LIBS += -llibcrypto -llibssl
         }

        # Other debug Libss
         CONFIG(debug, debug|release) {
             message("Set debug_lib path PjSIP and OpenSSL")
             LIBS += -llibcrypto -llibssl
         }


    } else {
        ## TxLibs
        INCLUDEPATH += "C:\Program Files\TxLib\include"
        LIBS += "-LC:\Program Files\TxLib\lib"

        INCLUDEPATH += "C:\Program Files\PjSIP_2_15\include"
        LIBS += "-LC:\Program Files\PjSIP_2_15\lib"

        #OpenSSL
        INCLUDEPATH += "C:\Program Files\OpenSSL\include"
        LIBS += "-LC:\Program Files\OpenSSL\lib" -llibcrypto -llibssl

        # Other release Libs
        CONFIG(release, debug|release) {
            ## Windows x64 (64bit) specific build here
            message("Set x64 release_lib path PjSIP")
            LIBS += -llibcrypto -llibssl
        }

        # Other debug Libs
        CONFIG(debug, debug|release) {
             message("Set x64 debug_lib path PjSIP")
             LIBS += -llibcrypto -llibssl
        }
    }
}

# The following define makes your compiler emit warnings if you use
# any Qt feature that has been marked deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
        main.cpp \
        qcurlrequest.cpp

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

HEADERS += \
    qcurlrequest.h
