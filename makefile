SOURCES_DIR = src
SOURCES = $(wildcard $(SOURCES_DIR)/*.cpp)

BUILD_DIR = build
OBJ_DIR = obj
OBJECTS = $(wildcard $(OBJ_DIR)/*.o)

HEADERS_DIR = headers

EXECUTABLE = mandelbrot-set

all:
	make compile
	make run

compile:
	export MESA_LOADER_DRIVER_OVERRIDE=llvmpipe
	export LIBGL_ALWAYS_SOFTWARE=1
	g++ $(SOURCES) -O3 -o $(BUILD_DIR)/$(EXECUTABLE) -I$(HEADERS_DIR) -lsfml-graphics -lsfml-window -lsfml-system -mavx2

run:
	$(BUILD_DIR)/$(EXECUTABLE) --graph
