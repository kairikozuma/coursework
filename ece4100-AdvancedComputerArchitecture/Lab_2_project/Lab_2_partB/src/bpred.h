#ifndef _BPRED_H_
#define _BPRED_H_
#include <inttypes.h>
#include <stdlib.h>


static inline uint32_t SatIncrement(uint32_t x, uint32_t max)
{
    if(x < max)
        return x+1;
    return x;
}

static inline uint32_t SatDecrement(uint32_t x)
{
    if(x > 0)
        return x-1;
    return x;
}

typedef enum BPRED_TYPE_ENUM {
    BPRED_PERFECT=0,
    BPRED_ALWAYS_TAKEN=1,
    BPRED_GSHARE=2,
    NUM_BPRED_TYPE=3
} BPRED_TYPE;

typedef enum BPRED_PHT_STATE_ENUM {
  STRONGLY_NOT_TAKEN = 0,
  WEAKLY_NOT_TAKEN = 1,
  WEAKLY_TAKEN = 2,
  STRONGLY_TAKEN = 3
} BPRED_PHT_STATE;

/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////

class BPRED{

public:
  BPRED_TYPE policy;

  uint64_t stat_num_branches;
  uint64_t stat_num_mispred;
  uint64_t op_id_mispred_branch; /* ID of mispredicted branch */

  /* gshare predictor fields */
  uint32_t global_history_register;
  uint32_t * pattern_history_table;

// The interface to the three functions below CAN NOT be changed
    BPRED(uint32_t policy);
    bool GetPrediction(uint32_t PC);
    void UpdatePredictor(uint32_t PC, bool resolveDir, bool predDir);
};

/***********************************************************/
#endif
