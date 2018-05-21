# Project: progettoCar4

CPP  = g++
CC   = gcc
BIN  = progettoCar4

OBJ  = main.o car.o mesh.o
LINKOBJ  = main.o car.o mesh.o

# Library linking
OS := $(shell uname)
ifeq ($(OS),Darwin)
## caso Mac OS
$(info Mac OS detected)
FRMPATH=-F /Library/Frameworks
LIBS =  -framework OpenGL -framework SDL2 -framework SDL2_image -lm
$(info SDL2 libraries must be in: $(FRMPATH))
else
ifeq ($(OS),MINGW32_NT-6.1)
## caso Windows MinGW
$(info Windows MinGW detected)
FRMPATH = -IC:\MinGW\include
LIBS = -LC:\MinGW\lib -lmingw32 -lSDL2main -lSDL2 -lSDL2_image -lopengl32 -lglu32 -lm
else
##caso Linux
$(info Linux detected)
#framework presenti solo nel caso MAC OS
FRMPATH =
LIBS = -lGL -lGLU -lSDL2 -lSDL2_image -lm
endif
endif

FLAG = -Wno-deprecated
RM = rm -f

all: $(BIN)

clean:
	${RM} $(OBJ) $(BIN)

$(BIN): $(OBJ)
	$(CPP) $(LINKOBJ) -o $(BIN) $(FRMPATH) $(LIBS)

main.o: main.cpp
	$(CPP) -c $(FRMPATH) main.cpp -o main.o

car.o: car.cpp
	$(CPP) -c $(FRMPATH) car.cpp -o car.o

mesh.o: mesh.cpp
	$(CPP) -c $(FRMPATH) mesh.cpp -o mesh.o
