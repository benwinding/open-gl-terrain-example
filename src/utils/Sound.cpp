#include <iostream>
#include <string>
#include <unistd.h>
#include <signal.h>

#include "utils/Sound.h"
#include "utils/Logger.h"

void Sound::init() {
  this->sound_pid = fork();
  if(this->sound_pid == 0) {
    Print("Starting sound...", this->sound_pid);
    const std::string soundFilePath = "res/sounds/Gilman_Mom_-_07_-_Cosmic_Evening.ogg";
    #if defined(__APPLE__) && defined(__MACH__)
      system(std::string("open " + soundFilePath).c_str());
    #elif defined(__linux__)
      system(std::string("xdg-open " + soundFilePath).c_str());
    #else
      std::cout << "Not sure how to start the sound on this platform" << std::endl;
    #endif
    usleep(141 * 1000000);
    Print("Finished sound...", this->sound_pid);
    exit(1);
  }
}

void Sound::kill() {
  //Sends the SIGINT Signal to the process, telling it to stop.
  ::kill(this->sound_pid, 15); 
}
