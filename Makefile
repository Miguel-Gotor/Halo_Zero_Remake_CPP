#=========== MAKEFILE ==========#

TARGET = 'Halo Zero.exe'

OBJ_DIR = ./obj
BIN_DIR = ./bin
SRC_DIR = ./src
INC_DIR = ./inc
LIB_DIR = ./lib
RSC_DIR = ./rsc

SRCS = $(SRC_DIR)/main.cpp \
$(SRC_DIR)/audio.cpp \
$(SRC_DIR)/anim.cpp \
$(SRC_DIR)/text.cpp 

RSCS = $(RSC_DIR)/icon.res

LIBS = -lraylib -lopengl32 -lgdi32 -lwinmm -mwindows

CXX    = g++
CPPFLAGS = -Wall -O2 

default:
	$(CXX) $(CPPFLAGS) $(SRCS) $(RSCS) -I $(INC_DIR) -L $(LIB_DIR) $(LIBS) -o $(TARGET)