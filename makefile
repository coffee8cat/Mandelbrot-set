SOURCES_DIR = src
SOURCES = $(wildcard $(SOURCES_DIR)/*.cpp)

BUILD_DIR = build
OBJ_DIR = obj
OBJECTS = $(wildcard $(OBJ_DIR)/*.o)
EXECUTABLE = mandelbrot-set

all:
	make compile
	make run

compile:
	export MESA_LOADER_DRIVER_OVERRIDE=llvmpipe
	export LIBGL_ALWAYS_SOFTWARE=1
	g++ $(SOURCES) -O3 -o $(BUILD_DIR)/$(EXECUTABLE) -lsfml-graphics -lsfml-window -lsfml-system

run:
	gdb $(BUILD_DIR)/$(EXECUTABLE)
