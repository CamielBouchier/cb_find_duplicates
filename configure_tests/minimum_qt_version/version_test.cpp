#include <stdio.h>
#include <stdlib.h>
#include <QtGlobal>
int main( int argc, char **argv ) {
  (void) argc;
  (void) argv;
  const int MinimumVersion = 0x050e01; // 5.14 needed e.g. for arg(Args&&)
  printf("Qt Version         : %x\n",QT_VERSION);
  printf("Qt Minimum version : %x\n",MinimumVersion);
  exit(QT_VERSION >= MinimumVersion?0:1);
}

