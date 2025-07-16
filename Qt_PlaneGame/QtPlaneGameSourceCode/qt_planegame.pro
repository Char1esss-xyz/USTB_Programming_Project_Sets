QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    bullet.cpp \
    enemy.cpp \
    enemyexplosion.cpp \
    gameview.cpp \
    main.cpp \
    playerexplosion.cpp \
    supply.cpp

HEADERS += \
    bullet.h \
    enemy.h \
    enemyexplosion.h \
    gameview.h \
    playerexplosion.h \
    supply.h

FORMS += \

TRANSLATIONS += \
    qt_planegame_zh_CN.ts
CONFIG += lrelease
CONFIG += embed_translations

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    planegame.qrc
