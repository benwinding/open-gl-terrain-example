## OpenGL Example - Walking around

A little OpenGL demo project, written in C++ years ago, and updated to work on Apple Silicon too.

https://github.com/benwinding/open-gl-terrain-world/assets/11782590/7f23a46e-64da-42e5-bb0d-1418068f1128

*Some artifacts of the terrain on the mac version*

## Features:
- First Person View (walk around with arrow keys)
- Particle simulation Fire/Water (_physics has left the chat_)
- Procedurally generated terrain and trees
- Environment sky box
- Height mapping
- Sound (_just launches in another window_)

## Dependencies

This project has the following dependencies:

- Libraries:
  - OpenGl 4 (compatible with Mac)
  - GLFW
  - GLEW
- Build System
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

## Why?

This was an old OpenGl UNI assignment from years ago that was fun to make!

I wanted to get it running on a new *Apple Silicon Macbook*, but it turned out to be really hard 💀 So I thought I'd share this example, in case anyone else is wondering how to get OpenGl running on Mac Silicon M1/M2/M3 processors...
