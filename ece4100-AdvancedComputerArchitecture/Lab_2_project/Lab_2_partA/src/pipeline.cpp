/***********************************************************************
 * File         : pipeline.cpp
 * Author       : Soham J. Desai
 * Date         : 14th January 2014
 * Description  : Superscalar Pipeline for Lab2 ECE 6100
 **********************************************************************/

#include "pipeline.h"
#include <cstdlib>

extern int32_t PIPE_WIDTH;
extern int32_t ENABLE_MEM_FWD;
extern int32_t ENABLE_EXE_FWD;
extern int32_t BPRED_POLICY;

/**********************************************************************
 * Support Function: Read 1 Trace Record From File and populate Fetch Op
 **********************************************************************/

void pipe_get_fetch_op(Pipeline *p, Pipeline_Latch* fetch_op){
    uint8_t bytes_read = 0;
    bytes_read = fread(&fetch_op->tr_entry, 1, sizeof(Trace_Rec), p->tr_file);

    // check for end of trace
    if( bytes_read < sizeof(Trace_Rec)) {
      fetch_op->valid=false;
      p->halt_op_id=p->op_id_tracker;
      return;
    }

    // got an instruction ... hooray!
    fetch_op->valid=true;
    fetch_op->stall=false;
    fetch_op->is_mispred_cbr=false;
    p->op_id_tracker++;
    fetch_op->op_id=p->op_id_tracker;
    return;
}


/**********************************************************************
 * Pipeline Class Member Functions
 **********************************************************************/

Pipeline * pipe_init(FILE *tr_file_in){
    printf("\n** PIPELINE IS %d WIDE **\n\n", PIPE_WIDTH);

    // Initialize Pipeline Internals
    Pipeline *p = (Pipeline *) calloc (1, sizeof (Pipeline));

    p->tr_file = tr_file_in;
    p->halt_op_id = ((uint64_t)-1) - 3;

    // Allocated Branch Predictor
    if(BPRED_POLICY){
      p->b_pred = new BPRED(BPRED_POLICY);
    }

    return p;
}


/**********************************************************************
 * Print the pipeline state (useful for debugging)
 **********************************************************************/

void pipe_print_state(Pipeline *p){
    std::cout << "--------------------------------------------" << std::endl;
    std::cout <<"cycle count : " << p->stat_num_cycle << " retired_instruction : " << p->stat_retired_inst << std::endl;

    uint8_t latch_type_i = 0;   // Iterates over Latch Types
    uint8_t width_i      = 0;   // Iterates over Pipeline Width
    for(latch_type_i = 0; latch_type_i < NUM_LATCH_TYPES; latch_type_i++) {
        switch(latch_type_i) {
            case 0:
                printf(" IF: ");
                break;
            case 1:
                printf(" ID: ");
                break;
            case 2:
                printf(" EX: ");
                break;
            case 3:
                printf(" MA: ");
                break;
            default:
                printf(" ---- ");
        }
    }
    printf("\n");
    for(width_i = 0; width_i < PIPE_WIDTH; width_i++) {
        for(latch_type_i = 0; latch_type_i < NUM_LATCH_TYPES; latch_type_i++) {
            if(p->pipe_latch[latch_type_i][width_i].valid == true) {
	      printf(" %6u ",(uint32_t)( p->pipe_latch[latch_type_i][width_i].op_id));
            } else {
                printf(" ------ ");
            }
        }
        printf("\n");
    }
    printf("\n");

}


/**********************************************************************
 * Pipeline Main Function: Every cycle, cycle the stage
 **********************************************************************/

void pipe_cycle(Pipeline *p)
{
    p->stat_num_cycle++;

    pipe_cycle_WB(p);
    pipe_cycle_MA(p);
    pipe_cycle_EX(p);
    pipe_cycle_ID(p);
    pipe_cycle_IF(p);

}
/**********************************************************************
 * -----------  DO NOT MODIFY THE CODE ABOVE THIS LINE ----------------
 **********************************************************************/

/* Flag for printing debugging information */
#define DEBUG 1

/* Counter to step through instructions after stopping for checking dependencies */
int countdown = 0;

