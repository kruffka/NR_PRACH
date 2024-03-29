#include <stdio.h>
#include "nr_prach.h"

// #define NR_PRACH_DEBUG
// #define PRACH_WRITE_OUTPUT_DEBUG

uint32_t X_u[64][839];
uint32_t nr_ZC_inv[839];

int16_t nr_ru[2*839];
int32_t nr_du[839];

#define PHY 0
// #undef LOG_D
#define LOG_I(x, ...) printf(__VA_ARGS__)
#define LOG_D(x, ...) printf(__VA_ARGS__)


void init_nr_prach_tables(int N_ZC)
{
  int i,m;

  // Compute the modular multiplicative inverse 'iu' of u s.t. iu*u = 1 mod N_ZC
  nr_ZC_inv[0] = 0;
  nr_ZC_inv[1] = 1;

  for (i=2; i<N_ZC; i++) {
    for (m=2; m<N_ZC; m++)
      if (((i*m)%N_ZC) == 1) {
        nr_ZC_inv[i] = m;
        break;
      }

#ifdef NR_PRACH_DEBUG

    if (i<16)
      printf("i %d : inv %d\n",i,nr_ZC_inv[i]);

#endif
  }

  // FILE *file = fopen("prach/nr_ru.m", "w");
  // if(file == NULL){
  //   exit_fun("PRACH OPEN FILE");
  // }
  //   fprintf(file, "nr_ru = [");

  // Compute quantized roots of unity
  for (i=0; i<N_ZC; i++) {
    nr_ru[i<<1]     = (int16_t)(floor(32767.0*cos(2*M_PI*(double)i/N_ZC)));
    nr_ru[1+(i<<1)] = (int16_t)(floor(32767.0*sin(2*M_PI*(double)i/N_ZC)));
    // printf("%d + j*(%d)\n", nr_ru[i<<1], nr_ru[(i<<1) + 1]);
    // fprintf(file, "%d + j*(%d)\n", nr_ru[i<<1], nr_ru[(i<<1) + 1]);
#ifdef NR_PRACH_DEBUG

    if (i<16)
      printf("i %d : runity %d,%d\n",i,nr_ru[i<<1],nr_ru[1+(i<<1)]);

#endif
  }

  // fprintf(file, "];");
  // fclose(file);
}

// This function computes the du
void nr_fill_du(uint16_t N_ZC, const uint16_t *prach_root_sequence_map)
{

  uint16_t iu,u,p;

  for (iu=0; iu<(N_ZC-1); iu++) {

    u=prach_root_sequence_map[iu];
    p=1;

    while (((u*p)%N_ZC)!=1)
      p++;

    nr_du[u] = ((p<(N_ZC>>1)) ? p : (N_ZC-p));
  }

}

