#include "bpred.h"
#include <stdio.h>
#include <bitset>
#include <iostream>

#define TAKEN   true
#define NOTTAKEN false


/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////

BPRED::BPRED(uint32_t policy) {
  /* Save policy */
  this->policy = (BPRED_TYPE) policy;

  /* Set mispredicted branch op id to 0 */
  op_id_mispred_branch = 0;

  /* If policy is gshare predictor, allocate and initialize GHR and PHT */
  if (policy == BPRED_GSHARE) {
    /* GHR is initially all 0 */
    global_history_register = 0x00000000;

    /* Calculate number of entries in PHT */
    uint64_t num_entries = 1 << 12;

    /* Initialize PHT to weakly taken */
    pattern_history_table = new uint32_t[num_entries];
    for (int i = 0; i < num_entries; i++) {
      pattern_history_table[i] = WEAKLY_TAKEN;
    }
  }
}

/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////

bool BPRED::GetPrediction(uint32_t PC){
  if (policy == BPRED_ALWAYS_TAKEN) {
    return TAKEN;
  } else if (policy == BPRED_GSHARE) {
    /* get index with XOR lower 12 bits of GHR and PC */
    uint32_t index = (global_history_register & 0x0000FFF) ^ ((PC >> 0) & 0x0000FFF);
    uint32_t state = pattern_history_table[index];
    if (state == STRONGLY_TAKEN || state == WEAKLY_TAKEN) {
      return TAKEN;
    } else if (state == STRONGLY_NOT_TAKEN || state == WEAKLY_NOT_TAKEN) {
      return NOTTAKEN;
    }
  }
  /* Default to taken */
  return TAKEN;
}


/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////

void  BPRED::UpdatePredictor(uint32_t PC, bool resolveDir, bool predDir) {
  if (policy == BPRED_GSHARE) {
    /* get index with XOR lower 12 bits of GHR and PC */
    uint32_t index = (global_history_register & 0x0000FFF) ^ ((PC >> 0) & 0x0000FFF);

    /* Update PHT */
    if (resolveDir == TAKEN) {
      pattern_history_table[index] = SatIncrement(pattern_history_table[index], 3);
    } else {
      pattern_history_table[index] = SatDecrement(pattern_history_table[index]);
    }

    /* Update GHR */
    global_history_register = (global_history_register << 1) | resolveDir;
  }
}

/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////
