-- Kairi Kozuma
-- GTID: 903050898
-- ECE 3056 SPRING 2016
--
-- Forwarding Unit
--
-- Changes:
-- Use rs and rt addresses from ex stage and write addresses from mem and wb stage
-- Combinational logic to implement forwarding
-- Output correct Rs and Rt 32 bit values to pass to execution stage
LIBRARY IEEE;
USE IEEE.STD_LOGIC_1164.ALL;

ENTITY forwarding_unit IS
PORT(--INPUT SIGNALS
    -- Input from EX stage
    -- Rs and Rt values, used if no dependence found
    -- rs and rt addresses, to check for dependence
    ex_rs_address       : IN STD_LOGIC_VECTOR( 4 DOWNTO 0);
    ex_rt_address       : IN STD_LOGIC_VECTOR( 4 DOWNTO 0);
    ex_register_rs      : IN STD_LOGIC_VECTOR(31 DOWNTO 0);
    ex_register_rt      : IN STD_LOGIC_VECTOR(31 DOWNTO 0);
    -- Signals needed to check dependence from MEM stage
    mem_RegWrite        : IN STD_LOGIC;
    mem_alu_result      : IN STD_LOGIC_VECTOR(31 DOWNTO 0);
    mem_wreg_addr       : IN STD_LOGIC_VECTOR( 4 DOWNTO 0);
    -- Signals needed to forward from WB stage
    wb_MemToReg         : IN STD_LOGIC;
    wb_RegWrite         : IN STD_LOGIC;
    wb_alu_result       : IN STD_LOGIC_VECTOR(31 DOWNTO 0);
    wb_memory_data      : IN STD_LOGIC_VECTOR(31 DOWNTO 0);
    wb_wreg_addr        : IN STD_LOGIC_VECTOR( 4 DOWNTO 0);
    -- OUTPUT SIGNALS
    -- Output signal for EX stage
    fw_register_rs      : OUT STD_LOGIC_VECTOR(31 DOWNTO 0);
    fw_register_rt      : OUT STD_LOGIC_VECTOR(31 DOWNTO 0)
    );
END forwarding_unit;

ARCHITECTURE behavioral OF forwarding_unit IS
-- Internal signals for forward selection mux
SIGNAL forwardA, forwardB : STD_LOGIC_VECTOR(1 DOWNTO 0);

BEGIN

-- Determine value passed as Rs to EX stage
forwardA <= "10" WHEN (mem_RegWrite = '1' AND mem_wreg_addr /= "00000") AND (mem_wreg_addr = ex_rs_address) ELSE
            "01" WHEN (wb_RegWrite  = '1' AND wb_wreg_addr  /= "00000") AND (wb_wreg_addr  = ex_rs_address) ELSE
            "00";

-- Determine value passed as Rt to EX stage
forwardB <= "10" WHEN (mem_RegWrite = '1' AND mem_wreg_addr /= "00000") AND (mem_wreg_addr = ex_rt_address) ELSE
            "01" WHEN (wb_RegWrite  = '1' AND wb_wreg_addr  /= "00000") AND (wb_wreg_addr  = ex_rt_address) ELSE
            "00";

-- MUX to select original Rs or forwarded value from MEM or WB
fw_register_rs   <= ex_register_rs WHEN forwardA = "00" ELSE
                    wb_alu_result  WHEN forwardA = "01" AND wb_MemToReg = '0' ELSE
                    wb_memory_data WHEN forwardA = "01" AND wb_MemToReg = '1' ELSE
                    mem_alu_result WHEN forwardA = "10";

-- MUX to select original Rt or forwarded value from MEM or WB
fw_register_rt   <= ex_register_rt WHEN forwardB = "00" ELSE
                    wb_alu_result  WHEN forwardB = "01" AND wb_MemToReg = '0' ELSE
                    wb_memory_data WHEN forwardB = "01" AND wb_MemToReg = '1' ELSE
                    mem_alu_result WHEN forwardB = "10";
END behavioral;
