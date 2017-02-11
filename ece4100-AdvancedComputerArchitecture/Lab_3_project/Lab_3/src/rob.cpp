#include <stdio.h>
#include <assert.h>

#include "rob.h"


extern int32_t NUM_ROB_ENTRIES;

/////////////////////////////////////////////////////////////
// Init function initializes the ROB
/////////////////////////////////////////////////////////////

ROB* ROB_init(void){
  int ii;
  ROB *t = (ROB *) calloc (1, sizeof (ROB));
  for(ii=0; ii<MAX_ROB_ENTRIES; ii++){
    t->ROB_Entries[ii].valid=false;
    t->ROB_Entries[ii].ready=false;
    t->ROB_Entries[ii].exec=false;
  }
  t->head_ptr=0;
  t->tail_ptr=0;
  return t;
}

/////////////////////////////////////////////////////////////
// Print State
/////////////////////////////////////////////////////////////
void ROB_print_state(ROB *t){
 int ii = 0;
  printf("Printing ROB \n");
  printf("Entry\tInst\tValid\tReady\tExecute\tD->tag\tsrc1->tag\tsrc2->tag\n");
  for(ii = 0; ii < 32; ii++) {
    printf("%5d ::  %d\t", ii, (int)t->ROB_Entries[ii].inst.inst_num);
    printf("%5d\t", t->ROB_Entries[ii].valid);
    printf("%5d\t", t->ROB_Entries[ii].ready);
    printf("%5d\t", t->ROB_Entries[ii].exec);
    printf("%2d->%2d\t", t->ROB_Entries[ii].inst.dest_reg, t->ROB_Entries[ii].inst.dr_tag);
    printf("[%2d] %2d->%2d\t", t->ROB_Entries[ii].inst.src1_ready, t->ROB_Entries[ii].inst.src1_reg, t->ROB_Entries[ii].inst.src1_tag);
    printf("[%2d] %2d->%2d\t", t->ROB_Entries[ii].inst.src2_ready, t->ROB_Entries[ii].inst.src2_reg, t->ROB_Entries[ii].inst.src2_tag);
    /* Print head_ptr and tail_ptr */
    if (ii == t->head_ptr) {
      printf(" <- [H]");
    }
    if (ii == t->tail_ptr) {
      printf(" <- [T]");
    }
    printf("\n");
  }
  printf("\n");
}

/////////////////////////////////////////////////////////////
// If there is space in ROB return true, else false
/////////////////////////////////////////////////////////////

bool ROB_check_space(ROB *t){
  /* No space left if tail_ptr points to valid entry */
  return !(t->ROB_Entries[t->tail_ptr].valid);
}

/////////////////////////////////////////////////////////////
// insert entry at tail, increment tail (do check_space first)
/////////////////////////////////////////////////////////////

int ROB_insert(ROB *t, Inst_Info inst){
  /* If no space, return -1 */
  if (!ROB_check_space(t)) {
    return -1;
  }

  /* Otherwise, insert into ROB */
  t->ROB_Entries[t->tail_ptr].inst = inst;
  t->ROB_Entries[t->tail_ptr].valid = true;
  t->ROB_Entries[t->tail_ptr].exec = false;
  t->ROB_Entries[t->tail_ptr].ready = false;

  /* Keep tag value in temporary variables */
  int tag = t->tail_ptr;

  t->ROB_Entries[t->tail_ptr].inst.dr_tag = tag;

  /* Increment tail_ptr */
  t->tail_ptr = (t->tail_ptr + 1) % NUM_ROB_ENTRIES;

  return tag;
}

/////////////////////////////////////////////////////////////
// When an inst gets scheduled for execution, mark exec
/////////////////////////////////////////////////////////////

void ROB_mark_exec(ROB *t, Inst_Info inst){

  /* Find the entry with the inst value */
  ROB_Entry * rob_entry = &(t->ROB_Entries[inst.dr_tag]);

  /* Mark exec if valid */
  if (rob_entry->valid) {
    rob_entry->exec = true;
  }
}


/////////////////////////////////////////////////////////////
// Once an instruction finishes execution, mark rob entry as done
/////////////////////////////////////////////////////////////

void ROB_mark_ready(ROB *t, Inst_Info inst){

  /* Find the entry with the inst value */
  ROB_Entry * rob_entry = &(t->ROB_Entries[inst.dr_tag]);

  /* Mark exec if valid */
  if (rob_entry->valid && rob_entry->exec) {
    rob_entry->ready = true;
  }
}

/////////////////////////////////////////////////////////////
// Find whether the prf (rob entry) is ready
/////////////////////////////////////////////////////////////

bool ROB_check_ready(ROB *t, int tag){
  if (tag == -1) {
    return true;
  }
  return t->ROB_Entries[tag].valid && t->ROB_Entries[tag].ready;
}


/////////////////////////////////////////////////////////////
// Check if the oldest ROB entry is ready for commit
/////////////////////////////////////////////////////////////

bool ROB_check_head(ROB *t){
  return ROB_check_ready(t, t->head_ptr);
}

/////////////////////////////////////////////////////////////
// For writeback of freshly ready tags, wakeup waiting inst
/////////////////////////////////////////////////////////////

void  ROB_wakeup(ROB *t, int tag){
  /* Initialize index to head pointer */
  int index = t->head_ptr;

  /* If head is invalid, return */
  if (!t->ROB_Entries[index].valid) {
    return;
  }

  /* Iterate from head to tail */
  do {
    /* Set ready to true if tags match */
    if (t->ROB_Entries[index].inst.src1_tag == tag) {
      t->ROB_Entries[index].inst.src1_ready = true;
    }
    if (t->ROB_Entries[index].inst.src2_tag == tag) {
      t->ROB_Entries[index].inst.src2_ready = true;
    }

    /* Increment index circularly */
    index = (index + 1) % NUM_ROB_ENTRIES;
  } while (index != t->tail_ptr);
}


/////////////////////////////////////////////////////////////
// Remove oldest entry from ROB (after ROB_check_head)
/////////////////////////////////////////////////////////////

Inst_Info ROB_remove_head(ROB *t){
  /* Declare empty inst */
  Inst_Info inst;

  /* If ROB is empty, return NULL */
  if (!t->ROB_Entries[t->head_ptr].valid) {
    return inst;
  }

  /* Get instruction at head */
  inst = t->ROB_Entries[t->head_ptr].inst;

  /* Invalidate instruction at head */
  t->ROB_Entries[t->head_ptr].valid = false;
  t->ROB_Entries[t->head_ptr].exec = false;
  t->ROB_Entries[t->head_ptr].ready = false;


  /* Increment head_ptr circularly */
  t->head_ptr = (t->head_ptr + 1) % NUM_ROB_ENTRIES;

  return inst;
}

/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////
