##Este make permite la compilacion del ejecutable TestEmision
CFLAGS = -std=gnu++0x `pkg-config --cflags opencv` -I/usr/local/include -I/home/pi/git/robidouille/raspicam_cv
CLIBS = `pkg-config --libs opencv` -L/usr/local/lib -lwiringPi -L/home/pi/git/robidouille/raspicam_cv\
 -lraspicamcv -L/home/pi/git/raspberrypi/userland/build/lib -lmmal_core -lmmal -l mmal_util -lvcos\
-lbcm_host
SRCS = Main_Emisor.cpp Emisor/EmisorDeImag.cpp 

EjecutableTestEmicion: 
	g++ $(CFLAGS) $(SRCS) -o RaspiCamEmision  $(CLIBS)
