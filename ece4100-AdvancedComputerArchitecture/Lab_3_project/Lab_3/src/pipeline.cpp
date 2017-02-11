/***********************************************************************
 * File         : pipeline.cpp
 * Author       : Moinuddin K. Qureshi
 * Date         : 19th February 2014
 * Description  : Out of Order Pipeline for Lab3 ECE 6100

 * Update       : Shravan Ramani, Tushar Krishna, 27th Sept, 2015
 **********************************************************************/

#include "pipeline.h"
#include <cstdlib>
#include <cstring>


extern int32_t PIPE_WIDTH;
extern int32_t SCHED_POLICY;
extern int32_t LOAD_EXE_CYCLES;

/**********************************************************************
 * Support Function: Read 1 Trace Record From File and populate Fetch Inst
 **********************************************************************/

void pipe_fetch_inst(Pipeline *p, Pipe_Latch* fe_latch){
    static int halt_fetch = 0;
    uint8_t bytes_read = 0;
    Trace_Rec trace;
    if(halt_fetch != 1) {
      bytes_read = fread(&trace, 1, sizeof(Trace_Rec), p->tr_file);
      Inst_Info *fetch_inst = &(fe_latch->inst);
    // check for end of trace
    // Send out a dummy terminate op
      if( bytes_read < sizeof(Trace_Rec)) {
        p->halt_inst_num=p->inst_num_tracker;
        halt_fetch = 1;
        fe_latch->valid=true;
        fe_latch->inst.dest_reg = -1;
        fe_latch->inst.src1_reg = -1;
        fe_latch->inst.src1_reg = -1;
        fe_latch->inst.inst_num=-1;
        fe_latch->inst.op_type=4;
        return;
      }

    // got an instruction ... hooray!
      fe_latch->valid=true;
      fe_latch->stall=false;
      p->inst_num_tracker++;
      fetch_inst->inst_num=p->inst_num_tracker;
      fetch_inst->op_type=trace.op_type;

      fetch_inst->dest_reg=trace.dest_needed? trace.dest:-1;
      fetch_inst->src1_reg=trace.src1_needed? trace.src1_reg:-1;
      fetch_inst->src2_reg=trace.src2_needed? trace.src2_reg:-1;

      fetch_inst->dr_tag=-1;
      fetch_inst->src1_tag=-1;
      fetch_inst->src2_tag=-1;
      fetch_inst->src1_ready=false;
      fetch_inst->src2_ready=false;
      fetch_inst->exe_wait_cycles=0;
    } else {
      fe_latch->valid = false;
    }
    return;
}


/**********************************************************************
 * Pipeline Class Member Functions
 **********************************************************************/

Pipeline * pipe_init(FILE *tr_file_in){
    printf("\n** PIPELINE IS %d WIDE **\n\n", PIPE_WIDTH);

    // Initialize Pipeline Internals
    Pipeline *p = (Pipeline *) calloc (1, sizeof (Pipeline));

    p->pipe_RAT=RAT_init();
    p->pipe_ROB=ROB_init();
    p->pipe_EXEQ=EXEQ_init();
    p->tr_file = tr_file_in;
    p->halt_inst_num = ((uint64_t)-1) - 3;
    int ii =0;
    for(ii = 0; ii < PIPE_WIDTH; ii++) {  // Loop over No of Pipes
      p->FE_latch[ii].valid = false;
      p->ID_latch[ii].valid = false;
      p->EX_latch[ii].valid = false;
      p->SC_latch[ii].valid = false;
    }
    return p;
}


/**********************************************************************
 * Print the pipeline state (useful for debugging)
 **********************************************************************/

void pipe_print_state(Pipeline *p){
    std::cout << "--------------------------------------------" << std::endl;
    std::cout <<"cycle count : " << p->stat_num_cycle << " retired_instruction : " << p->stat_retired_inst << std::endl;
    uint8_t latch_type_i = 0;
    uint8_t width_i      = 0;
   for(latch_type_i = 0; latch_type_i < 4; latch_type_i++) {
        switch(latch_type_i) {
        case 0:
            printf(" FE: ");
            break;
        case 1:
            printf(" ID: ");
            break;
        case 2:
            printf(" SCH: ");
            break;
        case 3:
            printf(" EX: ");
            break;
        default:
            printf(" -- ");
          }
    }
   printf("\n");
   for(width_i = 0; width_i < PIPE_WIDTH; width_i++) {
       if(p->FE_latch[width_i].valid == true) {
         printf("  %d  ", (int)p->FE_latch[width_i].inst.inst_num);
       } else {
         printf(" --  ");
       }
       if(p->ID_latch[width_i].valid == true) {
         printf("  %d  ", (int)p->ID_latch[width_i].inst.inst_num);
       } else {
         printf(" --  ");
       }
       if(p->SC_latch[width_i].valid == true) {
         printf("  %d  ", (int)p->SC_latch[width_i].inst.inst_num);
       } else {
         printf(" --  ");
       }
       if(p->EX_latch[width_i].valid == true) {
         for(int ii = 0; ii < MAX_WRITEBACKS; ii++) {
            if(p->EX_latch[ii].valid)
	      printf("  %d  ", (int)p->EX_latch[ii].inst.inst_num);
         }
       } else {
         printf(" --  ");
       }
        printf("\n");
     }
     printf("\n");

     RAT_print_state(p->pipe_RAT);
    //  EXEQ_print_state(p->pipe_EXEQ);
     ROB_print_state(p->pipe_ROB);
}


