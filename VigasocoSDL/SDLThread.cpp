// SDLThread.cpp
//
/////////////////////////////////////////////////////////////////////////////

#include "SDLThread.h"

/////////////////////////////////////////////////////////////////////////////
// initialization and cleanup
/////////////////////////////////////////////////////////////////////////////

SDLThread::SDLThread()
{
    _handle = NULL;
}

SDLThread::~SDLThread()
{
    end();
}

/////////////////////////////////////////////////////////////////////////////
// thread function
/////////////////////////////////////////////////////////////////////////////

int SDLThread::ThreadProc(SDLThread *thread)
{
    return thread->run();
}

/////////////////////////////////////////////////////////////////////////////
// thread life cycle
/////////////////////////////////////////////////////////////////////////////

bool SDLThread::start()
{
    if (_isRunning){
        return false;
    }

#ifndef __EMSCRIPTEN_PTHREADS__
    // SDL2: requires a thread name as second argument
    _handle = SDL_CreateThread((SDL_ThreadFunction)ThreadProc, "VigasocoThread", this);
#else
    pthread_create(&_handle, NULL, (void *(*)(void *))ThreadProc, this);
#endif

    if (_handle == NULL){
        return false;
    }

    _isRunning = true;
    return true;
}

void SDLThread::end()
{
    if (_handle != NULL){
        _isRunning = false;

#ifndef __EMSCRIPTEN_PTHREADS__
        // SDL_KillThread removed in SDL2 — wait for thread to finish naturally
        // _isRunning = false signals the thread loop to exit
        SDL_WaitThread(_handle, NULL);
#else
        pthread_cancel(_handle);
#endif
        _handle = NULL;
    }
}

void SDLThread::pause()
{
    // TODO: not implemented
}

void SDLThread::resume()
{
    // TODO: not implemented
}