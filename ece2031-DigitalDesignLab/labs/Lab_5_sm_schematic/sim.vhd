-- Do not modify this file!
LIBRARY IEEE;
LIBRARY LPM;

USE IEEE.STD_LOGIC_1164.ALL;
USE IEEE.STD_LOGIC_UNSIGNED.ALL;
USE LPM.LPM_COMPONENTS.ALL;

ENTITY SIM IS
  PORT(Z,
       Q1,
       Q0,
       CLK_25MHZ,
       RESET_SIM_N,
       RUN_SIM_N    : IN  STD_LOGIC;
       PASSED,
       H_SYNC,
       V_SYNC,
       R_OUT,
       G_OUT,
       B_OUT,
       VIDEO_BLANK,
       VIDEO_CLOCK,
       X1,
       X0,
       CLOCK,
       RESET_N,
       FAILED     : OUT STD_LOGIC );
END SIM;


ARCHITECTURE behavior of SIM IS
  TYPE   SIM_STATE IS (WAITING0, WAITING, RUNNING, TEST, STALL, DONE);
  TYPE   BUFFER_STATE IS (IDLE, SETUP, WRITING, STOPWRITING);
  TYPE   DATA_BUFFER IS ARRAY (0 TO 7) OF STD_LOGIC_VECTOR(5 DOWNTO 0);
  TYPE   ADDR_BUFFER IS ARRAY (0 TO 7) OF STD_LOGIC_VECTOR(10 DOWNTO 0);

  SIGNAL state         : SIM_STATE;
  SIGNAL VB_STATE      : BUFFER_STATE;
  SIGNAL DBUFFER       : DATA_BUFFER;
  SIGNAL ABUFFER       : ADDR_BUFFER;
  SIGNAL cindex,
         bindex        : INTEGER RANGE 0 TO 7;
  SIGNAL clk_sim_cnt   : INTEGER RANGE 0 TO 2500;
  SIGNAL AX1,
         AX0,
         AEXP,
         AACT,
         AZEXP,
         AZACT,
         VB_ADDR       : STD_LOGIC_VECTOR(10 DOWNTO 0);
  SIGNAL prow,
         pcol          : STD_LOGIC_VECTOR(9 DOWNTO 0);
  SIGNAL CR_CHAR       : STD_LOGIC_VECTOR(8 DOWNTO 0);
  SIGNAL CR_LINE       : STD_LOGIC_VECTOR(7 DOWNTO 0);
  SIGNAL VB_DATA,
         VB_CHAR       : STD_LOGIC_VECTOR(5 DOWNTO 0);
  SIGNAL TEST_ADDRESS,
         TEST_VECTOR   : STD_LOGIC_VECTOR(4 DOWNTO 0);
  SIGNAL CHARS2WRITE,
         EXP_out,
         ACT_out,
         RGB,
         RGB_BACK      : STD_LOGIC_VECTOR(2 DOWNTO 0);
  SIGNAL EXPECTED      : STD_LOGIC_VECTOR(1 DOWNTO 0);
  SIGNAL X1_LAST,
         X0_LAST,
         HS,
         CLR_WRITE,
         TEST_FAILED,
         VB_WE,
         CLOCK_SIM     : STD_LOGIC;


  -- Prototype declaration for the VGA sync module
  COMPONENT VGA_SYNC
    PORT (
      clock_25Mhz,
      red,
      green,
      blue            : IN  STD_LOGIC;
      red_out,
      green_out,
      blue_out        : OUT STD_LOGIC;
      horiz_sync_out,
      vert_sync_out,
      video_blank,
      video_clock     : OUT STD_LOGIC;
      pixel_row,
      pixel_column    : OUT STD_LOGIC_VECTOR(9 DOWNTO 0) );
  END COMPONENT;


  -- Prototype declaration for the synchronous RAM module
  COMPONENT altsyncram
    GENERIC (
      address_aclr_a                     : STRING  := "NONE";
      address_aclr_b                     : STRING  := "NONE";
      address_reg_b                      : STRING  := "CLOCK0";
      indata_aclr_a                      : STRING  := "NONE";
      init_file                          : STRING  := "";
      intended_device_family             : STRING  := "Cyclone";
      lpm_type                           : STRING  := "altsyncram";
      numwords_a                         : NATURAL := 256;
      numwords_b                         : NATURAL := 256;
      operation_mode                     : STRING  := "DUAL_PORT";
      outdata_aclr_a                     : STRING  := "NONE";
      outdata_aclr_b                     : STRING  := "NONE";
      outdata_reg_a                      : STRING  := "CLOCK0";
      outdata_reg_b                      : STRING  := "CLOCK1";
      power_up_uninitialized             : STRING  := "FALSE";
      read_during_write_mode_mixed_ports : STRING  := "DONT_CARE";
      widthad_a                          : NATURAL := 8;
      widthad_b                          : NATURAL := 8;
      width_a                            : NATURAL := 8;
      width_b                            : NATURAL := 8;
      width_byteena_a                    : NATURAL := 1;
      wrcontrol_aclr_a                   : STRING  := "NONE" );
    PORT (
      wren_a    : IN  STD_LOGIC;
      clock0    : IN  STD_LOGIC;
      clock1    : IN  STD_LOGIC;
      address_a : IN  STD_LOGIC_VECTOR ((widthad_a - 1) DOWNTO 0);
      address_b : IN  STD_LOGIC_VECTOR ((widthad_b - 1) DOWNTO 0);
      data_a    : IN  STD_LOGIC_VECTOR ((width_a - 1) DOWNTO 0);
      q_a       : OUT STD_LOGIC_VECTOR ((width_a - 1) DOWNTO 0);
      q_b       : OUT STD_LOGIC_VECTOR ((width_b - 1) DOWNTO 0) );
 END COMPONENT;


  BEGIN
    -- Video buffer memory
    vbuffer : altsyncram
    GENERIC MAP (
      init_file                          => "vbuffer.mif",
      numwords_a                         => 1200,
      numwords_b                         => 1200,
      widthad_a                          => 11,
      widthad_b                          => 11,
      width_a                            => 6,
      width_b                            => 6 )
    PORT MAP (
      wren_a    => VB_WE,
      clock0    => CLK_25MHZ,
      clock1    => NOT(CLK_25MHZ),
      address_a => VB_ADDR,
      address_b => VB_ADDR,
      data_a    => VB_DATA,
      q_b       => VB_CHAR );


    -- Alphanumeric character ROM
    charrom : altsyncram
    GENERIC MAP (
      init_file                          => "charrom.mif",
      numwords_a                         => 512,
      outdata_reg_a                      => "CLOCK1",
      operation_mode                     => "ROM",
      widthad_a                          => 9,
      width_a                            => 8,
      width_byteena_a                    => 1 )
    PORT MAP (
      clock0    => CLK_25MHZ,
      clock1    => NOT(CLK_25MHZ),
      address_a => CR_CHAR,
      q_a       => CR_LINE );


    -- Memory space for state machine test vectors
    vectors : altsyncram
    GENERIC MAP (
      init_file                          => "vectors.mif",
      numwords_a                         => 32,
      outdata_reg_a                      => "CLOCK1",
      operation_mode                     => "ROM",
      widthad_a                          => 5,
      width_a                            => 5,
      width_byteena_a                    => 1 )
    PORT MAP (
      clock0    => CLK_25MHZ,
      clock1    => NOT(CLK_25MHZ),
      address_a => TEST_ADDRESS,
      q_a       => TEST_VECTOR );


    vga: VGA_SYNC
      PORT MAP (
        clock_25Mhz    => CLK_25MHZ,
        red            => RGB(2),
        green          => RGB(1),
        blue           => RGB(0),
        red_out        => R_out,
        green_out      => G_out,
        blue_out       => B_out,
        horiz_sync_out => HS,
        vert_sync_out  => V_sync,
        video_blank    => VIDEO_BLANK,
        video_clock    => VIDEO_CLOCK,
        pixel_row      => prow,
        pixel_column   => pcol );


    PROCESS(CLK_25MHZ, RESET_SIM_N)
      BEGIN
        IF (RESET_SIM_N = '0') THEN
          clk_sim_cnt <= 0;
          CLOCK_SIM   <= '0';
        ELSIF (CLK_25MHZ'EVENT and CLK_25MHZ = '1') THEN
          IF (clk_sim_cnt = 2500) THEN
            clk_sim_cnt <= 0;
          ELSE
            clk_sim_cnt <= clk_sim_cnt + 1;
          END IF;

          IF (clk_sim_cnt < 1250) THEN
            CLOCK_SIM <= '0';
          ELSE
            CLOCK_SIM <= '1';
          END IF;
        END IF;
      END PROCESS;

--    CLOCK_SIM <= '0' WHEN (clk_sim_cnt < 1250) ELSE '1';

    PROCESS(RESET_SIM_N, CLOCK_SIM)
      BEGIN
        IF (RESET_SIM_N = '0') THEN
          state        <= WAITING0;
          CLOCK        <= '0';
          TEST_ADDRESS <= "00000";
          TEST_FAILED  <= '0';
          PASSED       <= '0';
          RESET_N      <= '0';
          RGB_BACK     <= "001";
          DBUFFER(0)   <= "000000";
          DBUFFER(1)   <= "000000";
          DBUFFER(2)   <= "000000";
          DBUFFER(3)   <= "000000";
          DBUFFER(4)   <= "000000";
          DBUFFER(5)   <= "000000";
          DBUFFER(6)   <= "000000";
          DBUFFER(7)   <= "000000";
          ABUFFER(0)   <= "00000000000";
          ABUFFER(1)   <= "00000000000";
          ABUFFER(2)   <= "00000000000";
          ABUFFER(3)   <= "00000000000";
          ABUFFER(4)   <= "00000000000";
          ABUFFER(5)   <= "00000000000";
          ABUFFER(6)   <= "00000000000";
          ABUFFER(7)   <= "00000000000";

          AX1          <= "00011001111";
          AX0          <= "00011110111";
          AEXP         <= "00101000111";
          AACT         <= "00101101111";
          AZEXP        <= "00110111111";
          AZACT        <= "00111100111";
        ELSE
          IF (CLOCK_SIM'EVENT AND CLOCK_SIM = '0') THEN
            CASE state IS
              WHEN WAITING =>
                    CLOCK        <= '0';
                    RESET_N      <= '1';
--                    X1_LAST      <= TEST_VECTOR(3);
--                    X0_LAST      <= TEST_VECTOR(2);
--                    EXPECTED     <= TEST_VECTOR(1 DOWNTO 0);
                    TEST_ADDRESS <= "00001";
                    state        <= STALL;
                    DBUFFER      <= DBUFFER;
                    ABUFFER      <= ABUFFER;

              WHEN WAITING0 =>
                IF (RUN_SIM_N = '0') THEN
                  IF (TEST_VECTOR(4) = '1') THEN
                    CLOCK    <= '0';
                    RESET_N  <= '1';
                    X1_LAST  <= TEST_VECTOR(3);
                    X0_LAST  <= TEST_VECTOR(2);
                    EXPECTED <= TEST_VECTOR(1 DOWNTO 0);
                    state    <= WAITING;
                    DBUFFER  <= DBUFFER;
                    ABUFFER  <= ABUFFER;
                  ELSE
                    CLOCK       <= '0';
                    RGB_BACK    <= "000";
                    TEST_FAILED <= '1';
                    PASSED      <= '1';
                    state       <= DONE;
                    DBUFFER(0)  <= "011111";
                    DBUFFER(1)  <= "001110";
                    DBUFFER(2)  <= "001100";
                    DBUFFER(3)  <= "011101";
                    DBUFFER(4)  <= "011000";
                    DBUFFER(5)  <= "011011";
                    DBUFFER(6)  <= "011100";
                    DBUFFER(7)  <= DBUFFER(7);
                    ABUFFER(0)  <= "00000111000";
                    ABUFFER(1)  <= "00000111001";
                    ABUFFER(2)  <= "00000111010";
                    ABUFFER(3)  <= "00000111011";
                    ABUFFER(4)  <= "00000111100";
                    ABUFFER(5)  <= "00000111101";
                    ABUFFER(6)  <= "00000111110";
                    ABUFFER(7)  <= ABUFFER(7);
                    CHARS2WRITE <= "111";
                  END IF;
                ELSE
                  CLOCK        <= '0';
                  TEST_ADDRESS <= "00000";
                  TEST_FAILED  <= '0';
                  PASSED       <= '0';
                  RESET_N      <= '0';
                  RGB_BACK     <= "001";
                  AX1          <= "00011001111";
                  AX0          <= "00011110111";
                  AEXP         <= "00101000111";
                  AACT         <= "00101101111";
                  AZEXP        <= "00110111111";
                  AZACT        <= "00111100111";
                  DBUFFER      <= DBUFFER;
                  ABUFFER      <= ABUFFER;
                END IF;

              WHEN RUNNING =>
                CLOCK   <= '0';
                state   <= TEST;
                DBUFFER <= DBUFFER;
                ABUFFER <= ABUFFER;

              WHEN TEST =>
                DBUFFER(0)  <= ("00000" & X1_LAST);
                DBUFFER(1)  <= ("00000" & X0_LAST);
                DBUFFER(2)  <= ("001" & EXP_out);
                DBUFFER(3)  <= ("001" & ACT_out);
                DBUFFER(4)  <= ("00000" & EXPECTED(1));
                DBUFFER(5)  <= ("00000" & Z);
                DBUFFER(6)  <= DBUFFER(6);   
                DBUFFER(7)  <= DBUFFER(7);   
                ABUFFER(0)  <= AX1;
                ABUFFER(1)  <= AX0;
                ABUFFER(2)  <= AEXP;
                ABUFFER(3)  <= AACT;
                ABUFFER(4)  <= AZEXP;
                ABUFFER(5)  <= AZACT;
                ABUFFER(6)  <= ABUFFER(6);   
                ABUFFER(6)  <= ABUFFER(6);   
                CHARS2WRITE <= "110";
                AX1         <= AX1 + "00000000001";
                AX0         <= AX0 + "00000000001";
                AEXP        <= AEXP + "00000000001";
                AACT        <= AACT + "00000000001";
                AZEXP       <= AZEXP + "00000000001";
                AZACT       <= AZACT + "00000000001";

                IF NOT (EXPECTED = (Q1 & Q0) AND Z = Q1) THEN
                  RGB_BACK    <= "000";
                  TEST_FAILED <= '1';
                END IF;

                IF (TEST_VECTOR(4) = '0') THEN
                  RGB_BACK <= ('0' & NOT(TEST_FAILED) & '0');
                  PASSED   <= NOT(TEST_FAILED);
                  state    <= DONE;
                ELSE
                  X1_LAST      <= TEST_VECTOR(3);
                  X0_LAST      <= TEST_VECTOR(2);
                  EXPECTED     <= TEST_VECTOR(1 DOWNTO 0);
                  TEST_ADDRESS <= TEST_ADDRESS + "00001";
                  state        <= STALL;
                END IF;

              WHEN STALL =>
                CHARS2WRITE <= "000";
                CLOCK       <= '1';
                DBUFFER     <= DBUFFER;
                ABUFFER     <= ABUFFER;
                state       <= RUNNING;
              WHEN DONE =>
            END CASE;
          END IF;
        END IF;
      END PROCESS;

    PROCESS(CLK_25MHZ)
      BEGIN 
        IF (CLK_25MHZ'EVENT and CLK_25MHZ = '0') THEN
          CASE VB_STATE IS
            WHEN IDLE =>
              IF (CLR_WRITE = '1') THEN
                IF (CHARS2WRITE = "000") THEN
                  CLR_WRITE <= '0';
                END IF;
              ElSIF (CHARS2WRITE /= "000" AND HS = '0') THEN
                bindex   <= CONV_INTEGER(CHARS2WRITE) - 1;
                VB_STATE <= SETUP;
              END IF;

              VB_ADDR <= ((prow(9 DOWNTO 4) & "00000") +
                          ("00" & prow(9 DOWNTO 4) & "000") +
                          ("00000" & pcol(9 DOWNTO 4)));
            WHEN SETUP =>
              VB_DATA  <= DBUFFER(bindex);
              VB_ADDR  <= ABUFFER(bindex);
              VB_STATE <= WRITING;
            WHEN WRITING =>
              VB_WE    <= '1';
              VB_STATE <= STOPWRITING;
            WHEN STOPWRITING =>
              VB_WE <= '0';

              IF (bindex = 0) THEN
                CLR_WRITE <= '1';
                VB_STATE  <= IDLE;
              ELSE
                bindex   <= bindex - 1;
                VB_STATE <= SETUP;
              END IF;
          END CASE;
        END IF;
      END PROCESS;

    PROCESS (cindex, CR_LINE, RGB_BACK)
      BEGIN
        IF (CR_LINE(cindex) = '1') THEN
          RGB <= "111";
        ELSE
          RGB <= RGB_BACK;
        END IF;
      END PROCESS;

    cindex  <= 7 - CONV_INTEGER(pcol(3 DOWNTO 1));
    CR_CHAR <= (VB_CHAR & prow(3 DOWNTO 1));
    FAILED  <= TEST_FAILED;
    X1      <= X1_LAST;
    X0      <= X0_LAST;
    EXP_out <= (('0' & EXPECTED) + "010");
    ACT_out <= (('0' & Q1 & Q0) + "010");
    H_sync  <= HS;
  END behavior;

