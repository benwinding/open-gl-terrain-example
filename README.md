## OpenGL Example - Walking around

A little OpenGL demo project, written in C++ years ago, and updated to work on Apple Silicon too.

*Image goes here*

This project has the following dependencies:

Libraries:
- OpenGl 4 (compatible with Mac)
- GLFW
- GLEW
Build System
- Make
- Clang

### Build - Linux

Make sure you have installed the relevant `apt` libs:

``` shell
sudo apt install libglfw3-dev libglew-dev libglm-dev clang
# Then build with
make 
```

### Build - Mac (silicon)

Make sure you have installed the relevant `brew` libs:

``` shell
brew install glfw --build-from-source # You might not need --build-from-source but I did
brew install glew glm clang
# Then build with
make
```

## Running it

It should build an executable that you can just run with:

```
./demo
```

This was an old OpenGl UNI assignment from years ago that was fun to make!

I wanted to get it running on a new *Apple Silicon Macbook*, but it turned out to be really hard, so I thought I'd share this example, in case anyone else is wondering how to get OpenGl running on Mac Silicon M1/M2/M3 processors...


