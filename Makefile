PLATFORM := $(shell uname)
$(info Platform="$(PLATFORM)")

# Project variables
CC = clang
EXE = demo
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
$(shell touch $(EXE))

# LINUX install these packages
# sudo apt-get install libglfw3-dev libglew-dev libglm-dev
FLAGS_BUILDING = `pkg-config --static --libs glfw3 glew` -std=c++11 -Iinclude -g
FLAGS_LINKING = `pkg-config --cflags glfw3` -std=c++11 -Iinclude -g

$(info CPP_FILES       = $(CPP_FILES) )
$(info O_FILES         = $(O_FILES) )
$(info D_FILES         = $(O_FILES) )
$(info FLAGS_BUILDING  = $(FLAGS_BUILDING) )
$(info FLAGS_LINKING   = $(FLAGS_LINKING) )

all: 
	$(EXE)

# Linking
$(OUT_DIR)%.o: $(SRC_DIR)%.cpp
	$(CC) $(FLAGS_LINKING) -c -MMD -o $@ $< -include $(D_FILES)

# Building
$(EXE): $(O_FILES)
	$(CC) -o $(EXE) $(O_FILES) $(FLAGS_BUILDING)

clean:
	rm -rf $(OUT_DIR) $(EXE)
