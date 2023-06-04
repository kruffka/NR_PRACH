#include <stdio.h>
#include "nr_prach.h"

// #define MAIN_DEBUG

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

void init_nr_ue_signal(PHY_VARS_NR_UE *ue, PHY_VARS_gNB *gNB, init_params_t *config) {

  int nb_rx = config->frame_parms.nb_antennas_rx;
  int N_ZC = (config->prach_config.prach_sequence_length == 0) ? 839:139;
  fapi_nr_prach_config_t *prach_config = &config->prach_config;

  ue->txdata = (int32_t **)malloc16(nb_rx*sizeof(int32_t *));
  if (ue->txdata == NULL) {
    printf("Error malloc in %s:%d\n", __FILE__, __LINE__);
    exit(1);
  }

  gNB->rxdata = (int32_t **)malloc16(nb_rx*sizeof(int32_t *));
  if (gNB->rxdata == NULL) {
    printf("Error malloc in %s:%d\n", __FILE__, __LINE__);
    exit(1);
  }

  for (int aa = 0; aa < nb_rx; aa++) {
    ue->txdata[aa] = (int32_t *)malloc16_clear(307200*sizeof(int32_t)); 
    gNB->rxdata[aa] = (int32_t *)malloc16_clear(307200*sizeof(int32_t));
  }

  for (int occ = 0; occ < NUMBER_OF_NR_PRACH_OCCASIONS_MAX; occ++) {
    gNB->prach_rxsigF_i[occ] = (int16_t **)malloc16(nb_rx*sizeof(int32_t *));
    if (gNB->prach_rxsigF_i[occ] == NULL) {
      printf("Error malloc in %s:%d\n", __FILE__, __LINE__);
      exit(1);
    }

    gNB->prach_rxsigF_f[occ] = (float **)malloc16(nb_rx*sizeof(float *));
    if (gNB->prach_rxsigF_f[occ] == NULL) {
      printf("Error malloc in %s:%d\n", __FILE__, __LINE__);
      exit(1);
    }

    for (int aa = 0; aa < nb_rx; aa++) {
      gNB->prach_rxsigF_i[occ][aa] = (int16_t*)malloc16_clear(4*98304*2*sizeof(int16_t));
      gNB->prach_rxsigF_f[occ][aa] = (float *)malloc16_clear(4*98304*2*sizeof(float));
    }
  }

  dfts_autoinit();
  init_nr_prach_tables(N_ZC);

  ue->frame_parms = gNB->frame_parms = config->frame_parms;
  ue->prach_config = gNB->prach_config = config->prach_config;
  ue->prach_pdu = gNB->prach_pdu = config->prach_pdu;

}

void deinit_nr_ue_signal(PHY_VARS_NR_UE *ue, PHY_VARS_gNB *gNB) {
    
    int nb_rx = ue->frame_parms.nb_antennas_rx;

    for (int aa = 0; aa < nb_rx; aa++) {
      free(ue->txdata[aa]); 
      free(gNB->rxdata[aa]);
    }

    free(ue->txdata);
    free(gNB->rxdata);

    for (int occ = 0; occ < NUMBER_OF_NR_PRACH_OCCASIONS_MAX; occ++) {
      for (int aa = 0; aa < nb_rx; aa++) {
        free(gNB->prach_rxsigF_i[occ][aa]);
        free(gNB->prach_rxsigF_f[occ][aa]);
      }
      
      free(gNB->prach_rxsigF_i[occ]);
      free(gNB->prach_rxsigF_f[occ]);
    }
}


int main(int argc, char *argv[]) {

    // if (argc < 2) {
    //     printf("Usage: ./nr_prach\n");
    // }

    //
    // ========================== Init frame params and buffers ==========================
    //
#ifdef MAIN_DEBUG
    printf("========================== Initializing frame params and buffers ==========================\n");
#endif

    PHY_VARS_NR_UE ue = {};
    PHY_VARS_gNB gNB = {};
    
    uint16_t max_preamble;
    uint16_t max_preamble_energy;
    uint16_t max_preamble_delay;
    int ra_PreambleIndex = 7;
    int slot = 1;
    int gNB_float = 0;
    int fd_occasion = 0;
    int tries = 64;
    int success = 0;
    gNB.N_TA_offset = 0;

    init_params_t frame_and_prach_config = {
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
      .prach_config = {
        .prach_sequence_length = 0, // 0 - Long sequence, 1 - Short sequence
        .prach_sub_c_spacing = 0, // mu
        .restricted_set_config = 0,
        .num_prach_fd_occasions = 1,
        .num_prach_fd_occasions_list[0] = {
          .prach_root_sequence_index = 1,
          .num_root_sequences = 64,
          .k1 = 0,
          .prach_zero_corr_conf = 0,
          .num_unused_root_sequences = 0,
          .unused_root_sequences_list = NULL,
        },
        .ssb_per_rach = 3,
        .prach_multiple_carriers_in_a_band = 0,
      },
      .prach_pdu = {
        .indexFD_RA = 0,
        .numCs = 0,
        .numPRACH_Ocas = 1,
        .prach_Format = 0,
        .prach_StartSymbol = 0,
      },
    };


    init_nr_ue_signal(&ue, &gNB, &frame_and_prach_config);

    if (tries > 64 || tries < 0) 
      AssertFatal(1 == 0, "Error, nof preamble idx only 0..63; tries %d\n", tries);

    printf("Running nof preamble idx %d\n", tries);

    for (int i = 0; i < tries; i++) {
      ra_PreambleIndex = i;

#ifdef MAIN_DEBUG
      //
      printf("========================== Generate preamble ==========================\n");
      //
#endif

      generate_nr_prach(&ue, slot, ra_PreambleIndex, fd_occasion);

#ifdef MAIN_DEBUG
      //
      printf("========================== Channel Simulation ==========================\n");
      //
#endif
      // freq_shift();
      // noise_randn();
      


      // LOG_M("txdata0.m","txdata", &ue.txdata[0][0], 307200, 1, 1);
      // LOG_M("rxdataF0.m","rxdataF", &rxdataF[0][0], 2048*14, 1, 1);

      memcpy(&gNB.rxdata[0][0], &ue.txdata[0][0], sizeof(int32_t)*ue.frame_parms.samples_per_frame);


#ifdef MAIN_DEBUG
      //
      printf("========================== Preamble detection ==========================\n");
      //
#endif

      max_preamble = 0;
      max_preamble_energy = 0;
      max_preamble_delay = 0;

      if (gNB_float)
        detect_nr_prach_f(&gNB, slot, fd_occasion, &max_preamble, &max_preamble_energy, &max_preamble_delay);
      else
        detect_nr_prach_i(&gNB, slot, fd_occasion, &max_preamble, &max_preamble_energy, &max_preamble_delay);

      if (max_preamble == ra_PreambleIndex) {
        printf("SUCCESS!!! idx = %d, delay = %d, energy = %d\n", max_preamble, max_preamble_delay, max_preamble_energy);
        success++;
      } else {
        printf("FAIL!!!\n");
      }
    }
    printf("Results: %d/%d\n", success, tries);

    deinit_nr_ue_signal(&ue, &gNB);
}