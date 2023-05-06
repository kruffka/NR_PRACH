#ifndef __NR_PRACH_H__
#define __NR_PRACH_H__

#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>

// COMMON TYPES
typedef struct NR_FRAME_PARMS {
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
} NR_FRAME_PARMS;

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

// gNB TYPES
typedef struct PHY_VARS_gNB {

    NR_FRAME_PARMS frame_parms;
    int32_t X_u[64][839];
    int32_t **rxdata;

} PHY_VARS_gNB;

#endif // __NR_PRACH_H__