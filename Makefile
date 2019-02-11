# Project: progettoSpaceShip

CPP  = g++ -std=c++0x -w
BIN  = Game
OBJ  = main.o mesh/mesh.o Bridge/Bridge.o Cube/cube.o Coin/Coin.o Ship/Ship.o Sky/Sky.o Lake/Lake.o Floor/Floor.o
LIBS = -lGL -lGLU -lSDL2 -lSDL2_image -lSDL2_ttf -lm

#FLAG = -w
RM = rm -f

all: $(BIN)

clean:
	${RM} $(OBJ) $(BIN)

$(BIN): $(OBJ)
	$(CPP) $(OBJ) -o $(BIN) $(LIBS)

main.o: main.cpp
	$(CPP) -c main.cpp -o main.o

Coin/Coin.o: Coin/Coin.cpp
	$(CPP) -c Coin/Coin.cpp -o Coin/Coin.o

Bridge/Bridge.o: Bridge/Bridge.cpp
	$(CPP) -c Bridge/Bridge.cpp -o Bridge/Bridge.o

Lake/Lake.o: Lake/Lake.cpp
	$(CPP) -c Lake/Lake.cpp -o Lake/Lake.o

GLText/GLText.o: GLText/GLText.cpp
	$(CPP) -c GLText/GLText.cpp -o GLText/GLText

mesh/mesh.o: mesh/mesh.cpp
	$(CPP) -c mesh/mesh.cpp -o mesh/mesh.o

Floor/Floor.o: Floor/Floor.cpp
	$(CPP) -c Floor/Floor.cpp -o Floor/Floor.o

Ship/Ship.o: Ship/Ship.cpp
	$(CPP) -c Ship/Ship.cpp -o Ship/Ship.o

Sky/Sky.o: Sky/Sky.cpp
	$(CPP) -c Sky/Sky.cpp -o Sky/Sky.o

Cube/cube.o: Cube/cube.cpp
	$(CPP) -c Cube/cube.cpp -o Cube/cube.o

run: $(BIN)
	./$(BIN)

