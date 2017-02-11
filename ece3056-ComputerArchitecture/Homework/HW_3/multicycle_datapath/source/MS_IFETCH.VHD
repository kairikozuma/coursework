-- Kairi Kozuma
-- GTID: 903050898
-- ECE 3056 Spring 2016
--
-- Multicycle MIPS Processor VHDL Behavioral Model
--
-- Ifetch module (provides the PC, instruction, and data memory)
--
-- Changes:
-- Add Local_inst signal to store PC + 4 value
-- Expand instruction memory size to 16 words to fit new assembly program
-- Increase bits read from PC to bit 5 for indexing 16 words
-- Local_inst signal added to hold instruction value for current instruction
LIBRARY IEEE;
USE IEEE.STD_LOGIC_1164.ALL;
USE IEEE.STD_LOGIC_ARITH.ALL;
USE IEEE.STD_LOGIC_UNSIGNED.ALL;

ENTITY Ifetch IS
    PORT(-- Input signals
        SIGNAL PC_Plus_4      : IN STD_LOGIC_VECTOR(31 DOWNTO 0);
        SIGNAL Branch_PC      : IN STD_LOGIC_VECTOR(31 DOWNTO 0);
        SIGNAL PCWrite        : IN STD_LOGIC;
        SIGNAL PCWriteCond    : IN STD_LOGIC;
        SIGNAL IRWrite        : IN STD_LOGIC;
        SIGNAL PCSource       : IN STD_LOGIC_VECTOR( 1 DOWNTO 0);
        SIGNAL Zero           : IN STD_LOGIC;
        SIGNAL MemRead        : IN STD_LOGIC;
        SIGNAL MemWrite       : IN STD_LOGIC;
        SIGNAL IorD           : IN STD_LOGIC;
        SIGNAL clock          : IN STD_LOGIC;
        SIGNAL reset          : IN STD_LOGIC;
        SIGNAL memory_data_in : IN STD_LOGIC_VECTOR(31 downto 0);
        SIGNAL memory_addr_in : IN STD_LOGIC_VECTOR(31 downto 0);
        -- Output signals
        SIGNAL PC_out          : OUT STD_LOGIC_VECTOR( 31 DOWNTO 0);
        SIGNAL Memory_data_out : OUT STD_LOGIC_VECTOR( 31 DOWNTO 0);
        SIGNAL Instruction     : OUT STD_LOGIC_VECTOR( 31 DOWNTO 0)
    );
END Ifetch;

ARCHITECTURE behavior OF Ifetch IS
   TYPE INST_MEM IS ARRAY (0 to 15) of STD_LOGIC_VECTOR (31 DOWNTO 0);
   SIGNAL iram : INST_MEM := (
        X"00222020",   -- add $4, $1, $2
        X"0c000003",   -- jal 0x0000000C [label1]
        X"00832820",   -- add $5, $4, $3
        X"00643020",   -- add $6, $3, $4             ;[label1]
        X"0c000006",   -- jal 0x00000018 [label2]
        X"00c33820",   -- add $7, $6, $3
        X"00c34020",   -- add $8, $6, $3             ;[label2]
        X"0c000009",   -- jal 0x00000024 [endlabel]
        X"01034820",   -- add $9, $8, $3
        X"01035020",   -- add $10, $8, $3            ;[endlabel]
        X"00000000",   -- nop
        X"00000000",   -- nop
        X"00000000",   -- nop
        X"00000000",   -- nop
        X"00000000",   -- nop
        X"00000000"    -- nop
   );

   TYPE DATA_RAM IS ARRAY (0 to 31) OF STD_LOGIC_VECTOR (31 DOWNTO 0);
   SIGNAL dram: DATA_RAM := (
        X"00000000",
        X"11111111",
        X"22222222",
        X"33333333",
        X"44444444",
        X"55555555",
        X"66666666",
        X"77777777",
        X"0000000A",
        X"1111111A",
        X"2222222A",
        X"3333333A",
        X"4444444A",
        X"5555555A",
        X"6666666A",
        X"7777777A",
        X"0000000B",
        X"1111111B",
        X"2222222B",
        X"3333333B",
        X"4444444B",
        X"5555555B",
        X"6666666B",
        X"7777777B",
        X"000000BA",
        X"111111BA",
        X"222222BA",
        X"333333BA",
        X"444444BA",
        X"555555BA",
        X"666666BA",
        X"777777BA"
    );
SIGNAL PC           : STD_LOGIC_VECTOR(31 DOWNTO 0);
SIGNAL Next_PC      : STD_LOGIC_VECTOR(31 DOWNTO 0);
SIGNAL Jump_PC      : STD_LOGIC_VECTOR(31 DOWNTO 0);
SIGNAL PC_Update    : BOOLEAN;
SIGNAL Local_inst   : STD_LOGIC_VECTOR(31 DOWNTO 0); -- New signal to hold Instruction
SIGNAL Local_IR     : STD_LOGIC_VECTOR(31 DOWNTO 0);
SIGNAL Local_data   : STD_LOGIC_VECTOR(31 DOWNTO 0);

BEGIN
Local_IR <= -- Expand to 5 bits
    iram(CONV_INTEGER('0' & PC(5 DOWNTO 2))) WHEN IorD ='0'
    -- leading '0' to prevent interpreting index as a
    -- negative number (2's complement)
    ELSE X"0000FFFF"; -- read instr pointed to by PC

Local_data <=
    dram(CONV_INTEGER('0' & memory_addr_in(6 DOWNTO 2))) WHEN IorD ='1'
    ELSE X"0000AAAA"; --read data from data addr

Jump_PC <= "0000" & Local_inst(25 DOWNTO 0) & "00"; -- compute jump address

-- compute value of next PC
Next_PC <=  PC_Plus_4    WHEN PCSource = "00" ELSE
            Branch_PC    WHEN PCSource = "01" ELSE
            Jump_PC      WHEN PCSource = "10" ELSE
            X"CCCCCCCC";

-- check if the PC is to be updated on the next clock cycle
PC_Update <= ((PCWrite = '1') OR ((PCWriteCond = '1') AND (Zero ='1')));

PROCESS
BEGIN
    WAIT UNTIL (rising_edge(clock));
        IF (reset = '1') THEN
            PC              <= X"00000000" ;
            pc_out          <= x"00000000";
            Instruction     <= x"00000000";
            Memory_data_out <= X"00000000";
        ELSE
            IF (PC_update) THEN
                PC <= next_PC;
                PC_out <= next_PC;
            END IF;
            IF (IRWrite  = '1') THEN -- Store LOCAL_IR in Local_inst
                Instruction <= Local_IR; Local_inst <= Local_IR; END IF;
            IF (MemRead  = '1') THEN
                Memory_data_out <= Local_Data; END IF;
            IF (MemWrite = '1') THEN
                dram(CONV_INTEGER(memory_addr_in(6 downto 2))) <= memory_data_in; END IF;
        END IF;
END PROCESS;

END behavior;