/**********************************************************************
 * Pipeline Main Function: Every cycle, cycle the stage
 **********************************************************************/

void pipe_cycle(Pipeline *p)
{
    p->stat_num_cycle++;

    pipe_cycle_commit(p);
    pipe_cycle_writeback(p);
    pipe_cycle_exe(p);
    pipe_cycle_schedule(p);
    pipe_cycle_issue(p);
    pipe_cycle_decode(p);
    pipe_cycle_fetch(p);

}

//--------------------------------------------------------------------//

void pipe_cycle_fetch(Pipeline *p){
  int ii = 0;
  Pipe_Latch fetch_latch;

  for(ii=0; ii<PIPE_WIDTH; ii++) {
    if((p->FE_latch[ii].stall) || (p->FE_latch[ii].valid)) {   // Stall
        continue;

    } else {  // No Stall and Latch Empty
        pipe_fetch_inst(p, &fetch_latch);
        // copy the op in FE LATCH
        p->FE_latch[ii]=fetch_latch;
    }
  }
}

//--------------------------------------------------------------------//

void pipe_cycle_decode(Pipeline *p){
   int ii = 0;

   int jj = 0;

   static uint64_t start_inst_id = 1;

   // Loop Over ID Latch
   for(ii=0; ii<PIPE_WIDTH; ii++){
     if((p->ID_latch[ii].stall == 1) || (p->ID_latch[ii].valid)) { // Stall
       continue;
     } else {  // No Stall & there is Space in Latch
       for(jj = 0; jj < PIPE_WIDTH; jj++) { // Loop Over FE Latch
         if(p->FE_latch[jj].valid) {
           if(p->FE_latch[jj].inst.inst_num == start_inst_id) { // In Order Inst Found
             p->ID_latch[ii]        = p->FE_latch[jj];
             p->ID_latch[ii].valid  = true;
             p->FE_latch[jj].valid  = false;
             start_inst_id++;
             break;
           }
         }
       }
     }
   }
}

//--------------------------------------------------------------------//

void pipe_cycle_exe(Pipeline *p){

  int ii;
  //If all operations are single cycle, simply copy SC latches to EX latches
  if(LOAD_EXE_CYCLES == 1) {
    for(ii=0; ii<PIPE_WIDTH; ii++){
      if(p->SC_latch[ii].valid) {
        p->EX_latch[ii]=p->SC_latch[ii];
        p->EX_latch[ii].valid = true;
        p->SC_latch[ii].valid = false;
      }
    }
    return;
  }

  //---------Handling exe for multicycle operations is complex, and uses EXEQ

  // All valid entries from SC get into exeq

  for(ii = 0; ii < PIPE_WIDTH; ii++) {
    if(p->SC_latch[ii].valid) {
      EXEQ_insert(p->pipe_EXEQ, p->SC_latch[ii].inst);
      p->SC_latch[ii].valid = false;
    }
  }

  // Cycle the exeq, to reduce wait time for each inst by 1 cycle
  EXEQ_cycle(p->pipe_EXEQ);

  // Transfer all finished entries from EXEQ to EX_latch
  int index = 0;

  while(1) {
    if(EXEQ_check_done(p->pipe_EXEQ)) {
      p->EX_latch[index].valid = true;
      p->EX_latch[index].stall = false;
      p->EX_latch[index].inst  = EXEQ_remove(p->pipe_EXEQ);
      index++;
    } else { // No More Entry in EXEQ
      break;
    }
  }
}



/**********************************************************************
 * -----------  DO NOT MODIFY THE CODE ABOVE THIS LINE ----------------
 **********************************************************************/

void pipe_cycle_issue(Pipeline *p) {

  // insert new instruction(s) into ROB (rename)
  // every cycle up to PIPEWIDTH instructions issued

  for (int i = 0; i < PIPE_WIDTH; i++) {
    /* If latch is valid and there is space, insert into ROB */
    if (p->ID_latch[i].valid && ROB_check_space(p->pipe_ROB)) {

      /* Pointer to instruction */
      Inst_Info * inst = &(p->ID_latch[i].inst);

      /* Remap sources from RAT if needed */
      inst->src1_tag = RAT_get_remap(p->pipe_RAT, inst->src1_reg);
      inst->src1_ready = ROB_check_ready(p->pipe_ROB, inst->src1_tag);

      inst->src2_tag = RAT_get_remap(p->pipe_RAT, inst->src2_reg);
      inst->src2_ready = ROB_check_ready(p->pipe_ROB, inst->src2_tag);

      /* Insert into ROB */
      inst->dr_tag = ROB_insert(p->pipe_ROB, *inst);

      /* Instruction successfully inserted */
      if (inst->dr_tag != -1) {
      	p->ID_latch[i].valid = false;
        RAT_set_remap(p->pipe_RAT, inst->dest_reg, inst->dr_tag);
      }
    }
  }
}

