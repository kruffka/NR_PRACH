
#include "defs.h"

// #include "lib_defs.h"

// #define NR_PRACH_DEBUG
#define LOG_M(filename, arrayname, array, len, start) write_matlab(filename, arrayname, array, len, start)


extern uint16_t nr_du[838];
extern int16_t nr_ru[2*839];
int16_t dB_table_times10[256] = {
  0,
  30,
  47,
  60,
  69,
  77,
  84,
  90,
  95,
  100,
  104,
  107,
  111,
  114,
  117,
  120,
  123,
  125,
  127,
  130,
  132,
  134,
  136,
  138,
  139,
  141,
  143,
  144,
  146,
  147,
  149,
  150,
  151,
  153,
  154,
  155,
  156,
  157,
  159,
  160,
  161,
  162,
  163,
  164,
  165,
  166,
  167,
  168,
  169,
  169,
  170,
  171,
  172,
  173,
  174,
  174,
  175,
  176,
  177,
  177,
  178,
  179,
  179,
  180,
  181,
  181,
  182,
  183,
  183,
  184,
  185,
  185,
  186,
  186,
  187,
  188,
  188,
  189,
  189,
  190,
  190,
  191,
  191,
  192,
  192,
  193,
  193,
  194,
  194,
  195,
  195,
  196,
  196,
  197,
  197,
  198,
  198,
  199,
  199,
  200,
  200,
  200,
  201,
  201,
  202,
  202,
  202,
  203,
  203,
  204,
  204,
  204,
  205,
  205,
  206,
  206,
  206,
  207,
  207,
  207,
  208,
  208,
  208,
  209,
  209,
  210,
  210,
  210,
  211,
  211,
  211,
  212,
  212,
  212,
  213,
  213,
  213,
  213,
  214,
  214,
  214,
  215,
  215,
  215,
  216,
  216,
  216,
  217,
  217,
  217,
  217,
  218,
  218,
  218,
  219,
  219,
  219,
  219,
  220,
  220,
  220,
  220,
  221,
  221,
  221,
  222,
  222,
  222,
  222,
  223,
  223,
  223,
  223,
  224,
  224,
  224,
  224,
  225,
  225,
  225,
  225,
  226,
  226,
  226,
  226,
  226,
  227,
  227,
  227,
  227,
  228,
  228,
  228,
  228,
  229,
  229,
  229,
  229,
  229,
  230,
  230,
  230,
  230,
  230,
  231,
  231,
  231,
  231,
  232,
  232,
  232,
  232,
  232,
  233,
  233,
  233,
  233,
  233,
  234,
  234,
  234,
  234,
  234,
  235,
  235,
  235,
  235,
  235,
  235,
  236,
  236,
  236,
  236,
  236,
  237,
  237,
  237,
  237,
  237,
  238,
  238,
  238,
  238,
  238,
  238,
  239,
  239,
  239,
  239,
  239,
  239,
  240,
  240,
  240,
  240,
  240
};

