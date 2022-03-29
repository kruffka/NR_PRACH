#include <fftw3.h>
#include <stdio.h>
#include <stdint.h>
#include <math.h>
#include <stdlib.h>
#include <string.h>

#ifndef _DEFS_H_
#define _DEFS_H_

int idft(int16_t *txdataF, int16_t *txdata);
int idft_1024(int16_t *txdataF, int16_t *txdata);
int dft_30720(int16_t *txdataF, int16_t *txdata);
int idft_30720(int16_t *txdataF, int16_t *txdata);

#define exit_fun(msg) exit_function(__FILE__,__FUNCTION__,__LINE__,msg)
void exit_function(const char *file, const char *function, const int line, const char *msg);




#endif //_DEFS_H_