#include "subprograms.h"
/*****************************************************/
/*  This file was automatically generated by Ocarina */
/*  Do NOT hand-modify this file, as your            */
/*  changes will be lost when you re-run Ocarina     */
/*****************************************************/
#include <libc/stdio.h>
#include <types.h>
int ntimes;
void pok_testevents__spg_receive_event(void) {

  printf("I received a signal %d times\n", ntimes);
  ntimes++;
}