/**********************************************************************
 * Print out latch information
 **********************************************************************/
 void print_latch(Pipeline_Latch* pl) {
   std::cout << "OP_id:\t\t" << pl->op_id << std::endl
             << "inst_addr: \t" << pl->tr_entry.inst_addr << std::endl
             << "op_type: \t" << (unsigned int) pl->tr_entry.op_type << std::endl
             << "dest:	\t" << (unsigned int) pl->tr_entry.dest << std::endl
             << "dest_needed:	" << (unsigned int) pl->tr_entry.dest_needed << std::endl
             << "src1_reg:	" << (unsigned int) pl->tr_entry.src1_reg << std::endl
             << "src2_reg:	" << (unsigned int) pl->tr_entry.src2_reg << std::endl
             << "src1_needed:	" << (unsigned int) pl->tr_entry.src1_needed << std::endl
             << "src2_needed:	" << (unsigned int) pl->tr_entry.src2_needed << std::endl
             << "cc_read:	" << (unsigned int) pl->tr_entry.cc_read << std::endl
             << "cc_write:	" << (unsigned int) pl->tr_entry.cc_write << std::endl
             << "mem_addr:	" << pl->tr_entry.mem_addr << std::endl
             << "mem_write:	" << (unsigned int) pl->tr_entry.mem_write << std::endl
             << "mem_read:	" << (unsigned int) pl->tr_entry.mem_read << std::endl
             << "br_dir:	\t" << (unsigned int) pl->tr_entry.br_dir << std::endl
             << "br_target:	" << pl->tr_entry.br_target << std::endl << std::endl;
 }
 void print_latch_short(Pipeline_Latch* pl) {
  char * op = (char * ) malloc(sizeof(char) * 5);
  switch (pl->tr_entry.op_type) {
    case OP_ALU:
      strcpy(op,"ALU");
      break;
    case OP_LD:
      strcpy(op,"LD");
      break;
    case OP_ST:
      strcpy(op,"ST");
      break;
    case OP_CBR:
      strcpy(op,"BR");
      break;
    case OP_OTHER:
      strcpy(op,"OTHER");
      break;
  }

  std::cout << "[" << pl->op_id << "] " << op << "["
            << (unsigned int) pl->tr_entry.dest << ":"
            << (unsigned int) pl->tr_entry.dest_needed << "]["
            << (unsigned int) pl->tr_entry.src1_reg << ":"
            << (unsigned int) pl->tr_entry.src1_needed << "]["
            << (unsigned int) pl->tr_entry.src2_reg << ":"
            << (unsigned int) pl->tr_entry.src2_needed << "]{"
            << (unsigned int) pl->tr_entry.cc_read << ":"
            << (unsigned int) pl->tr_entry.cc_write << "}<"
            << pl->tr_entry.mem_addr << ">["
            << (unsigned int) pl->tr_entry.mem_write << ":"
            << (unsigned int) pl->tr_entry.mem_read << "]["
            << (unsigned int) pl->tr_entry.br_dir << ":"
            << pl->tr_entry.br_target << "]" << std::endl;
 }

/**********************************************************************
 * Check if there are dependencies between latches
 **********************************************************************/
bool check_data_dependence(Pipeline_Latch *src_latch, Pipeline_Latch *dest_latch) {

  /* If dest_latch contains instruction same or later than src_latch, return false */
  if (dest_latch->op_id >= src_latch->op_id) {
    return false;
  }

  /* If src_latch or dest_latch is invalid, return false */
  if (!(src_latch->valid) || !(dest_latch->valid)) {
    return false;
  }

  /* Get first latch information */
  bool src1_needed = src_latch->tr_entry.src1_needed;
  bool src2_needed = src_latch->tr_entry.src2_needed;
  uint8_t src1_reg = src_latch->tr_entry.src1_reg;
  uint8_t src2_reg = src_latch->tr_entry.src2_reg;

  /* Get second latch information */
  uint8_t dest_reg = dest_latch->tr_entry.dest;
  bool dest_needed = dest_latch->tr_entry.dest_needed;

  /* Check if any dependencies exist */
  bool has_dependence = dest_needed && ((src1_needed && (dest_reg == src1_reg)) || (src2_needed && (dest_reg == src2_reg)));

  if (has_dependence) {
    /* Print out contents of source and destination latches */
    if (DEBUG) {
        if (dest_latch->tr_entry.op_type == OP_LD) {
        std::cout << "[L]" << src_latch->op_id << ":" << (int) src1_reg << ":" << (int) src1_needed << ":" << (int) src2_reg << ":" << (int) src2_needed << "<--" << dest_latch->op_id << ":" << (int) dest_reg << ":" << dest_needed << std::endl;
        countdown = 3;
        getchar();
      } else {
        std::cout << "[D]" << src_latch->op_id << ":" << (int) src1_reg << ":" << (int) src1_needed << ":" << (int) src2_reg << ":" << (int) src2_needed << "<--" << dest_latch->op_id << ":" << (int) dest_reg << ":" << dest_needed << std::endl;
        countdown = 3;
        getchar();
      }
    }
  }
  return has_dependence;
}

