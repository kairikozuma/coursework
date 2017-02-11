-- Kairi Kozuma
-- GTID: 903050898
-- ECE 3056 SPRING 2016
--
-- Sudhakar Yalamanchili
-- Pipelined MIPS Processor VHDL Behavioral Mode--
----
-- data memory component.
--
-- Changes: Initialize memory according to test program
--          12th memory location = 17
--          14th memory location = 17

LIBRARY IEEE;
USE IEEE.STD_LOGIC_1164.ALL;
USE IEEE.STD_LOGIC_ARITH.ALL;
USE IEEE.STD_LOGIC_UNSIGNED.ALL;

ENTITY memory IS
PORT(--INPUT SIGNALS
    address     : IN STD_LOGIC_VECTOR(31 DOWNTO 0);
    write_data  : IN STD_LOGIC_VECTOR(31 DOWNTO 0);
    MemWrite    : IN STD_LOGIC;
    MemRead     : IN STD_LOGIC;
    --OUTPUT SIGNALS
    read_data   : OUT STD_LOGIC_VECTOR(31 DOWNTO 0)
    );
END memory;

ARCHITECTURE behavioral OF memory IS

TYPE DATA_RAM IS ARRAY (0 to 31) OF STD_LOGIC_VECTOR (31 DOWNTO 0);
SIGNAL dram: DATA_RAM := (
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
    X"00000011", -- 12th memory location = 17
    X"00000000",
    X"00000011", -- 14th memory location = 17
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

BEGIN
-- memory read operation
read_data <= dram(CONV_INTEGER(address(6 DOWNTO 2))) WHEN MemRead = '1'
    ELSE X"FFFFFFFF";

-- memory write operation
dram(CONV_INTEGER(address(6 DOWNTO 2))) <= write_data WHEN MemWrite = '1'
    ELSE dram(CONV_INTEGER(address(6 DOWNTO 2)));

END behavioral;
