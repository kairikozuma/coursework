/***********************************************************************
 * File         : pipeline.cpp
 * Author       : Soham J. Desai
 * Date         : 14th January 2014
 * Description  : Superscalar Pipeline for Lab2 ECE 6100
 *
 * Version 2.0 (Updates for Solution to Part A)
 * Author       : Kartikay Garg
 * Date         : 1st September 2016
 * Description  : Superscalar Pipeline for Lab2 ECE 6100 [Fall 2016]
 *
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
    p->fetch_cbr_stall = false;

	if(BPRED_POLICY){
      p->b_pred = new BPRED(BPRED_POLICY);

	p->b_pred->stat_num_branches = 0;
	p->b_pred->stat_num_mispred = 0;

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
                printf("\t IF: ");
                break;
            case 1:
                printf("\t ID: ");
                break;
            case 2:
                printf("\t EX: ");
                break;
            case 3:
                printf("\t MA: ");
                break;
            default:
                printf("\t ---- ");
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

void pipe_cycle_WB(Pipeline *p){
  int ii;
  for(ii=0; ii<PIPE_WIDTH; ii++){
    if((p->pipe_latch[MA_LATCH][ii].is_mispred_cbr == true) && (p->pipe_latch[MA_LATCH][ii].valid == 1)){
		p->fetch_cbr_stall = false;
	}
	if(p->pipe_latch[MA_LATCH][ii].valid){
      p->stat_retired_inst++;
      if(p->pipe_latch[MA_LATCH][ii].op_id >= p->halt_op_id){
		p->halt=true;
      }
    }

  }
}

//--------------------------------------------------------------------//

void pipe_cycle_MA(Pipeline *p){
  int ii;
  for(ii=0; ii<PIPE_WIDTH; ii++){
	p->pipe_latch[MA_LATCH][ii]=p->pipe_latch[EX_LATCH][ii];
  }
}

//--------------------------------------------------------------------//

void pipe_cycle_EX(Pipeline *p){
  int ii;
  for(ii=0; ii<PIPE_WIDTH; ii++){
    p->pipe_latch[EX_LATCH][ii]=p->pipe_latch[ID_LATCH][ii];
  }
}

//--------------------------------------------------------------------//

typedef struct dependence_details_tracker{

	public:
	uint8_t op_type;
	uint64_t op_id;
	uint8_t stage;
	void init(Pipeline_Latch);
} Depend_track;

/**********************************************************************
* op_id helps record the id of the instruction with which a dependency is identified
* This shall helkp later identify the dependence of concern in case of multiple dependency scenarios to identify the youngest instructions amongst all elder instructions in pipeline
* Stage helps identify the stage within the pipeline that the instruction (with which a dependency is identified) exists in.
**********************************************************************/
void Depend_track::init(Pipeline_Latch latch_info){
	this->op_type=NUM_OP_TYPE+1;
	this->op_id=0;
	this->stage=0;
}

