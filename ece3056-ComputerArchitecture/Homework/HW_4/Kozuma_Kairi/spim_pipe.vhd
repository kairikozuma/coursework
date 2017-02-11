-- Kairi Kozuma
-- GTID: 903050898
-- ECE 3056 SPRING 2016
--
-- Sudhakar Yalamanchili
-- Pipelined MIPS Processor VHDL Behavioral Mode--
--
--  Top level SPIM module
--
-- A few things to note. In this pipelined model, the mux in the last stage has
-- been removed and the signals have been run directly into the
-- decode module. This means the choice of operand
-- to store (whether from memory or the ALU output must be made inside of
-- the decode module. Hence the MemtoReg control signal must also be
-- provided to the decode module.
--
-- As for signal naming conventions, all the signals used to connect the
-- modules are prefixed with if_, id_, ex_,  mem_ or wb_ depending on
-- the stage in which the signal is used (note: if a signal is generated in WB and
-- used in fetch it will be prefaced with wb_).
--
-- Changes:
-- Declare and connect signals for rt and rs values from decode
-- Add forwarding unit
--  Declare component forwarding unit
--  Instantiate forwarding unit
-- Add stalling unit
--  Declare component stalling unit
--  Instantiate stalling unit
--  Replace clock with clock_if_id in IFE and if_id
--  Connect stall_or_reset signal to reset input in id_ex
LIBRARY IEEE;
USE IEEE.STD_LOGIC_1164.ALL;
USE IEEE.STD_LOGIC_ARITH.ALL;
USE IEEE.STD_LOGIC_SIGNED.ALL;

entity spim_pipe is
PORT(
    Out_ID_Instr        : OUT STD_LOGIC_VECTOR(31 DOWNTO 0);
    Out_EX_Rs           : OUT STD_LOGIC_VECTOR(31 DOWNTO 0);
    Out_EX_Rt           : OUT STD_LOGIC_VECTOR(31 DOWNTO 0);
    Out_WB_MemOut       : OUT STD_LOGIC_VECTOR(31 DOWNTO 0);
    Out_MEM_ALURes      : OUT STD_LOGIC_VECTOR(31 DOWNTO 0);
    Out_MEM_MemInData   : OUT STD_LOGIC_VECTOR(31 DOWNTO 0);
    Out_WB_ALU_Result   : OUT STD_LOGIC_VECTOR(31 DOWNTO 0);
    Out_ID_PC           : OUT STD_LOGIC_VECTOR(31 DOWNTO 0);
    Out_EX_ALUOp        : OUT STD_LOGIC_VECTOR( 1 DOWNTO 0);
    Out_WB_WReg         : OUT STD_LOGIC_VECTOR( 4 DOWNTO 0);
    Out_WB_RegWrite     : OUT STD_LOGIC;
    Out_EX_ALUSrc       : OUT STD_LOGIC;
    Out_EX_RegDst       : OUT STD_LOGIC;
    Out_WB_MemToReg     : OUT STD_LOGIC;
    Out_MEM_MRead       : OUT STD_LOGIC;
    Out_MEM_MWrite      : OUT STD_LOGIC
    );
END spim_pipe;

ARCHITECTURE structural OF spim_pipe is
-- clock and reset generation

COMPONENT my_clock is
PORT(
    SIGNAL sys_clock, reset : OUT STD_LOGIC
    );
END COMPONENT my_clock;

-- Instruction fetch unit
COMPONENT fetch
PORT(--INPUT SIGNALS
    Branch_PC       : IN STD_LOGIC_VECTOR(31 DOWNTO 0);
    clock, reset    : IN STD_LOGIC;
    PCSource        : IN STD_LOGIC;
    --OUTPUT signals
    instruction     : OUT STD_LOGIC_VECTOR(31 DOWNTO 0);
    PC_out          : OUT STD_LOGIC_VECTOR(31 DOWNTO 0)
    );
END component;

-- Instruction decode
-- Add rs_reg_address and rt_reg_address as outputs
COMPONENT decode
PORT(--INPUT SIGNALS
    instruction     : IN STD_LOGIC_VECTOR(31 DOWNTO 0);
    memory_data     : IN STD_LOGIC_VECTOR(31 DOWNTO 0);
    alu_result      : IN STD_LOGIC_VECTOR(31 DOWNTO 0);
    RegWrite        : IN STD_LOGIC;
    MemToReg        : IN STD_LOGIC;
    reset           : IN STD_LOGIC;
    wreg_address    : IN STD_LOGIC_VECTOR( 4 DOWNTO 0);
    --OUTPUT SIGNALS
    rs_reg_address  : OUT STD_LOGIC_VECTOR( 4 DOWNTO 0);
    rt_reg_address  : OUT STD_LOGIC_VECTOR( 4 DOWNTO 0);
    wreg_rd         : OUT STD_LOGIC_VECTOR( 4 DOWNTO 0);
    wreg_rt         : OUT STD_LOGIC_VECTOR( 4 DOWNTO 0);
    register_rs     : OUT STD_LOGIC_VECTOR(31 DOWNTO 0);
    register_rt     : OUT STD_LOGIC_VECTOR(31 DOWNTO 0);
    Sign_extend     : OUT STD_LOGIC_VECTOR(31 DOWNTO 0));
END component;

-- Execution unit
COMPONENT execute
PORT(--INPUT SIGNALS
    PC4             : IN STD_LOGIC_VECTOR(31 DOWNTO 0);
    register_rs     : IN STD_LOGIC_VECTOR(31 DOWNTO 0);
    register_rt     : IN STD_LOGIC_VECTOR(31 DOWNTO 0);
    Sign_extend     : IN STD_LOGIC_VECTOR(31 DOWNTO 0);
    ALUOp           : IN STD_LOGIC_VECTOR(1 DOWNTO 0);
    ALUSrc          : IN STD_LOGIC;
    RegDst          : IN STD_LOGIC;
    wreg_rd         : IN STD_LOGIC_VECTOR(4 DOWNTO 0);
    wreg_rt         : IN STD_LOGIC_VECTOR(4 DOWNTO 0);
    --OUTPUT SIGNALS
    alu_result      : OUT STD_LOGIC_VECTOR(31 DOWNTO 0);
    branch_PC       : OUT STD_LOGIC_VECTOR(31 DOWNTO 0);
    wreg_address    : OUT STD_LOGIC_VECTOR(4 DOWNTO 0);
    zero            : OUT STD_LOGIC
    );
END COMPONENT;

-- Data Memory
COMPONENT memory
PORT(--INPUT SIGNALS
    address     : IN STD_LOGIC_VECTOR(31 DOWNTO 0);
    write_data  : IN STD_LOGIC_VECTOR(31 DOWNTO 0);
    MemWrite    : IN STD_LOGIC;
    MemRead     : IN STD_LOGIC;
    --OUTPUT SIGNALS
    read_data   : OUT STD_LOGIC_VECTOR(31 DOWNTO 0));
END COMPONENT;

-- Control
COMPONENT control
PORT(--INPUT SIGNALS
    opcode      : IN STD_LOGIC_VECTOR(5 DOWNTO 0);
    --OUTPUT SIGNALS
    RegDst      : OUT STD_LOGIC;
    MemRead     : OUT STD_LOGIC;
    MemToReg    : OUT STD_LOGIC;
    MemWrite    : OUT STD_LOGIC;
    Branch      : OUT STD_LOGIC;
    ALUSrc      : OUT STD_LOGIC;
    RegWrite    : OUT STD_LOGIC;
    ALUOp       : OUT STD_LOGIC_VECTOR(1 DOWNTO 0)
    );
END COMPONENT;

-- pipeline register IF/ID
COMPONENT pipe_reg1
PORT(--INPUT SIGNALS
    if_PC4          : IN STD_LOGIC_VECTOR(31 DOWNTO 0);
    if_instruction  : IN STD_LOGIC_VECTOR(31 DOWNTO 0);
    clk, reset      : IN STD_LOGIC;
    --OUTPUT SIGNALS
    id_PC4          : OUT STD_LOGIC_VECTOR(31 DOWNTO 0);
    id_instruction  : OUT STD_LOGIC_VECTOR(31 DOWNTO 0)
    );
END COMPONENT;

-- pipeline register ID/EX
-- Add inputs id_rs_address and id_rt_address
-- Add ouputs ex_rs_address and ex_rt_address
COMPONENT pipe_reg2
PORT(--INPUT SIGNALS
    id_MemToReg     : IN STD_LOGIC;
    id_RegWrite     : IN STD_LOGIC;
    id_MemWrite     : IN STD_LOGIC;
    id_MemRead      : IN STD_LOGIC;
    id_ALUSrc       : IN STD_LOGIC;
    id_RegDst       : IN STD_LOGIC;
    clk, reset      : IN STD_LOGIC;
    id_branch       : IN STD_LOGIC;
    id_ALUOp        : IN STD_LOGIC_VECTOR( 1 DOWNTO 0);
    id_PC4          : IN STD_LOGIC_VECTOR(31 DOWNTO 0);
    id_register_rs  : IN STD_LOGIC_VECTOR(31 DOWNTO 0);
    id_register_rt  : IN STD_LOGIC_VECTOR(31 DOWNTO 0);
    id_sign_extend  : IN STD_LOGIC_VECTOR(31 DOWNTO 0);
    id_rs_address   : IN STD_LOGIC_VECTOR( 4 DOWNTO 0);
    id_rt_address   : IN STD_LOGIC_VECTOR( 4 DOWNTO 0);
    id_wreg_rd      : IN STD_LOGIC_VECTOR( 4 DOWNTO 0);
    id_wreg_rt      : IN STD_LOGIC_VECTOR( 4 DOWNTO 0);
    --OUTPUT SIGNALS
    ex_MemToReg     : OUT STD_LOGIC;
    ex_RegWrite     : OUT STD_LOGIC;
    ex_MemWrite     : OUT STD_LOGIC;
    ex_MemRead      : OUT STD_LOGIC;
    ex_branch       : OUT STD_LOGIC;
    ex_ALUSrc       : OUT STD_LOGIC;
    ex_RegDst       : OUT STD_LOGIC;
    ex_ALUOp        : OUT STD_LOGIC_VECTOR( 1 DOWNTO 0);
    ex_PC4          : OUT STD_LOGIC_VECTOR(31 DOWNTO 0);
    ex_register_rs  : OUT STD_LOGIC_VECTOR(31 DOWNTO 0);
    ex_register_rt  : OUT STD_LOGIC_VECTOR(31 DOWNTO 0);
    ex_sign_extend  : OUT STD_LOGIC_VECTOR(31 DOWNTO 0);
    ex_rs_address   : OUT STD_LOGIC_VECTOR( 4 DOWNTO 0);
    ex_rt_address   : OUT STD_LOGIC_VECTOR( 4 DOWNTO 0);
    ex_wreg_rd      : OUT STD_LOGIC_VECTOR( 4 DOWNTO 0);
    ex_wreg_rt      : OUT STD_LOGIC_VECTOR( 4 DOWNTO 0));
END COMPONENT;

-- pipeline register EX/MEM
COMPONENT pipe_reg3
PORT(--INPUT SIGNALS
    ex_MemToReg     : IN STD_LOGIC;
    ex_RegWrite     : IN STD_LOGIC;
    ex_MemWrite     : IN STD_LOGIC;
    ex_MemRead      : IN STD_LOGIC;
    ex_branch       : IN STD_LOGIC;
    ex_zero         : IN STD_LOGIC;
    ex_alu_result   : IN STD_LOGIC_VECTOR(31 DOWNTO 0);
    ex_register_rt  : IN STD_LOGIC_VECTOR(31 DOWNTO 0);
    ex_branch_PC    : IN STD_LOGIC_VECTOR(31 DOWNTO 0);
    ex_wreg_addr    : IN STD_LOGIC_VECTOR(4 DOWNTO 0);
    clk, reset      : IN STD_LOGIC;
    --OUTPUT SIGNALS
    mem_MemToReg    : OUT STD_LOGIC;
    mem_RegWrite    : OUT STD_LOGIC;
    mem_MemWrite    : OUT STD_LOGIC;
    mem_MemRead     : OUT STD_LOGIC;
    mem_branch      : OUT STD_LOGIC;
    mem_zero        : OUT STD_LOGIC;
    mem_alu_result  : OUT STD_LOGIC_VECTOR(31 DOWNTO 0);
    mem_register_rt : OUT STD_LOGIC_VECTOR(31 DOWNTO 0);
    mem_branch_PC   : OUT STD_LOGIC_VECTOR(31 DOWNTO 0);
    mem_wreg_addr   : OUT STD_LOGIC_VECTOR(4 DOWNTO 0)
    );
END COMPONENT;

--
-- pipeline register MEM/WB
--
COMPONENT pipe_reg4
PORT(--INPUT SIGNALS
    mem_MemToReg        : IN STD_LOGIC;
    mem_RegWrite        : IN STD_LOGIC;
    mem_memory_data     : IN STD_LOGIC_VECTOR(31 DOWNTO 0);
    mem_alu_result      : IN STD_LOGIC_VECTOR(31 DOWNTO 0);
    mem_wreg_addr       : IN STD_LOGIC_VECTOR( 4 DOWNTO 0);
    clk,reset           : IN STD_LOGIC;
    --OUTPUT SIGNALS
    wb_MemToReg         : OUT STD_LOGIC;
    wb_RegWrite         : OUT STD_LOGIC;
    wb_memory_data      : OUT STD_LOGIC_VECTOR(31 DOWNTO 0);
    wb_alu_result       : OUT STD_LOGIC_VECTOR(31 DOWNTO 0);
    wb_wreg_addr        : OUT STD_LOGIC_VECTOR( 4 DOWNTO 0)
    );
END COMPONENT;

-- Forwarding Unit
COMPONENT forwarding_unit
PORT(--INPUT SIGNALS
    ex_rs_address       : IN STD_LOGIC_VECTOR( 4 DOWNTO 0);
    ex_rt_address       : IN STD_LOGIC_VECTOR( 4 DOWNTO 0);
    ex_register_rs      : IN STD_LOGIC_VECTOR(31 DOWNTO 0);
    ex_register_rt      : IN STD_LOGIC_VECTOR(31 DOWNTO 0);
    mem_RegWrite        : IN STD_LOGIC;
    mem_alu_result      : IN STD_LOGIC_VECTOR(31 DOWNTO 0);
    mem_wreg_addr       : IN STD_LOGIC_VECTOR( 4 DOWNTO 0);
    wb_RegWrite         : IN STD_LOGIC;
    wb_alu_result       : IN STD_LOGIC_VECTOR(31 DOWNTO 0);
    wb_memory_data      : IN STD_LOGIC_VECTOR(31 DOWNTO 0);
    wb_wreg_addr        : IN STD_LOGIC_VECTOR( 4 DOWNTO 0);
    wb_MemToReg         : IN STD_LOGIC;
    --OUTPUT SIGNALS
    fw_register_rs      : OUT STD_LOGIC_VECTOR(31 DOWNTO 0);
    fw_register_rt      : OUT STD_LOGIC_VECTOR(31 DOWNTO 0)
    );
END COMPONENT;

-- Stalling Unit
COMPONENT stalling_unit
PORT(--INPUT SIGNALS
    clock               : IN STD_LOGIC;
    reset               : IN STD_LOGIC;
    id_rs_address       : IN STD_LOGIC_VECTOR( 4 DOWNTO 0);
    id_rt_address       : IN STD_LOGIC_VECTOR( 4 DOWNTO 0);
    ex_memread          : IN STD_LOGIC;
    ex_wreg_addr        : IN STD_LOGIC_VECTOR( 4 DOWNTO 0);
    stall_or_reset      : OUT STD_LOGIC;
    clock_if_id         : OUT STD_LOGIC
    );
END COMPONENT;

-- declare local signals to connect ports of components in each stage

-- IF
SIGNAL if_PC4 : std_logic_vector (31 DOWNTO 0);
SIGNAL if_instruction: std_logic_vector(31 DOWNTO 0);

-- ID
-- Add signals id_rs_address and id_rt_address
SIGNAL id_PC4  : std_logic_vector (31 DOWNTO 0);
SIGNAL id_instruction : std_logic_vector(31 DOWNTO 0);
SIGNAL id_RegDst,id_MemRead, id_MemWrite,id_ALUSrc, id_Branch : std_logic;
SIGNAL id_MemToReg, id_RegWrite : std_logic ;
SIGNAL id_ALUOp : std_logic_vector(1 DOWNTO 0);
SIGNAL id_register_rs, id_register_rt :std_logic_vector(31 DOWNTO 0);
SIGNAL id_Sign_extend :std_logic_vector(31 DOWNTO 0);
SIGNAL id_rs_address, id_rt_address : std_logic_vector(4 DOWNTO 0);
SIGNAL id_wreg_rd, id_wreg_rt : std_logic_vector(4 DOWNTO 0);

-- EX
-- Add signals ex_rs_address and ex_rt_address
SIGNAL ex_MemToReg, ex_RegWrite, ex_MemWrite, ex_MemRead, ex_ALUSrc, ex_zero:std_logic;
SIGNAL ex_ALUOp :std_logic_vector(1 DOWNTO 0);
SIGNAL ex_RegDst, ex_branch: std_logic;
SIGNAL ex_PC4, ex_branch_PC : std_logic_vector(31 DOWNTO 0);
SIGNAL ex_rs_address, ex_rt_address : std_logic_vector(4 DOWNTO 0);
SIGNAL ex_register_rs, ex_register_rt, ex_sign_extend:std_logic_vector(31 DOWNTO 0);
SIGNAL ex_wreg_rd, ex_wreg_rt, ex_wreg_addr: std_logic_vector(4 DOWNTO 0);
SIGNAL ex_alu_result :std_logic_vector(31 DOWNTO 0);

-- MEM
SIGNAL mem_MemToReg, mem_RegWrite, mem_MemWrite, mem_MemRead, mem_zero : STD_LOGIC;
SIGNAL mem_alu_result, mem_write_data, mem_memory_data, mem_Branch_PC : STD_LOGIC_VECTOR(31 DOWNTO 0);
SIGNAL mem_wreg_addr : STD_LOGIC_VECTOR(4 DOWNTO 0);
SIGNAL mem_PCSource, mem_branch : STD_LOGIC;

-- WB
SIGNAL wb_MemToReg, wb_RegWrite :STD_LOGIC;
SIGNAL wb_memory_data, wb_alu_result: STD_LOGIC_VECTOR(31 DOWNTO 0);
SIGNAL wb_wreg_addr: STD_LOGIC_VECTOR(4 DOWNTO 0);

-- FW
SIGNAL fw_register_rs : STD_LOGIC_VECTOR(31 DOWNTO 0);
SIGNAL fw_register_rt : STD_LOGIC_VECTOR(31 DOWNTO 0);

-- global signals
SIGNAL clock, reset : STD_LOGIC;
SIGNAL clock_if_id: STD_LOGIC;
SIGNAL stall_or_reset: STD_LOGIC;

BEGIN
-- identify all signals that will show up on the trace and
-- connnect them to internal signals on the datapath
--
-- what signals do we want to see from ID?
Out_ID_Instr        <= id_instruction;
Out_ID_PC           <= id_PC4;
--
-- signals traced from EX?
--
Out_EX_Rs           <= ex_register_rs;
Out_EX_Rt           <= ex_register_rt;
Out_EX_ALUSrc       <= ex_ALUSrc;
Out_EX_RegDst       <= ex_RegDst;
Out_EX_ALUOp        <= ex_ALUOp;
--
-- signals traced from MEM?
--
Out_MEM_ALURes      <= mem_alu_result;
Out_MEM_MemInData   <= mem_write_data;
Out_MEM_MRead       <= mem_MemRead;
Out_MEM_MWrite      <= mem_MemWrite;
--
--signals traced from WB?
--
Out_WB_MemOut       <= wb_memory_data;
Out_WB_ALU_Result   <= wb_alu_result;
Out_WB_WReg         <= wb_wreg_addr;
Out_WB_RegWrite     <= wb_RegWrite;
Out_WB_MemToReg     <= wb_MemToReg;



-- instantiate clock module
SCLK: my_clock
PORT MAP(
    sys_clock   => clock,
    reset       => reset
    );

IFE: fetch    -- instantiate the fetch component
PORT MAP(
    PC_out      => if_PC4,
    instruction => if_instruction,
    Branch_PC   => mem_Branch_PC,
    PCSource    => mem_PCSource,
    reset       => reset,
    clock       => clock_if_id
    );

if_id: pipe_reg1  -- instantiate the pipeline registers IF/ID
PORT MAP(
    clk             => clock_if_id,
    reset           => reset,
    if_PC4          => if_PC4,
    if_instruction  => if_instruction,
    id_PC4          => id_PC4,
    id_instruction  => id_instruction
    );

spim_control: control -- instantiate the control component
PORT MAP(
    opcode      => id_instruction(31 DOWNTO 26),
    RegDst      => id_RegDst,
    MemRead     => id_MemRead,
    MemToReg    => id_MemToReg,
    MemWrite    => id_MemWrite,
    ALUSrc      => id_ALUSrc,
    Branch      => id_Branch,
    RegWrite    => id_RegWrite,
    ALUOp       => id_ALUOp
    );

-- Add and connect the rs_reg_address and rt_reg_address
ID: decode  -- instantiate the decode component
PORT MAP(
    instruction     => id_instruction,
    memory_data     => wb_memory_data,
    alu_result      => wb_alu_result,
    RegWrite        => wb_RegWrite,
    MemToReg        => wb_MemToReg,
    reset           => reset,
    rs_reg_address  => id_rs_address,
    rt_reg_address  => id_rt_address,
    register_rs     => id_register_rs,
    register_rt     => id_register_rt,
    Sign_extend     => id_Sign_extend,
    wreg_address    => wb_wreg_addr,
    wreg_rd         => id_wreg_rd,
    wreg_rt         => id_wreg_rt
    );

-- Add and connect id_rs_address and id_rt_address as inputs
-- Add and connect ex_rs_address and ex_rt_address as inputs
id_ex: pipe_reg2 -- instantiate the pipeline register ID/EX
PORT MAP(
    clk             => clock,
    reset           => stall_or_reset,
    id_branch       => id_branch,
    id_MemToReg     => id_MemToReg,
    id_RegWrite     => id_RegWrite,
    id_MemWrite     => id_MemWrite,
    id_MemRead      => id_MemRead,
    id_ALUSrc       => id_ALUSrc,
    id_RegDst       => id_RegDst,
    id_ALUOp        => id_ALUOp,
    id_PC4          => id_PC4,
    id_rs_address   => id_rs_address,
    id_rt_address   => id_rt_address,
    id_register_rs  => id_register_rs,
    id_register_rt  => id_register_rt,
    id_sign_extend  => id_sign_extend,
    id_wreg_rd      => id_wreg_rd,
    id_wreg_rt      => id_wreg_rt,

    ex_branch       => ex_branch,
    ex_MemToReg     => ex_MemToReg,
    ex_RegWrite     => ex_RegWrite,
    ex_MemWrite     => ex_MemWrite,
    ex_MemRead      => ex_MemRead,
    ex_ALUSrc       => ex_ALUSrc,
    ex_RegDst       => ex_RegDst,
    ex_ALUOp        => ex_ALUOp,
    ex_PC4          => ex_PC4,
    ex_register_rs  => ex_register_rs,
    ex_register_rt  => ex_register_rt,
    ex_rs_address   => ex_rs_address,
    ex_rt_address   => ex_rt_address,
    ex_sign_extend  => ex_sign_extend,
    ex_wreg_rd      => ex_wreg_rd,
    ex_wreg_rt      => ex_wreg_rt
    );

EX: execute  -- instantiate the COMPONENT EX?
PORT MAP(
    PC4             => ex_PC4,
    register_rs     => fw_register_rs,
    register_rt     => fw_register_rt,
    sign_extend     => ex_sign_extend,
    RegDst          => ex_RegDst,
    ALUOp           => ex_ALUOp,
    ALUSrc          => ex_ALUSrc,
    alu_result      => ex_alu_result,
    wreg_rd         => ex_wreg_rd,
    wreg_rt         => ex_wreg_rt,
    wreg_address    => ex_wreg_addr,
    branch_pc       => ex_branch_PC,
    zero            => ex_zero
    );

ex_mem: pipe_reg3 -- instantiate the pipeline registers EX/MEM
PORT MAP(
    clk             => clock,
    reset           => reset,
    ex_branch_pc    => ex_branch_pc,
    ex_branch       => ex_branch,
    ex_MemToReg     => ex_MemToReg,
    ex_RegWrite     => ex_RegWrite,
    ex_MemWrite     => ex_MemWrite,
    ex_MemRead      => ex_MemRead,
    ex_alu_result   => ex_alu_result,
    ex_register_rt  => ex_register_rt,
    ex_wreg_addr    => ex_wreg_addr,
    ex_zero         => ex_zero,

    mem_branch_pc   => mem_branch_pc,
    mem_branch      => mem_branch,
    mem_MemToReg    => mem_MemToReg,
    mem_RegWrite    => mem_RegWrite,
    mem_MemWrite    => mem_MemWrite,
    mem_MemRead     => mem_MemRead,
    mem_alu_result  => mem_alu_result,
    mem_register_rt => mem_write_data,
    mem_wreg_addr   => mem_wreg_addr,
    mem_zero        => mem_zero
    );

    -- generate the branch condition for fetch
    mem_PCSource <= mem_branch and mem_zero;

MEM: memory -- instantiate the memory component
PORT MAP(
    address     => mem_alu_result,
    write_data  => mem_write_data,
    MemWrite    => mem_MemWrite,
    MemRead     => mem_MemRead,
    read_data   => mem_memory_data
    );


mem_wb: pipe_reg4   -- instantiate the pipeline register MEM/WB
PORT MAP(
    clk             => clock,
    reset           => reset,
    mem_MemToReg    => mem_MemToReg,
    mem_RegWrite    => mem_RegWrite,
    mem_memory_data => mem_memory_data,
    mem_alu_result  => mem_alu_result,
    mem_wreg_addr   => mem_wreg_addr,

    wb_MemToReg     => wb_MemToReg,
    wb_RegWrite     => wb_RegWrite,
    wb_memory_data  => wb_memory_data,
    wb_alu_result   => wb_alu_result,
    wb_wreg_addr    => wb_wreg_addr
);

-- Forwarding Unit
-- Connect all signals necessary from EX, MEM, and WB stages
fwd_unit : forwarding_unit
PORT MAP(--INPUT SIGNALS
    ex_rs_address       => ex_rs_address,
    ex_rt_address       => ex_rt_address,
    ex_register_rs      => ex_register_rs,
    ex_register_rt      => ex_register_rt,
    mem_RegWrite        => mem_RegWrite,
    mem_alu_result      => mem_alu_result,
    mem_wreg_addr       => mem_wreg_addr,
    wb_RegWrite         => wb_RegWrite,
    wb_alu_result       => wb_alu_result,
    wb_memory_data      => wb_memory_data,
    wb_wreg_addr        => wb_wreg_addr,
    wb_MemToReg         => wb_MemToReg,
    --OUTPUT SIGNALS
    fw_register_rs      => fw_register_rs,
    fw_register_rt      => fw_register_rt
);

-- Stalling Unit
-- Connect all signals necessary from ID, EX stages
stall_unit : stalling_unit
PORT MAP(--INPUT SIGNALS
    clock               => clock,
    reset               => reset,
    id_rs_address       => id_rs_address,
    id_rt_address       => id_rt_address,
    ex_memread          => ex_memread,
    ex_wreg_addr        => ex_wreg_addr,
    stall_or_reset      => stall_or_reset,
    clock_if_id         => clock_if_id
);
END structural;
