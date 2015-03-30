# Compiler flags
CXX			=g++
CPPFLAGS	+=-Wall #-std=c++11
#CPPFLAGS	+=-static-libgcc -static-libstdc++ 
CPPFLAGS	+=-ansi -pedantic 

# For Debug
#CPPFLAGS	+=-Wextra
CPPFLAGS	+=-g

# For Optimization
CPPFLAGS	+=-O3

ifeq ($(OS),Windows_NT)
	# For GLFW
	CPPFLAGS	+=-Id:/SDK/glfw-3.1/include
	LIBS		+=-Ld:/SDK/glfw-3.1/src
	LDFLAGS		+=-lglfw3 -lgdi32

	# For OpenGL
	LDFLAGS		+=-lopengl32 -lglu32

	# For GLEW
	CPPFLAGS	+=-Id:/SDK/glew-1.12.0/include
	LIBS		+=-Ld:/SDK/glew-1.12.0/lib
	LDFLAGS		+=-lglew32
else
	# For GLFW
	LDFLAGS		+=-lglfw3 -lXinerama -lX11 -lXrandr -lXi -lXxf86vm -lXcursor -lpthread

	# For OpenGL
	LDFLAGS		+=-lGL -lGLU -lGLEW
endif

# File objects
SOURCES		=$(wildcard *.cpp) $(wildcard */*.cpp)
OBJECTS		=$(SOURCES:.cpp=.o)
WINOBJECTS	=$(subst /,\,$(OBJECTS))
EXECUTABLE	=test.exe

.PHONY: default clean cleanexe

default: cleanexe $(EXECUTABLE)

$(EXECUTABLE): $(OBJECTS)
		$(CXX) $(CPPFLAGS) -o  $(EXECUTABLE) $(OBJECTS) $(LIBS) $(LDFLAGS)

clean: cleanexe
ifeq ($(OS),Windows_NT)
	del $(WINOBJECTS)
else
	rm -rf $(OBJECTS)
endif

cleanexe:
ifeq ($(OS),Windows_NT)
	del $(EXECUTABLE)
else
	rm -rf $(EXECUTABLE)
endif
