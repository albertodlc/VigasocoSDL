// IThread.h
//
//	Abstract class that defines the interface of a thread
//

#ifndef _ITHREAD_H_
#define _ITHREAD_H_

class IThread {
protected:
  IThread();

  // member variables
  bool _isRunning;
  volatile bool _shouldStop;

public:
  // constructors && destructors
  virtual ~IThread();

  // abstract methods
  virtual bool start(char *threadName) = 0;
  virtual void stop() = 0;
  virtual void end() = 0;
  virtual int run();
  virtual void pause() = 0;
  virtual void resume() = 0;

  // getters && setter
  bool isRunning() { return _isRunning; }
  bool shouldStop() { return _shouldStop; }
};

#endif // _ITHREAD_H_
