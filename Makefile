CC=g++
# LIBS on osx are in the framework
#LIBS= -lglut -lGL -lGLU
INCLUDES=  
<<<<<<< HEAD
CCFLAGS= -O0 -g -I. -DOSX -framework OpenGL -framework GLUT
=======
CCFLAGS= -O0 -I. -DOSX -Wno-deprecated -framework OpenGL -framework GLUT
>>>>>>> d8ef21633a1ef64ef1ebf20b7afa3892f08c22ce
SOURCES=
OBJECTS=$(SOURCES:.cc=.o)
EXECUTABLE=jet

# --- targets
all:  $(SOURCES) $(EXECUTABLE)
	
$(EXECUTABLE): jet.cc $(OBJECTS)
	$(CC) jet.cc $(OBJECTS) -o $(EXECUTABLE) $(CCFLAGS) $(INCLUDES)  $(LIBS) 

.cc.o:
	$(CC) $(CCFLAGS) $(INCLUDES) $< -c

clean:
	rm -f $(EXECUTABLE) $(OBJECTS)
