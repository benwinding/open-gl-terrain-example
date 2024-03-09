SRC_DIR="./src"
INCLUDE_DIR="./include"
CPP_FILES=$(find $SRC_DIR -name "*.cpp")
H_FILES=$(find $INCLUDE_DIR -name "*.h")

# echo "$CPP_FILES"
# clang++ $H_FILES -c -I$(brew --prefix)/include -L$(brew --prefix)/lib -lglfw -framework OpenGL -std=c++11 -Iinclude -g
# clang++ -o main -

clang -std=c++11 \
  -c \
  -I/include \
  -I/opt/homebrew/Cellar/glfw/3.3.8/include \
  -I/opt/homebrew/Cellar/glew/2.2.0_1/include/GL \
  -I/usr/local/include \
  -Iinclude \
  -g \
  -MMD \
  -o out//main.o src//main.cpp