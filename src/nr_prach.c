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
            .samples_per_frame = 307200,
            .samples_per_subframe = 30720,
            .N_RB_UL = 106,
            .nb_antennas_rx = 1,
            .slots_per_subframe = 1,
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
                .root_seq_id = 1,
                .num_cs = 0,
                .prach_format = 0,
                .ra_PreambleIndex = 52,
                .prach_start_symbol = 0,
            },
        },
    };

    ue.txdata = (int32_t **)malloc16(1*sizeof(int32_t *));
    ue.txdata[0] = (int32_t *)malloc16_clear(307200*sizeof(int32_t)); 
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

    PHY_VARS_gNB gNB = {};
    gNB.rxdata = (int32_t **)malloc16(1*sizeof(int32_t *));
    gNB.rxdata[0] = (int32_t *)malloc16_clear(307200*sizeof(int32_t));
    // for (int i .. max_prach_occ)
    gNB.prach_rxsigF[0] = (int16_t **)malloc16(1*sizeof(int32_t *));
    gNB.prach_rxsigF[0][0] = (int16_t*)malloc16_clear( 4*98304*2*sizeof(int16_t) );
    memcpy(&gNB.frame_parms, &ue.frame_parms, sizeof(ue.frame_parms));

    dfts_autoinit();
    init_nr_prach_tables(839);

    // init_nr_ue_signal();
    int slot = 1;

    int success = 0;
    for (int i = 0; i < 64; i++) {
      ue.prach_vars.prach_pdu.ra_PreambleIndex = i;
      //
      // ========================== Generation ==========================
      //
      printf("========================== Generate preamble ==========================\n");
      generate_nr_prach(&ue, 0, slot);


      //
      // ========================== Channel Simulation ==========================
      //
      printf("========================== Channel Simulation ==========================\n");
      // freq_shift();
      // noise_randn();
      
      // FILE *file = fopen("../txdata.m", "r");
      // if (file == NULL) {
      //   printf("exit no file\n");
      //   exit(1);
      // }

      // fscanf(file, "txs = [");

      // for (int i = 0; i < 307200; i++) {
      //   fscanf(file, "%hd + j*(%hd)\n", &((int16_t *)&ue.txdata[0][0])[2*i], &((int16_t *)&ue.txdata[0][0])[2*i + 1]);
      // }

      // fclose(file);



      // int32_t **rxdataF = (int32_t **)malloc16(1*sizeof(int32_t *));
      // rxdataF[0] = (int32_t *)malloc16_clear(2048*14*sizeof(int32_t));
      uint16_t max_preamble;
      uint16_t max_preamble_energy;
      uint16_t max_preamble_delay;
      gNB.N_TA_offset = 0;

      // int offset = 30720;
      // int16_t *rxdata_ptr;
      // rxdata_ptr = (int16_t *)&ue.txdata[0][offset];
      // dft(DFT_24576, rxdata_ptr, (int16_t *)&rxdataF[0][0], 1);

      // LOG_M("txdata0.m","txdata", &ue.txdata[0][0], 307200, 1, 1);
      // LOG_M("rxdataF0.m","rxdataF", &rxdataF[0][0], 2048*14, 1, 1);

      memcpy(&gNB.rxdata[0][0], &ue.txdata[0][0], sizeof(int32_t)*ue.frame_parms.samples_per_frame);

      //
      // ========================== Detection ==========================
      //
      printf("========================== Preamble detection ==========================\n");
      detect_nr_prach(&gNB, slot, &max_preamble, &max_preamble_energy, &max_preamble_delay, ue.X_u);

      if (max_preamble == ue.prach_vars.prach_pdu.ra_PreambleIndex) {
        printf("SUCCESS!!! %d\n", max_preamble);
        success++;
      } else {
        printf("FAIL!!!\n");
      }
    }
    printf("Results: %d/%d\n", success, 64);
}