void pipe_cycle_ID(Pipeline *p){
int ii,jj,aa;
int flag=0;
int exe_passed=0,mem_passed=0,passed=0;
Depend_track dep_track[PIPE_WIDTH][3];
Depend_track dep_fin[3];
/**********************************************************************
* Sources of Stall index:
* op_type:  no stall for NUM_OP_TYPE+1	(since illegal opcodes) | check to prevent code from crashing
* stage:
* 4: stall by MA dependence
* 3: stall by EX dependence
* 2: stall by ID dependence
* dep_type:
* [][0]: stall cause of Src1 need
* [][1]: stall cause of Src2 need
* [][2]: stall cause of Cwrite/read
**********************************************************************/

 /**********************************************************************
 //Forward all instructions from IF to ID irrespective of stalls
 // Stall if any needed to be implemented shall be taken care in ID stage, owing to no progress in IF stage in the urrent cycle
 // While invalidating the entry in ID after copying over to ID from IF, in case of a stall
 // Initialize dependency tracking enteries
 **********************************************************************/
  for(ii=0; ii<PIPE_WIDTH; ii++){
	p->pipe_latch[ID_LATCH][ii]=p->pipe_latch[IF_LATCH][ii];
	dep_track[ii][0].init(p->pipe_latch[ID_LATCH][ii]);
	dep_track[ii][1].init(p->pipe_latch[ID_LATCH][ii]);
	dep_track[ii][2].init(p->pipe_latch[ID_LATCH][ii]);
  }
  for(ii=0; ii<PIPE_WIDTH; ii++){ 			//run Dependency tracking database
	//Check dependencies for only valid instructions in the ID stage [performance optimization]
    if(p->pipe_latch[ID_LATCH][ii].valid == 1){
		flag=0;
		exe_passed=0;mem_passed=0;passed=0;
		/**********************************************************************
		* Dependency checks shall be implemented in this order in order to make sure that the dependence with the youngest instruction amongst all the other elder instructions is recorded
		* MA -> EX -> ID
		* "Op_id" field helps keep track of the youngest, elder instruction
		* Dependencies shall be tracked only due to elder instructions
		**********************************************************************/
		for (jj=0;jj<PIPE_WIDTH;++jj)
		{
			if((p->pipe_latch[MA_LATCH][jj].valid == 1) && (p->pipe_latch[ID_LATCH][ii].tr_entry.src1_reg == p->pipe_latch[MA_LATCH][jj].tr_entry.dest) && (p->pipe_latch[MA_LATCH][jj].tr_entry.dest_needed == 1) && (p->pipe_latch[ID_LATCH][ii].tr_entry.src1_needed == 1) && (p->pipe_latch[ID_LATCH][ii].op_id > p->pipe_latch[MA_LATCH][jj].op_id) ){
				dep_track[ii][0].op_type=((p->pipe_latch[MA_LATCH][jj].op_id) > dep_track[ii][0].op_id) ? (p->pipe_latch[MA_LATCH][jj].tr_entry.op_type) : dep_track[ii][0].op_type ;
				dep_track[ii][0].stage = ((p->pipe_latch[MA_LATCH][jj].op_id) > dep_track[ii][0].op_id) ? 4 : dep_track[ii][0].stage ;
				dep_track[ii][0].op_id = ((p->pipe_latch[MA_LATCH][jj].op_id) > dep_track[ii][0].op_id) ? (p->pipe_latch[MA_LATCH][jj].op_id) : dep_track[ii][0].op_id ;
				flag=1;
				p->pipe_latch[ID_LATCH][ii].valid = 0;
				p->pipe_latch[IF_LATCH][ii].stall = 1;
			}
			if((p->pipe_latch[MA_LATCH][jj].valid == 1) && (p->pipe_latch[ID_LATCH][ii].tr_entry.src2_reg == p->pipe_latch[MA_LATCH][jj].tr_entry.dest) && (p->pipe_latch[MA_LATCH][jj].tr_entry.dest_needed == 1) && (p->pipe_latch[ID_LATCH][ii].tr_entry.src2_needed == 1) && (p->pipe_latch[ID_LATCH][ii].op_id > p->pipe_latch[MA_LATCH][jj].op_id)){
				dep_track[ii][1].op_type=((p->pipe_latch[MA_LATCH][jj].op_id) > dep_track[ii][1].op_id) ? (p->pipe_latch[MA_LATCH][jj].tr_entry.op_type) : dep_track[ii][1].op_type ;
				dep_track[ii][1].stage = ((p->pipe_latch[MA_LATCH][jj].op_id) > dep_track[ii][1].op_id) ? 4 : dep_track[ii][1].stage ;
				dep_track[ii][1].op_id = ((p->pipe_latch[MA_LATCH][jj].op_id) > dep_track[ii][1].op_id) ? (p->pipe_latch[MA_LATCH][jj].op_id) : dep_track[ii][1].op_id ;
				flag=1;
				p->pipe_latch[ID_LATCH][ii].valid = 0;
				p->pipe_latch[IF_LATCH][ii].stall = 1;
			}
			if((p->pipe_latch[MA_LATCH][jj].valid == 1) && (p->pipe_latch[ID_LATCH][ii].tr_entry.cc_read == 1) && (p->pipe_latch[MA_LATCH][jj].tr_entry.cc_write == 1) && (p->pipe_latch[ID_LATCH][ii].op_id > p->pipe_latch[MA_LATCH][jj].op_id) ){
				dep_track[ii][2].stage = ((p->pipe_latch[MA_LATCH][jj].op_id) > dep_track[ii][2].op_id) ? 4 : dep_track[ii][2].stage ;
				dep_track[ii][2].op_type=((p->pipe_latch[MA_LATCH][jj].op_id) > dep_track[ii][2].op_id) ? (p->pipe_latch[MA_LATCH][jj].tr_entry.op_type) : dep_track[ii][2].op_type ;
				dep_track[ii][2].op_id = ((p->pipe_latch[MA_LATCH][jj].op_id) > dep_track[ii][2].op_id) ? (p->pipe_latch[MA_LATCH][jj].op_id) : dep_track[ii][2].op_id ;
				flag=1;
				p->pipe_latch[ID_LATCH][ii].valid = 0;
				p->pipe_latch[IF_LATCH][ii].stall = 1;
			}
			if((p->pipe_latch[EX_LATCH][jj].valid == 1) && (p->pipe_latch[ID_LATCH][ii].tr_entry.src1_reg == p->pipe_latch[EX_LATCH][jj].tr_entry.dest)&& (p->pipe_latch[EX_LATCH][jj].tr_entry.dest_needed == 1) && (p->pipe_latch[ID_LATCH][ii].tr_entry.src1_needed == 1)  && (p->pipe_latch[ID_LATCH][ii].op_id > p->pipe_latch[EX_LATCH][jj].op_id) ){
				dep_track[ii][0].op_type=((p->pipe_latch[EX_LATCH][jj].op_id) > dep_track[ii][0].op_id) ? (p->pipe_latch[EX_LATCH][jj].tr_entry.op_type) : dep_track[ii][0].op_type ;
				dep_track[ii][0].stage = ((p->pipe_latch[EX_LATCH][jj].op_id) > dep_track[ii][0].op_id) ? 3 : dep_track[ii][0].stage ;
				dep_track[ii][0].op_id = ((p->pipe_latch[EX_LATCH][jj].op_id) > dep_track[ii][0].op_id) ? (p->pipe_latch[EX_LATCH][jj].op_id) : dep_track[ii][0].op_id ;
				flag=1;
				p->pipe_latch[ID_LATCH][ii].valid = 0;
				p->pipe_latch[IF_LATCH][ii].stall = 1;
			}
			if((p->pipe_latch[EX_LATCH][jj].valid == 1) && (p->pipe_latch[ID_LATCH][ii].tr_entry.src2_reg == p->pipe_latch[EX_LATCH][jj].tr_entry.dest)&& (p->pipe_latch[EX_LATCH][jj].tr_entry.dest_needed == 1) && (p->pipe_latch[ID_LATCH][ii].tr_entry.src2_needed == 1) && (p->pipe_latch[ID_LATCH][ii].op_id > p->pipe_latch[EX_LATCH][jj].op_id)){
				dep_track[ii][1].op_type=((p->pipe_latch[EX_LATCH][jj].op_id) > dep_track[ii][1].op_id) ? (p->pipe_latch[EX_LATCH][jj].tr_entry.op_type) : dep_track[ii][1].op_type ;
				dep_track[ii][1].stage = ((p->pipe_latch[EX_LATCH][jj].op_id) > dep_track[ii][1].op_id) ? 3 : dep_track[ii][1].stage ;
				dep_track[ii][1].op_id = ((p->pipe_latch[EX_LATCH][jj].op_id) > dep_track[ii][1].op_id) ? (p->pipe_latch[EX_LATCH][jj].op_id) : dep_track[ii][1].op_id ;
				flag=1;
				p->pipe_latch[ID_LATCH][ii].valid = 0;
				p->pipe_latch[IF_LATCH][ii].stall = 1;
			}
			if((p->pipe_latch[EX_LATCH][jj].valid == 1) && (p->pipe_latch[ID_LATCH][ii].tr_entry.cc_read == 1) && (p->pipe_latch[EX_LATCH][jj].tr_entry.cc_write == 1) && (p->pipe_latch[ID_LATCH][ii].op_id > p->pipe_latch[EX_LATCH][jj].op_id) ){
				dep_track[ii][2].op_type=((p->pipe_latch[EX_LATCH][jj].op_id) > dep_track[ii][2].op_id) ? (p->pipe_latch[EX_LATCH][jj].tr_entry.op_type) : dep_track[ii][2].op_type ;
				dep_track[ii][2].stage = ((p->pipe_latch[EX_LATCH][jj].op_id) > dep_track[ii][2].op_id) ? 3 : dep_track[ii][2].stage ;
				dep_track[ii][2].op_id = ((p->pipe_latch[EX_LATCH][jj].op_id) > dep_track[ii][2].op_id) ? (p->pipe_latch[EX_LATCH][jj].op_id) : dep_track[ii][2].op_id ;
				p->pipe_latch[ID_LATCH][ii].valid = 0;
				p->pipe_latch[IF_LATCH][ii].stall = 1;
				flag=1;
			}
			if((p->pipe_latch[ID_LATCH][ii].op_id > p->pipe_latch[ID_LATCH][jj].op_id) && (ii != jj) && ((p->pipe_latch[ID_LATCH][jj].valid == 1) && (p->pipe_latch[ID_LATCH][ii].tr_entry.src1_reg == p->pipe_latch[ID_LATCH][jj].tr_entry.dest) && (p->pipe_latch[ID_LATCH][jj].tr_entry.dest_needed == 1) && (p->pipe_latch[ID_LATCH][ii].tr_entry.src1_needed == 1)) ){
				dep_track[ii][0].op_type=((p->pipe_latch[ID_LATCH][jj].op_id) > dep_track[ii][0].op_id) ? (p->pipe_latch[ID_LATCH][jj].tr_entry.op_type) : dep_track[ii][0].op_type ;
				dep_track[ii][0].stage = ((p->pipe_latch[ID_LATCH][jj].op_id) > dep_track[ii][0].op_id) ? 2 : dep_track[ii][0].stage ;
				dep_track[ii][0].op_id = ((p->pipe_latch[ID_LATCH][jj].op_id) > dep_track[ii][0].op_id) ? (p->pipe_latch[ID_LATCH][jj].op_id) : dep_track[ii][0].op_id ;
				p->pipe_latch[ID_LATCH][ii].valid = 0;
				p->pipe_latch[IF_LATCH][ii].stall = 1;
				flag=1;
			}
			if((p->pipe_latch[ID_LATCH][ii].op_id > p->pipe_latch[ID_LATCH][jj].op_id) && (ii != jj)&& ((p->pipe_latch[ID_LATCH][jj].valid == 1) && (p->pipe_latch[ID_LATCH][ii].tr_entry.src2_reg == p->pipe_latch[ID_LATCH][jj].tr_entry.dest) && (p->pipe_latch[ID_LATCH][jj].tr_entry.dest_needed == 1) && (p->pipe_latch[ID_LATCH][ii].tr_entry.src2_needed == 1))  ){
				dep_track[ii][1].op_type=((p->pipe_latch[ID_LATCH][jj].op_id) > dep_track[ii][1].op_id) ? (p->pipe_latch[ID_LATCH][jj].tr_entry.op_type) : dep_track[ii][1].op_type ;
				dep_track[ii][1].stage = ((p->pipe_latch[ID_LATCH][jj].op_id) > dep_track[ii][1].op_id) ? 2 : dep_track[ii][1].stage ;
				dep_track[ii][1].op_id = ((p->pipe_latch[ID_LATCH][jj].op_id) > dep_track[ii][1].op_id) ? (p->pipe_latch[ID_LATCH][jj].op_id) : dep_track[ii][1].op_id ;
				flag=1;
				p->pipe_latch[ID_LATCH][ii].valid = 0;
				p->pipe_latch[IF_LATCH][ii].stall = 1;
			}
			if((p->pipe_latch[ID_LATCH][ii].op_id > p->pipe_latch[ID_LATCH][jj].op_id) && (ii != jj)&& ((p->pipe_latch[ID_LATCH][jj].valid == 1) && (p->pipe_latch[ID_LATCH][ii].tr_entry.cc_read == 1) && (p->pipe_latch[ID_LATCH][jj].tr_entry.cc_write == 1) )){
				dep_track[ii][2].op_type=((p->pipe_latch[ID_LATCH][jj].op_id) > dep_track[ii][2].op_id) ? (p->pipe_latch[ID_LATCH][jj].tr_entry.op_type) : dep_track[ii][2].op_type ;
				dep_track[ii][2].stage = ((p->pipe_latch[ID_LATCH][jj].op_id) > dep_track[ii][2].op_id) ? 2 : dep_track[ii][2].stage ;
				dep_track[ii][2].op_id = ((p->pipe_latch[ID_LATCH][jj].op_id) > dep_track[ii][2].op_id) ? (p->pipe_latch[ID_LATCH][jj].op_id) : dep_track[ii][2].op_id ;
				flag=1;
				p->pipe_latch[ID_LATCH][ii].valid = 0;
				p->pipe_latch[IF_LATCH][ii].stall = 1;
			}
			if (flag == 0) {
				p->pipe_latch[ID_LATCH][ii].valid = 1;
				p->pipe_latch[IF_LATCH][ii].stall = 0;
			}
		}		//check for Pipeline width wise all elements

		/**********************************************************************
		* Remove stalls if needed for forwarding enabled performance models
		*
		* The idea behind forwarding units is to isolate one's functionality completely from another's.
		* Each forwarding hardware block takes care of dependencies arising due to its owner stage, ignoring dependencies (if any) due to another stage.
		* Thus a forwarding unit can remove a stall iff all the dependencies (if any) due to its owner stage.
	    **********************************************************************/
		if(ENABLE_MEM_FWD){
			if( ( (dep_track[ii][0].stage == 0) || (dep_track[ii][0].stage == 4)) && ( (dep_track[ii][1].stage == 0) || (dep_track[ii][1].stage == 4)) && ( (dep_track[ii][2].stage == 4) || (dep_track[ii][2].stage == 4 ) ) ){
				/**********************************************************************
				* No cc read/write dep or if any, exists solely on satge 4 i.e. MA
				* Either src1 dep doesn't exist or is dep on stage 4 or if it does then dep type instr != LD
				* Either src2 dep doesn't exist or is dep on stage 4 or if it does then dep type instr != LD
				* Dependence is on MA completely, and thus stall can be resolved independently by MA stage
				**********************************************************************/
				p->pipe_latch[ID_LATCH][ii].valid = 1;
				p->pipe_latch[IF_LATCH][ii].stall = 0;
				passed=1;
			}
			mem_passed = 1;
			//Mem_passed is true when forwarding is true irrespective of conditions since any dependence on Mem_access if any is resolved irreespectively
		}

    	if(ENABLE_EXE_FWD){
			if( ( (dep_track[ii][2].stage == 0) || ((dep_track[ii][2].stage == 3) && (dep_track[ii][2].op_type != OP_LD ) )) && ( (dep_track[ii][0].stage == 0) || ((dep_track[ii][0].stage == 3) && (dep_track[ii][0].op_type != OP_LD ) )) && ( (dep_track[ii][1].stage == 0) || ((dep_track[ii][1].stage == 3) && (dep_track[ii][1].op_type != OP_LD ) )) ) {
			// Similar to that in MA stage
			// Stall can be resolved by EX stage independently, since all dependencies (if any) exist completely on EX stage
				p->pipe_latch[ID_LATCH][ii].valid = 1;
				p->pipe_latch[IF_LATCH][ii].stall = 0;
				exe_passed = 1;
				passed=1;
			}
			else if( (( dep_track[ii][2].stage != 3) || ((dep_track[ii][2].stage == 3) && (dep_track[ii][2].op_type != OP_LD ))) && ((dep_track[ii][1].stage != 2) || ((dep_track[ii][1].stage == 3) && (dep_track[ii][1].op_type != OP_LD ))) && ((dep_track[ii][0].stage != 2) || ((dep_track[ii][0].stage == 3) && (dep_track[ii][0].op_type != OP_LD ))) ) {
				exe_passed = 1;
				// "exe_passed" is made true to denote resolutiopn of dependencies arising due to EX stage
			}
			// "exe_passed" cannot be made true in certain cases such as when OP_TYPE is LD and instruction is in EX stage
        }
		/**********************************************************************
		* This block coordinates activity between MEM_FORWARDING unit and the EXE_FORWARDING unit.
		* A stall is resolved only if:
		* 1) If both the blocks signal the resolution of their respective dependencies
		* 2) No other unresolvable dependencies (due to ID stages) is identified
		**********************************************************************/
		if((ENABLE_EXE_FWD>0) && (ENABLE_MEM_FWD>0)){
				if((exe_passed==1) && (mem_passed=1) && (dep_track[ii][0].stage != 2) && (dep_track[ii][1].stage != 2) && ( dep_track[ii][2].stage != 2) ) {
					p->pipe_latch[ID_LATCH][ii].valid = 1;
					p->pipe_latch[IF_LATCH][ii].stall = 0;
					passed = 1;
				}
		}
	} //fetched instrn validity check
  }	//main for for PIPE width ends here

	/**********************************************************************
	* All dependecies have been tracked uptill this point for all instructions within the ID stage.
	* Stalls (owing to dependencies) implemented if necessary.
	*
	* To maintain In-order processor feature. Stall all instructions (in ID and IF) younger to the currently stalled instruction.
	* Stall in IF as well in order to prevent illegal scenarios such as
	* 	IF	ID	EX	MA	WB
	*	6	5	-	3	1
	*	8	7	4	2
	* When I2 <-> I1 , I5 <-> I4  | where <-> denotes a dependency resolvable by EXE forwarding
    **********************************************************************/
	for(ii=0;ii<PIPE_WIDTH;++ii){
		if(p->pipe_latch[IF_LATCH][ii].stall == 1){
			for(jj=0;jj<PIPE_WIDTH;++jj){
				if((p->pipe_latch[ID_LATCH][jj].op_id > p->pipe_latch[ID_LATCH][ii].op_id) ){
					p->pipe_latch[ID_LATCH][jj].valid = 0;
					p->pipe_latch[IF_LATCH][jj].stall = 1;
				}
			}
		}
	}

}

