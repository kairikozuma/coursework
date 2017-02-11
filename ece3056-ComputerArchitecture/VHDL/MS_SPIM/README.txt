This associate archive contains the following files. 

MS_MIPS.VHD	    : The top level SPIM file that instantiates all of
	              the modules listed below and defines their interconnect. 
MS_IFETCH.VHD	    : The instruction fetch module. This module models *BOTH*
		      instruction and data memory as well as program counter
		      management
MS_IDECODE.VHD	    : The instruction decode unit that models the register file
		      access and sign extension
MS_EXECUTE.VHD	    : The execution unit that models the ALU
MS_CONTROL.VHD	    : The control unit that implements the microcode ROM
MS_CLOCK.VHD	    : The clock module that implements the reset signal and 
		      generates the clock for the datapath.