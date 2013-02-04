OSTYPE := $(shell uname -s)

BIN_DIR = bin

INC_DIRS = /home_nfs/2012ws_greifarm_kinect_b/kinect/openni/Platform/Linux-x86/Redist/Include /usr/include/ni

#SRC_FILES = \
#	testInverse.cpp \
#	SceneDrawer.cpp	\
#	actionfield.cpp \
#	serialline.cpp \
#	servo.cpp \
#	servoboard.cpp \
#	roboarm.cpp \
#	Inverse.cpp 

SRC_FILES = \
	main.cpp \
	SceneDrawer.cpp	\
	actionfield.cpp \
	serialline.cpp \
	servo.cpp \
	servoboard.cpp \
	Inverse.cpp \
	roboarm.cpp  


EXE_NAME = Greifarm

ifneq "$(GLES)" "1"
ifeq ("$(OSTYPE)","Darwin")
	LDFLAGS += -framework OpenGL -framework GLUT
else
	USED_LIBS += glut
endif
else
	DEFINES += USE_GLES
	USED_LIBS += GLES_CM IMGegl srv_um
	SRC_FILES += opengles.cpp
endif

USED_LIBS += OpenNI

LIB_DIRS += /usr/lib
include /home_nfs/2012ws_greifarm_kinect_b/kinect/openni/Platform/Linux-x86/Redist/Samples/Build/Common/CommonCppMakefile

