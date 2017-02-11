-- Kairi Kozuma
-- GTID: 903050898
-- ECE 3056 Spring 2016
--
-- MultiCycle MIPS Processor VHDL Behavioral Model
--
-- Execute module (implements the data ALU and Branch Address Adder)
--
-- No changes except formatting

LIBRARY IEEE;
USE IEEE.STD_LOGIC_1164.ALL;
USE IEEE.STD_LOGIC_ARITH.ALL;
USE IEEE.STD_LOGIC_SIGNED.ALL;

ENTITY Execute IS
    PORT(
        -- INPUT SIGNALS
        Read_data_1     : IN STD_LOGIC_VECTOR(31 DOWNTO 0);
        Read_data_2     : IN STD_LOGIC_VECTOR(31 DOWNTO 0);
        Sign_extend     : IN STD_LOGIC_VECTOR(31 DOWNTO 0);
        Function_opcode : IN STD_LOGIC_VECTOR( 5 DOWNTO 0);
        ALUOp           : IN STD_LOGIC_VECTOR( 1 DOWNTO 0);
        ALUSrcB         : IN STD_LOGIC_VECTOR( 1 DOWNTO 0);
        ALUSrcA         : IN STD_LOGIC;
        PC              : IN STD_LOGIC_VECTOR(31 DOWNTO 0);
        clock           : IN STD_LOGIC;
        reset           : IN STD_LOGIC;

        -- OUTPUT SIGNALS
        Zero            : OUT STD_LOGIC;
        ALU_Result      : OUT STD_LOGIC_VECTOR(31 DOWNTO 0);
        ALUOut          : OUT STD_LOGIC_VECTOR(31 DOWNTO 0)
    );
END Execute;

ARCHITECTURE behavior OF Execute IS
    SIGNAL Ainput, Binput : STD_LOGIC_VECTOR(31 DOWNTO 0);
    SIGNAL ALU_Internal   : STD_LOGIC_VECTOR(31 DOWNTO 0);
    SIGNAL ALU_ctl        : STD_LOGIC_VECTOR( 2 DOWNTO 0);
BEGIN
-- compute the two ALU inputs
Ainput <=
    Read_data_1 WHEN ALUSrcA = '1' ELSE
    PC          WHEN ALUSrcA = '0' ELSE
    X"AAAAAAAA";

-- ALU input mux
Binput <=
    Read_data_2 WHEN ALUSrcB = "00" ELSE
    X"00000004" WHEN ALUSrcB = "01" ELSE
    Sign_extend(31 DOWNTO 0) WHEN ALUSrcB = "10" ELSE
    (Sign_extend (29 DOWNTO 0) & "00") WHEN ALUSrcB = "11" ELSE
    X"BBBBBBBB";

-- Generate ALU control bits
ALU_ctl(0) <= (Function_opcode(0) OR Function_opcode(3)) AND ALUOp(1);
ALU_ctl(1) <= (NOT Function_opcode(2)) OR (NOT ALUOp(1));
ALU_ctl(2) <= (Function_opcode(1) AND ALUOp(1)) OR ALUOp(0);

-- Generate Zero Flag
Zero <= '1' WHEN (ALU_internal = X"00000000") ELSE '0';

ALU_result <= ALU_internal;

PROCESS (ALU_ctl, Ainput, Binput)
BEGIN
    -- Select ALU operation
    CASE ALU_ctl IS
        -- ALU performs ALUresult = A_input AND B_input
        WHEN "000" =>ALU_internal <= Ainput AND Binput;
        -- ALU performs ALUresult = A_input OR B_input
        WHEN "001" =>ALU_internal <= Ainput OR Binput;
        -- ALU performs ALUresult = A_input + B_input
        WHEN "010" =>ALU_internal <= Ainput + Binput;
        -- ALU performs ?
        WHEN "011" =>ALU_internal <= X"00000000";
        -- ALU performs ?
        WHEN "100" =>ALU_internal <= X"00000000";
        -- ALU performs ?
        WHEN "101" =>ALU_internal <= X"00000000";
        -- ALU performs ALUresult = A_input -B_input
        WHEN "110" =>ALU_internal <= (Ainput - Binput);
        -- ALU performs SLT
        WHEN "111" =>ALU_internal <= (Ainput - Binput);
        WHEN OTHERS=>ALU_internal <= X"FFFFFFFF" ;
    END CASE;
END PROCESS;

PROCESS
BEGIN
    WAIT UNTIL (rising_edge(clock));
        IF (reset = '1') THEN
            ALUOut <= X"00000000";
        ELSE
            ALUOut <= ALU_internal(31 DOWNTO 0);
        END IF;
END PROCESS;

END behavior;
