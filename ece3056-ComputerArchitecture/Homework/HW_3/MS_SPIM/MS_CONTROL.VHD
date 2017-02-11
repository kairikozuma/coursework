-- Kairi Kozuma
-- GTID: 903050898
-- ECE 3056 Spring 2016
--
-- Multicycle MIPS Processor VHDL Behavioral Model
--
-- control module (implements MIPS control unit)
--
-- Changes:
-- Expand rom to array of 0 to 10 STD_LOGIC_VECTOR
-- Reencode the microcode ROM to 22 bits
-- Include Jal and Jump in dispatch_1 table
-- Increase microinstruction signal to 22 bits
-- Add Jump signal, asserted when opcode is "000010"
-- Add Jal signal, asserted when opcode is "000011"
-- Increase RegDst and MemtoReg to 2 bit signal
LIBRARY IEEE;
USE IEEE.STD_LOGIC_1164.ALL;
USE IEEE.STD_LOGIC_ARITH.ALL;
USE IEEE.STD_LOGIC_SIGNED.ALL;

ENTITY control IS
    PORT( -- INPUT SIGNALS
        SIGNAL Opcode      : IN STD_LOGIC_VECTOR( 5 DOWNTO 0 );
        SIGNAL clock, reset: IN STD_LOGIC;

        -- OUTPUT SIGNALS
        SIGNAL PCWrite     : OUT STD_LOGIC;
        SIGNAL PCWriteCond : OUT STD_LOGIC;
        SIGNAL IorD        : OUT STD_LOGIC;
        SIGNAL MemRead     : OUT STD_LOGIC;
        SIGNAL MemWrite    : OUT STD_LOGIC;
        SIGNAL IRWrite     : OUT STD_LOGIC;
        SIGNAL MemtoReg    : OUT STD_LOGIC_VECTOR(1 DOWNTO 0);
        SIGNAL PCSource    : OUT STD_LOGIC_VECTOR(1 DOWNTO 0);
        SIGNAL ALUOp       : OUT STD_LOGIC_VECTOR(1 DOWNTO 0);
        SIGNAL ALUSrcB     : OUT STD_LOGIC_VECTOR(1 DOWNTO 0);
        SIGNAL ALUSrcA     : OUT STD_LOGIC;
        SIGNAL RegWrite    : OUT STD_LOGIC;
        SIGNAL RegDst      : OUT STD_LOGIC_VECTOR(1 DOWNTO 0);
        SIGNAL micropc     : OUT INTEGER
    );
END control;

ARCHITECTURE behavior OF control IS
    -- Implementation of the microcode ROM
    TYPE ROM_MEM IS ARRAY (0 TO 10) OF STD_LOGIC_VECTOR (21 DOWNTO 0);
    SIGNAL   IROM : ROM_MEM := (
    -- Microcode ROM reencoded to 22 bits
    -- Jump and link instruction added
        "10" & X"50103" ,   -- fetch
        "00" & X"00301" ,   -- decode
        "00" & X"00282" ,   -- memory address
        "00" & X"C0003" ,   -- memory load
        "00" & X"04040" ,   -- memory writeback
        "00" & X"A0000" ,   -- memory store
        "00" & X"00883" ,   -- rformat execution
        "00" & X"00050" ,   -- rformat writeback
        "01" & X"01480" ,   -- BEQ
        "10" & X"02000" ,   -- jump
        "10" & X"0A060"     -- jump and link
    );
    SIGNAL addr_control     : STD_LOGIC_VECTOR( 3 DOWNTO 0);
    SIGNAL microinstruction : STD_LOGIC_VECTOR(21 DOWNTO 0);
    SIGNAL R_format, Lw, Sw, Beq, Jump, Jal   : STD_LOGIC;
    SIGNAL dispatch_1, dispatch_2, next_micro : integer;

BEGIN

-- record the type of instruction
R_format <=  '1'  WHEN  Opcode = "000000"  ELSE '0';
Lw       <=  '1'  WHEN  Opcode = "100011"  ELSE '0';
Sw       <=  '1'  WHEN  Opcode = "101011"  ELSE '0';
Beq      <=  '1'  WHEN  Opcode = "000100"  ELSE '0';
Jump     <=  '1'  WHEN  Opcode = "000010"  ELSE '0';
Jal      <=  '1'  WHEN  Opcode = "000011"  ELSE '0';

-- Implementation of dispatch table 1
dispatch_1  <=  6 WHEN R_format = '1' ELSE
                8 WHEN Beq  = '1'     ELSE
                2 WHEN (LW  = '1') OR (SW = '1') ELSE
                9 WHEN Jump = '1'     ELSE
               10 WHEN Jal  = '1'     ELSE
                0;

-- Implementation of dispatch table 2
dispatch_2 <= 3 when (LW = '1') else 5;

-- Get microinstruction from ROM
microinstruction <= IROM(next_micro) when next_micro >= 0 else "00" & X"12340";

-- Route control bits to correct wire
PCWrite        <= microinstruction(21);
PCWriteCond    <= microinstruction(20);
IorD           <= microinstruction(19);
MemRead        <= microinstruction(18);
MemWrite       <= microinstruction(17);
IRWrite        <= microinstruction(16);
MemtoReg       <= microinstruction(15 DOWNTO 14);
PCSource       <= microinstruction(13 DOWNTO 12);
ALUOp          <= microinstruction(11 DOWNTO 10);
ALUSrcB        <= microinstruction( 9 DOWNTO  8);
ALUSrcA        <= microinstruction(7);
RegWrite       <= microinstruction(6);
RegDst         <= microinstruction( 5 DOWNTO  4);
addr_control   <= microinstruction( 3 DOWNTO  0);

-- Update micropc
micropc <= next_micro;

PROCESS
-- implement the microcode interpreter loop
BEGIN
    WAIT UNTIL (rising_edge(clock));
    IF (reset = '1') THEN
        next_micro <= 0;
        ELSE
        -- select the next microinstruction
        CASE addr_control IS
            WHEN "0000" => next_micro <= 0;
            WHEN "0001" => next_micro <= dispatch_1;
            WHEN "0010" => next_micro <= dispatch_2;
            WHEN "0011" => next_micro <= (next_micro + 1);
            WHEN OTHERS => next_micro <= 0;
        END CASE;
    END IF;
END PROCESS;

END behavior;