void compute_nr_prach_seq(uint8_t short_sequence,
                          uint8_t num_sequences,
                          uint8_t rootSequenceIndex,
                          uint32_t X_u[64][839]) {

  // Compute DFT of x_u => X_u[k] = x_u(inv(u)*k)^* X_u[k] = exp(j\pi u*inv(u)*k*(inv(u)*k+1)/N_ZC)
  unsigned int k,inv_u,i;
  int N_ZC;

  const uint16_t *prach_root_sequence_map;
  uint16_t u;

  #ifdef NR_PRACH_DEBUG
    printf("compute_prach_seq: prach short sequence %x, num_sequences %d, rootSequenceIndex %d\n", short_sequence, num_sequences, rootSequenceIndex);
  #endif

  N_ZC = (short_sequence) ? 139 : 839;
  
  if (short_sequence) {
    // FIXME cannot be reached
    prach_root_sequence_map = prach_root_sequence_map_abc;
  } else {
    prach_root_sequence_map = prach_root_sequence_map_0_3;
  }
  #ifdef NR_PRACH_DEBUG
    printf("compute_prach_seq: done init prach_tables\n" );
  #endif

  for (i=0; i<num_sequences; i++) {
    int index = (rootSequenceIndex+i) % (N_ZC-1);

    if (short_sequence) {
      // prach_root_sequence_map points to prach_root_sequence_map4
      if(index > sizeof(prach_root_sequence_map_abc) / sizeof(prach_root_sequence_map_abc[0])) {
          printf("index > sizeof(prach_root_sequence_map_abc) / sizeof(prach_root_sequence_map_abc[0])\n");
          abort();
      }
 
    } else {
      // prach_root_sequence_map points to prach_root_sequence_map0_3
      if(index > sizeof(prach_root_sequence_map_abc) / sizeof(prach_root_sequence_map_0_3[0])) {
          printf("index > sizeof(prach_root_sequence_map_0_3) / sizeof(prach_root_sequence_map_0_3[0])\n");
          abort();
      }

    }

    u = prach_root_sequence_map[index];
    #ifdef NR_PRACH_DEBUG
      printf("prach index %d => u=%d\n",index,u);
    #endif
    inv_u = nr_ZC_inv[u]; // multiplicative inverse of u


    // X_u[0] stores the first ZC sequence where the root u has a non-zero number of shifts
    // for the unrestricted case X_u[0] is the first root indicated by the rootSequenceIndex

    for (k=0; k<N_ZC; k++) {
      // multiply by inverse of 2 (required since ru is exp[j 2\pi n])
      X_u[i][k] = ((uint32_t*)nr_ru)[(((k*(1+(inv_u*k)))%N_ZC)*nr_ZC_inv[2])%N_ZC];


      // if(i == 0 && k < 10){
      //   printf("[%d]: %d + j*(%d) %d\n", k, ((int16_t *)&X_u[i][0])[2*k], ((int16_t *)&X_u[i][0])[2*k + 1], (((k*(1+(inv_u*k)))%N_ZC)*nr_ZC_inv[2])%N_ZC);
      // }
    }
  }
  // printf("\n");

}

int max(int a, int b) {
    return a > b;
}

int min(int a, int b) {
    return a < b;
}

#define PRACH_AMP_INT 512


