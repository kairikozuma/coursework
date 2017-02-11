-- Kairi Kozuma
-- GTID: 903050898
-- ECE 3056 SPRING 2016
--
-- Mem/WB stage pipeline register
--
-- Changes: None
LIBRARY IEEE;
USE IEEE.STD_LOGIC_1164.ALL;

ENTITY pipe_reg4 IS
PORT(--INPUT SIGNALS
    mem_MemToReg    : IN STD_LOGIC;
    mem_RegWrite    : IN STD_LOGIC;
    mem_memory_data : IN STD_LOGIC_VECTOR(31 DOWNTO 0);
    mem_alu_result  : IN STD_LOGIC_VECTOR(31 DOWNTO 0);
    mem_wreg_addr   : IN STD_LOGIC_VECTOR( 4 DOWNTO 0);
    clk,reset       : IN STD_LOGIC;
    --OUTPUT SIGNALS
    wb_MemToReg     : OUT STD_LOGIC;
    wb_RegWrite     : OUT STD_LOGIC;
    wb_memory_data  : OUT STD_LOGIC_VECTOR(31 DOWNTO 0);
    wb_alu_result   : OUT STD_LOGIC_VECTOR(31 DOWNTO 0);
    wb_wreg_addr    : OUT STD_LOGIC_VECTOR( 4 DOWNTO 0)
    );
END pipe_reg4;

ARCHITECTURE behavioral OF pipe_reg4 IS
BEGIN
PROCESS
    BEGIN
    WAIT UNTIL (RISING_EDGE(clk));
    IF reset = '1'  THEN
        wb_MemToReg     <= '0';
        wb_RegWrite     <= '0';
        wb_memory_data  <= x"00000000";
        wb_alu_result   <= x"00000000";
        wb_wreg_addr    <= "00000";
    ELSE
        wb_MemToReg     <= mem_MemToReg;
        wb_RegWrite     <= mem_RegWrite;
        wb_memory_data  <= mem_memory_data;
        wb_alu_result   <= mem_alu_result;
        wb_wreg_addr    <= mem_wreg_addr;
    END IF;
END PROCESS;
END behavioral;
