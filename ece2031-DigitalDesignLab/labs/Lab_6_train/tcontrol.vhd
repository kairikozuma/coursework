--
--
-- State machine to control trains
--
--

LIBRARY IEEE;
USE  IEEE.STD_LOGIC_1164.all;
USE  IEEE.STD_LOGIC_ARITH.all;
USE  IEEE.STD_LOGIC_UNSIGNED.all;


ENTITY Tcontrol IS
  PORT(reset, clock, sensor1, sensor2         : IN std_logic;
      sensor3, sensor4, sensor5, sensor6      : IN std_logic;
      switch1, switch2, switch3, switch4      : OUT std_logic;
      dirA, dirB                              : OUT std_logic_vector(1 DOWNTO 0));
END Tcontrol;


ARCHITECTURE a OF Tcontrol IS
  TYPE STATE_TYPE IS (M1Ain1Bin3, M1Ain2Bin3, M1Ain1Bin2, 
					  M1BstopS3 , M1AstopS1,
					  M4Ain2Bin3, M4Ain1Bin3, M4Ain4Bin2, M4Ain1Bin2,
					  M4AstopS1 , M4AstopS4 , M4BstopS3 , M4BstopS2);
  SIGNAL state                                  : STATE_TYPE;
  SIGNAL sensor12, sensor13, sensor34, sensor26 : std_logic_vector(1 DOWNTO 0);

