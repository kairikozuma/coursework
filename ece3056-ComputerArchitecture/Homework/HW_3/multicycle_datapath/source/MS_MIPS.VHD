-- Kairi Kozuma
-- GTID: 903050898
-- ECE 3056 Spring 2016
--
-- Multicycle MIPS Processor VHDL Behavioral Model
--
-- Top Level Structural Model for MIPS Processor Core
--
-- Changes:
-- Add micropc_out signal to MIPS port
-- Add PC_Plus_4 signal input to Idecode module
-- Increase RegDst and MemtoReg to 2 bits for Control module
-- Connect micropc_out to s_micropc
LIBRARY IEEE;
USE IEEE.STD_LOGIC_1164.ALL;
USE IEEE.STD_LOGIC_ARITH.ALL;

ENTITY MIPS IS
    PORT(
        -- Output important signals to pins for easy display in Simulator
        reset_out        : OUT STD_LOGIC;
        sys_clock_out    : OUT STD_LOGIC;
        Zero_out         : OUT STD_LOGIC;
        Memwrite_out     : OUT STD_LOGIC;
        Regwrite_out     : OUT STD_LOGIC;
        PC_out           : OUT STD_LOGIC_VECTOR(31 DOWNTO 0);
        read_data_1_out  : OUT STD_LOGIC_VECTOR(31 DOWNTO 0);
        read_data_2_out  : OUT STD_LOGIC_VECTOR(31 DOWNTO 0);
        Instruction_out  : OUT STD_LOGIC_VECTOR(31 DOWNTO 0);
        write_data_out   : OUT STD_LOGIC_VECTOR(31 DOWNTO 0);
        ALU_result_out   : OUT STD_LOGIC_VECTOR(31 DOWNTO 0);
        s_ALUOut_out     : OUT STD_LOGIC_VECTOR(31 DOWNTO 0);
        micropc_out      : OUT INTEGER -- Add micropc_out signal
    );
END MIPS;

ARCHITECTURE structure OF MIPS IS

COMPONENT my_clock
    PORT (
        sys_clock, reset : OUT STD_LOGIC
    );
    END COMPONENT;

COMPONENT Ifetch
    PORT(-- input signals
        PCWrite        : IN STD_LOGIC;
        PCWriteCond    : IN STD_LOGIC;
        IRWrite        : IN STD_LOGIC;
        Zero           : IN STD_LOGIC;
        MemRead        : IN STD_LOGIC;
        MemWrite       : IN STD_LOGIC;
        clock          : IN STD_LOGIC;
        reset          : IN STD_LOGIC;
        IorD           : IN STD_LOGIC;
        PCSource       : IN STD_LOGIC_VECTOR( 1 DOWNTO 0);
        PC_Plus_4      : IN STD_LOGIC_VECTOR(31 DOWNTO 0);
        Branch_PC      : IN STD_LOGIC_VECTOR(31 DOWNTO 0);
        memory_data_in : IN std_logic_vector(31 downto 0);
        memory_addr_in : IN std_logic_vector(31 downto 0);

        -- Output signals
        PC_out            : OUT STD_LOGIC_VECTOR(31 DOWNTO 0);
        Memory_data_out   : OUT STD_LOGIC_VECTOR(31 DOWNTO 0);
        Instruction       : OUT STD_LOGIC_VECTOR(31 DOWNTO 0)
    );
  END COMPONENT Ifetch;

COMPONENT Idecode
    PORT(--INPUT SIGNALS
        clock       : IN STD_LOGIC;
        reset       : IN STD_LOGIC;
        RegWrite    : IN STD_LOGIC;
        MemtoReg    : IN STD_LOGIC_VECTOR( 1 DOWNTO 0);
        RegDst      : IN STD_LOGIC_VECTOR( 1 DOWNTO 0);
        PC_Plus_4   : IN STD_LOGIC_VECTOR(31 DOWNTO 0); -- Add PC_Plus_4 signal
        Instruction : IN STD_LOGIC_VECTOR(31 DOWNTO 0);
        read_data   : IN STD_LOGIC_VECTOR(31 DOWNTO 0);
        ALU_result  : IN STD_LOGIC_VECTOR(31 DOWNTO 0);
        -- OUTPUT SIGNALS
        read_data_1 : OUT STD_LOGIC_VECTOR(31 DOWNTO 0);
        read_data_2 : OUT STD_LOGIC_VECTOR(31 DOWNTO 0);
        Sign_extend : OUT STD_LOGIC_VECTOR(31 DOWNTO 0)
    );
