CXX = g++
CXXFLAGS = -Wall -Wextra -pipe -pedantic
LDFLAGS = -lm -lSDL2 -lSDL2_image -lSDL2_ttf
SRC_DIR = ./src
OBJ_DIR = ./obj
TARGET = loopcube
SRC = $(wildcard src/*.cpp)
OBJ = $(patsubst %.cpp,obj/%.o,$(SRC))

ifeq ($(strip $(DATA_LOCATION)),)
$(error "Please set DATA_LOCATION to your data folder location in your environment. export DATA_LOCATION=<path to data/>.")
else
CXXFLAGS += -DDATA_LOCATION=\"$(DATA_LOCATION)\"
endif

all: release

obj/%.o: %.cpp %.hpp
	$(CXX) $(CXXFLAGS) $(LDFLAGS) -c $< -o $@ 

$(TARGET): $(OBJ)
	$(CXX) $(CXXFLAGS) $(LDFLAGS) -o $(TARGET) $^


debug: CXXFLAGS += -g
debug: setup
debug: $(TARGET)

release: CXXFLAGS += -O2 -ffast-math
release: setup
release: $(TARGET)

setup:
	@mkdir -p obj/src/

.PHONY: all clean setup debug release
clean:
	rm -rf $(OBJ_DIR)
	rm -rf $(TARGET)
