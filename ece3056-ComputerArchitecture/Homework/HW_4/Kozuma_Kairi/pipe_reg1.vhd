-- Kairi Kozuma
-- GTID: 903050898
-- ECE 3056 SPRING 2016
--
-- IF/ID STAGE PIPELINE REGISTER
--
-- Changes: None
LIBRARY IEEE;
USE IEEE.STD_LOGIC_1164.ALL;

ENTITY pipe_reg1 IS
PORT(--INPUT SIGNALS
    if_pc4          : IN  STD_LOGIC_VECTOR(31 DOWNTO 0);
    if_instruction  : IN  STD_LOGIC_VECTOR(31 DOWNTO 0);
    clk, reset      : IN  STD_LOGIC;
    --OUTPUT SIGNALS
    id_pc4          : OUT STD_LOGIC_VECTOR(31 DOWNTO 0);
    id_instruction  : OUT STD_LOGIC_VECTOR(31 DOWNTO 0)
    );
END pipe_reg1;

ARCHITECTURE behavioral OF pipe_reg1 IS
BEGIN
PROCESS
    BEGIN
    WAIT UNTIL (RISING_EDGE(clk));
    IF RESET = '1' THEN
        id_pc4 <= X"00000000";
        id_instruction <= X"00000000";
    ELSE
        id_pc4 <= if_pc4;
        id_instruction <= if_instruction;
    END IF;
END PROCESS;
END behavioral;
