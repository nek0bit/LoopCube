CXX = g++
CXXFLAGS = -Wall -Wextra -pipe -pedantic
LDFLAGS = -lm -lSDL2 -lSDL2_image -lSDL2_ttf
SRC_DIR = ./src
OBJ_DIR = ./obj
TARGET = loopcube
SRC = $(wildcard src/*.cpp)
OBJ = $(patsubst %.cpp,obj/%.o,$(SRC))

ifeq ($(strip $(DATA_LOCATION)),)
CXXFLAGS += -DDATA_LOCATION=\".\"
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

install:
	@echo "Installing... Make sure your binary has DATA_LOCATION set to a loopcube folder"
	mkdir -p $(DATA_LOCATION)
	cp -r data $(DATA_LOCATION)/data
	cp $(TARGET) $(PREFIX)

uninstall:
	@echo "Uninstalling... type yes if prompted (be careful)"
	rm -Ir $(DATA_LOCATION)
	rm -ir $(PREFIX)/$(TARGET)

setup:
	@mkdir -p obj/src/

.PHONY: all clean setup debug release install
clean:
	rm -rf $(OBJ_DIR)
	rm -rf $(TARGET)