int32_t generate_nr_prach(PHY_VARS_NR_UE *ue, int slot, int ra_PreambleIndex, uint8_t fd_occasion) {

  NR_FRAME_PARMS *fp = &ue->frame_parms;
  fapi_nr_prach_config_t *prach_config = &ue->prach_config;
  fapi_nr_prach_pdu_t *prach_pdu = &ue->prach_pdu;

  uint8_t preamble_index, restricted_set, not_found;
  uint16_t rootSequenceIndex, prach_fmt_id, NCS, preamble_offset = 0;
  const uint16_t *prach_root_sequence_map;
  uint16_t preamble_shift = 0, preamble_index0, n_shift_ra, n_shift_ra_bar, d_start = INT16_MAX, numshift, N_ZC, u, offset, offset2, first_nonzero_root_idx;

  int16_t prach_tmp[(4688+4*24576)*4*2] __attribute__((aligned(32))) = {0};
  int16_t prachF_tmp[(4688+4*24576)*4*2] __attribute__((aligned(32))) = {0};

  int16_t Ncp = 0, amp, *prach, *prach2, *prachF, *Xu;
  int32_t Xu_re, Xu_im;
  int prach_start, prach_sequence_length, i, prach_len, dftlen, mu, kbar, K, n_ra_prb, k, prachStartSymbol, sample_offset_slot;

  preamble_index          = ra_PreambleIndex;
  prach                   = prach_tmp;
  prachF                  = prachF_tmp;
  amp                     = PRACH_AMP_INT;
  NCS                     = prach_pdu->numCs;
  prach_fmt_id            = prach_pdu->prach_Format;
  prachStartSymbol        = prach_pdu->prach_StartSymbol;
  prach_sequence_length   = prach_config->prach_sequence_length;
  mu                      = prach_config->prach_sub_c_spacing;
  n_ra_prb                = prach_config->num_prach_fd_occasions_list[fd_occasion].k1;
  restricted_set          = prach_config->restricted_set_config;
  rootSequenceIndex       = prach_config->num_prach_fd_occasions_list[fd_occasion].prach_root_sequence_index;
  kbar                    = 1;
  K                       = 24;
  k                       = 12*n_ra_prb - 6*fp->N_RB_UL;
  N_ZC                    = (prach_sequence_length == 0) ? 839:139;
  prach_len               = 0;
  dftlen                  = 0;
  first_nonzero_root_idx  = 0;

#ifdef NR_PRACH_DEBUG
  printf("Generate NR PRACH in slot %d\n", slot);
#endif

  // compute_nr_prach_seq(prach_config->prach_sequence_length,
  //                      prach_config->num_prach_fd_occasions_list[fd_occasion].num_root_sequences,
  //                      prach_config->num_prach_fd_occasions_list[fd_occasion].prach_root_sequence_index,
  //                      ue->X_u);

  if (prachStartSymbol == 0) {
    sample_offset_slot = 0;
  } else if (fp->slots_per_subframe == 1) {
    if (prachStartSymbol <= 7)
      sample_offset_slot = (fp->ofdm_symbol_size + fp->nb_prefix_samples) * (prachStartSymbol - 1) + (fp->ofdm_symbol_size + fp->nb_prefix_samples0);
    else
      sample_offset_slot = (fp->ofdm_symbol_size + fp->nb_prefix_samples) * (prachStartSymbol - 2) + (fp->ofdm_symbol_size + fp->nb_prefix_samples0) * 2;
  } else {
    if (slot % (fp->slots_per_subframe / 2) == 0)
      sample_offset_slot = (fp->ofdm_symbol_size + fp->nb_prefix_samples) * (prachStartSymbol - 1) + (fp->ofdm_symbol_size + fp->nb_prefix_samples0);
    else
      sample_offset_slot = (fp->ofdm_symbol_size + fp->nb_prefix_samples) * prachStartSymbol;
  }

  prach_start = fp->get_samples_slot_timestamp(slot, fp, 0) + sample_offset_slot;

#ifdef NR_PRACH_DEBUG
  printf("prachstartsymbold %d, sample_offset_slot %d, prach_start %d\n",prachStartSymbol, sample_offset_slot, prach_start);
#endif

  // First compute physical root sequence
  /************************************************************************
  * 4G and NR NCS tables are slightly different and depend on prach format
  * Table 6.3.3.1-5:  for preamble formats with delta_f_RA = 1.25 Khz (formats 0,1,2)
  * Table 6.3.3.1-6:  for preamble formats with delta_f_RA = 5 Khz (formats 3)
  * NOTE: Restricted set type B is not implemented
  *************************************************************************/

  prach_root_sequence_map = (prach_sequence_length == 0) ? prach_root_sequence_map_0_3 : prach_root_sequence_map_abc;

  if (restricted_set == 0) {
    // This is the relative offset (for unrestricted case) in the root sequence table (5.7.2-4 from 36.211) for the given preamble index
    preamble_offset = ((NCS==0)? preamble_index : (preamble_index/(N_ZC/NCS)));
    // This is the \nu corresponding to the preamble index
    preamble_shift  = (NCS==0)? 0 : (preamble_index % (N_ZC/NCS));
    preamble_shift *= NCS;
  } else { // This is the high-speed case

    #ifdef NR_PRACH_DEBUG
      LOG_I(PHY, "PRACH High-speed mode, NCS %d\n", NCS);
    #endif

    not_found = 1;
    nr_fill_du(N_ZC,prach_root_sequence_map);
    preamble_index0 = preamble_index;
    // set preamble_offset to initial rootSequenceIndex and look if we need more root sequences for this
    // preamble index and find the corresponding cyclic shift
    preamble_offset = 0; // relative rootSequenceIndex;

    while (not_found == 1) {
      // current root depending on rootSequenceIndex and preamble_offset
      int index = (rootSequenceIndex + preamble_offset) % N_ZC;
      uint16_t n_group_ra = 0;

      if (prach_fmt_id<4) {
        // prach_root_sequence_map points to prach_root_sequence_map0_3
        // DevAssert( index < sizeof(prach_root_sequence_map_0_3) / sizeof(prach_root_sequence_map_0_3[0]) );
        abort();
      } else {
        // prach_root_sequence_map points to prach_root_sequence_map4
        // DevAssert( index < sizeof(prach_root_sequence_map_abc) / sizeof(prach_root_sequence_map_abc[0]) );
        abort();

      }

      u = prach_root_sequence_map[index];

      if ( (nr_du[u]<(N_ZC/3)) && (nr_du[u]>=NCS) ) {
        n_shift_ra     = nr_du[u]/NCS;
        d_start        = (nr_du[u]<<1) + (n_shift_ra * NCS);
        n_group_ra     = N_ZC/d_start;
        n_shift_ra_bar = max(0,(N_ZC-(nr_du[u]<<1)-(n_group_ra*d_start))/N_ZC);
      } else if  ( (nr_du[u]>=(N_ZC/3)) && (nr_du[u]<=((N_ZC - NCS)>>1)) ) {
        n_shift_ra     = (N_ZC - (nr_du[u]<<1))/NCS;
        d_start        = N_ZC - (nr_du[u]<<1) + (n_shift_ra * NCS);
        n_group_ra     = nr_du[u]/d_start;
        n_shift_ra_bar = min(n_shift_ra,max(0,(nr_du[u]- (n_group_ra*d_start))/NCS));
      } else {
        n_shift_ra     = 0;
        n_shift_ra_bar = 0;
      }

      // This is the number of cyclic shifts for the current root u
      numshift = (n_shift_ra*n_group_ra) + n_shift_ra_bar;

      if (numshift>0 && preamble_index0==preamble_index)
        first_nonzero_root_idx = preamble_offset;

      if (preamble_index0 < numshift) {
        not_found      = 0;
        preamble_shift = (d_start * (preamble_index0/n_shift_ra)) + ((preamble_index0%n_shift_ra)*NCS);

      } else { // skip to next rootSequenceIndex and recompute parameters
        preamble_offset++;
        preamble_index0 -= numshift;
      }
    }
  }

  // now generate PRACH signal
#ifdef NR_PRACH_DEBUG
const char *prachfmt[]={"0","1","2","3", "A1","A2","A3","B1","B4","C0","C2","A1/B1","A2/B2","A3/B3"};

    if (NCS>0)
      LOG_I(PHY, "PRACH generate PRACH in slot %%d for RootSeqIndex %d, Preamble Index %d, PRACH Format %s, NCS %d (N_ZC %d): Preamble_offset %d, Preamble_shift %d msg1 frequency start %d\n",
        slot,
        rootSequenceIndex,
        preamble_index,
        prachfmt[prach_fmt_id],
        NCS,
        N_ZC,
        preamble_offset,
        preamble_shift,
        n_ra_prb);
  #endif

  if (prach_sequence_length == 0) {
    if (prach_fmt_id == 0) {
      K = 12;
      kbar = 7;
    } else if (prach_fmt_id == 3) {
      K = 4;
      kbar = 10;
    } else {
      AssertFatal(1==0, "Need to fill K and kbar for PRACH format %d for sequence length 839\n", prach_fmt_id);
    }

  } else if (prach_sequence_length == 1) {
    K = 1;
    kbar = 2;
  }

  if (k < 0)
    k += fp->ofdm_symbol_size;

  k = K * k + kbar;
  k *= 2;
#ifdef NR_PRACH_DEBUG
  printf("PRACH in slot %d, placing PRACH in position %d, msg1 frequency start %d (k1 %d), preamble_offset %d, first_nonzero_root_idx %d\n",
        slot,
        k,
        n_ra_prb,
        prach_config->num_prach_fd_occasions_list[fd_occasion].k1,
        preamble_offset,
        first_nonzero_root_idx);
#endif

  // Ncp and dftlen here is given in terms of T_s wich is 30.72MHz sampling
  if (prach_sequence_length == 0) {
    switch (prach_fmt_id) {
    case 0:
      Ncp = 3168;
      dftlen = 24576;
      break;

    case 1:
      Ncp = 21024;
      dftlen = 24576;
      break;

    case 2:
      Ncp = 4688;
      dftlen = 24576;
      break;

    case 3:
      Ncp = 3168;
      dftlen = 6144;
      break;

    default:
      AssertFatal(1==0, "Illegal PRACH format %d for sequence length 839\n", prach_fmt_id);
      break;
    }
  } else {
    switch (prach_fmt_id) {
    case 4: //A1
      Ncp = 288 >> mu;
      break;

    case 5: //A2
      Ncp = 576 >> mu;
      break;

    case 6: //A3
      Ncp = 864 >> mu;
      break;

    case 7: //B1
      Ncp = 216 >> mu;
      break;

    /*
    case 4: //B2
      Ncp = 360 >> mu;
      break;

    case 5: //B3
      Ncp = 504 >> mu;
      break;
    */

    case 8: //B4
      Ncp = 936 >> mu;
      break;

    case 9: //C0
      Ncp = 1240 >> mu;
      break;

    case 10: //C2
      Ncp = 2048 >> mu;
      break;

    default:
      AssertFatal(1==0, "Unknown PRACH format ID %d\n", prach_fmt_id);
      break;
    }
    dftlen = 2048 >> mu;
  }

  k += dftlen; // fftshift

  if (k >= dftlen) k -= dftlen;

  //actually what we should be checking here is how often the current prach crosses a 0.5ms boundary. I am not quite sure for which paramter set this would be the case, so I will ignore it for now and just check if the prach starts on a 0.5ms boundary
  if (fp->numerology_index == 0) {
    if (prachStartSymbol == 0 || prachStartSymbol == 7)
      Ncp += 16;
  } else {
    if (slot%(fp->slots_per_subframe/2)==0 && prachStartSymbol == 0)
      Ncp += 16;
  }

  switch(fp->samples_per_subframe) {
  case 7680:
    // 5 MHz @ 7.68 Ms/s
    Ncp >>= 2;
    dftlen >>= 2;
    break;

  case 15360:
    // 10, 15 MHz @ 15.36 Ms/s
    Ncp >>= 1;
    dftlen >>= 1;
    break;

  case 30720:
    // 20, 25, 30 MHz @ 30.72 Ms/s
    break;

  case 46080:
    // 40 MHz @ 46.08 Ms/s
    Ncp = (Ncp*3)/2;
    dftlen = (dftlen*3)/2;
    break;

  case 61440:
    // 40, 50, 60 MHz @ 61.44 Ms/s
    Ncp <<= 1;
    dftlen <<= 1;
    break;

  case 92160:
    // 50, 60, 70, 80, 90 MHz @ 92.16 Ms/s
    Ncp *= 3;
    dftlen *= 3;
    break;

  case 122880:
    // 70, 80, 90, 100 MHz @ 122.88 Ms/s
    Ncp <<= 2;
    dftlen <<= 2;
    break;

  default:
    AssertFatal(1==0, "sample rate %f MHz not supported for numerology %d\n", fp->samples_per_subframe / 1000.0, mu);
  }

  #ifdef NR_PRACH_DEBUG
    LOG_I(PHY, "PRACH Ncp %d, dftlen %d\n", Ncp, dftlen);
  #endif

  /********************************************************
   *
   * In function init_prach_tables:
   * to compute quantized roots of unity ru(n) = 32767 * exp j*[ (2 * PI * n) / N_ZC ]
   *
   * In compute_prach_seq:
   * to calculate Xu = DFT xu = xu (inv_u*k) * Xu[0] (This is a Zadoff-Chou sequence property: DFT ZC sequence is another ZC sequence)
   *
   * In generate_prach:
   * to do the cyclic-shifted DFT by multiplying Xu[k] * ru[k*preamble_shift] as:
   * If X[k] = DFT x(n) -> X_shifted[k] = DFT x(n+preamble_shift) = X[k] * exp -j*[ (2*PI*k*preamble_shift) / N_ZC ]
   *
   *********************************************************/

  Xu = (int16_t*)ue->X_u[preamble_offset-first_nonzero_root_idx];

  #if defined (PRACH_WRITE_OUTPUT_DEBUG)
    LOG_M("X_u.m", "X_u", (int16_t*)ue->X_u[preamble_offset-first_nonzero_root_idx], N_ZC, 1, 1);
  #endif
  
  // printf("k = %d\n", k/2);

  for (offset=0,offset2=0; offset<N_ZC; offset++,offset2+=preamble_shift) {
    if (offset2 >= N_ZC)
      offset2 -= N_ZC;

    Xu_re = (((int32_t)Xu[offset<<1]*amp)>>15);
    Xu_im = (((int32_t)Xu[1+(offset<<1)]*amp)>>15);
    prachF[k++]= ((Xu_re*nr_ru[offset2<<1]) - (Xu_im*nr_ru[1+(offset2<<1)]))>>15;
    prachF[k++]= ((Xu_im*nr_ru[offset2<<1]) + (Xu_re*nr_ru[1+(offset2<<1)]))>>15;
    // if (offset < 10)
    // printf("k: %d Xu(%d + j*(%d)) prachF(%d + j*(%d))\n", k, ((int16_t *)&prachF[0])[k-1], ((int16_t *)&prachF[0])[k], prachF[k-1], prachF[k], Xu_im);

    if (k == dftlen) k = 0;
  }

  #if defined (PRACH_WRITE_OUTPUT_DEBUG)
    LOG_M("prachF.m", "prachF", &prachF[0], dftlen, 1, 1);
    LOG_M("Xu.m", "Xu", Xu, N_ZC, 1, 1);
  #endif

  // This is after cyclic prefix
  prach2 = prach+(2*Ncp); // times 2 for complex samples
  const idft_size_idx_t idft_size = get_idft(dftlen);
  idft_oai(idft_size, prachF, prach, 1);
  memmove(prach2, prach, (dftlen<<2));

  if (prach_sequence_length == 0) {
    if (prach_fmt_id == 0) {
      // here we have | empty  | Prach |
      memcpy(prach, prach+(dftlen<<1), (Ncp<<2));
      // here we have | Prefix | Prach |
      prach_len = dftlen+Ncp;
    } else if (prach_fmt_id == 1) {
      // here we have | empty  | Prach | empty |
      memcpy(prach2+(dftlen<<1), prach2, (dftlen<<2));
      // here we have | empty  | Prach | Prach |
      memcpy(prach, prach+(dftlen<<2), (Ncp<<2));
      // here we have | Prefix | Prach | Prach |
      prach_len = (dftlen*2)+Ncp;
    } else if (prach_fmt_id == 2 || prach_fmt_id == 3) {
      // here we have | empty  | Prach | empty | empty | empty |
      memcpy(prach2+(dftlen<<1), prach2, (dftlen<<2));
      // here we have | empty  | Prach | Prach | empty | empty |
      memcpy(prach2+(dftlen<<2), prach2, (dftlen<<3));
      // here we have | empty  | Prach | Prach | Prach | Prach |
      memcpy(prach, prach+(dftlen<<3), (Ncp<<2));
      // here we have | Prefix | Prach | Prach | Prach | Prach |
      prach_len = (dftlen*4)+Ncp;
    }
  } else { // short PRACH sequence
    if (prach_fmt_id == 9) {
      // here we have | empty  | Prach |
      memcpy(prach, prach+(dftlen<<1), (Ncp<<2));
      // here we have | Prefix | Prach |
      prach_len = (dftlen*1)+Ncp;
    } else if (prach_fmt_id == 4 || prach_fmt_id == 7) {
      // here we have | empty  | Prach | empty |
      memcpy(prach2+(dftlen<<1), prach2, (dftlen<<2));
      // here we have | empty  | Prach | Prach |
      memcpy(prach, prach+(dftlen<<1), (Ncp<<2));
      // here we have | Prefix | Prach | Prach |
      prach_len = (dftlen*2)+Ncp;
    } else if (prach_fmt_id == 5) { // 4xdftlen
      // here we have | empty  | Prach | empty | empty | empty |
      memcpy(prach2+(dftlen<<1), prach2, (dftlen<<2));
      // here we have | empty  | Prach | Prach | empty | empty |
      memcpy(prach2+(dftlen<<2), prach2, (dftlen<<3));
      // here we have | empty  | Prach | Prach | Prach | Prach |
      memcpy(prach, prach+(dftlen<<1), (Ncp<<2));
      // here we have | Prefix | Prach | Prach | Prach | Prach |
      prach_len = (dftlen*4)+Ncp;
    } else if (prach_fmt_id == 6) { // 6xdftlen
      // here we have | empty  | Prach | empty | empty | empty | empty | empty |
      memcpy(prach2+(dftlen<<1), prach2, (dftlen<<2));
      // here we have | empty  | Prach | Prach | empty | empty | empty | empty |
      memcpy(prach2+(dftlen<<2), prach2, (dftlen<<3));
      // here we have | empty  | Prach | Prach | Prach | Prach | empty | empty |
      memcpy(prach2+(dftlen<<3), prach2, (dftlen<<3));
      // here we have | empty  | Prach | Prach | Prach | Prach | Prach | Prach |
      memcpy(prach, prach+(dftlen<<1), (Ncp<<2));
      // here we have | Prefix | Prach | Prach | Prach | Prach | Prach | Prach |
      prach_len = (dftlen*6)+Ncp;
    } else if (prach_fmt_id == 8) { // 12xdftlen
      // here we have | empty  | Prach | empty | empty | empty | empty | empty | empty | empty | empty | empty | empty | empty |
      memcpy(prach2+(dftlen<<1), prach2, (dftlen<<2));
      // here we have | empty  | Prach | Prach | empty | empty | empty | empty | empty | empty | empty | empty | empty | empty |
      memcpy(prach2+(dftlen<<2), prach2, (dftlen<<3));
      // here we have | empty  | Prach | Prach | Prach | Prach | empty | empty | empty | empty | empty | empty | empty | empty |
      memcpy(prach2+(dftlen<<3), prach2, (dftlen<<3));
      // here we have | empty  | Prach | Prach | Prach | Prach | Prach | Prach | empty | empty | empty | empty | empty | empty |
      memcpy(prach2+(dftlen<<1)*6, prach2, (dftlen<<2)*6);
      // here we have | empty  | Prach | Prach | Prach | Prach | Prach | Prach | Prach | Prach | Prach | Prach | Prach | Prach |
      memcpy(prach, prach+(dftlen<<1), (Ncp<<2));
      // here we have | Prefix | Prach | Prach | Prach | Prach | Prach | Prach | Prach | Prach | Prach | Prach | Prach | Prach |
      prach_len = (dftlen*12)+Ncp;
    }
  }

  #ifdef NR_PRACH_DEBUG
    LOG_I(PHY, "PRACH N_RB_UL %d prach_start %d, prach_len %d\n",
      fp->N_RB_UL,
      prach_start,
      prach_len);
  #endif

  for (i=0; i<prach_len; i++) {
    ((int16_t*)(&ue->txdata[0][prach_start]))[2*i] = prach[2*i];
    ((int16_t*)(&ue->txdata[0][prach_start]))[2*i+1] = prach[2*i+1];
  }

  //printf("----------------------\n");
  //for(int ii = prach_start; ii<2*(prach_start + prach_len); ii++){
  //  printf("PRACH rx data[%d] = %d\n", ii, ue->common_vars.txdata[0][ii]);
  //}
  //printf(" \n");

  #ifdef PRACH_WRITE_OUTPUT_DEBUG
    LOG_M("prach_tx0.m", "prachtx0", prach+(Ncp<<1), prach_len-Ncp, 1, 1);
    LOG_M("Prach_txsig.m","txs",(int16_t*)(&ue->txdata[0][prach_start]), 2*(prach_start+prach_len), 1, 1);
  #endif

  return 0; // signal_energy((int*)prach, 256);
}