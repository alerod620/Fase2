QT -= gui

CONFIG += c++11 console
CONFIG -= app_bundle

# The following define makes your compiler emit warnings if you use
# any feature of Qt which as been marked deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += main.cpp \
    analizar_comando.cpp \
    comando.cpp \
    ejecutar_comando.cpp \
    escribir_disco.cpp \
    estructuras_disco.cpp \
    estructuras.cpp \
    leer_entrada.cpp \
    reporte.cpp \
    validar_comando.cpp \
    analizar_parametro.cpp

SUBDIRS += \
    Fase2.pro

DISTFILES += \
    Fase2.pro.user

HEADERS += \
    analizar_comando.h \
    comando.h \
    ejecutar_comando.h \
    escribir_disco.h \
    estructuras_disco.h \
    estructuras.h \
    leer_entrada.h \
    reporte.h \
    validar_comando.h \
    analizar_parametro.h
