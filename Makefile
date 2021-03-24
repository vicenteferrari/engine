zombies_common_source := $(wildcard zombies_common/*.cpp)
zombies_common_headers := $(wildcard zombies_common/*.h)
zombies_server_source := $(wildcard zombies_server/*.cpp)
zombies_server_headers := $(wildcard zombies_server/*.h)
zombies_source := $(wildcard zombies/*.cpp)
zombies_headers := $(wildcard zombies/*.h)

.PHONY: all
all: build/zombies_server.exe build/zombies.exe

build/zombies_common.dll: inter/dllmain.o
	g++ -shared inter/dllmain.o -o build/zombies_common.dll

inter/dllmain.o: $(zombies_common_source) $(zombies_common_headers)
	g++ -g -c -DZOMBIESCOMMON_EXPORTS -DZOMBIES_DEBUG zombies_common/dllmain.cpp -o inter/dllmain.o

build/zombies_server.exe: inter/servermain.o build/zombies_common.dll
	g++ inter/servermain.o -o build/zombies_server.exe -Lbuild/ -lzombies_common

inter/servermain.o: $(zombies_server_source) $(zombies_server_headers)
	g++ -g -c -DZOMBIES_DEBUG zombies_server/servermain.cpp -o inter/servermain.o -Izombies_common/

build/zombies.exe: inter/main.o build/zombies_common.dll
	g++ inter/main.o -o build/zombies.exe -Lbuild/ -lzombies_common -Lextern/lib -lassimp.dll -lmingw32 -lSDL2main -lSDL2.dll -ld3dcompiler_47 -lxinput -ld3d11 -ldxgi -lgdi32 -ldwrite -ld2d1 -lole32

inter/main.o: $(zombies_source) $(zombies_headers)
	g++ -g -c -DZOMBIES_DEBUG zombies/main.cpp -o inter/main.o -Izombies_common/ -Iextern/include/

.PHONY: clean
clean:
	del inter\dllmain.o inter\servermain.o inter\main.o
	del build\zombies_common.dll
	del build\zombies_server.exe build\zombies.exe