//Table 6.3.3.1-3: Mapping from logical index i to sequence number u for preamble formats with L_RA = 839
uint16_t prach_root_sequence_map_0_3[838] = {
129, 710, 140, 699, 120, 719, 210, 629, 168, 671, 84 , 755, 105, 734, 93 , 746, 70 , 769, 60 , 779,
2  , 837, 1  , 838, 56 , 783, 112, 727, 148, 691, 80 , 759, 42 , 797, 40 , 799, 35 , 804, 73 , 766,
146, 693, 31 , 808, 28 , 811, 30 , 809, 27 , 812, 29 , 810, 24 , 815, 48 , 791, 68 , 771, 74 , 765,
178, 661, 136, 703, 86 , 753, 78 , 761, 43 , 796, 39 , 800, 20 , 819, 21 , 818, 95 , 744, 202, 637,
190, 649, 181, 658, 137, 702, 125, 714, 151, 688, 217, 622, 128, 711, 142, 697, 122, 717, 203, 636,
118, 721, 110, 729, 89 , 750, 103, 736, 61 , 778, 55 , 784, 15 , 824, 14 , 825, 12 , 827, 23 , 816,
34 , 805, 37 , 802, 46 , 793, 207, 632, 179, 660, 145, 694, 130, 709, 223, 616, 228, 611, 227, 612,
132, 707, 133, 706, 143, 696, 135, 704, 161, 678, 201, 638, 173, 666, 106, 733, 83 , 756, 91 , 748,
66 , 773, 53 , 786, 10 , 829, 9  , 830, 7  , 832, 8  , 831, 16 , 823, 47 , 792, 64 , 775, 57 , 782,
104, 735, 101, 738, 108, 731, 208, 631, 184, 655, 197, 642, 191, 648, 121, 718, 141, 698, 149, 690,
216, 623, 218, 621, 152, 687, 144, 695, 134, 705, 138, 701, 199, 640, 162, 677, 176, 663, 119, 720,
158, 681, 164, 675, 174, 665, 171, 668, 170, 669, 87 , 752, 169, 670, 88 , 751, 107, 732, 81 , 758,
82 , 757, 100, 739, 98 , 741, 71 , 768, 59 , 780, 65 , 774, 50 , 789, 49 , 790, 26 , 813, 17 , 822,
13 , 826, 6  , 833, 5  , 834, 33 , 806, 51 , 788, 75 , 764, 99 , 740, 96 , 743, 97 , 742, 166, 673,
172, 667, 175, 664, 187, 652, 163, 676, 185, 654, 200, 639, 114, 725, 189, 650, 115, 724, 194, 645,
195, 644, 192, 647, 182, 657, 157, 682, 156, 683, 211, 628, 154, 685, 123, 716, 139, 700, 212, 627,
153, 686, 213, 626, 215, 624, 150, 689, 225, 614, 224, 615, 221, 618, 220, 619, 127, 712, 147, 692,
124, 715, 193, 646, 205, 634, 206, 633, 116, 723, 160, 679, 186, 653, 167, 672, 79 , 760, 85 , 754,
77 , 762, 92 , 747, 58 , 781, 62 , 777, 69 , 770, 54 , 785, 36 , 803, 32 , 807, 25 , 814, 18 , 821,
11 , 828, 4  , 835, 3  , 836, 19 , 820, 22 , 817, 41 , 798, 38 , 801, 44 , 795, 52 , 787, 45 , 794,
63 , 776, 67 , 772, 72 , 767, 76 , 763, 94 , 745, 102, 737, 90 , 749, 109, 730, 165, 674, 111, 728,
209, 630, 204, 635, 117, 722, 188, 651, 159, 680, 198, 641, 113, 726, 183, 656, 180, 659, 177, 662,
196, 643, 155, 684, 214, 625, 126, 713, 131, 708, 219, 620, 222, 617, 226, 613, 230, 609, 232, 607,
262, 577, 252, 587, 418, 421, 416, 423, 413, 426, 411, 428, 376, 463, 395, 444, 283, 556, 285, 554,
379, 460, 390, 449, 363, 476, 384, 455, 388, 451, 386, 453, 361, 478, 387, 452, 360, 479, 310, 529,
354, 485, 328, 511, 315, 524, 337, 502, 349, 490, 335, 504, 324, 515, 323, 516, 320, 519, 334, 505,
359, 480, 295, 544, 385, 454, 292, 547, 291, 548, 381, 458, 399, 440, 380, 459, 397, 442, 369, 470,
377, 462, 410, 429, 407, 432, 281, 558, 414, 425, 247, 592, 277, 562, 271, 568, 272, 567, 264, 575,
259, 580, 237, 602, 239, 600, 244, 595, 243, 596, 275, 564, 278, 561, 250, 589, 246, 593, 417, 422,
248, 591, 394, 445, 393, 446, 370, 469, 365, 474, 300, 539, 299, 540, 364, 475, 362, 477, 298, 541,
312, 527, 313, 526, 314, 525, 353, 486, 352, 487, 343, 496, 327, 512, 350, 489, 326, 513, 319, 520,
332, 507, 333, 506, 348, 491, 347, 492, 322, 517, 330, 509, 338, 501, 341, 498, 340, 499, 342, 497,
301, 538, 366, 473, 401, 438, 371, 468, 408, 431, 375, 464, 249, 590, 269, 570, 238, 601, 234, 605,
257, 582, 273, 566, 255, 584, 254, 585, 245, 594, 251, 588, 412, 427, 372, 467, 282, 557, 403, 436,
396, 443, 392, 447, 391, 448, 382, 457, 389, 450, 294, 545, 297, 542, 311, 528, 344, 495, 345, 494,
318, 521, 331, 508, 325, 514, 321, 518, 346, 493, 339, 500, 351, 488, 306, 533, 289, 550, 400, 439,
378, 461, 374, 465, 415, 424, 270, 569, 241, 598, 231, 608, 260, 579, 268, 571, 276, 563, 409, 430,
398, 441, 290, 549, 304, 535, 308, 531, 358, 481, 316, 523, 293, 546, 288, 551, 284, 555, 368, 471,
253, 586, 256, 583, 263, 576, 242, 597, 274, 565, 402, 437, 383, 456, 357, 482, 329, 510, 317, 522,
307, 532, 286, 553, 287, 552, 266, 573, 261, 578, 236, 603, 303, 536, 356, 483, 355, 484, 405, 434,
404, 435, 406, 433, 235, 604, 267, 572, 302, 537, 309, 530, 265, 574, 233, 606, 367, 472, 296, 543,
336, 503, 305, 534, 373, 466, 280, 559, 279, 560, 419, 420, 240, 599, 258, 581, 229, 610
};
// Table 6.3.3.1-4: Mapping from logical index i to sequence number u for preamble formats with L_RA = 139
uint16_t prach_root_sequence_map_abc[138] = {
1 , 138, 2 , 137, 3 , 136, 4 , 135, 5 , 134, 6 , 133, 7 , 132, 8 , 131, 9 , 130, 10, 129,
11, 128, 12, 127, 13, 126, 14, 125, 15, 124, 16, 123, 17, 122, 18, 121, 19, 120, 20, 119,
21, 118, 22, 117, 23, 116, 24, 115, 25, 114, 26, 113, 27, 112, 28, 111, 29, 110, 30, 109,
31, 108, 32, 107, 33, 106, 34, 105, 35, 104, 36, 103, 37, 102, 38, 101, 39, 100, 40, 99 ,
41, 98 , 42, 97 , 43, 96 , 44, 95 , 45, 94 , 46, 93 , 47, 92 , 48, 91 , 49, 90 , 50, 89 ,
51, 88 , 52, 87 , 53, 86 , 54, 85 , 55, 84 , 56, 83 , 57, 82 , 58, 81 , 59, 80 , 60, 79 ,
61, 78 , 62, 77 , 63, 76 , 64, 75 , 65, 74 , 66, 73 , 67, 72 , 68, 71 , 69, 70
};

