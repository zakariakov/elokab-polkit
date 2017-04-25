 QT       += core gui dbus
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets
CONFIG += qt \
          release
TARGET = elokab-polkit-agent
DESTDIR = usr/bin
TEMPLATE = app

OBJECTS_DIR = build
MOC_DIR = build
UI_DIR = build
INCLUDEPATH +=build
INCLUDEPATH +=user


#isEmpty(LIBPOLKIT_QT) {
#    LIBPOLKIT_QT = LIBPOLKIT_QT_1_0
#}
DEFINES +=$${LIBPOLKIT_QT}

 equals(QT_MAJOR_VERSION, 4) {
       LIBS += $(SUBLIBS)   -lpolkit-qt-agent-1 -lpolkit-qt-core-1
}

 equals(QT_MAJOR_VERSION, 5) {
      LIBS += $(SUBLIBS)   -lpolkit-qt5-agent-1 -lpolkit-qt5-core-1
}
#QMAKE_INCDIR += $$PWD/../library $$PWD/../library/include

HEADERS += \
    dialogagent.h \
    authenticationagent.h \
    notificationutill.h

SOURCES += \
    main.cpp \
    dialogagent.cpp \
    authenticationagent.cpp

FORMS += \
    dialogagent.ui

TRANSLATIONS    =../usr/share/elokab/translations/ar/elokab-polkit-agent.ts\
                 ../usr/share/elokab/translations/fr/elokab-polkit-agent.ts\
                 ../usr/share/elokab/translations/en/elokab-polkit-agent.ts\


 target.path = /usr/bin

 INSTALLS += target

linux-g++*: {
             # Provide relative path from application to elokab librarys
             # ex:app=usr/bin/elokab-applications  library=usr/lib/libelokabmimicon.so
             QMAKE_LFLAGS += -Wl,--rpath=\\\$\$ORIGIN/../lib
             }

DISTFILES += \
    usr/share/elokab/translations/ar/elokab-polkit-agent.qm \
    usr/share/elokab/translations/ar/elokab-polkit-agent.ts \
    usr/share/elokab/translations/en/elokab-polkit-agent.ts \
    usr/share/elokab/translations/fr/elokab-polkit-agent.ts
