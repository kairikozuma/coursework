-- Kairi Kozuma
-- GTID: 903050898
-- ECE 3056 SPRING 2016
--
-- Stalling Unit
-- Use combinational logic to generate stall signal
-- Use stall signal to control clock for IF and ID stages
LIBRARY IEEE;
USE IEEE.STD_LOGIC_1164.ALL;

ENTITY stalling_unit IS
PORT(--INPUT SIGNALS
    -- Input from ID stage
    --  rs and rt addresses, to check for dependence
    clock               : IN STD_LOGIC;
    reset               : IN STD_LOGIC;
    id_rs_address       : IN STD_LOGIC_VECTOR( 4 DOWNTO 0);
    id_rt_address       : IN STD_LOGIC_VECTOR( 4 DOWNTO 0);
    --  Logic values to implement load-use stall
    ex_MemRead          : IN STD_LOGIC;
    ex_wreg_addr        : IN STD_LOGIC_VECTOR( 4 DOWNTO 0);
    -- Output signal that controls IF, ID stages for load to use stalls
    stall_or_reset      : OUT STD_LOGIC;
    clock_if_id         : OUT STD_LOGIC
    );
END stalling_unit;

ARCHITECTURE behavioral OF stalling_unit IS
-- Internal signal which is asserted if a load to use is necessary
SIGNAL load_use_stall : STD_LOGIC;

-- Internal signals latched on falling edge
SIGNAL internal_id_rs_address       : STD_LOGIC_VECTOR( 4 DOWNTO 0);
SIGNAL internal_id_rt_address       : STD_LOGIC_VECTOR( 4 DOWNTO 0);
SIGNAL internal_ex_memread          : STD_LOGIC;
SIGNAL internal_ex_wreg_addr        : STD_LOGIC_VECTOR( 4 DOWNTO 0);

BEGIN

-- Check if Load to Use stall is necessary

-- Turn off clock for ID and IF stage if load to use stall is asserted
clock_if_id <= clock and NOT load_use_stall;

-- Output stall or reset signal
stall_or_reset <= load_use_stall OR reset;

-- Logic to determine if load to use stall is necessary
load_use_stall <= '1' WHEN (internal_ex_memread = '1' AND internal_ex_wreg_addr /= "00000") AND
    (internal_ex_wreg_addr = internal_id_rs_address OR internal_ex_wreg_addr = internal_id_rt_address) ELSE '0';

-- Latch the values in the falling edge to allow load_use_stall signal to change
-- before positive clock edge
PROCESS
BEGIN
    WAIT UNTIL (falling_edge(clock));
        internal_id_rs_address       <= id_rs_address;
        internal_id_rt_address       <= id_rt_address;
        internal_ex_memread          <= ex_memread;
        internal_ex_wreg_addr        <= ex_wreg_addr;
END PROCESS;

END behavioral;
