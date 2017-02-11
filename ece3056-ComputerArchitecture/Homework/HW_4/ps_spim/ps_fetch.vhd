-- Kairi Kozuma
-- GTID: 903050898
-- ECE 3056 SPRING 2016
--
-- Sudhakar Yalamanchili
-- Pipelined MIPS Processor VHDL Behavioral Mode--
--
--
-- Instruction fetch behavioral model. Instruction memory is
-- provided within this model. IF increments the PC,
-- and writes the appropriate output signals.
--
-- Changes: Sample program put in instruction memory
LIBRARY IEEE;
USE IEEE.STD_LOGIC_1164.ALL;
USE IEEE.STD_LOGIC_ARITH.ALL;
USE IEEE.STD_LOGIC_UNSIGNED.ALL;


ENTITY fetch IS
PORT(--INPUT SIGNALS
    clock, reset    : IN STD_LOGIC;
    PCSource        : IN STD_LOGIC;
    Branch_PC       : IN STD_LOGIC_VECTOR(31 DOWNTO 0);
    --OUTPUT SIGNALS
    instruction     : OUT STD_LOGIC_VECTOR(31 DOWNTO 0);
    PC_out          : OUT STD_LOGIC_VECTOR(31 DOWNTO 0)
    );
END fetch;

ARCHITECTURE behavioral OF fetch IS
TYPE INST_MEM IS ARRAY (0 TO 31) OF STD_LOGIC_VECTOR (31 DOWNTO 0);
   SIGNAL iram : INST_MEM := (
        X"00853820", -- add $7, $4, $5
        X"00e43020", -- add $6, $7, $4
        X"00000000", -- nop
        X"00000000", -- nop
        X"00000000", -- nop
        X"00000000", -- nop
        X"00000000", -- nop
        X"8c020030", -- lw $2, 48($0)
        X"8c030038", -- lw $3, 56($0)
        X"00430820", -- add $1, $2, $3
        X"00000000", -- nop
        X"00000000", -- nop
        X"00000000", -- nop
        X"00000000", -- nop
        X"00000000", -- nop
        X"00001020", -- add $2, $0, $0
        X"00001820", -- add $3, $0, $0
        X"00430820", -- add $1, $2, $3
        X"00000000", -- nop
        X"00000000", -- nop
        X"00000000", -- nop
        X"00000000", -- nop
        X"00000000", -- nop
        X"00000000",
        X"00000000",
        X"00000000",
        X"00000000",
        X"00000000",
        X"00000000",
        X"00000000",
        X"00000000",
        X"00000000"
   );

   SIGNAL PC        : STD_LOGIC_VECTOR( 31 DOWNTO 0 );
   SIGNAL Next_PC   : STD_LOGIC_VECTOR( 31 DOWNTO 0 );

BEGIN
-- access instruction pointed to by current PC
-- and increment PC by 4. This is combinational
-- Increase index to 4 bits for 32 words
Instruction <=  iram(CONV_INTEGER(PC(6 DOWNTO 2)));  -- since the instruction
                                                     -- memory is indexed by integer
PC_out<= (PC + 4);

-- compute value of next PC
Next_PC <=  (PC + 4)    WHEN PCSource = '0' ELSE
            Branch_PC   WHEN PCSource = '1' ELSE
            X"CCCCCCCC";

-- update the PC on the next clock
PROCESS
BEGIN
    WAIT UNTIL (rising_edge(clock));
    IF (reset = '1') THEN
        PC<= X"00000000" ;
    ELSE
        PC <= Next_PC;    -- cannot read/write a port hence need to duplicate info
    END IF;

END PROCESS;

END behavioral;
