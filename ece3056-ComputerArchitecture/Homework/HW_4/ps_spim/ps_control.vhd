-- Kairi Kozuma
-- GTID: 903050898
-- ECE 3056 SPRING 2016
--
-- control unit. simply implements the truth table for a small set of
-- instructions
--
-- Changes: None

LIBRARY IEEE;
USE IEEE.STD_LOGIC_1164.ALL;

ENTITY control IS
PORT(
    opcode      : IN STD_LOGIC_VECTOR(5 DOWNTO 0);
    RegDst      : OUT STD_LOGIC;
    MemRead     : OUT STD_LOGIC;
    MemToReg    : OUT STD_LOGIC;
    MemWrite    : OUT STD_LOGIC;
    ALUSrc      : OUT STD_LOGIC;
    RegWrite    : OUT STD_LOGIC;
    Branch      : OUT STD_LOGIC;
    ALUOp       : OUT STD_LOGIC_VECTOR(1 DOWNTO 0)
    );
END control;

ARCHITECTURE behavioral OF control IS
SIGNAL rformat, lw, sw, beq  : STD_LOGIC; -- define local signals
				    -- corresponding to instruction
				    -- type
BEGIN
    -- recognize opcode for each instruction type
    -- these variable should be inferred as wires
    rformat 	<=  '1'  WHEN  Opcode = "000000"  ELSE '0';
    Lw          <=  '1'  WHEN  Opcode = "100011"  ELSE '0';
    Sw          <=  '1'  WHEN  Opcode = "101011"  ELSE '0';
    Beq         <=  '1'  WHEN  Opcode = "000100"  ELSE '0';

    -- implement each output signal as the column of the truth
    -- table  which defines the control
    RegDst      <= rformat;
    ALUSrc      <= (lw or sw);
    MemToReg    <= lw;
    RegWrite    <= (rformat or lw);
    MemRead     <= lw;
    MemWrite    <= sw;
    Branch      <= beq;

    -- note the use of the concatenation operator
    -- to form  2 bit signal
    ALUOp(1 downto 0) <=  rformat & '0';
END behavioral;
