-- Kairi Kozuma
-- GTID: 903050898
-- ECE 3056 SPRING 2016
--
-- ID/EX STAGE PIPELINE REGISTER
--
-- Changes:
-- Hold rs and rt register addresses in register
--  Add id_rs_address and id_rt_address as inputs
--  Add ex_rs_address and ex_rt_address as outputs
--  Reset new output signals
--  Latch new output signals to input values `on rising edge
LIBRARY IEEE;
USE IEEE.STD_LOGIC_1164.ALL;

ENTITY pipe_reg2 IS
PORT(--INPUT SIGNALS
    id_memtoreg     : IN STD_LOGIC;
    id_regwrite     : IN STD_LOGIC;
    id_memwrite     : IN STD_LOGIC;
    id_memread      : IN STD_LOGIC;
    id_alusrc       : IN STD_LOGIC;
    id_regdst       : IN STD_LOGIC;
    clk, reset      : IN STD_LOGIC;
    id_branch       : IN STD_LOGIC;
    id_aluop        : IN STD_LOGIC_VECTOR( 1 DOWNTO 0);
    id_pc4          : IN STD_LOGIC_VECTOR(31 DOWNTO 0);
    id_rs_address   : IN STD_LOGIC_VECTOR( 4 DOWNTO 0);
    id_rt_address   : IN STD_LOGIC_VECTOR( 4 DOWNTO 0);
    id_register_rs  : IN STD_LOGIC_VECTOR(31 DOWNTO 0);
    id_register_rt  : IN STD_LOGIC_VECTOR(31 DOWNTO 0);
    id_sign_extend  : IN STD_LOGIC_VECTOR(31 DOWNTO 0);
    id_wreg_rd      : IN STD_LOGIC_VECTOR( 4 DOWNTO 0);
    id_wreg_rt      : IN STD_LOGIC_VECTOR( 4 DOWNTO 0);

    --OUTPUT SIGNALS
    ex_memtoreg     : OUT STD_LOGIC;
    ex_regwrite     : OUT STD_LOGIC;
    ex_memwrite     : OUT STD_LOGIC;
    ex_memread      : OUT STD_LOGIC;
    ex_branch       : OUT STD_LOGIC;
    ex_alusrc       : OUT STD_LOGIC;
    ex_regdst       : OUT STD_LOGIC;
    ex_aluop        : OUT STD_LOGIC_VECTOR( 1 DOWNTO 0);
    ex_pc4          : OUT STD_LOGIC_VECTOR(31 DOWNTO 0);
    ex_rs_address   : OUT STD_LOGIC_VECTOR( 4 DOWNTO 0);
    ex_rt_address   : OUT STD_LOGIC_VECTOR( 4 DOWNTO 0);
    ex_register_rs  : OUT STD_LOGIC_VECTOR(31 DOWNTO 0);
    ex_register_rt  : OUT STD_LOGIC_VECTOR(31 DOWNTO 0);
    ex_sign_extend  : OUT STD_LOGIC_VECTOR(31 DOWNTO 0);
    ex_wreg_rd      : OUT STD_LOGIC_VECTOR( 4 DOWNTO 0);
    ex_wreg_rt      : OUT STD_LOGIC_VECTOR( 4 DOWNTO 0)
    );
END pipe_reg2;


ARCHITECTURE behavioral OF pipe_reg2 IS
BEGIN
PROCESS
    BEGIN
    WAIT UNTIL (RISING_EDGE(clk));
    IF reset ='1' THEN
        ex_branch       <= '0';
        ex_memtoreg     <= '0';
        ex_regwrite     <= '0';
        ex_memwrite     <= '0';
        ex_memread      <= '0';
        ex_alusrc       <= '0';
        ex_regdst       <= '0';
        ex_aluop        <= "00";
        ex_pc4          <= x"00000000";
        ex_rs_address   <= "00000";     -- Reset output rs address
        ex_rt_address   <= "00000";     -- Reset output rt address
        ex_register_rs  <= x"00000000";
        ex_register_rt  <= x"00000000";
        ex_sign_extend  <= x"00000000";
        ex_wreg_rd      <= "00000";
        ex_wreg_rt      <= "00000";
    ELSE
        ex_branch       <= id_branch;
        ex_memtoreg     <= id_memtoreg;
        ex_regwrite     <= id_regwrite;
        ex_memwrite     <= id_memwrite;
        ex_memread      <= id_memread;
        ex_alusrc       <= id_alusrc;
        ex_regdst       <= id_regdst;
        ex_aluop        <= id_aluop;
        ex_pc4          <= id_pc4;
        ex_rs_address   <= id_rs_address;   -- Latch id rs value
        ex_rt_address   <= id_rt_address;   -- Latch id rt value
        ex_register_rs  <= id_register_rs;
        ex_register_rt  <= id_register_rt;
        ex_sign_extend  <= id_sign_extend;
        ex_wreg_rd      <= id_wreg_rd;
        ex_wreg_rt      <= id_wreg_rt;
    END IF;
END PROCESS;
END behavioral;
