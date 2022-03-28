/* Start reading here */


#include "defs.h"

#define NUM_POINTS 24576   


/* Never mind this bit */

#define REAL 0
#define IMAG 1
int symb = 0;
char filename[40];

void int_to_double(int16_t *int_prachF, fftw_complex* signal, int num_ps) {

    for(int i = 0; i < num_ps; i++){
        signal[i][REAL] = ((int16_t *)&int_prachF[0])[2*i] / 32767.0f;
        signal[i][IMAG] = ((int16_t *)&int_prachF[0])[2*i + 1] / 32767.0f;
    }

}

void double_to_int(fftw_complex* result, int16_t* int_prach, int num_ps) {

    for(int i = 0; i < num_ps; i++){
        ((int16_t *)&int_prach[0])[2*i] = result[i][REAL] * 32767;
        ((int16_t *)&int_prach[0])[2*i+1] = result[i][IMAG] * 32767;
    }

}


int idft(int16_t *txdataF, int16_t *txdata) {
    fftw_complex signal[NUM_POINTS];
    fftw_complex result[NUM_POINTS];

    fftw_plan plan = fftw_plan_dft_1d(NUM_POINTS,
                                      signal,
                                      result,
                                      FFTW_FORWARD,
                                      FFTW_ESTIMATE);

    int num_ps = NUM_POINTS;

    int_to_double(txdataF, signal, num_ps);
    fftw_execute(plan);
    double_to_int(result, txdata, num_ps);    


    fftw_destroy_plan(plan);

    printf("idft done\n");

    return 0;
}

int idft_1024(int16_t *txdataF, int16_t *txdata) {

    int num_ps = 1024;
    fftw_complex signal[num_ps];
    fftw_complex result[num_ps];

    fftw_plan plan = fftw_plan_dft_1d(num_ps,
                                      signal,
                                      result,
                                      FFTW_FORWARD,
                                      FFTW_ESTIMATE);

    int_to_double(txdataF, signal, num_ps);
    fftw_execute(plan);
    double_to_int(result, txdata, num_ps);    


    fftw_destroy_plan(plan);

    printf("idft done\n");

    return 0;
}