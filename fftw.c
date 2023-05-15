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
    int amp_div = 100;

    for(int i = 0; i < num_ps; i++){
        ((int16_t *)&int_prach[0])[2*i] = result[i][REAL]/amp_div * 32767;
        ((int16_t *)&int_prach[0])[2*i+1] = result[i][IMAG]/amp_div * 32767;
    }

}


int idft(int16_t *txdataF, int16_t *txdata) {
    fftw_complex signal[NUM_POINTS];
    fftw_complex result[NUM_POINTS];

    fftw_plan plan = fftw_plan_dft_1d(NUM_POINTS,
                                      signal,
                                      result,
                                      FFTW_BACKWARD, //idft
                                      FFTW_ESTIMATE);

    int num_ps = NUM_POINTS;

    int_to_double(txdataF, signal, num_ps); // from txdataF to signal
    fftw_execute(plan);
    double_to_int(result, txdata, num_ps); // from result to txdata


    fftw_destroy_plan(plan);

    return 0;
}

int idft_30720(int16_t *txdataF, int16_t *txdata) {

    int num_ps = 30720; //30720

    fftw_complex signal[num_ps];
    fftw_complex result[num_ps];


    fftw_plan plan = fftw_plan_dft_1d(num_ps,
                                      signal,
                                      result,
                                      FFTW_BACKWARD, //idft
                                      FFTW_ESTIMATE);


    int_to_double(txdataF, signal, num_ps); // from txdataF to signal
    fftw_execute(plan);
    // for(int i = 0; i < num_ps; i++) printf("%lf + j*(%lf)\n", result[i][REAL], result[i][IMAG]);
    double_to_int(result, txdata, num_ps); // from result to txdata


    fftw_destroy_plan(plan);

    return 0;
}

int idft_1024(int16_t *txdataF, int16_t *txdata) {

    int num_ps = 1024;
    fftw_complex signal[num_ps];
    fftw_complex result[num_ps];

    fftw_plan plan = fftw_plan_dft_1d(num_ps,
                                      signal,
                                      result,
                                      FFTW_BACKWARD, //idft
                                      FFTW_ESTIMATE);

    int_to_double(txdataF, signal, num_ps); // from txdataF to signal
    fftw_execute(plan);
    double_to_int(result, txdata, num_ps); // from result to txdata

    fftw_destroy_plan(plan);


    return 0;
}

int dft_30720(int16_t *txdataF, int16_t *txdata) {
    int num_ps = 30720;
    fftw_complex signal[num_ps];
    fftw_complex result[num_ps];

    fftw_plan plan = fftw_plan_dft_1d(num_ps,
                                      signal,
                                      result,
                                      FFTW_FORWARD, //dft
                                      FFTW_ESTIMATE);

    int_to_double(txdata, signal, num_ps); // from txdata to signal
    fftw_execute(plan);
    double_to_int(result, txdataF, num_ps); // from result to txdataF  

    fftw_destroy_plan(plan);

    return 0;
}