//--------------------------------------------------------------------//
extern uint32_t NUM_ROB_ENTRIES;
void pipe_cycle_schedule(Pipeline *p) {

  // select instruction(s) to Execute
  // every cycle up to PIPEWIDTH instructions scheduled

  /* Inorder scheduling
   * Find all valid entries, if oldest is stalled then stop
   * Else mark it as ready to execute and send to SC_latch
   */

  ROB * rob = p->pipe_ROB;

  if(SCHED_POLICY==0){
    // inorder scheduling
    // Find all valid entries, if oldest is stalled then stop
    // Else mark it as ready to execute and send to SC_latch

    /* If ROB is empty, nothing to schedule */
    if (!rob->ROB_Entries[rob->head_ptr].valid) {
      return;
    }

    for (int i = 0; i < PIPE_WIDTH; i++) {
      /* Skip all instructions currently executing */
      int next_ptr = p->pipe_ROB->head_ptr;
      bool next_ready = false;

      while (!next_ready) {
        next_ready = p->pipe_ROB->ROB_Entries[next_ptr].valid && !p->pipe_ROB->ROB_Entries[next_ptr].exec;

        if (!next_ready) {
          next_ptr = (next_ptr + 1) % NUM_ROB_ENTRIES;
        }

        if (next_ptr == p->pipe_ROB->tail_ptr) {
          break;
        }
      }

      /* Check if sources are ready */
      bool oldest_ready = rob->ROB_Entries[next_ptr].inst.src1_ready && rob->ROB_Entries[next_ptr].inst.src2_ready;

      /* If sources are ready, schedule to execute */
      if (oldest_ready) {
        ROB_mark_exec(rob, rob->ROB_Entries[next_ptr].inst);
        p->SC_latch[i].valid = true;
        p->SC_latch[i].inst = rob->ROB_Entries[next_ptr].inst;
      } else {
        p->SC_latch[i].valid = false;
      }
    }
  }

  /* Out of order scheduling
   * Find valid + src1ready + src2ready + !exec entries in ROB
   * Mark ROB entry as ready to execute  and transfer instruction to SC_latch
   */
  if(SCHED_POLICY==1){
    /* If ROB is empty, return */
    if (!p->pipe_ROB->ROB_Entries[p->pipe_ROB->head_ptr].valid) {
      return;
    }

    /* Attempt to fill pipeline width with instructions */
    for (int i = 0; i< PIPE_WIDTH; i++) {
      /* Check ROB for next available instruction to execute */
      int next_ptr = p->pipe_ROB->head_ptr;
      bool next_ready = false;

      while (!next_ready) {
        next_ready = p->pipe_ROB->ROB_Entries[next_ptr].inst.src1_ready && p->pipe_ROB->ROB_Entries[next_ptr].inst.src2_ready && !p->pipe_ROB->ROB_Entries[next_ptr].exec;

        if (!next_ready) {
          next_ptr = (next_ptr + 1) % NUM_ROB_ENTRIES;
        }

        if (next_ptr == p->pipe_ROB->tail_ptr) {
          break;
        }
      }

      /* If sources are ready, mark execute and pass to SC latch */
      if (next_ready) {
        ROB_mark_exec(p->pipe_ROB, p->pipe_ROB->ROB_Entries[next_ptr].inst);
        p->SC_latch[i].valid = true;
        p->SC_latch[i].inst = p->pipe_ROB->ROB_Entries[next_ptr].inst;
      } else { /* Otherwise, invalidate SC latch */
        p->SC_latch[i].valid = false;
      }
    }
  }
}

//--------------------------------------------------------------------//

void pipe_cycle_writeback(Pipeline *p){

  /* Mark ready in ROB, and call wakeup */
  for(int i = 0; i < MAX_WRITEBACKS; i++) {
    if (p->EX_latch[i].valid) {
      ROB_wakeup(p->pipe_ROB, p->EX_latch[i].inst.dr_tag);
      ROB_mark_ready(p->pipe_ROB, p->EX_latch[i].inst);
    }
  }
}


//--------------------------------------------------------------------//


void pipe_cycle_commit(Pipeline *p) {

  for (int i = 0; i < PIPE_WIDTH; i++) {

    /* If check is ready, commit */
    if (ROB_check_head(p->pipe_ROB)) {

      /* Remove head from ROB */
      Inst_Info inst = ROB_remove_head(p->pipe_ROB);

      /* Halt pipeline if last instruction */
      if (inst.inst_num >= p->halt_inst_num) {
        p->halt = true;
      }

      /* If mapping is relevant, update RAT */
      if (inst.dest_reg != -1 && inst.dr_tag != -1) {
        RAT_reset_entry(p->pipe_RAT, inst.dest_reg);
      }

      /* Increment instruction count */
      p->stat_retired_inst++;
    }
  }
}

//--------------------------------------------------------------------//
