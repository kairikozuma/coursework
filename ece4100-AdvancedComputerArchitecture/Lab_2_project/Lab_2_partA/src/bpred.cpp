#include "bpred.h"

#define TAKEN   true
#define NOTTAKEN false

/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////

BPRED::BPRED(uint32_t policy) {

  
}

/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////

bool BPRED::GetPrediction(uint32_t PC){
    return TAKEN;  
}


/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////

void  BPRED::UpdatePredictor(uint32_t PC, bool resolveDir, bool predDir) {


}

/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////
