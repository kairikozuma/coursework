-- Kairi Kozuma
-- GTID: 903050898
-- ECE 3056 SPRING 2016
--
-- Sudhakar Yalamanchili
-- Pipelined MIPS Processor VHDL Behavioral Mode--
--
--
-- execution unit. only a subset of instructions are supported in this
-- model, specifically add, sub, lw, sw, beq, and, or
--
-- Changes: None
LIBRARY IEEE;
USE IEEE.STD_LOGIC_1164.ALL;
USE IEEE.STD_LOGIC_ARITH.ALL;
USE IEEE.STD_LOGIC_SIGNED.ALL;

ENTITY execute IS
PORT(--INPUT SIGNAL
    PC4             : IN STD_LOGIC_VECTOR(31 DOWNTO 0);
    register_rs     : IN STD_LOGIC_VECTOR(31 DOWNTO 0);
    register_rt     : IN STD_LOGIC_VECTOR(31 DOWNTO 0);
    Sign_extend     : IN STD_LOGIC_VECTOR(31 DOWNTO 0);
    ALUOp           : IN STD_LOGIC_VECTOR( 1 DOWNTO 0);
    ALUSrc          : IN STD_LOGIC;
    RegDst          : IN STD_LOGIC;
    wreg_rd         : IN STD_LOGIC_VECTOR( 4 DOWNTO 0);
    wreg_rt         : IN STD_LOGIC_VECTOR( 4 DOWNTO 0);
    --OUTPUT SIGNAL
    alu_result      : OUT STD_LOGIC_VECTOR(31 DOWNTO 0);
    Branch_PC       : OUT STD_LOGIC_VECTOR(31 DOWNTO 0);
    wreg_address    : OUT STD_LOGIC_VECTOR( 4 DOWNTO 0);
    zero            : OUT STD_LOGIC
    );
END execute;

ARCHITECTURE behavioral OF execute IS
SIGNAL ainput           : STD_LOGIC_VECTOR(31 DOWNTO 0);
SIGNAL binput           : STD_LOGIC_VECTOR(31 DOWNTO 0);
SIGNAL alu_internal     : STD_LOGIC_VECTOR(31 DOWNTO 0);
SIGNAL function_opcode  : STD_LOGIC_VECTOR( 5 DOWNTO 0);
SIGNAL ALU_ctl          : STD_LOGIC_VECTOR( 2 DOWNTO 0);

BEGIN
-- compute the two ALU inputs
Ainput <= register_rs;

-- ALU input mux
Binput <= register_rt WHEN ( ALUSrc = '0' ) ELSE
        Sign_extend(31 downto 0) when ALUSrc = '1' ELSE
        X"BBBBBBBB";

-- Compute branch address
Branch_PC <= PC4 + (Sign_extend(29 downto 0) & "00");

-- Get the function field. This will be the least significant
-- 6 bits of  the sign extended offset
Function_opcode <= Sign_extend(5 downto 0);

-- Generate ALU control bits
ALU_ctl( 0 ) <= ( Function_opcode( 0 ) OR Function_opcode( 3 ) ) AND ALUOp( 1 );
ALU_ctl( 1 ) <= ( NOT Function_opcode( 2 ) ) OR (NOT ALUOp( 1 ) );
ALU_ctl( 2 ) <= ( Function_opcode( 1 ) AND ALUOp( 1 )) OR ALUOp( 0 );

-- Generate Zero Flag
Zero <= '1' WHEN ( ALU_internal = X"00000000" ) ELSE '0';

-- implement the RegDst mux in this pipeline stage
wreg_address <= wreg_rd when RegDst = '1' else wreg_rt;

-- Output alu result
ALU_result <= ALU_internal;

PROCESS ( ALU_ctl, Ainput, Binput )
BEGIN
    -- Select ALU operation
    CASE ALU_ctl IS
    -- ALU performs ALUresult = A_input AND B_input
    WHEN "000"     =>    ALU_internal     <= Ainput AND Binput;
    -- ALU performs ALUresult = A_input OR B_input
    WHEN "001"     =>    ALU_internal     <= Ainput OR Binput;
    -- ALU performs ALUresult = A_input + B_input
    WHEN "010"     =>    ALU_internal     <= Ainput + Binput;
    -- ALU performs ?
    WHEN "011"     =>    ALU_internal     <= X"00000000";
    -- ALU performs ?
    WHEN "100"     =>    ALU_internal     <= X"00000000";
    -- ALU performs ?
    WHEN "101"     =>    ALU_internal     <= X"00000000";
    -- ALU performs ALUresult = A_input -B_input
    WHEN "110"     =>    ALU_internal     <= (Ainput - Binput);
    -- ALU performs SLT
    WHEN "111"     =>    ALU_internal     <= (Ainput - Binput) ;
    WHEN OTHERS    =>    ALU_internal     <= X"FFFFFFFF" ;
    END CASE;
END PROCESS;
END behavioral;
