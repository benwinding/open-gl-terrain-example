#ifndef SOUND___H
#define SOUND___H

#if !defined(__EMSCRIPTEN__)
#include <sys/types.h>
#endif

class Sound
{
#if defined(__EMSCRIPTEN__)
  int sound_pid;
#else
  pid_t sound_pid;
#endif

public:
	void init();
	void kill();
};

#endif