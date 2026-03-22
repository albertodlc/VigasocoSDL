// SDLThread.cpp
//

#include "Thread.h"

// initialization and cleanup
SDLThread::SDLThread() { _handle = NULL; }

SDLThread::~SDLThread() { end(); }

// thread function
int SDLThread::ThreadProc(SDLThread *thread) { return thread->run(); }

// thread life cycle
/**
 * @brief Start a Thread
 *
 * @return true
 * @return false
 */
bool SDLThread::start(char *threadName) {
  if (_isRunning) {
    return false;
  }

#ifndef __EMSCRIPTEN_PTHREADS__
  _handle = SDL_CreateThread((SDL_ThreadFunction)ThreadProc, threadName, this);
#else
  pthread_create(&_handle, NULL, (void *(*)(void *))ThreadProc, this);
#endif

  if (_handle == NULL) {
    return false;
  }

  _isRunning = true;

  printf("THREAD %s created and running\n", threadName);

  return true;
}

void SDLThread::stop() {
  if (_handle != NULL) {
    _shouldStop = true;
    _isRunning = false;
#ifndef __EMSCRIPTEN_PTHREADS__
    SDL_WaitThread(_handle, NULL);
#else
    pthread_join(_handle, NULL);
#endif
    _handle = NULL;
  }
}

void SDLThread::end() {
  if (_handle != NULL) {
    _shouldStop = true;
    _isRunning = false;
#ifndef __EMSCRIPTEN_PTHREADS__
    SDL_DetachThread(_handle); // fallback — don't wait
#else
    pthread_cancel(_handle);
#endif
    _handle = NULL;
  }
}

void SDLThread::pause() {
  // TODO: not implemented
}

void SDLThread::resume() {
  // TODO: not implemented
}