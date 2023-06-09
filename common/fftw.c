#include <fftw3.h>
#include <stdint.h>

#define REAL 0
#define IMAG 1

int dft_fftw3(int size, float *input, float *output) {
    fftwf_complex signal[size];
    fftwf_complex result[size];

    for (int i = 0; i < size; i++) {
        signal[i][REAL] = input[2*i];
        signal[i][IMAG] = input[2*i + 1];
    }

    fftwf_plan plan = fftwf_plan_dft_1d(size,
                                        signal,
                                        result,
                                        FFTW_FORWARD, // dft
                                        FFTW_ESTIMATE);

    fftwf_execute(plan);


    fftwf_destroy_plan(plan);

    for (int i = 0; i < size; i++) {
        output[2*i] = result[i][REAL];
        output[2*i + 1] = result[i][IMAG];
    }

    return 0;
}

int idft_fftw3(int size, float *input, float *output) {

    fftwf_complex signal[size];
    fftwf_complex result[size];

    for (int i = 0; i < size; i++) {
        signal[i][REAL] = input[2*i];
        signal[i][IMAG] = input[2*i + 1];
    }

    fftwf_plan plan = fftwf_plan_dft_1d(size,
                                        signal,
                                        result,
                                        FFTW_BACKWARD, // idft
                                        FFTW_ESTIMATE);


    fftwf_execute(plan);

    fftwf_destroy_plan(plan);

    for (int i = 0; i < size; i++) {
        output[2*i] = result[i][REAL];
        output[2*i + 1] = result[i][IMAG];
    }

    return 0;
}