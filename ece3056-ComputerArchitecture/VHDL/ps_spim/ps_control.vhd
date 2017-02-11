--
-- control unit. simply implements the truth table for a small set of
-- instructions 
--
--

Library IEEE;
use IEEE.std_logic_1164.all;

entity control is
port(opcode: in std_logic_vector(5 downto 0);
     RegDst, MemRead, MemToReg, MemWrite :out  std_logic;
     ALUSrc, RegWrite, Branch: out std_logic;
     ALUOp: out std_logic_vector(1 downto 0));
end control;

architecture behavioral of control is

signal rformat, lw, sw, beq  :std_logic; -- define local signals
				    -- corresponding to instruction
				    -- type 
 begin 
--
-- recognize opcode for each instruction type
-- these variable should be inferred as wires	 

	rformat 	<=  '1'  WHEN  Opcode = "000000"  ELSE '0';
	Lw          <=  '1'  WHEN  Opcode = "100011"  ELSE '0';
 	Sw          <=  '1'  WHEN  Opcode = "101011"  ELSE '0';
   	Beq         <=  '1'  WHEN  Opcode = "000100"  ELSE '0';

--
-- implement each output signal as the column of the truth
-- table  which defines the control
--

RegDst <= rformat;
ALUSrc <= (lw or sw) ;

MemToReg <= lw ;
RegWrite <= (rformat or lw);
MemRead <= lw ;
MemWrite <= sw;	   
Branch <= beq;

ALUOp(1 downto 0) <=  rformat & '0'; -- note the use of the concatenation operator
				     -- to form  2 bit signal

end behavioral;
