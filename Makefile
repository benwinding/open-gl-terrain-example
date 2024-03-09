PLATFORM := $(shell uname)

# Project variables
CC = clang++
EXE_FILE = demo
OUT_DIR = out/
SRC_DIR = src/

# Directory variables
SRC_DIRS = $(shell cd $(SRC_DIR) && find . -type d)
OUT_DIRS = $(addprefix $(OUT_DIR), $(SRC_DIRS))
# File path variables
CPP_FILES = $(shell find $(SRC_DIR) -name "*.cpp")
O_FILES = $(addprefix $(OUT_DIR), $(CPP_FILES:$(SRC_DIR)%.cpp=%.o ))
D_FILES = $(addprefix $(OUT_DIR), $(CPP_FILES:$(SRC_DIR)%.cpp=%.d ))

# Create output paths
$(shell mkdir -p $(OUT_DIRS))

# LINUX install these packages
# sudo apt-get install libglfw3-dev libglew-dev libglm-dev
FLAGS_BUILDING = `pkg-config --static --libs glfw3 glew`
FLAGS_LINKING = `pkg-config --cflags glfw3` -std=c++11 -Iinclude -g

ifeq ($(PLATFORM), Darwin)
  # mac specific, breaks on Linux
	FLAGS_BUILDING += -framework OpenGL
endif

$(info Platform        = $(PLATFORM) )
$(info EXE_FILE        = $(EXE_FILE) )
$(info CPP_FILES       = $(CPP_FILES) )
$(info O_FILES         = $(O_FILES) )
$(info D_FILES         = $(O_FILES) )
$(info FLAGS_BUILDING  = $(FLAGS_BUILDING) )
$(info FLAGS_LINKING   = $(FLAGS_LINKING) )

all: $(EXE_FILE)

# Linking
$(OUT_DIR)%.o: $(SRC_DIR)%.cpp
	$(CC) $(FLAGS_LINKING) -c -MMD -o $@ $<
-include $(D_FILES)

# Building
$(EXE_FILE): $(O_FILES)
	$(CC) -o $(EXE_FILE) $(O_FILES) $(FLAGS_BUILDING)

clean:
	rm -rf $(OUT_DIR) $(EXE_FILE)
