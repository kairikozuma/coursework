-- Kairi Kozuma
-- GTID: 903050898
-- ECE 3056 SPRING 2016
--
-- EX/MEM stage pipeline register
--
-- Changes: None
LIBRARY IEEE;
USE IEEE.STD_LOGIC_1164.ALL;

ENTITY pipe_reg3 IS
PORT(--INPUT SIGNALS
    ex_MemToReg     : IN STD_LOGIC;
    ex_RegWrite     : IN STD_LOGIC;
    ex_MemWrite     : IN STD_LOGIC;
    ex_MemRead      : IN STD_LOGIC;
    ex_branch       : IN STD_LOGIC;
    ex_zero         : IN STD_LOGIC;
    ex_alu_result   : IN STD_LOGIC_VECTOR(31 DOWNTO 0);
    ex_register_rt  : IN STD_LOGIC_VECTOR(31 DOWNTO 0);
    ex_branch_PC    : IN STD_LOGIC_VECTOR(31 DOWNTO 0);
    ex_wreg_addr    : IN STD_LOGIC_VECTOR( 4 DOWNTO 0);
    clk, reset      : IN STD_LOGIC;
    --OUTPUT SIGNALS
    mem_MemToReg    : OUT STD_LOGIC;
    mem_RegWrite    : OUT STD_LOGIC;
    mem_MemWrite    : OUT STD_LOGIC;
    mem_MemRead     : OUT STD_LOGIC;
    mem_branch      : OUT STD_LOGIC;
    mem_zero        : OUT STD_LOGIC;
    mem_alu_result  : OUT STD_LOGIC_VECTOR(31 DOWNTO 0);
    mem_register_rt : OUT STD_LOGIC_VECTOR(31 DOWNTO 0);
    mem_branch_PC   : OUT STD_LOGIC_VECTOR(31 DOWNTO 0);
    mem_wreg_addr   : OUT STD_LOGIC_VECTOR( 4 DOWNTO 0));
END pipe_reg3;


ARCHITECTURE behavioral OF pipe_reg3 IS
BEGIN
PROCESS
    BEGIN
    WAIT UNTIL (RISING_EDGE(clk));
    IF reset ='1' THEN
        mem_branch_PC   <= x"00000000";
        mem_zero        <= '0';
        mem_branch      <= '0';
        mem_MemToReg    <= '0';
        mem_RegWrite    <= '0';
        mem_MemWrite    <= '0';
        mem_MemRead     <= '0';
        mem_alu_result  <= x"00000000";
        mem_register_rt <= x"00000000";
        mem_wreg_addr   <= "00000";
    ELSE
        mem_branch_PC   <= ex_branch_PC;
        mem_zero        <= ex_zero;
        mem_branch      <= ex_branch;
        mem_MemToReg    <= ex_MemToReg;
        mem_RegWrite    <= ex_RegWrite;
        mem_MemWrite    <= ex_MemWrite;
        mem_MemRead     <= ex_MemRead;
        mem_alu_result  <= ex_alu_result;
        mem_register_rt <= ex_register_rt;
        mem_wreg_addr   <= ex_wreg_addr;
    END IF;
END PROCESS;
END behavioral;
