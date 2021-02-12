#include <QtCore>
#ifdef _OPENMP
  #include <omp.h>
#endif

#ifdef WIN32
  #include <windows.h>
#endif

const int  Size = 10000;
const int  ShouldSum = (Size-1)*Size/2;

short Verbose = 0;

short ThreadOK[3] = {0,0,0}; // Main, Thread1, Thread2

// Thread
class MyThread: public QThread {
public : 
short Id;
void  run();
};

void MyThread::run() {
  int i;
  int x[Size];

  #ifdef _OPENMP
    omp_set_num_threads(4);
  #endif
  #pragma omp parallel for
  for ( i = 0; i < Size; i++ ) {
    #ifdef _OPENMP
    if (Verbose && i%1000==0) {
      int tid = omp_get_thread_num();
      printf("thread %p : tid %d handles %d\n",currentThread(),tid,i);
    }
    #endif

    x[i] = i;
  }

  int Sum=0;
  for ( i = 0; i < Size; i++ ) {
    Sum += x[i];
  }
  if (Verbose) {
    printf("Id %d : %s: %d(should %d)\n",Id, __FUNCTION__, Sum,ShouldSum);
  }
  if (Sum == ShouldSum) ThreadOK[Id] = 1;
  return;
}

// MainThread
void MainThread() {
  int i;
  int x[Size];

  #ifdef _OPENMP
    omp_set_num_threads(4);
  #endif
  #pragma omp parallel for
  for ( i = 0; i < Size; i++ ) {
    #ifdef _OPENMP
    if (Verbose && i%1000==0) {
      int tid = omp_get_thread_num();
      printf("Main : tid %d handles %d\n",tid,i);
    }
    #endif

    x[i] = i;
  }

  int Sum=0;
  for ( i = 0; i < Size; i++ ) {
    Sum += x[i];
  }
  int ShouldSum=0;
  for ( i = 0; i < Size; i++ ) {
    ShouldSum += i;
  }
  if (Verbose) {
    printf("Main : %s: %d(should %d)\n", __FUNCTION__, Sum,ShouldSum);
  }
  if (Sum == ShouldSum) ThreadOK[0] = 1;
  return;
}

// Comment in/out for checking the effect of multiple threads.
#define SPAWN_THREADS

// main
int main(int argc, char *argv[]) {

  #ifdef WIN32
    // Avoid 'popup' windows at failing during configuration.
    SetErrorMode(SEM_FAILCRITICALERRORS | SEM_NOGPFAULTERRORBOX);
  #endif

  if (argc>1) Verbose = 1;

  MainThread();

  #ifndef SPAWN_THREADS
    return 0;
  #endif

  MyThread ThreadA;
  MyThread ThreadB;

  bool StatusOk;

  ThreadA.Id = 1;
  ThreadB.Id = 2;
  ThreadA.start();
  ThreadB.start();

  StatusOk = ThreadA.wait();
  if (!StatusOk ) {
    printf("Failed to join thread 1\n");
    return (-1);
  }

  StatusOk = ThreadB.wait();
  if ( !StatusOk) {
    printf("Failed to join thread 2\n");
    return (-1);
  }

  short OK = 0;
  // Check that we have OpenMP before declaring things OK formally.
  #ifdef _OPENMP
    OK = 1;
  #else
    printf("OpenMP seems not enabled ...\n");
  #endif
  for (short i=0;i<3;i++) OK *= ThreadOK[i];
  if (OK) printf("OMP : All looks good\n");

  return OK?0:1;
}
