#ifndef __NR_PRACH_H__
#define __NR_PRACH_H__

#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include "tools_defs.h"

static const char *prachfmt[]={"0","1","2","3", "A1","A2","A3","B1","B4","C0","C2","A1/B1","A2/B2","A3/B3"};

//Table 6.3.3.1-3: Mapping from logical index i to sequence number u for preamble formats with L_RA = 839
static const uint16_t prach_root_sequence_map_0_3[838] = {
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
static const uint16_t prach_root_sequence_map_abc[138] = {
1 , 138, 2 , 137, 3 , 136, 4 , 135, 5 , 134, 6 , 133, 7 , 132, 8 , 131, 9 , 130, 10, 129,
11, 128, 12, 127, 13, 126, 14, 125, 15, 124, 16, 123, 17, 122, 18, 121, 19, 120, 20, 119,
21, 118, 22, 117, 23, 116, 24, 115, 25, 114, 26, 113, 27, 112, 28, 111, 29, 110, 30, 109,
31, 108, 32, 107, 33, 106, 34, 105, 35, 104, 36, 103, 37, 102, 38, 101, 39, 100, 40, 99 ,
41, 98 , 42, 97 , 43, 96 , 44, 95 , 45, 94 , 46, 93 , 47, 92 , 48, 91 , 49, 90 , 50, 89 ,
51, 88 , 52, 87 , 53, 86 , 54, 85 , 55, 84 , 56, 83 , 57, 82 , 58, 81 , 59, 80 , 60, 79 ,
61, 78 , 62, 77 , 63, 76 , 64, 75 , 65, 74 , 66, 73 , 67, 72 , 68, 71 , 69, 70
};



// COMMON TYPES
typedef struct NR_FRAME_PARMS NR_FRAME_PARMS;

typedef uint32_t (*get_samples_per_slot_t)(int slot, NR_FRAME_PARMS* fp);
typedef uint32_t (*get_samples_slot_timestamp_t)(int slot, NR_FRAME_PARMS* fp, uint8_t sl_ahead);
struct NR_FRAME_PARMS {
  /// Number of resource blocks (RB) in DL
  int N_RB_DL;
  /// Number of resource blocks (RB) in UL
  int N_RB_UL;
  /// Cell ID
  uint16_t Nid_cell;
  /// subcarrier spacing (15,30,60,120)
  uint32_t subcarrier_spacing;
  /// 3/4 sampling
  uint8_t threequarter_fs;
  /// Size of FFT
  uint16_t ofdm_symbol_size;
  /// Number of prefix samples in all but first symbol of slot
  uint16_t nb_prefix_samples;
  /// Number of prefix samples in first symbol of slot
  uint16_t nb_prefix_samples0;
  /// Carrier offset in FFT buffer for first RE in PRB0
  uint16_t first_carrier_offset;
  /// Number of OFDM/SC-FDMA symbols in one slot
  uint16_t symbols_per_slot;
  /// Number of slots per subframe
  uint16_t slots_per_subframe;
  /// Number of slots per frame
  uint16_t slots_per_frame;
  /// Number of samples in a subframe
  uint32_t samples_per_subframe;
  /// Number of samples in 0th and center slot of a subframe
  uint32_t samples_per_slot0;
  /// Number of samples in other slots of the subframe
  uint32_t samples_per_slotN0;
  /// Number of samples in a radio frame
  uint32_t samples_per_frame;
  /// Number of samples in a subframe without CP
  uint32_t samples_per_subframe_wCP;
  /// Number of samples in a slot without CP
  uint32_t samples_per_slot_wCP;
  /// Number of samples in a radio frame without CP
  uint32_t samples_per_frame_wCP;
  /// NR numerology index [0..5] as specified in 38.211 Section 4 (mu). 0=15khZ SCS, 1=30khZ, 2=60kHz, etc
  uint8_t numerology_index;
  /// Number of Physical transmit antennas in node (corresponds to nrOfAntennaPorts)
  uint8_t nb_antennas_tx;
  /// Number of Receive antennas in node
  uint8_t nb_antennas_rx;
  /// Number of common transmit antenna ports in eNodeB (1 or 2)
  uint8_t nb_antenna_ports_gNB;
  /// Number of samples in current slot
  get_samples_per_slot_t get_samples_per_slot;
  /// Number of samples before slot
  get_samples_slot_timestamp_t get_samples_slot_timestamp;
};

// NR UE TYPES

// fapi L1 <===> L2
typedef struct 
{
  uint8_t  num_prach_fd_occasions;
  uint16_t prach_root_sequence_index;//Starting logical root sequence index, 𝑖, equivalent to higher layer parameter prach-RootSequenceIndex [38.211, sec 6.3.3.1] Value: 0 -> 837
  uint8_t  num_root_sequences;//Number of root sequences for a particular FD occasion that are required to generate the necessary number of preambles
  uint16_t k1;//Frequency offset (from UL bandwidth part) for each FD. [38.211, sec 6.3.3.2] Value: from 0 to 272
  uint8_t  prach_zero_corr_conf;//PRACH Zero CorrelationZone Config which is used to dervive 𝑁𝑐𝑠 [38.211, sec 6.3.3.1] Value: from 0 to 15
  uint8_t  num_unused_root_sequences;//Number of unused sequences available for noise estimation per FD occasion. At least one unused root sequence is required per FD occasion.
  uint8_t* unused_root_sequences_list;//Unused root sequence or sequences per FD occasion. Required for noise estimation.

} fapi_nr_num_prach_fd_occasions_t;

typedef struct 
{
  uint8_t prach_sequence_length;//RACH sequence length. Only short sequence length is supported for FR2. [38.211, sec 6.3.3.1] Value: 0 = Long sequence 1 = Short sequence
  uint8_t prach_sub_c_spacing;//Subcarrier spacing of PRACH. [38.211 sec 4.2] Value:0->4
  uint8_t restricted_set_config;//PRACH restricted set config Value: 0: unrestricted 1: restricted set type A 2: restricted set type B
  uint8_t num_prach_fd_occasions;//Corresponds to the parameter 𝑀 in [38.211, sec 6.3.3.2] which equals the higher layer parameter msg1FDM Value: 1,2,4,8
  fapi_nr_num_prach_fd_occasions_t* num_prach_fd_occasions_list;
  uint8_t ssb_per_rach;//SSB-per-RACH-occasion Value: 0: 1/8 1:1/4, 2:1/2 3:1 4:2 5:4, 6:8 7:16
  uint8_t prach_multiple_carriers_in_a_band;//0 = disabled 1 = enabled

} fapi_nr_prach_config_t;

typedef struct {
//   fapi_nr_ue_carrier_config_t carrier_config;
//   fapi_nr_cell_config_t cell_config;
//   fapi_nr_ssb_config_t ssb_config;
//   fapi_nr_ssb_table_t ssb_table;
//   fapi_nr_tdd_table_t tdd_table;
  fapi_nr_prach_config_t prach_config;

} fapi_nr_config_request_t;

/// This struct replaces:
/// PRACH-ConfigInfo from 38.331 RRC spec
/// PRACH-ConfigSIB or PRACH-Config
typedef struct {
  /// PHY cell ID
  uint16_t phys_cell_id;
  /// Num PRACH occasions
  uint8_t  num_prach_ocas;
  /// PRACH format
  uint8_t  prach_format;
  /// Num RA
  uint8_t  num_ra;
  uint8_t  prach_slot;
  uint8_t  prach_start_symbol;
  /// 38.211 (NCS 38.211 6.3.3.1).
  uint16_t num_cs;
  /// Parameter: prach-rootSequenceIndex, see TS 38.211 (6.3.3.2).
  uint16_t root_seq_id;
  /// Parameter: High-speed-flag, see TS 38.211 (6.3.3.1). 1 corresponds to Restricted set and 0 to Unrestricted set.
  uint8_t  restricted_set;
  /// see TS 38.211 (6.3.3.2).
  uint16_t freq_msg1;
  /// Preamble index for PRACH (0-63)
  uint8_t ra_PreambleIndex;
  /// PRACH TX power (TODO possibly modify to uint)
  int16_t prach_tx_power;
} fapi_nr_ul_config_prach_pdu;

typedef struct {
  int16_t amp;
  bool active;
  fapi_nr_ul_config_prach_pdu prach_pdu;
} NR_UE_PRACH;

// phy vars
typedef struct PHY_VARS_NR_UE {

    NR_FRAME_PARMS frame_parms;
    fapi_nr_config_request_t nrUE_config;
    NR_UE_PRACH prach_vars;
    int32_t X_u[64][839];
    int32_t **txdata;

} PHY_VARS_NR_UE;

int32_t generate_nr_prach(PHY_VARS_NR_UE *ue, int frame, uint8_t slot);

// gNB TYPES
typedef struct PHY_VARS_gNB {

    NR_FRAME_PARMS frame_parms;
    int32_t X_u[64][839];
    int32_t **rxdata;
    int N_TA_offset;
    int **prach_rxsigF[12]; // NUMBER_OF_NR_RU_PRACH_OCCASIONS_MAX

} PHY_VARS_gNB;

int detect_nr_prach(PHY_VARS_gNB *gNB,
                    int slot,
                    uint16_t *max_preamble,
                    uint16_t *max_preamble_energy,
                    uint16_t *max_preamble_delay,
                    uint32_t X_u[64][839]);


#endif // __NR_PRACH_H__