-- Kairi Kozuma
-- GTID: 903050898
-- ECE 3056 Spring 2016
--
-- Multicycle MIPS Processor VHDL Behavioral Model
--
-- Idecode module (implements the register file)
--
-- Changes:
-- Increase MemtoReg and RegDst to 2 bits
-- Add a PC_plus_4 signal of 32 bits as input
-- Add PC_plus_4 signal as choice for write_data signal
-- Add "11111" register $31 as choice for write_register_address signal
-- Initialize register values
--     r0 : 0
--     r1 : 1
--     r2 : 2
--     r3 : 3
--     r5 : 3
--     otherwise : 1
LIBRARY IEEE;
USE IEEE.STD_LOGIC_1164.ALL;
USE IEEE.STD_LOGIC_ARITH.ALL;
USE IEEE.STD_LOGIC_UNSIGNED.ALL;

ENTITY Idecode IS
    PORT(-- INPUT SIGNALS
        PC_plus_4   : IN STD_LOGIC_VECTOR(31 DOWNTO 0);
        Instruction : IN STD_LOGIC_VECTOR(31 DOWNTO 0);
        read_data   : IN STD_LOGIC_VECTOR(31 DOWNTO 0);
        ALU_result  : IN STD_LOGIC_VECTOR(31 DOWNTO 0);
        RegWrite    : IN STD_LOGIC;
        MemtoReg    : IN STD_LOGIC_VECTOR( 1 DOWNTO 0);
        RegDst      : IN STD_LOGIC_VECTOR( 1 DOWNTO 0);
        clock       : IN STD_LOGIC;
        reset       : IN STD_LOGIC;
        -- OUTPUT SIGNALS
        read_data_1 : OUT STD_LOGIC_VECTOR(31 DOWNTO 0);
        read_data_2 : OUT STD_LOGIC_VECTOR(31 DOWNTO 0);
        Sign_extend : OUT STD_LOGIC_VECTOR(31 DOWNTO 0)
    );
END Idecode;

ARCHITECTURE behavior OF Idecode IS
    TYPE register_file IS ARRAY ( 0 TO 31) OF STD_LOGIC_VECTOR(31 DOWNTO 0);

    SIGNAL register_array              : register_file;
    SIGNAL write_register_address      : STD_LOGIC_VECTOR( 4 DOWNTO 0);
    SIGNAL write_data                  : STD_LOGIC_VECTOR(31 DOWNTO 0);
    SIGNAL read_register_1_address     : STD_LOGIC_VECTOR( 4 DOWNTO 0);
    SIGNAL read_register_2_address     : STD_LOGIC_VECTOR( 4 DOWNTO 0);
    SIGNAL write_register_address_1    : STD_LOGIC_VECTOR( 4 DOWNTO 0);
    SIGNAL write_register_address_0    : STD_LOGIC_VECTOR( 4 DOWNTO 0);
    SIGNAL Instruction_immediate_value : STD_LOGIC_VECTOR(15 DOWNTO 0);

BEGIN
read_register_1_address     <= Instruction( 25 DOWNTO 21 );
read_register_2_address     <= Instruction( 20 DOWNTO 16 );
write_register_address_1    <= Instruction( 15 DOWNTO 11 );
write_register_address_0    <= Instruction( 20 DOWNTO 16 );
Instruction_immediate_value <= Instruction( 15 DOWNTO  0 );

-- Mux for Register Write Address
write_register_address <=
    write_register_address_0 WHEN RegDst = "00" ELSE
    write_register_address_1 WHEN RegDst = "01" ELSE
    "11111"; -- $ra: $31

-- Mux to bypass data memory for Rformat instructions
Write_data <=
    ALU_result( 31 DOWNTO 0 ) WHEN ( MemtoReg = "00" ) ELSE
    read_data                 WHEN ( MemtoReg = "01" ) ELSE
    PC_plus_4; -- Return address

-- Sign Extend 16-bits to 32-bits
Sign_extend <=
    X"0000" & Instruction_immediate_value WHEN Instruction_immediate_value(15) = '0' ELSE
    X"FFFF" & Instruction_immediate_value;

PROCESS
BEGIN
    WAIT UNTIL (rising_edge(clock));
        IF reset = '1' THEN
        -- Initial register values on reset are register = reg# for r0 to r3
        -- r5 set to 3
        -- All other registers set to 1
            -- Set r0 to r3 to register = reg#
            FOR i IN 0 TO 3 LOOP
                register_array(i) <= CONV_STD_LOGIC_VECTOR(i, 32);
            END LOOP;
            -- Set all other registers to 1
            FOR i IN 4 TO 31 LOOP
                register_array(i) <= CONV_STD_LOGIC_VECTOR(1, 32);
            END LOOP;
        read_data_1 <= X"00000000";
        read_data_2 <= X"00000000";
        -- Write back to register - don't write to register 0
        ELSE
            IF RegWrite = '1' AND write_register_address /= 0 THEN
                register_array( CONV_INTEGER(write_register_address)) <= write_data;
            END IF;
            -- Read Register 1 Operation
            read_data_1 <= register_array(
            CONV_INTEGER( read_register_1_address ) );
            -- Read Register 2 Operation
            read_data_2 <= register_array(
            CONV_INTEGER( read_register_2_address ) );
        END IF;
END PROCESS;
END behavior;
