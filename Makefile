#Este es el make file para construir el Detector de rostros desde la webcam de mi laptop
#MODIFICADO: 30 de abril del 2014

CFLAGS = `pkg-config --cflags opencv`
LIBS = `pkg-config --libs opencv`

OBJECTS = Main_Prueba_Detector.o Camara.o RostrosDector.o
SOURCES = Main_Prueba.cpp Camara/Camara.cpp Deteccion_Facial/RostrosDector.cpp\
	 Reconocimiento_Facial/ReconocerPersona.cpp

detector_de_Rostros: 
	g++ $(CFLAGS) $(SOURCES) -o Reconoce $(LIBS)


