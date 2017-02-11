The model.zip file should contain the following files. 

control.vhd - Pipeline controller

execute.vhd - models the execution stage

decode.vhd  - models the register file and sign extension unit

fetch.vhd - models instruction memory, full 32-bit instructions

memory.vhd - models data memory

pipe_reg1.vhd - pipeline register IF/ID
pipe_reg2.vhd - pipeline register ID/EX
pipe_reg3.vhd - pipeline register EX/MEM
pipe_reg4.vhd - pipeline register MEM/WB


spim_pipe.vhd - the top level model of the complete pipelined datapath