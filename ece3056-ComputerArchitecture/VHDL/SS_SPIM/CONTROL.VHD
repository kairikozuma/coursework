-- ECE 3055 Computer Architecture and Operating Systems
--
-- MIPS Processor VHDL Behavioral Model
--		
-- control module (implements MIPS control unit)
--
-- School of Electrical & Computer Engineering
-- Georgia Institute of Technology
-- Atlanta, GA 30332
-- 
LIBRARY IEEE;
USE IEEE.STD_LOGIC_1164.ALL;
USE IEEE.STD_LOGIC_ARITH.ALL;
USE IEEE.STD_LOGIC_SIGNED.ALL;

ENTITY control IS
   PORT( 	
	SIGNAL Opcode 		: IN 	STD_LOGIC_VECTOR( 5 DOWNTO 0 );
	SIGNAL RegDst 		: OUT 	STD_LOGIC;
	SIGNAL ALUSrc 		: OUT 	STD_LOGIC;
	SIGNAL MemtoReg 	: OUT 	STD_LOGIC;
	SIGNAL RegWrite 	: OUT 	STD_LOGIC;
	SIGNAL MemRead 		: OUT 	STD_LOGIC;
	SIGNAL MemWrite 	: OUT 	STD_LOGIC;
	SIGNAL Branch 		: OUT 	STD_LOGIC;
	SIGNAL ALUop 		: OUT 	STD_LOGIC_VECTOR( 1 DOWNTO 0 );
	SIGNAL clock, reset	: IN 	STD_LOGIC );

END control;

ARCHITECTURE behavior OF control IS

	SIGNAL  R_format, Lw, Sw, Beq 	: STD_LOGIC;

BEGIN           
				-- Code to generate control signals using opcode bits
	R_format 	<=  '1'  WHEN  Opcode = "000000"  ELSE '0';
	Lw          <=  '1'  WHEN  Opcode = "100011"  ELSE '0';
 	Sw          <=  '1'  WHEN  Opcode = "101011"  ELSE '0';
   	Beq         <=  '1'  WHEN  Opcode = "000100"  ELSE '0';
  	RegDst    	<=  R_format;
 	ALUSrc  	<=  Lw OR Sw;
	MemtoReg 	<=  Lw;
  	RegWrite 	<=  R_format OR Lw;
  	MemRead 	<=  Lw;
   	MemWrite 	<=  Sw; 
 	Branch      <=  Beq;
	ALUOp( 1 ) 	<=  R_format;
	ALUOp( 0 ) 	<=  Beq; 

   END behavior;