bool check_ctrl_dependence(Pipeline_Latch *src_latch, Pipeline_Latch *dest_latch) {

  /* If dest_latch contains instruction same or later than src_latch, return false */
  if (dest_latch->op_id >= src_latch->op_id) {
    return false;
  }

  /* If src_latch or dest_latch is invalid, return false */
  if (!(src_latch->valid) || !(dest_latch->valid)) {
    return false;
  }

  /* Get later latch information */
  bool cc_read = src_latch->tr_entry.cc_read;

  /* Get earlier latch information */
  bool cc_write = dest_latch->tr_entry.cc_write;

  /* Check if any dependencies exist */
  bool has_dependence = cc_read && cc_write;

  if (has_dependence) {
    if (DEBUG) {
      std::cout << "[C]" << src_latch->op_id << "<--" << dest_latch->op_id << std::endl;
    //   getchar();
    }
  }
  return has_dependence;
}

bool check_same_instruction(Pipeline_Latch *src_latch, Pipeline_Latch *dest_latch) {
  bool same_inst = src_latch->tr_entry.dest == dest_latch->tr_entry.dest &&
  src_latch->tr_entry.dest_needed == dest_latch->tr_entry.dest_needed &&
  src_latch->tr_entry.src1_reg == dest_latch->tr_entry.src1_reg &&
  src_latch->tr_entry.src1_needed == dest_latch->tr_entry.src1_needed &&
  src_latch->tr_entry.src2_reg == dest_latch->tr_entry.src2_reg &&
  src_latch->tr_entry.src2_needed == dest_latch->tr_entry.src2_needed &&
  src_latch->tr_entry.cc_read == dest_latch->tr_entry.cc_read &&
  src_latch->tr_entry.cc_write == dest_latch->tr_entry.cc_write &&
  src_latch->tr_entry.mem_addr == dest_latch->tr_entry.mem_addr &&
  src_latch->tr_entry.mem_write == dest_latch->tr_entry.mem_write &&
  src_latch->tr_entry.mem_read == dest_latch->tr_entry.mem_read &&
  src_latch->tr_entry.br_dir == dest_latch->tr_entry.br_dir &&
  src_latch->tr_entry.br_target == dest_latch->tr_entry.br_target;
  if (same_inst && DEBUG) {
    std::cout << "[S]" << src_latch->op_id << "==" << dest_latch->op_id << std::endl;
  }
  return same_inst;
}

//--------------------------------------------------------------------//
void pipe_cycle_WB(Pipeline *p){
  for(int i = 0; i < PIPE_WIDTH; i++){
    if(p->pipe_latch[MA_LATCH][i].valid){
      p->stat_retired_inst++;
      if (DEBUG) {
        print_latch_short(&(p->pipe_latch[MA_LATCH][i]));
      }
      if(p->pipe_latch[MA_LATCH][i].op_id >= p->halt_op_id){
	       p->halt=true;
      }
    }
  }
  // std::cout << std::endl;
}

//--------------------------------------------------------------------//

void pipe_cycle_MA(Pipeline *p){
  for(int i = 0; i < PIPE_WIDTH; i++){
    p->pipe_latch[MA_LATCH][i]=p->pipe_latch[EX_LATCH][i];
  }
}

//--------------------------------------------------------------------//

void pipe_cycle_EX(Pipeline *p){
  for(int i = 0; i < PIPE_WIDTH; i++){
    p->pipe_latch[EX_LATCH][i]=p->pipe_latch[ID_LATCH][i];
  }
}