END COMPONENT Idecode;


COMPONENT Control
    PORT(-- INPUT SIGNALS
        clock       : IN STD_LOGIC;
        reset       : IN STD_LOGIC;
        Opcode      : IN STD_LOGIC_VECTOR(5 DOWNTO 0);
        -- OUTPUT SIGNALS
        PCWrite     : OUT STD_LOGIC;
        PCWriteCond : OUT STD_LOGIC;
        IorD        : OUT STD_LOGIC;
        MemRead     : OUT STD_LOGIC;
        MemWrite    : OUT STD_LOGIC;
        IRWrite     : OUT STD_LOGIC;
        MemtoReg    : OUT STD_LOGIC_VECTOR(1 DOWNTO 0); -- Increase to 2 bits
        PCSource    : OUT STD_LOGIC_VECTOR(1 DOWNTO 0);
        ALUOp       : OUT STD_LOGIC_VECTOR(1 DOWNTO 0);
        ALUSrcA     : OUT STD_LOGIC;
        ALUSrcB     : OUT STD_LOGIC_VECTOR(1 DOWNTO 0);
        RegWrite    : OUT STD_LOGIC;
        RegDst      : OUT STD_LOGIC_VECTOR(1 DOWNTO 0); -- Increase to 2 bits
        micropc     : OUT INTEGER
    );
END COMPONENT Control;

COMPONENT  Execute
    PORT(-- INPUT SIGNALS
        clock           : IN STD_LOGIC;
        reset           : IN STD_LOGIC;
        Read_data_1     : IN STD_LOGIC_VECTOR(31 DOWNTO 0);
        Read_data_2     : IN STD_LOGIC_VECTOR(31 DOWNTO 0);
        Sign_extend     : IN STD_LOGIC_VECTOR(31 DOWNTO 0);
        Function_opcode : IN STD_LOGIC_VECTOR( 5 DOWNTO 0);
        ALUOp           : IN STD_LOGIC_VECTOR( 1 DOWNTO 0);
        ALUSrcA         : IN STD_LOGIC;
        ALUSrcB         : IN STD_LOGIC_VECTOR( 1 DOWNTO 0);
        PC              : IN STD_LOGIC_VECTOR(31 DOWNTO 0);
        -- OUTPUT SIGNALS
        Zero            : OUT STD_LOGIC;
        ALU_Result      : OUT STD_LOGIC_VECTOR(31 DOWNTO 0);
        ALUOut          : OUT STD_LOGIC_VECTOR(31 DOWNTO 0)
    );
END COMPONENT Execute;

-- declare signals used to connect VHDL components
SIGNAL s_PC_Out          : STD_LOGIC_VECTOR(31 DOWNTO 0);
SIGNAL s_Instruction     : STD_LOGIC_VECTOR(31 DOWNTO 0);
SIGNAL s_read_data_1     : STD_LOGIC_VECTOR(31 DOWNTO 0);
SIGNAL s_read_data_2     : STD_LOGIC_VECTOR(31 DOWNTO 0);
SIGNAL s_memory_data_out : STD_LOGIC_VECTOR(31 DOWNTO 0);
SIGNAL s_Sign_Extend     : STD_LOGIC_VECTOR(31 DOWNTO 0);
SIGNAL s_ALU_result      : STD_LOGIC_VECTOR(31 DOWNTO 0);
SIGNAL s_ALUOut          : STD_LOGIC_VECTOR(31 DOWNTO 0);
SIGNAL s_ALUSrcB         : STD_LOGIC_vector( 1 downto 0);
SIGNAL s_ALUop           : STD_LOGIC_VECTOR( 1 DOWNTO 0);
SIGNAL s_PCSource        : STD_LOGIC_VECTOR( 1 DOWNTO 0);
SIGNAL s_MemToReg        : STD_LOGIC_VECTOR( 1 DOWNTO 0);
SIGNAL s_RegDst          : STD_LOGIC_VECTOR( 1 DOWNTO 0);
SIGNAL s_reset           : STD_LOGIC;
SIGNAL s_sys_clock       : STD_LOGIC;
SIGNAL s_ALUSrcA         : STD_LOGIC;
SIGNAL s_Regwrite        : STD_LOGIC;
SIGNAL s_Zero            : STD_LOGIC;
SIGNAL s_MemWrite        : STD_LOGIC;
SIGNAL s_MemRead         : STD_LOGIC;
SIGNAL s_PCWrite         : STD_LOGIC;
SIGNAL s_PCWriteCond     : STD_LOGIC;
SIGNAL s_IRWrite         : STD_LOGIC;
SIGNAL s_IorD            : STD_LOGIC;
SIGNAL s_micropc         : INTEGER;