const char *prachfmt[]={"0","1","2","3", "A1","A2","A3","B1","B4","C0","C2","A1/B1","A2/B2","A3/B3"};

uint32_t X_u[64][839];
int16_t nr_ru[2*839];
uint32_t nr_ZC_inv[839];
uint16_t nr_du[838];

void exit_function(const char *file, const char *function, const int line, const char *msg) {
  printf("%s: %s():%d - %s\n", file, function, line, msg);
  exit(0);
}


#define MAX_LEN 307200

void write_matlab(char *filename, char *arrayname, int32_t *array, uint32_t len, int start) {
  
  FILE* file = fopen(filename, "w");
  if(file == NULL) {
    exit_fun("cant open file");
  }

  if(start + len > MAX_LEN) {
    exit_fun("> max_len");
  }

  fprintf(file, "%s = [", arrayname);
  for (int i = 0; i < len; i++) {


    // ((int16_t *)&array[start])[2*i] /= 8;
    // ((int16_t *)&array[start])[2*i + 1] /= 8;


    fprintf(file, "%d + j*(%d)\n", ((int16_t *)&array[start])[2*i], ((int16_t *)&array[start])[2*i + 1]);
  }
  fprintf(file, "]");

  fclose(file);

}

int max(int a, int b) {
    return a > b ? a : b;
}