/* Check if branch has been resolved */
bool branch_resolved(Pipeline *p) {
  /* Return true if op_id_mispred_branch is set to 0 */
  if (p->b_pred->op_id_mispred_branch == 0) {
    return true;
  }

  for (int i = 0; i < PIPE_WIDTH; i++) {
    for (int j = 0; j < NUM_LATCH_TYPES; j++) {
      if (p->pipe_latch[j][i].op_id == p->b_pred->op_id_mispred_branch) {
        return false;
      }
    }
  }

  /* Set to op_id_mispred_branch to 0 if not found within pipeline */
  p->b_pred->op_id_mispred_branch = 0;

  /* Return false here, so that op_id_mispred_branch is in WB to start fetching */
  return false;
}

//--------------------------------------------------------------------//
void pipe_cycle_IF(Pipeline *p){
  Pipeline_Latch fetch_op;
  bool tr_read_success;

  if (BPRED_POLICY > 0) {
    /* Check for any cbr stalls */
    p->fetch_cbr_stall = !branch_resolved(p);

    /* Find mispredicted branch within IF stage, only if there is no stall because of another mispredicted branch */
    if (!p->fetch_cbr_stall) {
      int pipe_num = -1; // TODO: Some other value
      for (int i = 0; i < PIPE_WIDTH; i++) {
        if (p->pipe_latch[IF_LATCH][i].is_mispred_cbr) {
          if (pipe_num == -1 || pipe_num > i) {
            pipe_num = i;
          }
        }
      }

      if (pipe_num != -1) {
        p->fetch_cbr_stall = true;
        p->pipe_latch[IF_LATCH][pipe_num].is_mispred_cbr = false;
        p->b_pred->op_id_mispred_branch = p->pipe_latch[IF_LATCH][pipe_num].op_id;
      }
    }

    if (p->fetch_cbr_stall) {
      for (int i = 0; i < PIPE_WIDTH; i++) {
        if (p->pipe_latch[IF_LATCH][i].op_id > p->b_pred->op_id_mispred_branch) {
          p->pipe_latch[IF_LATCH][i].stall = true;
          p->pipe_latch[ID_LATCH][i].valid = false;
        }
      }
    }
  }

  for(int ii=0; ii<PIPE_WIDTH; ii++){
    if(!(p->pipe_latch[IF_LATCH][ii].stall)) {
		// no stall for this pipeline and the fetching continues

			pipe_get_fetch_op(p, &fetch_op);

			if(BPRED_POLICY>0){
				pipe_check_bpred(p, &fetch_op);
			}
			p->pipe_latch[IF_LATCH][ii]=fetch_op;

    }
  }
}


//--------------------------------------------------------------------//

void pipe_check_bpred(Pipeline *p, Pipeline_Latch *fetch_op){

  /* If not a branch, return */
  if (fetch_op->tr_entry.op_type != OP_CBR) {
    return;
  }

  /* Increment branch count */
  p->b_pred->stat_num_branches++;

  // call branch predictor here, if mispred then mark in fetch_op
  bool pred_dir = p->b_pred->GetPrediction(fetch_op->tr_entry.inst_addr);
  bool actu_dir = fetch_op->tr_entry.br_dir;

  // update the predictor instantly
  p->b_pred->UpdatePredictor(fetch_op->tr_entry.inst_addr, actu_dir, pred_dir);

  // stall if mispredicted
  if (actu_dir != pred_dir) {
    fetch_op->is_mispred_cbr = true;
  } else {
    fetch_op->is_mispred_cbr = false;
  }

  /* Incrmeent branch mispredicted count */
  if (actu_dir != pred_dir) {
    p->b_pred->stat_num_mispred++;
  }
}


//--------------------------------------------------------------------//
