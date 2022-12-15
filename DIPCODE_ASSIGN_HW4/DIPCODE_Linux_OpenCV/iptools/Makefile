# source files.
SRC =  image/image.cpp utility/utility.cpp

OBJ = $(SRC:.cpp=.o)

OUT = ../lib/libiptools.a

# include directories
INCLUDES = -I. -I/usr/local/include -Iimage/  -Iutility/ -I/usr/include/c++/4.2.1 

# opencv library for FSprime server
OPENCV = `PKG_CONFIG_PATH=../lib pkg-config --cflags --libs opencv4`
# opencv library for local
#OPENCV = `pkg-config --cflags --libs opencv`

#opencv library
OPENCVLIBS = -L/apps/opencv/4.0.1/lib64 -lopencv_core -lopencv_highgui -lopencv_imgcodecs -lopencv_imgproc -lopencv_objdetect -Wl,-rpath=/apps/opencv/4.0.1/lib64

#include opencv directory
OPENCVINCLUDE = -I/apps/opencv/4.0.1/include/opencv4


# C++ compiler flags (-g -O2 -Wall)
CCFLAGS = -g -std=c++11

# compiler
CCC = g++


# compile flags
LDFLAGS = -g

.SUFFIXES: .cpp

.cpp.o:
	$(CCC) $(OPENCVLIBS) $(OPENCVINCLUDE) $(OPENCV) $(INCLUDES) $(CCFLAGS) -c $< -o $@

$(OUT): $(OBJ)
	ar rcs $(OUT) $(OBJ)

clean:
	rm -f $(OBJ) $(OUT) Makefile.bak# DO NOT DELETE
