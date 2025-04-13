QT       += core gui widgets network
RESOURCES += mainmenu.qrc \
    photos.qrc

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    drawit.cpp \
    main.cpp

HEADERS += \
    drawit.h

FORMS +=

TRANSLATIONS += drawit_en.ts \
                drawit_ru.ts

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    mainmenu.qrc

TRANSLATIONS += \
    drawit_en_US.ts \
    drawit_ru_RU.ts