//--------------------------------------------------------------------//
void pipe_cycle_ID(Pipeline *p){
  /* Flag for hazards */
  bool* data_flag = (bool *) malloc(sizeof(bool) * PIPE_WIDTH);
  bool* ctrl_flag = (bool *) malloc(sizeof(bool) * PIPE_WIDTH);
  bool ctrl_flag_pipeline = false;

  /* Flag for control hazard */
  unsigned int op_id_dep = 0x0;

  for(int i=0; i < PIPE_WIDTH; i++){

    /* Declare pointer to if and id latch */
    Pipeline_Latch *if_pl = &(p->pipe_latch[IF_LATCH][i]);

    /* Dependency flags */
    bool id_data_dep = false;
    bool ex_data_dep = false;
    bool ma_data_dep = false;
    bool id_ctrl_dep = false;
    bool ex_ctrl_dep = false;
    bool ma_ctrl_dep = false;

    /* Iterate through all latches and check for dependencies */
    int jj = 0;
    while (jj < PIPE_WIDTH) {

      /* Check for dependencies from other IF/ID latches */
      id_data_dep = check_data_dependence(if_pl, &(p->pipe_latch[IF_LATCH][jj]));
      id_ctrl_dep = check_ctrl_dependence(if_pl, &(p->pipe_latch[IF_LATCH][jj]));

      /* Check for dependencies from MA stage (in MA/WB latch because already latched) */
      if(!ENABLE_MEM_FWD) {
        ma_data_dep = check_data_dependence(if_pl, &(p->pipe_latch[MA_LATCH][jj]));
        ma_ctrl_dep = check_ctrl_dependence(if_pl, &(p->pipe_latch[MA_LATCH][jj]));
      }

      /* Check for dependencies from EX stage (in EX/MA latch because already latched) */
      if(!ENABLE_EXE_FWD){
        ex_data_dep = check_data_dependence(if_pl, &(p->pipe_latch[EX_LATCH][jj]));
        ex_ctrl_dep = check_ctrl_dependence(if_pl, &(p->pipe_latch[EX_LATCH][jj]));
      } else {
        /* Only if not a load instruction and not a store */
        if (/*if_pl->tr_entry.op_type != OP_ST && if_pl->tr_entry.op_type != OP_LD &&*/  p->pipe_latch[EX_LATCH][jj].tr_entry.op_type == OP_LD) {
          // ex_ctrl_dep = check_ctrl_dependence(if_pl, &(p->pipe_latch[EX_LATCH][jj]));
          ex_data_dep = check_data_dependence(if_pl, &(p->pipe_latch[EX_LATCH][jj]));
          if (ex_data_dep) {
            Pipeline_Latch * pl = &(p->pipe_latch[EX_LATCH][jj]);
            int kk = 0;
            bool flag = false;
            while (ex_data_dep && kk < PIPE_WIDTH) {
              if (pl->op_id == p->pipe_latch[MA_LATCH][kk].op_id + 1 && check_same_instruction(pl, &(p->pipe_latch[MA_LATCH][kk]))) {
                ex_data_dep = false;
              }
              kk++;
            }
          }
        }
      }

      /* Set flag as true if data dependence found */
      if (ma_data_dep || ex_data_dep || id_data_dep) {
        data_flag[i] = true;
      }

      /* Set flag as true if control dependence found */
      if (ma_ctrl_dep || ex_ctrl_dep || id_ctrl_dep) {
        ctrl_flag[i] = true;
        ctrl_flag_pipeline = true;

        /* Get latest instruction that is stalled due to unresolved branch */
        if (op_id_dep < if_pl->op_id) {
          op_id_dep = if_pl->op_id;
        }
      }
      jj++;
    }
  }

  /* If stall needed, stall the IF, ID stage and insert nop at ID latch */
  for(int i = 0; i < PIPE_WIDTH; i++){
    if (data_flag[i] || ctrl_flag[i] || (ctrl_flag_pipeline && (p->pipe_latch[IF_LATCH][i].op_id >= op_id_dep))) {
      p->pipe_latch[ID_LATCH][i].stall = true;  // Stall ID/EX latch
      p->pipe_latch[ID_LATCH][i].valid = false; // Insert no op
      p->pipe_latch[IF_LATCH][i].stall = true;  // Stall IF/ID latch
    } else { /* Otherwise, disable stalls and set valid to true for ID latch */
      p->pipe_latch[ID_LATCH][i].stall = false;  // Unstall ID/EX latch
      p->pipe_latch[ID_LATCH][i].valid = true; // Remove no op
      p->pipe_latch[IF_LATCH][i].stall = false;  // Unstall IF/ID latch
    }
  }

  /* Free allocated memory */
  free(data_flag);
  free(ctrl_flag);

  for(int i = 0; i < PIPE_WIDTH; i++){
    if(p->pipe_latch[ID_LATCH][i].stall) {
      continue;
    } else {
      p->pipe_latch[ID_LATCH][i]=p->pipe_latch[IF_LATCH][i]; // Latch next instruction
    }
  }
}

//--------------------------------------------------------------------//

void pipe_cycle_IF(Pipeline *p){
  Pipeline_Latch fetch_op;
  bool tr_read_success;

  for(int i = 0; i < PIPE_WIDTH; i++){

    if (p->pipe_latch[IF_LATCH][i].stall) {
      continue;
    }

    pipe_get_fetch_op(p, &fetch_op);

    if(BPRED_POLICY){
      pipe_check_bpred(p, &fetch_op);
    }

    // copy the op in IF LATCH
    p->pipe_latch[IF_LATCH][i]=fetch_op;
  }

  /* Print out pipeline information when all stages finished */
  if (DEBUG) {
      pipe_print_state(p);
    if (countdown != 0) {
      getchar();
      countdown--;
    }
    // getchar();
  }

  // if (p->stat_num_cycle > 500) {
  //   p->halt = true;
  // }
}


//--------------------------------------------------------------------//

void pipe_check_bpred(Pipeline *p, Pipeline_Latch *fetch_op){
  // call branch predictor here, if mispred then mark in fetch_op
  // update the predictor instantly
  // stall fetch using the flag p->fetch_cbr_stall
}


//--------------------------------------------------------------------//
