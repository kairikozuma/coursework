LIBRARY IEEE;
USE  IEEE.STD_LOGIC_1164.all;
USE  IEEE.STD_LOGIC_ARITH.all;
USE  IEEE.STD_LOGIC_UNSIGNED.all;

ENTITY acc_clk_gen IS

	PORT
	(
		clock_50Mhz			  : IN	STD_LOGIC;
		clock_10KHz				: OUT	STD_LOGIC;
		clock_100Hz				: OUT	STD_LOGIC;
		clock_10Hz				: OUT	STD_LOGIC
	);
	
END acc_clk_gen;

ARCHITECTURE a OF acc_clk_gen IS

	SIGNAL	count_10hz      : STD_LOGIC_VECTOR(22 DOWNTO 0); 
	SIGNAL	count_100hz     : STD_LOGIC_VECTOR(18 DOWNTO 0);
	SIGNAL  count_10Khz     : STD_LOGIC_VECTOR(12 DOWNTO 0); 
	SIGNAL	clock_10hz_int  : STD_LOGIC; 
	SIGNAL	clock_100hz_int : STD_LOGIC;
	SIGNAL  clock_10Khz_int : STD_LOGIC; 
BEGIN
	PROCESS 
	BEGIN

		WAIT UNTIL clock_50Mhz'EVENT and clock_50Mhz = '1';
		clock_10Khz <= clock_10Khz_int;
		clock_100hz <= clock_100hz_int;
		clock_10hz <= clock_10hz_int;
			IF count_10hz < 2500000 THEN
				count_10hz <= count_10hz + 1;
			ELSE
				count_10hz <= "00000000000000000000000";
				clock_10hz_int <= NOT(clock_10hz_int);
			END IF;
			IF count_100hz < 250000 THEN
				count_100hz <= count_100hz + 1;
			ELSE
				count_100hz <= "0000000000000000000";
				clock_100hz_int <= NOT(clock_100hz_int);
			END IF;	
			IF count_10Khz < 2500 THEN
				count_10Khz <= count_10Khz + 1;
			ELSE
				count_10Khz <= "0000000000000";
				clock_10Khz_int <= NOT(clock_10Khz_int);
			END IF;	

	END PROCESS;	

END a;

