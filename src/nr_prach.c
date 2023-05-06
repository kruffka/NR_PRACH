#include <stdio.h>
#include "nr_prach.h"

int32_t generate_nr_prach(PHY_VARS_NR_UE *ue, int frame, uint8_t slot);

int main(int argc, char *argv[]) {

    // if (argc < 2) {
    //     printf("Usage: ./nr_prach\n");
    // }

    //
    // ========================== Init frame params and buffers ==========================
    //
    PHY_VARS_NR_UE ue = {
        .frame_parms = {
            .ofdm_symbol_size = 2048,
        },
        .nrUE_config = {

        },
        .prach_vars = {

        },
    };

    memset(&ue.X_u[0], 0, 839*sizeof(int32_t));
    // init_nr_ue_signal();


    //
    // ========================== Generate preamble ==========================
    //
    generate_nr_prach(&ue, 0, 0);


    //
    // ========================== Modulation? ==========================
    //

    //
    // ========================== Channel Sim ==========================
    //
    // freq_shift();
    // noise_randn();
    

    //
    // ========================== Demodulation? ==========================
    //

    //
    // ========================== Detection ==========================
    //
    // prach_detection();

}