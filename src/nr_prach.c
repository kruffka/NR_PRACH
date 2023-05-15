#include <stdio.h>
#include "nr_prach.h"

int32_t generate_nr_prach(PHY_VARS_NR_UE *ue, int frame, uint8_t slot);

uint32_t get_samples_per_slot(int slot, NR_FRAME_PARMS *fp)
{
  uint32_t samp_count;

  if(fp->numerology_index == 0)
    samp_count = fp->samples_per_subframe;
  else
    samp_count = (slot%(fp->slots_per_subframe/2)) ? fp->samples_per_slotN0 : fp->samples_per_slot0;

  return samp_count;
}

uint32_t get_samples_slot_timestamp(int slot, NR_FRAME_PARMS *fp, uint8_t sl_ahead)
{
  uint32_t samp_count = 0;

  if(!sl_ahead) {
    for(uint8_t idx_slot = 0; idx_slot < slot; idx_slot++)
      samp_count += fp->get_samples_per_slot(idx_slot, fp);
  } else {
    for(uint8_t idx_slot = slot; idx_slot < slot+sl_ahead; idx_slot++)
      samp_count += fp->get_samples_per_slot(idx_slot, fp);
  }
  return samp_count;
}

int main(int argc, char *argv[]) {

    // if (argc < 2) {
    //     printf("Usage: ./nr_prach\n");
    // }

    //
    // ========================== Init frame params and buffers ==========================
    //
    printf("========================== Initializing frame params and buffers ==========================\n");
    PHY_VARS_NR_UE ue = {
        .frame_parms = {
            .ofdm_symbol_size = 2048,
            .get_samples_slot_timestamp = &get_samples_slot_timestamp,
            .get_samples_per_slot = &get_samples_per_slot,
        },
        .nrUE_config = {
            .prach_config = {
                .prach_sequence_length = 0,
                .prach_sub_c_spacing = 0, // mu
                .num_prach_fd_occasions = 1,
                // fill in later and only allocate memory (malloc works with 0)
                .num_prach_fd_occasions_list = malloc(ue.nrUE_config.prach_config.num_prach_fd_occasions*sizeof(fapi_nr_num_prach_fd_occasions_t)),
                // .restricted_set_config = 0,
                // .ssb_per_rach = 3,
                // .prach_multiple_carriers_in_a_band = 0,
            },
        },
        .prach_vars = {
            .amp = 512,
            .prach_pdu = {
                .restricted_set = 0, 
                .root_seq_id = 0,
                .num_cs = 0,
                .prach_format = 0,
                .ra_PreambleIndex = 0,
                .prach_start_symbol = 0,
            },
        },
    };

    fapi_nr_num_prach_fd_occasions_t tmp_fd_occ_list = {
        .num_prach_fd_occasions = 0,
        .prach_root_sequence_index = 1,
        .num_root_sequences = 64,
        .k1 = 0,
        .prach_zero_corr_conf = 0,
        .num_unused_root_sequences = 0,
        .unused_root_sequences_list = NULL,
    };
    memcpy(&ue.nrUE_config.prach_config.num_prach_fd_occasions_list[0], &tmp_fd_occ_list, sizeof(tmp_fd_occ_list));

    memset(&ue.X_u[0], 0, 839*sizeof(int32_t));
    // init_nr_ue_signal();


    //
    // ========================== Generate preamble ==========================
    //
    printf("========================== Generate preamble ==========================\n");
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