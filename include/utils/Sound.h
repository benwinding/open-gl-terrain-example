#ifndef SOUND___H
#define SOUND___H

class Sound
{
  pid_t sound_pid;

public:
	void init();
	void kill();
};

#endif