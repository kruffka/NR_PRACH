#include <stdio.h>
#include <complex.h>
#include <time.h>
#include "nr_prach.h"

#define RAND_BUFSIZE 512
int randn(double *x, int n, double mu, double sigma) 
{ 
    int k, m; 
    double x1[RAND_BUFSIZE], x2[RAND_BUFSIZE]; 
    int res = 0; 
    if (!x) { 
        return -1; 
    } 
 
    if (n < 1) { 
        return -1; 
    } 
 
    if (sigma < 0.0) { 
        return -1; 
    } 
 
    k = 0; 
    while (k < n) { 

        m = 0; 
        while (k < n && m < RAND_BUFSIZE) { 
            if (x1[m] != 0.0) { 
                x[k] = sqrt(-2.0 * log(x1[m])) * cos(2*M_PI * x2[m]) * sigma + mu; 
                k++; 
                m++; 
            } else { 
                x1[m] = (double)rand() / RAND_MAX; 
            } 
        } 
    } 
 

    return res; 
}

// #define MAIN_DEBUG

double s_time = 0.0; // sampling time

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

void init_nr_signal(PHY_VARS_NR_UE *ue, PHY_VARS_gNB *gNB, init_params_t *config) {

  int nb_rx = config->frame_parms.nb_antennas_rx;
  int N_ZC = (config->prach_config.prach_sequence_length == 0) ? 839:139;

  ue->txdata = (int32_t **)malloc16(nb_rx*sizeof(int32_t *));
  if (ue->txdata == NULL) {
    printf("Error malloc in %s:%d\n", __FILE__, __LINE__);
    exit(1);
  }

  gNB->rxdata_int = (int32_t **)malloc16(nb_rx*sizeof(int32_t *));
  if (gNB->rxdata_int == NULL) {
    printf("Error malloc in %s:%d\n", __FILE__, __LINE__);
    exit(1);
  }

  gNB->rxdata_float = (float **)malloc16(nb_rx*sizeof(float *));
  if (gNB->rxdata_float == NULL) {
    printf("Error malloc in %s:%d\n", __FILE__, __LINE__);
    exit(1);
  }

  for (int aa = 0; aa < nb_rx; aa++) {
    ue->txdata[aa] = (int32_t *)malloc16_clear(307200*sizeof(int32_t)); 
    gNB->rxdata_int[aa] = (int32_t *)malloc16_clear(307200*sizeof(int32_t));
    gNB->rxdata_float[aa] = (float *)malloc16_clear(307200*sizeof(float));
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
  init_nr_prach_tables_float(N_ZC);

  ue->frame_parms = gNB->frame_parms = config->frame_parms;
  ue->prach_config = gNB->prach_config = config->prach_config;
  ue->prach_pdu = gNB->prach_pdu = config->prach_pdu;

  
  compute_nr_prach_seq(gNB->prach_config.prach_sequence_length,
                             gNB->prach_config.num_prach_fd_occasions_list[0].num_root_sequences,
                             gNB->prach_config.num_prach_fd_occasions_list[0].prach_root_sequence_index,
                             gNB->X_u);

  compute_nr_prach_seq_float(gNB->prach_config.prach_sequence_length,
                             gNB->prach_config.num_prach_fd_occasions_list[0].num_root_sequences,
                             gNB->prach_config.num_prach_fd_occasions_list[0].prach_root_sequence_index,
                             gNB->X_u_float);

  s_time = 1.0/(1.0e3*ue->frame_parms.samples_per_subframe); // sampling time for freq shift

}

void deinit_nr_signal(PHY_VARS_NR_UE *ue, PHY_VARS_gNB *gNB) {
    
    int nb_rx = ue->frame_parms.nb_antennas_rx;

    for (int aa = 0; aa < nb_rx; aa++) {
      free(ue->txdata[aa]); 
      free(gNB->rxdata_int[aa]);
    }

    free(ue->txdata);
    free(gNB->rxdata_int);

    for (int occ = 0; occ < NUMBER_OF_NR_PRACH_OCCASIONS_MAX; occ++) {
      for (int aa = 0; aa < nb_rx; aa++) {
        free(gNB->prach_rxsigF_i[occ][aa]);
        free(gNB->prach_rxsigF_f[occ][aa]);
      }
      
      free(gNB->prach_rxsigF_i[occ]);
      free(gNB->prach_rxsigF_f[occ]);
    }
}

void freq_shift(void *input, void *output, int size, int freq_shift_Hz) {
        
        double off_angle = -2*M_PI*s_time*freq_shift_Hz;   // offset rotation angle compensation per sample

        off_angle *= -1;

        // loop over samples
        int re, im;
        for(int n=0; n<size; n++){
            re = ((double)(((short *)input))[2*n]);
            im = ((double)(((short *)input))[2*n+1]);
            ((short *)output)[2*n] = (short)(round(re*cos(n*off_angle) - im*sin(n*off_angle)));
            ((short *)output)[2*n+1] = (short)(round(re*sin(n*off_angle) + im*cos(n*off_angle)));
        }
}

void noise_randn(void *input, void *output, int start, int end, int size, float SNR_dB) {

        float re, im;
        double power_tot = 0.0;
        double power_avg = 0.0;
        for(int n=start; n<end; n++) {
          re = ((double)(((short *)input))[2*n]);
          im = ((double)(((short *)input))[2*n+1]);
          power_tot += re*re + im*im;
        }
        int nb_rx = 1;
        
        // SNR_dB = SNR_dB - 20;//10*log10(2048); //10*log10(2048);
// int tx_lev = 10;
        power_avg = power_tot / (end-start); // todo remove hardcode
        float scalingFactor = 1.0/sqrtf((2.0*nb_rx*2048*powf(10.0, SNR_dB / 10.0)))*32767;
        // scalingFactor = 30000;
                  // double sigma2_dB = 10*log10((double)tx_lev) - SNR_dB - 10*log10(106*12/839);
            // printf("sigma2_dB %f (SNR %f dB) tx_lev_dB %f\n",sigma2_dB,SNR_dB,10*log10((double)tx_lev));
          //AWGN
          // double sigma2 = pow(10,sigma2_dB/10);
        // float SNR = powf(10.0, SNR_dB/10.0);
        // float scalingFactor = 1.0/sqrtf(2.0*1*2048*SNR)*power_avg;
        double complex noise[size];
          printf("%f %f\n", power_avg, scalingFactor);

        int ret = randn(noise, size, 0.0, 1.0);
        for (int n = 0; n < size; n++) {
          re = (float)rand() / RAND_MAX;
          im = (float)rand() / RAND_MAX;
          // re = creal(noise[n]);
          // im = cimag(noise[n]);
          // if (n < 5)
          // ((short *)output)[2*n] = (short)(((short *)input)[2*n] + (short)scalingFactor*creal(noise[n]));
          // ((short *)output)[2*n + 1] = (short)(((short *)input)[2*n + 1] + (short)scalingFactor*cimag(noise[n]));
          ((short *)output)[2*n] = (short)(((short *)input)[2*n] + re*scalingFactor);
          ((short *)output)[2*n + 1] = (short)(((short *)input)[2*n + 1]+ im*scalingFactor);
        }
        
}

void time_delay_in_samples(void *input, void *output, int size, int delay_samples) {

        int re, im, n = 0;
        for (int i = 0; i < size; i++)
        {
            if (n + delay_samples >= size) {
              n = 0;
              delay_samples = 0;
            }
            re = ((double)(((short *)input))[2*i]);
            im = ((double)(((short *)input))[2*i + 1]);
            ((short *)output)[2*(n + delay_samples)] = re;
            ((short *)output)[2*(n + delay_samples)+1] = im;
            n++;
        }
}

int main(int argc, char *argv[]) {

    if (argc < 3) {
      printf("Usage: ./nr_prach is_float\n");
      exit(0);
    }

    int gNB_float = atoi(argv[1]);
    // int nof_preambles = atoi(argv[2]);
    int nof_preambles = 64;
    int ra_PreambleIndex = 0;


    //
    // ========================== Init frame params and buffers ==========================
    //
#ifdef MAIN_DEBUG
    printf("========================== Initializing frame params and buffers ==========================\n");
#endif
    srand(time(NULL));

    PHY_VARS_NR_UE ue = {};
    PHY_VARS_gNB gNB = {};
    
    uint16_t max_preamble;
    uint16_t max_preamble_energy;
    uint16_t max_preamble_delay;
    int slot = 0;
    int fd_occasion = 0;
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


    init_nr_signal(&ue, &gNB, &frame_and_prach_config);
    int32_t **txdata_ue = &ue.txdata[0];
    int32_t **rxdata_gNB = &gNB.rxdata_int[0];

    if (nof_preambles > 64 || nof_preambles < 0) 
      AssertFatal(1 == 0, "Error, nof preamble idx only 0..63; nof_preambles %d\n", nof_preambles);

    int Monte_Carlo = 1, 
        freq_shift_start = 10, 
        freq_shift_step = 5,
        freq_shift_end = 25, 
        delay_start = 5, // in samples
        delay_step = 5,
        delay_end = 15;
    float SNR_start = 0.0, 
          SNR_step = 1,
          SNR_end = 1.0;

    printf("Running nof preamble idx %d\n", nof_preambles);

    int size = 61440;
    // int32_t 

    for (int delay = delay_start; delay < delay_end; delay += delay_step) {
    
      for (int freq_shift_Hz = freq_shift_start; freq_shift_Hz < freq_shift_end; freq_shift_Hz += freq_shift_step) {

        for (float SNR_dB = SNR_start; SNR_dB < SNR_end; SNR_dB += SNR_step) {
    
          for (int mc = 0; mc < Monte_Carlo; mc++) {

          // memset(&ue.txdata[0][0], 0, sizeof(int32_t)*size);
          // memset(&gNB.rxdata_int[0][0], 0, sizeof(int32_t)*size);
          // memset(&gNB.rxdata_float[0][0], 0, sizeof(int32_t)*size);

#ifdef MAIN_DEBUG
            //
            printf("========================== Generate preamble ==========================\n");
            //
#endif
            ra_PreambleIndex = rand() % nof_preambles;
            generate_nr_prach(&ue, slot, ra_PreambleIndex, fd_occasion);

#ifdef MAIN_DEBUG
          //
          printf("========================== Channel Simulation ==========================\n");
          //
#endif
              for (int aa = 0; aa < ue.frame_parms.nb_antennas_rx; aa++) {
                time_delay_in_samples(&txdata_ue[aa][0], &rxdata_gNB[aa][0], size, delay);
                freq_shift(&rxdata_gNB[aa][0], &rxdata_gNB[aa][0], size, freq_shift_Hz);
                noise_randn(&rxdata_gNB[aa][0], &rxdata_gNB[aa][0], slot*30720, slot*30720+24576+3184, size, atof(argv[2]));
              }

        // generate_nr_prach(&ue, slot, ra_PreambleIndex, fd_occasion);

            // LOG_M("txdata0.m", "txdata", &txdata_ue[0][0], 307200, 1, 1);
            // for (int aa = 0; aa < ue.frame_parms.nb_antennas_rx; aa++) {
            //   freq_shift(&txdata_ue[aa][0], &rxdata_gNB[aa][0], 307200, freq_shift_Hz);
            //   noise_randn(&txdata_ue[aa][0], &rxdata_gNB[aa][0], slot*30720, slot*30720+30720, 307200, SNR_dB);
            //   time_delay(&txdata_ue[aa][0], &rxdata_gNB[aa][0], 307200, samples_delay);
            // }


            // memcpy(&gNB.rxdata_int[0][0], &ue.txdata[0][0], sizeof(int32_t)*ue.frame_parms.samples_per_frame);
            // memcpy(&rxdata_gNB[0sd][0], &txdata_ue[0][0], sizeof(int32_t)*ue.frame_parms.samples_per_frame);

            // LOG_M("rxdata0.m","rxdata",  &rxdata_gNB[0][0], 307200, 1, 1);

              if (gNB_float) {
                for (int aa = 0; aa < gNB.frame_parms.nb_antennas_rx; aa++) {
                  for (int i = 30720*slot; i < 30720*slot+30720; i++) {
                    gNB.rxdata_float[aa][2*i] = (float)((int16_t *)&gNB.rxdata_int[0][0])[2*i] / 32767.0;
                    gNB.rxdata_float[aa][2*i+1] = (float)((int16_t *)&gNB.rxdata_int[0][0])[2*i + 1] / 32767.0;
                  }
                }
              }
            // LOG_M("rxdata0.m","rxdata",  &rxdata_gNB[0][0], 307200, 1, 1);

              // LOG_M("rxdata0.m","rxdata",  &gNB.rxdata_float[0][0], 307200, 1, 13);

        // LOG_M("rxdata0_int.m", "rxdata", &ue.txdata[0][0], 30720*2, 1, 1);
        // LOG_M("rxdata0_float.m","rxdata", &gNB.rxdata_float[0][0], 30720*2, 1, 13);


#ifdef MAIN_DEBUG
          //
          printf("========================== Preamble detection ==========================\n");
          //
#endif
            max_preamble = 123; // not valid

            if (gNB_float)
              detect_nr_prach_float(&gNB, slot, fd_occasion, &max_preamble, &max_preamble_energy, &max_preamble_delay);
            else
              detect_nr_prach_int(&gNB, slot, fd_occasion, &max_preamble, &max_preamble_energy, &max_preamble_delay);

            // printf("[%d]\t%d\t%d\t%d\n", mc, SNR_dB, freq_shift_Hz, delay);
            if (max_preamble == ra_PreambleIndex && max_preamble_energy > 0) {
              printf("SUCCESS!!!\n");
              success++;
            } else {
              printf("FAIL!!!\n");
            }
          }
        }
      }
    }
    printf("Results: %d/%d\n", success, (Monte_Carlo*(int)((SNR_end-SNR_start)/SNR_step)*(delay_end-delay_start)*(freq_shift_end-freq_shift_start))/delay_step/freq_shift_step);

    deinit_nr_signal(&ue, &gNB);
}