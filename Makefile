# Main target
#EXE = ws
all: project

#  MinGW
ifeq "$(OS)" "Windows_NT"
CFLG=-O3 -Wall
LIBS=-lglfw 
CLEAN=del *.exe *.o *.a
else
#  OSX
ifeq "$(shell uname)" "Darwin"
glfw = /usr/local/Cellar/glfw/3.3.2
glfw_inc = $(glfw)/include
glfw_lib = $(glfw)/lib
CFLG=-O3 -Wall -Wno-deprecated-declarations -I$(glfw_inc)
LIBS= -L$(glfw_lib)
#  Linux/Unix/Solaris
else
CFLG=-O3 -Wall
LIBS=-lglfw -lGLU -lGL -lm -dl
endif
#  OSX/Linux/Unix/Solaris
CLEAN=rm -f *.o *.a
endif

# Dependencies
project.o: project.cpp
	g++ -c $(CFLG) -o project.o project.cpp -std=c++11
glad.o: glad.c glad.h khrplatform.h
#loadtexbmp.o: loadtexbmp.c CSCIx229.h
#errcheck.o: errcheck.c CSCIx229.h
#fatal.o: fatal.c CSCIx229.h
axis.o: axis.cpp axis.h
landscape.o: landscape.cpp landscape.h
skybox.o: skybox.cpp skybox.h
particles.o: particles.cpp particles.h
grass.o: grass.cpp grass.h
imgui.o: imgui.cpp imgui.h
imgui_impl_glfw.o: imgui_impl_glfw.cpp imgui_impl_glfw.h
imgui_impl_opengl3.o: imgui_impl_opengl3.cpp imgui_impl_opengl3.h
imgui_demo.o: imgui_demo.cpp
imgui_draw.o: imgui_draw.cpp
imgui_widgets.o: imgui_widgets.cpp
#weather.o: weather.cpp weather.h

#print.o: print.c CSCIx229.h
# Compile rules
.c.o:
	gcc -c $(CFLG) $<
.cpp.o:
	g++ -c $(CFLG) -std=c++11 $<

#  Link
project: project.o glad.o  axis.o landscape.o skybox.o particles.o grass.o imgui.o imgui_impl_glfw.o imgui_impl_opengl3.o imgui_demo.o imgui_draw.o imgui_widgets.o
	g++ -O3 -o $@ $^   $(LIBS) $(CFLG) -lglfw -framework OpenGL
	rm -f *.o *.a

#  Clean
clean:
	rm -f project