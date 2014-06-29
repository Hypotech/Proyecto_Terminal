#Este es el make file para construir el Detector y reconocimiento de rostros desde la camara de la raspberry
#MODIFICADO: 29 de mayo del 2014

CFLAGS = -std=gnu++11 `pkg-config --cflags opencv`
LIBS = `pkg-config --libs opencv`

OBJECTS = Main_Prueba_Detector.o Camara.o RostrosDector.o
SOURCES = Main_Prueba.cpp Camara/Camara.cpp Deteccion_Facial/RostrosDector.cpp\
	 Reconocimiento_Facial/ReconocerPersona.cpp Comunicacion/Receptor/ReceptorDeImag.cpp

detector_de_Rostros: 
	g++ $(CFLAGS) $(SOURCES) -o Reconoce $(LIBS)

