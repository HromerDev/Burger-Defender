.PHONY: default windows

default:
	rm -rf LinuxBuild
	mkdir -p LinuxBuild

	gcc \
		Core/main.c \
		Core/dynamic_array.c \
		Core/Game/Map/map_reader.c \
		Core/Game/Map/map_grid.c \
		Core/Game/Towers/tower_logic.c \
		Core/Game/Enemy/enemy_logic.c \
		Core/UI/user_interface.c \
		Core/Game/gameloop_logic.c \
		-I/usr/include/SDL2 \
		-lm \
		-lSDL2 \
		-lSDL2_ttf \
		-lSDL2_image \
		-g \
		-fsanitize=address \
		-o LinuxBuild/BurgerDefender

	cp -r Assets LinuxBuild/
	cp -r Maps LinuxBuild/
	cp README.MD LinuxBuild/


windows:
	rm -rf WinBuild
	mkdir -p WinBuild

	x86_64-w64-mingw32-gcc \
		Core/main.c \
		Core/dynamic_array.c \
		Core/Game/Map/map_reader.c \
		Core/Game/Map/map_grid.c \
		Core/Game/Towers/tower_logic.c \
		Core/Game/Enemy/enemy_logic.c \
		Core/UI/user_interface.c \
		Core/Game/gameloop_logic.c \
		-Isrc/include \
		-Lsrc/lib \
		-lmingw32 \
		-lSDL2main \
		-lSDL2 \
		-lSDL2_ttf \
		-lSDL2_image \
		-lm \
		-g \
		-o WinBuild/BurgerDefender.exe

	cp src/bin/SDL2.dll WinBuild/
	cp src/bin/SDL2_image.dll WinBuild/
	cp src/bin/SDL2_ttf.dll WinBuild/
	cp -r Assets WinBuild/
	cp -r Maps WinBuild/
	cp README.MD WinBuild/
