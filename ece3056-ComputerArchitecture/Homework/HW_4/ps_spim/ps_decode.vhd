-- Kairi Kozuma
-- GTID: 903050898
-- ECE 3056 SPRING 2016
--
-- Sudhakar Yalamanchili
-- Pipelined MIPS Processor VHDL Behavioral Mode--
--
--
-- instruction decode unit.
--
-- Note that this module differs from the text in the following ways
-- 1. The MemToReg Mux is implemented in this module instead of a (syntactically)
-- different pipeline stage.
--
-- Changes:
-- Output rs and rt addresses from decode module
-- Initialize register values according to sample program

LIBRARY IEEE;
USE IEEE.STD_LOGIC_1164.ALL;
USE IEEE.STD_LOGIC_ARITH.ALL;
USE IEEE.STD_LOGIC_UNSIGNED.ALL;

ENTITY decode IS
PORT(--INPUT SIGNALS
    instruction     : IN STD_LOGIC_VECTOR(31 DOWNTO 0);
    memory_data     : IN STD_LOGIC_VECTOR(31 DOWNTO 0);
    alu_result      : IN STD_LOGIC_VECTOR(31 DOWNTO 0);
    RegWrite        : IN STD_LOGIC;
    MemToReg        : IN STD_LOGIC;
    reset           : IN STD_LOGIC;
    wreg_address    : IN STD_LOGIC_VECTOR( 4 DOWNTO 0);
    --OUTPUT SIGNALS
    rs_reg_address  : OUT STD_LOGIC_VECTOR( 4 DOWNTO 0); -- rs address
    rt_reg_address  : OUT STD_LOGIC_VECTOR( 4 DOWNTO 0); -- rt address
    register_rs     : OUT STD_LOGIC_VECTOR(31 DOWNTO 0);
    register_rt     : OUT STD_LOGIC_VECTOR(31 DOWNTO 0);
    Sign_extend     : OUT STD_LOGIC_VECTOR(31 DOWNTO 0);
    wreg_rd         : OUT STD_LOGIC_VECTOR( 4 DOWNTO 0);
    wreg_rt         : OUT STD_LOGIC_VECTOR( 4 DOWNTO 0)
    );
END decode;

ARCHITECTURE behavioral OF decode IS
TYPE register_file IS ARRAY ( 0 TO 31 ) OF STD_LOGIC_VECTOR( 31 DOWNTO 0 );
SIGNAL register_array: register_file := (
    X"00000000",
    X"00000000", -- $1 = 0
    X"00000030", -- $2 = 48
    X"00000038", -- $3 = 56
    X"00000002", -- $4 = 2
    X"00000006", -- $5 = 6
    X"00000001", -- $6 = 1
    X"00000000", -- $7 = 0
    X"00000000",
    X"00000000",
    X"00000000",
    X"00000000",
    X"00000000",
    X"00000000",
    X"00000000",
    X"00000000",
    X"00000000",
    X"00000000",
    X"00000000",
    X"00000000",
    X"00000000",
    X"00000000",
    X"00000000",
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

SIGNAL write_data                   : STD_LOGIC_VECTOR(31 DOWNTO 0);
SIGNAL read_register_1_address      : STD_LOGIC_VECTOR( 4 DOWNTO 0);
SIGNAL read_register_2_address      : STD_LOGIC_VECTOR( 4 DOWNTO 0);
SIGNAL Instruction_immediate_value  : STD_LOGIC_VECTOR(15 DOWNTO 0);

BEGIN
    -- Decode register rs and rt addresses
    read_register_1_address     <= Instruction(25 DOWNTO 21);
    read_register_2_address     <= Instruction(20 DOWNTO 16);

    -- Output rs and rt register addresses
    rs_reg_address  <= read_register_1_address;
    rt_reg_address  <= read_register_2_address;

    -- Decode and output immediate value field
    Instruction_immediate_value <= Instruction(15 DOWNTO  0);

    -- MemToReg Mux for Writeback
    write_data <= ALU_result(31 DOWNTO 0)
        WHEN ( MemtoReg = '0' )
        ELSE memory_data;

    -- Sign Extend 16-bits to 32-bits
    Sign_extend <= X"0000" & Instruction_immediate_value
        WHEN Instruction_immediate_value(15) = '0'
        ELSE X"FFFF" & Instruction_immediate_value;

    -- Read Register 1 Operation
    register_rs <= register_array(CONV_INTEGER(read_register_1_address));
    -- Read Register 2 Operation
    register_rt <= register_array(CONV_INTEGER(read_register_2_address));

    -- Register write operation
    register_array(CONV_INTEGER(wreg_address)) <= write_data
        WHEN RegWrite = '1'
        ELSE register_array(CONV_INTEGER(wreg_address));

    -- move possible write destinations to execute stage
    wreg_rd <= instruction(15 downto 11);
    wreg_rt <= instruction(20 downto 16);

END behavioral;
