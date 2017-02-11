-- Kairi Kozuma
-- GTID: 903050898
-- ECE 3056 Spring 2016
--
-- No changes except formatting

LIBRARY IEEE;
USE IEEE.STD_LOGIC_1164.ALL;
USE IEEE.STD_LOGIC_ARITH.ALL;
USE IEEE.STD_LOGIC_UNSIGNED.ALL;

ENTITY my_clock IS
    PORT (
        SIGNAL sys_clock, reset : OUT std_logic
    );
END ENTITY my_clock;

ARCHITECTURE behavior OF my_clock IS
BEGIN
PROCESS
    BEGIN
    -- generate clock
    sys_clock <= '0', '1' AFTER 50 ns;
    WAIT FOR 100 ns;
END PROCESS;
    -- following statement executes only once
reset <= '1', '0' AFTER 75 ns;
END ARCHITECTURE behavior;
