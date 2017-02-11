#include <assert.h>
#include <stdio.h>

#include "rat.h"


/////////////////////////////////////////////////////////////
// Init function initializes the RAT
/////////////////////////////////////////////////////////////

RAT* RAT_init(void){
  int ii;
  RAT *t = (RAT *) calloc (1, sizeof (RAT));
  for(ii=0; ii<MAX_ARF_REGS; ii++){
    t->RAT_Entries[ii].valid=false;
  }
  return t;
}

/////////////////////////////////////////////////////////////
// Print State
/////////////////////////////////////////////////////////////
void RAT_print_state(RAT *t){
 int ii = 0;
  printf("Printing RAT \n");
  printf("Entry  Valid  prf_id\n");
  for(ii = 0; ii < MAX_ARF_REGS; ii++) {
    printf("%5d ::  %d \t\t", ii, t->RAT_Entries[ii].valid);
    printf("%5d \n", (int)t->RAT_Entries[ii].prf_id);
  }
  printf("\n");
}

/////////////////////////////////////////////////////////////
//------- DO NOT CHANGE THE CODE ABOVE THIS LINE -----------
/////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////
// For source registers, we need RAT to provide renamed reg
/////////////////////////////////////////////////////////////

int  RAT_get_remap(RAT *t, int arf_id){
  /* If arf is out of bounds, return -1 */
  if (arf_id < 0 || arf_id >= MAX_ARF_REGS) {
    return -1;
  }

  /* If entry is valid, return corresponding prf id */
  if (t->RAT_Entries[arf_id].valid) {
    return t->RAT_Entries[arf_id].prf_id;
  } else { /* Else, return -1 */
    return -1;
  }
}

/////////////////////////////////////////////////////////////
// For destination regs, we need to remap ARF to PRF
/////////////////////////////////////////////////////////////

void RAT_set_remap(RAT *t, int arf_id, int prf_id){
  /* If arf is out of bounds, return */
  if (arf_id < 0 || arf_id >= MAX_ARF_REGS) {
    return;
  }

  /* Set the prf_id of the arf to the one supplied, and set valid to true */
  t->RAT_Entries[arf_id].prf_id = prf_id;
  t->RAT_Entries[arf_id].valid = true;
}

/////////////////////////////////////////////////////////////
// On commit, we may need to reset RAT information
/////////////////////////////////////////////////////////////

void RAT_reset_entry(RAT *t, int arf_id){
  /* If arf is out of bounds, return */
  if (arf_id < 0 || arf_id >= MAX_ARF_REGS) {
    return;
  }

  /* Invalidate the valid bit */
  t->RAT_Entries[arf_id].valid = false;
  t->RAT_Entries[arf_id].prf_id = -1;
}


/***********************************************************/