int min(int a, int b) {
    return a < b ? a : b;
}

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
void nr_fill_du(uint16_t N_ZC,uint16_t *prach_root_sequence_map)
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
                          uint32_t X_u[64][839]){

  // Compute DFT of x_u => X_u[k] = x_u(inv(u)*k)^* X_u[k] = exp(j\pi u*inv(u)*k*(inv(u)*k+1)/N_ZC)
  unsigned int k,inv_u,i;
  int N_ZC;

  uint16_t *prach_root_sequence_map;
  uint16_t u;

  #ifdef NR_PRACH_DEBUG
    printf("compute_prach_seq: prach short sequence %x, num_sequences %d, rootSequenceIndex %d\n", short_sequence, num_sequences, rootSequenceIndex);
  #endif

  N_ZC = (short_sequence) ? 139 : 839;
  //init_prach_tables(N_ZC); //moved to phy_init_lte_ue/eNB, since it takes to long in real-time
  
  init_nr_prach_tables(N_ZC);

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
    //   DevAssert( index < sizeof(prach_root_sequence_map_abc) / sizeof(prach_root_sequence_map_abc[0]) );
    if(index > sizeof(prach_root_sequence_map_abc) / sizeof(prach_root_sequence_map_abc[0]))
        printf("DevAssert( index < sizeof(prach_root_sequence_map_abc) / sizeof(prach_root_sequence_map_abc[0]) );\n");
    } else {
      // prach_root_sequence_map points to prach_root_sequence_map0_3
    //   DevAssert( index < sizeof(prach_root_sequence_map_0_3) / sizeof(prach_root_sequence_map_0_3[0]) );
    if(index > sizeof(prach_root_sequence_map_abc) / sizeof(prach_root_sequence_map_0_3[0]))
        printf("DevAssert( index < sizeof(prach_root_sequence_map_0_3) / sizeof(prach_root_sequence_map_0_3[0]) );\n");

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

      // if(i == 0 || i == 1){
      //   printf("[%d]: %d + j*(%d)\n", k, ((int16_t *)&X_u[i][0])[2*k], ((int16_t *)&X_u[i][0])[2*k + 1]);
      // }
    }
  }

}



