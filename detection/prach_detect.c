#include <stdio.h>
#include <stdint.h>
#include "nr_prach.h"

#define NR_PRACH_DEBUG

int detect_nr_prach(int16_t *txdataF, 
                    uint16_t *max_preamble,
                    uint16_t *max_preamble_energy,
                    uint16_t *max_preamble_delay,
                    uint32_t X_u[64][839]) {

int i;

  uint16_t           rootSequenceIndex;  
  int                numrootSequenceIndex;
  uint8_t            restricted_set;      
  uint8_t            n_ra_prb=0xFF;
  int16_t            *prachF=NULL;
  int                nb_rx;

  int16_t *rxsigF            = txdataF;
  uint8_t preamble_index;
  uint16_t NCS=99,NCS2;
  uint16_t preamble_offset=0,preamble_offset_old;
  int16_t preamble_shift=0;
  uint32_t preamble_shift2;
  uint16_t preamble_index0=0,n_shift_ra=0,n_shift_ra_bar;
  uint16_t d_start=0;
  uint16_t numshift=0;
  uint16_t *prach_root_sequence_map;
  uint8_t not_found;
  uint16_t u;
  int16_t *Xu=0;
  uint16_t offset;
  uint16_t first_nonzero_root_idx=0;
  uint8_t new_dft=0;
  uint8_t aa;
  int32_t lev;
  int16_t levdB;
  int log2_ifft_size=10;
  int16_t prach_ifft_tmp[2048*2] __attribute__((aligned(32)));
  int32_t *prach_ifft=(int32_t*)NULL;
  

  nb_rx = 1;
  rootSequenceIndex   = 1;
  numrootSequenceIndex   = 1;
  NCS          = 0;//cfg->num_prach_fd_occasions_list[0].prach_zero_corr_conf.value;
  int prach_sequence_length = 0; // 0 for 839
  int msg1_frequencystart   = 0;
  //  int num_unused_root_sequences = cfg->num_prach_fd_occasions_list[0].num_unused_root_sequences.value;
  // cfg->num_prach_fd_occasions_list[0].unused_root_sequences_list

  restricted_set      = 0;

  uint8_t prach_fmt = 0;
  uint16_t N_ZC = (prach_sequence_length==0)?839:139;

  #ifdef NR_PRACH_DEBUG
    printf("L1 PRACH RX: rooSequenceIndex %d, numRootSeqeuences %d, NCS %d, N_ZC %d, format %d \n",rootSequenceIndex,numrootSequenceIndex,NCS,N_ZC,prach_fmt);
    printf("PRACH (gNB) : running rx_prach for subframe %d, msg1_frequencystart %d, rootSequenceIndex %d\n", 1,msg1_frequencystart,rootSequenceIndex);
  #endif

  prach_ifft        = (int32_t *)malloc(sizeof(int32_t)*2048);
  prachF            = (int16_t *)malloc(sizeof(int32_t)*2048);


  prach_root_sequence_map = (prach_sequence_length==0) ? prach_root_sequence_map_0_3 : prach_root_sequence_map_abc;

  // PDP is oversampled, e.g. 1024 sample instead of 839
  // Adapt the NCS (zero-correlation zones) with oversampling factor e.g. 1024/839
  NCS2 = (N_ZC==839) ? ((NCS<<10)/839) : ((NCS<<8)/139);

  if (NCS2==0) NCS2 = N_ZC;


  preamble_offset_old = 99;

  
  *max_preamble_energy=0;
  *max_preamble_delay=0;
  *max_preamble=0;

  for (preamble_index=0 ; preamble_index<64 ; preamble_index++) {

    #ifdef NR_PRACH_DEBUG      
      printf("frame %d, subframe %d : Trying preamble %d \n",0,1,preamble_index);
    #endif

    if (restricted_set == 0) {
      // This is the relative offset in the root sequence table (5.7.2-4 from 36.211) for the given preamble index
      preamble_offset = ((NCS==0)? preamble_index : (preamble_index/(N_ZC/NCS)));
   
      if (preamble_offset != preamble_offset_old) {
        preamble_offset_old = preamble_offset;
        new_dft = 1;
        // This is the \nu corresponding to the preamble index
        preamble_shift  = 0;
      }
      
      else {
        preamble_shift  -= NCS;

        if (preamble_shift < 0)
          preamble_shift+=N_ZC;
      }
    }
    // Compute DFT of RX signal (conjugate input, results in conjugate output) for each new rootSequenceIndex
    // if (LOG_DEBUGFLAG(PRACH)) {
    //   int en = dB_fixed(signal_energy((int32_t*)&rxsigF[0][0],840));
    //   if (en>60) LOG_I(PHY,"frame %d, subframe %d : preamble index %d, NCS %d, N_ZC/NCS %d: offset %d, preamble shift %d , en %d)\n",
		//        frame,subframe,preamble_index,NCS,N_ZC/NCS,preamble_offset,preamble_shift,en);
    // }
    #ifdef NR_PRACH_DEBUG
      printf("PRACH RX preamble_index %d, preamble_offset %d\n",preamble_index,preamble_offset);
    #endif

    if (new_dft == 1) {
      new_dft = 0;

      Xu=(int16_t*)X_u[preamble_offset-first_nonzero_root_idx];

      #ifdef NR_PRACH_DEBUG
        printf("PRACH RX new dft preamble_offset-first_nonzero_root_idx %d\n",preamble_offset-first_nonzero_root_idx);
      #endif

      memset(prach_ifft,0,((N_ZC==839) ? 2048 : 256)*sizeof(int32_t));
    

      memset(prachF, 0, sizeof(int16_t)*2*1024 );
      // if (LOG_DUMPFLAG(PRACH)) {      
	       LOG_M("prach_rxF0.m","prach_rxF0",rxsigF[0],N_ZC,1,1);
	    //    LOG_M("prach_rxF1.m","prach_rxF1",rxsigF[1],6144,1,1);
      // }
   
      for (aa=0;aa<nb_rx; aa++) {
	  // Do componentwise product with Xu* on each antenna 

	       for (offset=0; offset<(N_ZC<<1); offset+=2) {
	          prachF[offset]   = (int16_t)(((int32_t)Xu[offset]*rxsigF[offset]   + (int32_t)Xu[offset+1]*rxsigF[offset+1])>>15);
	          prachF[offset+1] = (int16_t)(((int32_t)Xu[offset]*rxsigF[offset+1] - (int32_t)Xu[offset+1]*rxsigF[offset])>>15);
	       }
	
	       // Now do IFFT of size 1024 (N_ZC=839) or 256 (N_ZC=139)
	       if (N_ZC == 839) {
	         idft(IDFT_1024,prachF,prach_ifft_tmp,1);
	         // compute energy and accumulate over receive antennas
	         for (i=0;i<1024;i++)
	           prach_ifft[i] += (int32_t)prach_ifft_tmp[i<<1]*(int32_t)prach_ifft_tmp[i<<1] + (int32_t)prach_ifft_tmp[1+(i<<1)]*(int32_t)prach_ifft_tmp[1+(i<<1)];
	       } else {
	         idft(IDFT_256,prachF,prach_ifft_tmp,1);
	         log2_ifft_size = 8;
           // compute energy and accumulate over receive antennas and repetitions for BR
           for (i=0;i<256;i++)
             prach_ifft[i] += (int32_t)prach_ifft_tmp[i<<1]*(int32_t)prach_ifft_tmp[(i<<1)] + (int32_t)prach_ifft_tmp[1+(i<<1)]*(int32_t)prach_ifft_tmp[1+(i<<1)];
         }

        // if (0) {
          if (aa==0) LOG_M("prach_rxF_comp0.m","prach_rxF_comp0",prachF,1024,1,1);
        //   if (aa==1) LOG_M("prach_rxF_comp1.m","prach_rxF_comp1",prachF,1024,1,1);
        // }

      }// antennas_rx

      // Normalization of energy over ifft and receive antennas
      if (N_ZC == 839) {
        log2_ifft_size = 10;
        for (i=0;i<1024;i++)
          prach_ifft[i] = (prach_ifft[i]>>log2_ifft_size)/nb_rx;
      } else {
        log2_ifft_size = 8;
        for (i=0;i<256;i++)
          prach_ifft[i] = (prach_ifft[i]>>log2_ifft_size)/nb_rx;
      }

    } // new dft
    
    // check energy in nth time shift, for 

    preamble_shift2 = ((preamble_shift==0) ? 0 : ((preamble_shift<<log2_ifft_size)/N_ZC));

    for (i=0; i<NCS2; i++) {
      lev = (int32_t)prach_ifft[(preamble_shift2+i)];
      levdB = dB_fixed_times10(lev);

      if (levdB>*max_preamble_energy) {
        #ifdef NR_PRACH_DEBUG
  	      printf("preamble_index %d, delay %d en %d dB > %d dB\n",preamble_index,i,levdB,*max_preamble_energy);
        #endif
	      *max_preamble_energy  = levdB;
	      *max_preamble_delay   = i; // Note: This has to be normalized to the 30.72 Ms/s sampling rate 
	      *max_preamble         = preamble_index;
      }
    }
  }// preamble_index

  // The conversion from *max_preamble_delay from TA value is done here.
  // It is normalized to the 30.72 Ms/s, considering the numerology, N_RB and the sampling rate
  // See table 6.3.3.1 -1 and -2 in 38211.

  // Format 0, 1, 2: 24576 samples @ 30.72 Ms/s, 98304 samples @ 122.88 Ms/s
  // By solving:
  // max_preamble_delay * ( (24576*(fs/30.72M)) / 1024 ) / fs = TA * 16 * 64 / 2^mu * Tc

  // Format 3: 6144 samples @ 30.72 Ms/s, 24576 samples @ 122.88 Ms/s
  // By solving:
  // max_preamble_delay * ( (6144*(fs/30.72M)) / 1024 ) / fs = TA * 16 * 64 / 2^mu * Tc

  // Format >3: 2048/2^mu samples @ 30.72 Ms/s, 2048/2^mu * 4 samples @ 122.88 Ms/s
  // By solving:
  // max_preamble_delay * ( (2048/2^mu*(fs/30.72M)) / 256 ) / fs = TA * 16 * 64 / 2^mu * Tc
  // uint16_t *TA = max_preamble_delay;
  // int mu = fp->numerology_index;
  // if (cfg->prach_sequence_length.value==0) {
  //   if (prach_fmt == 0 || prach_fmt == 1 || prach_fmt == 2) *TA = *TA*3*(1<<mu)/2;
  //   else if (prach_fmt == 3)                                *TA = *TA*3*(1<<mu)/8;
  // }
  // else *TA = *TA/2;


  // if (LOG_DUMPFLAG(PRACH)) {
  //   //int en = dB_fixed(signal_energy((int32_t*)&rxsigF[0][0],840));
  //   //    if (en>60) {
  //     int k = (12*n_ra_prb) - 6*fp->N_RB_UL;
      
  //     if (k<0) k+=fp->ofdm_symbol_size;
      
  //     k*=12;
  //     k+=13;
  //     k*=2;
      

  //     LOG_M("rxsigF.m","prach_rxF",&rxsigF[0][0],12288,1,1);
  //     LOG_M("prach_rxF_comp0.m","prach_rxF_comp0",prachF,1024,1,1);
  //     LOG_M("Xu.m","xu",Xu,N_ZC,1,1);
  //     LOG_M("prach_ifft0.m","prach_t0",prach_ifft,1024,1,1);
  //     //    }
  // } /* LOG_DUMPFLAG(PRACH) */

  free(prach_ifft);
  free(prachF);

  return 0;
}

