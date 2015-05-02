QT       += core gui multimedia multimediawidgets

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = ServidorCERF
TEMPLATE = app

INCLUDEPATH += ../../Camara ../../Comunicacion/Receptor \# ruta donde buscara los headers
    ../../Deteccion_Facial ../../Reconocimiento_Facial ../../Deteccion_Ojos \
    ../../manejoArchivos

CONFIG += link_pkgconfig static #habilita soporte para PKGCONFIG
#CONFIG += gnu++11
QMAKE_CXXFLAGS += -std=gnu++11
PKGCONFIG = opencv # habilita usar los flags especificos de OpenCV

SOURCES += \
    ventanaprincipal.cpp \
    agregar_usuario.cpp \
    agregar_usur_foto.cpp \
    ../../Camara/Camara.cpp \
    ../../Comunicacion/Receptor/ReceptorDeImag.cpp \
    ../../Deteccion_Facial/RostrosDector.cpp \
    ../../Reconocimiento_Facial/ReconocerPersona.cpp\
    ../../main.cpp \
    ver_raspicam.cpp \
    ventanaquitar.cpp \
    ventana_config.cpp \
    ../../Deteccion_Ojos/ojosdetector.cpp \
    agregar_usur_finalizar.cpp \
    ../../manejoArchivos/manejoarchivos.cpp

HEADERS += \
    ventanaprincipal.h \
    agregar_usuario.h \
    agregar_usur_foto.h \
    ver_raspicam.h \
    ventanaquitar.h \
    ventana_config.h \
    agregar_usur_finalizar.h \

FORMS += \
    ventanaprincipal.ui \
    agregar_usuario.ui \
    agregar_usur_foto.ui \
    ver_raspicam.ui \
    ventanaquitar.ui \
    ventana_config.ui \
    agregar_usur_finalizar.ui

OTHER_FILES +=

RESOURCES += \
    utiles.qrc

QMAKE_CXXFLAGS += -Wall