BEGIN
-- copy important signals to output pins for easy
-- display in Simulator
Instruction_out  <= s_Instruction;
ALU_result_out   <= s_ALU_result;
s_ALUOut_out     <= s_ALUOut;
read_data_1_out  <= s_read_data_1;
read_data_2_out  <= s_read_data_2;
write_data_out   <= s_ALUOut          WHEN s_MemtoReg = "00" ELSE
                    s_memory_data_out WHEN s_MemtoReg = "01" ELSE
                    s_ALUOut          WHEN s_MemtoReg = "10";
Zero_out         <= s_Zero;
RegWrite_out     <= s_RegWrite;
MemWrite_out     <= s_MemWrite;
reset_out        <= s_reset;
sys_clock_out    <= s_sys_clock;
PC_Out           <= s_PC_Out;
micropc_out      <= s_micropc; -- Make micropc visible

-- connect the MIPS components
MC: my_clock
    PORT MAP (
        sys_clock => s_sys_clock,
        reset     => s_reset
    );
IFE : Ifetch
    PORT MAP (
        PC_Plus_4       => s_ALU_Result,
        Branch_PC       => s_ALUOut,
        PCWrite         => s_PCWrite,
        PCWriteCond     => s_PCWriteCond,
        IRWrite         => s_IRWrite,
        PCSource        => s_PCSource,
        IorD            => s_IorD,
        Zero            => s_Zero,
        MemRead         => s_MemRead,
        MemWrite        => s_MemWrite,
        clock           => s_sys_clock,
        reset           => s_reset,
        memory_data_in  => s_read_data_2,
        memory_addr_in  => s_ALUOut,
        PC_out          => s_PC_Out,
        Memory_data_out => s_memory_data_out,
        Instruction     => s_instruction
    );

ID : Idecode
    PORT MAP (
        read_data_1 => s_read_data_1,
        read_data_2 => s_read_data_2,
        Sign_extend => s_Sign_extend,
        PC_Plus_4   => s_ALU_Result, -- Add PC_Plus_4 as input signal
        Instruction => s_instruction,
        read_data   => s_memory_data_out,
        ALU_result  => s_ALUOut,
        RegWrite    => s_RegWrite,
        MemtoReg    => s_MemToReg,
        RegDst      => s_RegDst,
        clock       => s_sys_clock,
        reset       => s_reset
    );

CTL:   control
    PORT MAP (
        Opcode      => s_instruction(31 downto 26),
        clock       => s_sys_clock,
        reset       => s_reset,
        PCWrite     => s_PCWrite,
        PCWriteCond => s_PCWriteCond,
        IorD        => s_IorD,
        MemRead     => s_MemRead,
        MemWrite    => s_MemWrite,
        IRWrite     => s_IRWrite,
        MemtoReg    => s_MemToReg,
        PCSource    => s_PCSource,
        ALUOp       => s_ALUOp,
        ALUSrcB     => s_ALUSrcB,
        ALUSrcA     => s_ALUSrcA,
        RegWrite    => s_RegWrite,
        RegDst      => s_RegDst,
        micropc     => s_micropc
    );

EXE:  Execute
    PORT MAP (
        Read_data_1     => s_read_data_1,
        Read_data_2     => s_read_data_2,
        Sign_extend     => s_sign_extend,
        Function_opcode => s_instruction(5 downto 0),
        ALUOp           => s_ALUOp,
        ALUSrcB         => s_ALUSrcB,
        ALUSrcA         => s_ALUSrcA,
        PC              => s_PC_Out,
        clock           => s_sys_clock,
        reset           => s_reset,
        Zero            => s_Zero,
        ALU_Result      => s_ALU_Result,
        ALUOut          => s_ALUOut
    );

END structure;
