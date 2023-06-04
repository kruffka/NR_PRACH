#include <fftw3.h>
#include <stdint.h>

#define REAL 0
#define IMAG 1

int dft_fftw3(int size, float *txdataF, float *txdata) {
    fftw_complex signal[size];
    fftw_complex result[size];

    fftw_plan plan = fftw_plan_dft_1d(size,
                                      signal,
                                      result,
                                      FFTW_BACKWARD, //idft
                                      FFTW_ESTIMATE);

    fftw_execute(plan);


    fftw_destroy_plan(plan);

    return 0;
}

int idft_fftw3(int size, float *txdataF, float *txdata) {

    fftw_complex signal[size];
    fftw_complex result[size];


    fftw_plan plan = fftw_plan_dft_1d(size,
                                      signal,
                                      result,
                                      FFTW_BACKWARD, //idft
                                      FFTW_ESTIMATE);


    fftw_execute(plan);

    fftw_destroy_plan(plan);

    return 0;
}