// Note:
// - prach_fmt_id is an ID used to map to the corresponding PRACH format value in prachfmt
// WIP todo:
// - take prach start symbol into account
// - idft for short sequence assumes we are transmitting starting in symbol 0 of a PRACH slot
// - Assumes that PRACH SCS is same as PUSCH SCS @ 30 kHz, take values for formats 0-2 and adjust for others below
// - Preamble index different from 0 is not detected by gNB
int32_t generate_nr_prach(int32_t *txdata, int16_t *txdataF, uint8_t gNB_id, uint8_t slot, uint8_t preamble_idx, int prach_start){

  uint8_t Mod_id, fd_occasion, preamble_index, restricted_set, not_found;
  uint16_t rootSequenceIndex, prach_fmt_id, NCS, *prach_root_sequence_map, preamble_offset = 0;
  uint16_t preamble_shift = 0, preamble_index0, n_shift_ra, n_shift_ra_bar, d_start=INT16_MAX, numshift, N_ZC, u, offset, offset2, first_nonzero_root_idx;
  int16_t prach_tmp[98304*2*4] __attribute__((aligned(32)));

  int16_t Ncp = 0, amp, *prach, *prach2, *prachF, *Xu;
  int32_t Xu_re, Xu_im;
  int prach_sequence_length, i, prach_len, dftlen, mu, kbar, K, n_ra_prb, k, prachStartSymbol, sample_offset_slot;
  //int restricted_Type;


  int k1 = 0;
  int samples_per_subframe = 30720;
  int prachrootseqindex = 1;

  fd_occasion             = 0;
  prach_len               = 30720;
  dftlen                  = 0;
  first_nonzero_root_idx  = 0;
  prach                   = prach_tmp;
  prachF                  = txdataF;
  amp                     = 512;
  Mod_id                  = 0;
  prach_sequence_length   = 0;
  N_ZC                    = (prach_sequence_length == 0) ? 839:139;
  mu                      = 0;
  restricted_set          = 0;
  rootSequenceIndex       = 1;
  n_ra_prb                = 0,//prach_pdu->freq_msg1;
  NCS                     = 0;
  prach_fmt_id            = 0;
  preamble_index          = preamble_idx;//rand()%5;
  kbar                    = 1;
  K                       = 24;
  k                       = 12*n_ra_prb - 6*106;
  prachStartSymbol        = 0;
  //restricted_Type         = 0;

  // printf("k = %d n_ra_prb = %d fp->N_RB_UL %d\n", k, n_ra_prb, fp->N_RB_UL);

  compute_nr_prach_seq(prach_sequence_length,
                       64,
                       prachrootseqindex, //prachrootseqindex
                       X_u);

  // printf("slot %d, slots_per_subframe %d\n", slot, fp->slots_per_subframe);
  if (1){ // slot % (fp->slots_per_subframe / 2) == 0
    sample_offset_slot = (prachStartSymbol==0?0:2048*prachStartSymbol+160+144*(prachStartSymbol-1));

  }
  else{
    sample_offset_slot = (2048 + 144) * prachStartSymbol;

  }


  prach_start = 30720 + sample_offset_slot;

  // printf("prachstartsymbold %d, sample_offset_slot %d, prach_start %d\n",prachStartSymbol, sample_offset_slot, prach_start);

  // First compute physical root sequence
  /************************************************************************
  * 4G and NR NCS tables are slightly different and depend on prach format
  * Table 6.3.3.1-5:  for preamble formats with delta_f_RA = 1.25 Khz (formats 0,1,2)
  * Table 6.3.3.1-6:  for preamble formats with delta_f_RA = 5 Khz (formats 3)
  * NOTE: Restricted set type B is not implemented
  *************************************************************************/


  prach_root_sequence_map = (prach_sequence_length == 0) ? prach_root_sequence_map_0_3 : prach_root_sequence_map_abc;

  if (restricted_set == 0) {
    // printf("// This is the relative offset (for unrestricted case) in the root sequence table (5.7.2-4 from 36.211) for the given preamble index\n");
    // This is the relative offset (for unrestricted case) in the root sequence table (5.7.2-4 from 36.211) for the given preamble index
    preamble_offset = ((NCS==0)? preamble_index : (preamble_index/(N_ZC/NCS)));
    // This is the \nu corresponding to the preamble index
    preamble_shift  = (NCS==0)? 0 : (preamble_index % (N_ZC/NCS));
    preamble_shift *= NCS;
  }

  // now generate PRACH signal
#ifdef NR_PRACH_DEBUG
    // if (NCS>0)
      printf("PRACH [UE %d] generate PRACH in slot %d for RootSeqIndex %d, Preamble Index %d, PRACH Format %s, NCS %d (N_ZC %d): Preamble_offset %d, Preamble_shift %d msg1 frequency start %d\n",
        Mod_id,
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
  //  nsymb = (frame_parms->Ncp==0) ? 14:12;
  //  subframe_offset = (unsigned int)frame_parms->ofdm_symbol_size*slot*nsymb;

  if (prach_sequence_length == 0 && prach_fmt_id == 3) {
    K = 4;
    kbar = 10;
  } else if (prach_sequence_length == 1) {
    K = 1;
    kbar = 2;
  }
  
  if(prach_sequence_length == 0 && prach_fmt_id == 0){
    K = 12;
    kbar = 7;
  }

  Xu = (int16_t*)X_u[preamble_offset-first_nonzero_root_idx];

  #if defined (PRACH_WRITE_OUTPUT_DEBUG)
    LOG_M("tx/X_u.m", "X_u", (int16_t*)ue->X_u[preamble_offset-first_nonzero_root_idx], N_ZC, 1, 1);
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

if(mu == 0){
  switch(samples_per_subframe) {

    case 30720:
      // 20, 25, 30 MHz @ 30.72 Ms/s
      if (prach_sequence_length == 0) {
        if (prach_fmt_id == 0 || prach_fmt_id == 1 || prach_fmt_id == 2)
          dftlen = 24576;
        if (prach_fmt_id == 3)
          dftlen = 6144;
      } else { // 839 sequence
        dftlen = 1024;
      }
      break;

    default:
    //   AssertFatal(1==0,"sample rate %f MHz not supported for numerology %d\n", 30720 / 1000.0, mu);
        printf("sample rate %f MHz not supported for numerology %d\n", 30720 / 1000.0, mu);
    }
  }

  k = K*k + kbar + dftlen/2;
  k *= 2; // re and im

  #ifdef NR_PRACH_DEBUG
    printf("PRACH [UE %d] in slot %d, placing PRACH in position %d, msg1 frequency start %d (k1 %d), preamble_offset %d, first_nonzero_root_idx %d\n", Mod_id,
        slot,
        k,
        n_ra_prb,
        k1,
        preamble_offset,
        first_nonzero_root_idx);
  #endif


  for (offset=0,offset2=0; offset<N_ZC; offset++,offset2+=preamble_shift) {

    if (offset2 >= N_ZC)
      offset2 -= N_ZC;

    Xu_re = (((int32_t)Xu[offset<<1]*amp)>>15);
    Xu_im = (((int32_t)Xu[1+(offset<<1)]*amp)>>15);
    // printf("offset: %d Xu(%d + j*(%d)) offset2 %d\n", offset, Xu_re, Xu_im, offset2);
    prachF[k++]= ((Xu_re*nr_ru[offset2<<1]) - (Xu_im*nr_ru[1+(offset2<<1)]))>>15;
    prachF[k++]= ((Xu_im*nr_ru[offset2<<1]) + (Xu_re*nr_ru[1+(offset2<<1)]))>>15;
    // printf("k: %d Xu(%d + j*(%d)) prachF(%d + j*(%d))\n", k, ((int16_t *)&prachF[0])[k-1], ((int16_t *)&prachF[0])[k], prachF[k-1], prachF[k], Xu_im);

    if (k==dftlen) k=0;
  }

  // fft shift
  int32_t tmp = 0;
  for(int i = 0; i < dftlen/2; i++){

    tmp = ((int32_t *)&prachF[0])[i];
    ((int32_t *)&prachF[0])[i] = ((int32_t *)&prachF[0])[dftlen/2 + i];
    ((int32_t *)&prachF[0])[dftlen/2 + i] = tmp;

  }


#if 0 // idft
if (mu == 0) {
    switch (samples_per_subframe) {

    case 30720: // full sampling @ 30.72 Ms/s
      Ncp = Ncp*1; // to account for 30.72 Ms/s
      // This is after cyclic prefix
      prach2 = prach+(2*Ncp); // times 2 for complex samples
      if (prach_sequence_length == 0){
        if (prach_fmt_id == 0) { // 24576 samples @ 30.72 Ms/s
          // idft(IDFT_24576,prachF,prach2,1);
          idft(prachF, prach2);

          // here we have | empty  | Prach24576 |
          memmove(prach,prach+(24576<<1),(Ncp<<2));
          // here we have | Prefix | Prach24576 |
          prach_len = 24576+Ncp;
        } else if (prach_fmt_id != 0) { // 24576 samples @ 30.72 Ms/s
          printf("only prach format 0 for mu 0");
          exit(0);
        }
      } else { // short PRACH sequence
        printf("No short PRACH sequence mu 0");
        exit(0);
      }
      break;

    default:
      printf("sample rate %f MHz not supported for numerology %d\n", samples_per_subframe / 1000.0, mu);
      exit(0);
    }
  }


    
  #ifdef NR_PRACH_DEBUG
    printf("PRACH [UE %d] N_RB_UL %d prach_start %d, prach_len %d prach_fmt_id %d\n", Mod_id,
      106,
      prach_start,
      prach_len,
      prach_fmt_id);
  #endif

  for (i=0; i<prach_len; i++) { // prach_len
    ((int16_t*)(&txdata[prach_start]))[2*i] = prach[2*i];
    ((int16_t*)(&txdata[prach_start]))[2*i+1] = prach[2*i+1];
        // printf("%d + j*(%d)\n", ((int16_t *)&txdata[0])[2*i], ((int16_t *)&txdata[0])[2*i+1]);

  }

  #ifdef PRACH_WRITE_OUTPUT_DEBUG
    // LOG_M("tx/prach_tx0.m", "prachtx0", prach+(Ncp<<1), prach_len-Ncp, 1, 1);
    LOG_M("tx/Prach_txsig.m","txs",(int16_t*)(&ue->common_vars.txdata[0][prach_start]), 30720, 1, 1);
  #endif

#endif



    return 89;//signal_energy((int*)prach, 256);

}


int detect_nr_prach(int16_t *txdataF, uint16_t *max_preamble, uint16_t *max_preamble_energy, uint16_t *max_preamble_delay);


void main() { 

    // load_dftslib();
    int subframe_len = 30720;
    int frame_len = subframe_len*10;
    int correct = 0, incorrect = 0;

    int32_t txdata[307200] = {0};
    int16_t *txdataF = (int16_t *)malloc(sizeof(int32_t)*subframe_len);
    if(txdataF == NULL) {
      exit_fun("txdataF");
    }
    uint16_t max_preamble[4]={0},max_preamble_energy[4]={0},max_preamble_delay[4]={0};
    
    
    int preamble_index = 6;
    int prach_start = 30720; // in time domain (0 < prach_start+24576 < frame_len)
    int max_shift = 100;
    
    for (int shift = prach_start; shift < prach_start+max_shift; shift++) {

      printf("Current prach_start = %d shift = %d\n", shift, shift - prach_start);
    
      memset(txdataF, 0, subframe_len*sizeof(int32_t));

      // >>>>>>>>>>>>>>> PRACH GENERATION <<<<<<<<<<<<<<
      generate_nr_prach(txdata, txdataF, 0, 1, preamble_index, 0); // idft is done bellow



      // memset(txdataF, 0, subframe_len*sizeof(int32_t));
      memset(txdata, 0, frame_len*sizeof(int32_t));

      // LOG_M("txdataF_b.m", "txF", txdataF, subframe_len, 0);
      // time domain from nrUE

      idft_30720(txdataF, (int16_t *)&txdata[shift]); // 
      LOG_M("txdata.m", "tx", txdata, frame_len, 0);

      //
      //    Air interface
      //

      // set txdataF buffer to 0 before new dft
      memset(txdataF, 0, subframe_len*sizeof(int32_t));

      // frequency domain at gNB
      dft_30720(txdataF, (int16_t *)&txdata[prach_start]); // extract txdataF from txdata[30720] - first slot (should be from prach_start)

      // LOG_M("txdataF.m", "txF", txdataF, subframe_len, 0);
      

      

      // >>>>>>>>>>>>>>> PRACH DETECTION <<<<<<<<<<<<<<
      detect_nr_prach(&txdataF[16952*2], max_preamble, max_preamble_energy, max_preamble_delay);
      #if 0
        printf("[RAPROC] Frame %d, slot %d, occasion %d (prachStartSymbol %d) : Most likely preamble %d, energy %d.%d dB delay %d (prach_energy counter %d)\n",
            0,
            0,
            0,
            0,
            max_preamble[0],
            max_preamble_energy[0]/10,
            max_preamble_energy[0]%10,
            max_preamble_delay[0],
            0);
      #endif

      printf(">>>>>>>>>>>>>>> PRACH PREAMBLE DETECTED %d %d %d %d <<<<<<<<<<<<<<\n", max_preamble[0], max_preamble[1], max_preamble[2], max_preamble[3]);


      if (max_preamble[0] == preamble_index) {
        printf(">>>>>>>>>>>>>>>>>>>>>>>>> CORRECT!\n");
        correct++;
      } else {
        printf("<<<<<<<<<<<<<<<<<<<<<<< Incorrect!\n");
        incorrect++;
      }
    
    }

    printf(">>>>>>>>>>>>>>> Total: %d samples shift - detected = %d, incorrect = %d\n", max_shift, correct, incorrect);


    free(txdataF);

}

int16_t dB_fixed_times10(uint32_t x)
{
  int16_t dB_power=0;

  if (x==0) {
    dB_power = 0;
  } else if ( (x&0xff000000) != 0 ) {
    dB_power = dB_table_times10[((x>>24)&255)-1];
    dB_power += 3*dB_table_times10[255];
  } else if ( (x&0x00ff0000) != 0 ) {
    dB_power = dB_table_times10[((x>>16)&255)-1];
    dB_power += 2*dB_table_times10[255];
  } else if ( (x&0x0000ff00) != 0 ) {
    dB_power = dB_table_times10[((x>>8)&255)-1];
    dB_power += dB_table_times10[255];
  } else {
    dB_power = dB_table_times10[(x&255)-1];
  }

  if (dB_power > 900)
    return(900);

  return dB_power;
}

int detect_nr_prach(int16_t *txdataF, 
     uint16_t *max_preamble,
		 uint16_t *max_preamble_energy,
		 uint16_t *max_preamble_delay) {

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
	    //    LOG_M("prach_rxF0.m","prach_rxF0",rxsigF[0],N_ZC,1,1);
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
	        //  idft(IDFT_1024,prachF,prach_ifft_tmp,1);
            idft_1024(prachF, prach_ifft_tmp);
	         // compute energy and accumulate over receive antennas
	         for (i=0;i<1024;i++)
	           prach_ifft[i] += (int32_t)prach_ifft_tmp[i<<1]*(int32_t)prach_ifft_tmp[i<<1] + (int32_t)prach_ifft_tmp[1+(i<<1)]*(int32_t)prach_ifft_tmp[1+(i<<1)];
	       } else {
	        //  idft(IDFT_256,prachF,prach_ifft_tmp,1);
	         log2_ifft_size = 8;
           // compute energy and accumulate over receive antennas and repetitions for BR
           for (i=0;i<256;i++)
             prach_ifft[i] += (int32_t)prach_ifft_tmp[i<<1]*(int32_t)prach_ifft_tmp[(i<<1)] + (int32_t)prach_ifft_tmp[1+(i<<1)]*(int32_t)prach_ifft_tmp[1+(i<<1)];
         }

        // if (0) {
        //   if (aa==0) LOG_M("prach_rxF_comp0.m","prach_rxF_comp0",prachF,1024,1,1);
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

