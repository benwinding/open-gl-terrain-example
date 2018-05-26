PLATFORM := $(shell uname)
$(info Platform="$(PLATFORM)")

# LINUX install these packages
# sudo apt-get install libglfw3-dev libglew-dev libglm-dev
GL_LIBS = `pkg-config --static --libs glfw3 glew`
EXT =
CPPFLAGS = `pkg-config --cflags glfw3` -std=c++11 -Iinclude -g

# Project variables
CC = g++
EXE = assign3_part2
OUT_DIR = out/
SRC_DIR = src/

# Directory variables
SRC_DIRS = $(shell cd $(SRC_DIR) && find . -type d)
OUT_DIRS = $(addprefix $(OUT_DIR), $(SRC_DIRS))
# File path variables
CPP_FILES = $(shell find $(SRC_DIR) -name "*.cpp")
O_FILES = $(addprefix $(OUT_DIR), $(CPP_FILES:$(SRC_DIR)%.cpp=%.o ))
D_FILES = $(addprefix $(OUT_DIR), $(CPP_FILES:$(SRC_DIR)%.cpp=%.d ))

# $(info CPP_FILES = $(CPP_FILES) )
# $(info O_FILES   = $(O_FILES) )
# $(info D_FILES   = $(D_FILES) )

# Create output paths
$(shell mkdir -p $(OUT_DIRS))

all: $(EXE)

$(OUT_DIR)%.o: $(SRC_DIR)%.cpp
	$(CC) $(CPPFLAGS) -c -MMD -o $@ $<
-include $(D_FILES)

$(EXE): $(O_FILES)
	$(CC) -o $(EXE) $(O_FILES) $(GL_LIBS)

clean:
	rm -rf $(OUT_DIR)* $(EXE)$(EXT)
