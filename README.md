## `open-gl-terrain-example`

This is a little OpenGL demo project, written in C++ years ago ([my original code](https://github.com/benwinding/CGA3-part2)), and updated to work on Apple Silicon too!

https://github.com/benwinding/open-gl-terrain-example/assets/11782590/e67bb6ea-5012-41d4-8c62-a193dc05d9bb

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

- C++ Libraries:
  - OpenGL (compatible with Mac Silicon)
  - GLFW
  - GLEW
- Build System
  - Make
  - Clang

### Build - Nix (Mac + Linux)

If you prefer isolated deps on macOS or Linux:

``` shell
# Flakes
nix develop
# Or non-flake
nix-shell
# Then build with
make
```

Or use the helper script:

``` shell
./scripts/nix-build.sh
```

### Build - Web (Emscripten)

With Nix (includes emscripten) you can build a WebGL version:

``` shell
make web
# or
./scripts/nix-web-build.sh
```

This outputs `web/index.html`, `web/index.js`, and `web/index.wasm`. Serve the `web/` directory with a static server. Browsers require a user gesture to start audio, so click the page once to enable sound.

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
