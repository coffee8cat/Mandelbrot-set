SOURCES_DIR = src
SOURCES = $(wildcard $(SOURCES_DIR)/*.cpp)

BUILD_DIR = build
OBJ_DIR = obj
OBJECTS = $(wildcard $(OBJ_DIR)/*.o)

HEADERS_DIR = headers

EXECUTABLE_O0 = mandelbrot-set-O0
EXECUTABLE_O1 = mandelbrot-set-O1
EXECUTABLE_O2 = mandelbrot-set-O2
EXECUTABLE_O3 = mandelbrot-set-O3

all:
	make compile
	make run

compile:
	export MESA_LOADER_DRIVER_OVERRIDE=llvmpipe
	export LIBGL_ALWAYS_SOFTWARE=1
	g++ $(SOURCES) -O3 -o $(BUILD_DIR)/$(EXECUTABLE_O3) -I$(HEADERS_DIR) -lsfml-graphics -lsfml-window -lsfml-system -mavx2

compile_On:
	export MESA_LOADER_DRIVER_OVERRIDE=llvmpipe
	export LIBGL_ALWAYS_SOFTWARE=1
	g++ $(SOURCES) -O0 -o $(BUILD_DIR)/$(EXECUTABLE_O0) -I$(HEADERS_DIR) -lsfml-graphics -lsfml-window -lsfml-system -mavx2
	g++ $(SOURCES) -O1 -o $(BUILD_DIR)/$(EXECUTABLE_O1) -I$(HEADERS_DIR) -lsfml-graphics -lsfml-window -lsfml-system -mavx2
	g++ $(SOURCES) -O2 -o $(BUILD_DIR)/$(EXECUTABLE_O2) -I$(HEADERS_DIR) -lsfml-graphics -lsfml-window -lsfml-system -mavx2
	g++ $(SOURCES) -O3 -o $(BUILD_DIR)/$(EXECUTABLE_O3) -I$(HEADERS_DIR) -lsfml-graphics -lsfml-window -lsfml-system -mavx2

run_tests:
	$(BUILD_DIR)/$(EXECUTABLE_O0) --testing
	$(BUILD_DIR)/$(EXECUTABLE_O1) --testing
	$(BUILD_DIR)/$(EXECUTABLE_O2) --testing
	$(BUILD_DIR)/$(EXECUTABLE_O3) --testing

run_graph:
	$(BUILD_DIR)/$(EXECUTABLE_O3) --graph
