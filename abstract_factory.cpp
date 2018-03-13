#include <iostream>

using namespace std;

class OsOpAbFactory {
 protected:
   OsOpAbFactory() {}
   OsOpAbFactory(const OsOpAbFactory&);
   void operator=(const OsOpAbFactory&);
 public:
   ~OsOpAbFactory() {}
   virtual void createSocket() = 0;
   virtual void createPipe() = 0;
   virtual void createThread() = 0;
   static OsOpAbFactory* getInstance() {
     static OsOpAbFactory* inst;
     return inst;
   }
};


class UnixOpFactory: public OsOpAbFactory {
 public:
   virtual void createSocket()  { cout << "UnixFactory: createUnixSocket" << endl; }
   virtual void createPipe()    { cout << "UnixFactory: createUnixPipe" << endl; }
   virtual void createThread()  { cout << "UnixFactory: createUnixThread" << endl; }
};

class VmsOpFactory: public OsOpAbFactory {
 public:
   virtual void createSocket()   { cout << "VmsFactory: createVmsSocket" << endl; }
   virtual void createPipe()     { cout << "VmsFactory: createVmsPipe" << endl; }
   virtual void createThread()   { cout << "VmsFactory: createVmsThread" << endl; }
};

class NtOpFactory: public OsOpAbFactory {
 public:
   virtual void createSocket()    { cout << "NtFactory: createNtSocket" << endl; }
   virtual void createPipe()      { cout << "NtFactory: createNtPipe" << endl; }
   virtual void createThread()    { cout << "NtFactory: createNtThread" << endl; }
};


void doOneLaneIPC(OsOpAbFactory* factory) {
  factory->createSocket();
}

void doTwoLaneIPC(OsOpAbFactory* factory) {
  factory->createPipe();
}

void doParallelProcessing(OsOpAbFactory* factory) {
  factory->createThread();
}


int main() {
   OsOpAbFactory* f = OsOpAbFactory::getInstance();
#ifdef UNIX
   f = new UnixOpFactory;
#elif VMS
   f = new VmsOpFactory;
#else
   f = new NtOpFactory;
#endif
   doOneLaneIPC(f);
   doTwoLaneIPC(f);
   doParallelProcessing(f);
   cout << "main: complete" << endl;

   delete f;
   return 0;
}

// -- current output --
// createVmsSocket:
// createVmsPipe:
// createVmsThread:
// main: complete

// -- target output --
// VmsFactory: createSocket
// VmsFactory: createPipe
// VmsFactory: createThread
// main: complete