BEGIN
  PROCESS (clock, reset)
  BEGIN
        -- Reset to this state
    IF reset = '1' THEN
      state <= M1Ain1Bin3;
    ELSIF clock'EVENT AND clock = '1' THEN
		-- Case statement to determine next state
      CASE state IS
        WHEN M1Ain1Bin3 =>
          CASE Sensor13 IS
            WHEN "00" => state <= M1Ain1Bin3;
            WHEN "01" => state <= M1Ain1Bin2;
            WHEN "10" => state <= M1Ain2Bin3;
            WHEN "11" => state <= M1BstopS3 ;
            WHEN OTHERS => state <= M1Ain1Bin3;
          END CASE;
        WHEN M1Ain2Bin3 =>
          CASE Sensor34 IS
            WHEN "00" => state <= M1Ain2Bin3;
            WHEN "01" => state <= M1Ain1Bin3;
            WHEN "10" => state <= M1BstopS3;
            WHEN "11" => state <= M1Ain1Bin2;
            WHEN OTHERS => state <= M1Ain1Bin3;
          END CASE;
        WHEN M1Ain1Bin2 =>
          CASE Sensor12 IS
            WHEN "00" => state <= M1Ain1Bin2;
            WHEN "01" => state <= M1Ain1Bin3;
            WHEN "10" => state <= M4AstopS1 ;
            WHEN "11" => state <= M1Ain2Bin3;
            WHEN OTHERS => state <= M1Ain1Bin3;
          END CASE;
        WHEN M4Ain2Bin3 =>
          CASE Sensor34 IS
            WHEN "00" => state <= M4Ain2Bin3;
            WHEN "01" => state <= M4AstopS4 ;
            WHEN "10" => state <= M4BstopS3 ;
            WHEN "11" => state <= M4Ain4Bin2;
            WHEN OTHERS => state <= M1Ain1Bin3;
          END CASE;
        WHEN M4Ain1Bin3 =>
          CASE Sensor13 IS
            WHEN "00" => state <= M4Ain1Bin3;
            WHEN "01" => state <= M4Ain1Bin2;
            WHEN "10" => state <= M4Ain2Bin3;
            WHEN "11" => state <= M4BstopS3 ;
            WHEN OTHERS => state <= M1Ain1Bin3;
          END CASE;
        WHEN M4Ain1Bin2 =>
          CASE Sensor12 IS
            WHEN "00" => state <= M4Ain1Bin2;
            WHEN "01" => state <= M4Ain1Bin3;
            WHEN "10" => state <= M1AstopS1 ;
            WHEN "11" => state <= M4Ain2Bin3;
            WHEN OTHERS => state <= M1Ain1Bin3;
          END CASE;
        WHEN M4Ain4Bin2 =>
          CASE Sensor26 IS
            WHEN "00" => state <= M4Ain4Bin2;
            WHEN "01" => state <= M4Ain1Bin2;
            WHEN "10" => state <= M4BstopS2 ;
            WHEN "11" => state <= M4Ain1Bin3;
            WHEN OTHERS => state <= M1Ain1Bin3;
          END CASE;
          
          -- stop states

        WHEN M1BstopS3 =>
          IF Sensor4 = '1' THEN
            state <= M1Ain1Bin2;
          ELSE
            state <= M1BstopS3;
          END IF;

        WHEN M4AstopS1 =>
          IF Sensor2 = '1' THEN
            state <= M4Ain2Bin3;
          ELSE
            state <= M4AstopS1;
          END IF;
        
        WHEN M4BstopS3 =>
          IF Sensor4 = '1' THEN
            state <= M4Ain4Bin2;
          ELSE
            state <= M4BstopS3;
          END IF;

        WHEN M4AstopS4=>
          IF Sensor3 = '1' THEN
            state <= M4Ain4Bin2;
          ELSE
            state <= M4AstopS4;
          END IF;
          
        WHEN M1AstopS1 =>
          IF Sensor2 = '1' THEN
            state <= M1Ain2Bin3;
          ELSE
            state <= M1AstopS1;
          END IF;

        WHEN M4BstopS2 =>
          IF Sensor6 = '1' THEN
            state <= M4Ain1Bin3;
          ELSE
            state <= M4BstopS2;
          END IF;

      END CASE;
    END IF;
  END PROCESS;

	  -- combine bits for sensor bits
  sensor12 <= sensor1 & sensor2;
  sensor13 <= sensor1 & sensor3;
  sensor34 <= sensor3 & sensor4;
  sensor26 <= sensor2 & sensor6;

      -- Outputs that depend on state
  
      -- set Switch1 to '1' when B is in track 2
      -- set to '0' otherwise
  WITH state SELECT
    Switch1 <=  
		  '1' WHEN M1Ain1Bin2,
          '1' WHEN M4AstopS1 ,
          '1' WHEN M1AstopS1 ,
          '1' WHEN M4Ain4Bin2,
          '1' WHEN M4Ain1Bin2,
          '0' WHEN OTHERS;
      -- set Switch2 to '1' when B is in track 2
      -- set to '0' otherwise
  WITH state SELECT
    Switch2 <= 
		  '1' WHEN M1Ain1Bin2,
          '1' WHEN M4AstopS1 ,
          '1' WHEN M1AstopS1 ,
          '1' WHEN M4Ain4Bin2,
          '1' WHEN M4Ain1Bin2,
          '0' WHEN OTHERS;
      -- set Switch3 to '1' when in M4 (mode 4)
      -- set to '0' otherwise
  WITH state SELECT
    Switch3 <=  
		  '1' WHEN M4Ain2Bin3,
		  '1' WHEN M4Ain1Bin3, 
		  '1' WHEN M4Ain4Bin2,
		  '1' WHEN M4Ain1Bin2,
		  '1' WHEN M4AstopS1 , 
		  '1' WHEN M4AstopS4 , 
		  '1' WHEN M4BstopS3 , 
		  '1' WHEN M4BstopS2 ,
		  '0' WHEN OTHERS    ;
      -- set Switch3 to '1' when in M4 (mode 4)
      -- set to '0' otherwise
  WITH state SELECT
    Switch4 <=  
		  '1' WHEN M4Ain2Bin3,
		  '1' WHEN M4Ain1Bin3, 
		  '1' WHEN M4Ain4Bin2,
		  '1' WHEN M4Ain1Bin2,
		  '1' WHEN M4AstopS1 , 
		  '1' WHEN M4AstopS4 , 
		  '1' WHEN M4BstopS3 , 
		  '1' WHEN M4BstopS2 ,
		  '0' WHEN OTHERS    ;
      -- set DirA to "00" when stopped
      -- set counter clockwise direction "01" as default
  WITH state SELECT
    DirA  <=
          "00" WHEN M1AstopS1,
          "00" WHEN M4AstopS1,
          "00" WHEN M4AstopS4,
          "01" WHEN OTHERS;
      -- set DirB to "00" when stopped
      -- set counter clockwise direction "10" as default
  WITH state SELECT
    DirB  <=  
          "00" WHEN M1BstopS3,
          "00" WHEN M4BstopS2,
          "00" WHEN M4BstopS3,
          "01" WHEN OTHERS;
END a;


