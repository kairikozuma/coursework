-- ECE 3055 Computer Architecture and Operating Systems
--
-- Multicycle MIPS Processor VHDL Behavioral Model
--				
-- Top Level Structural Model for MIPS Processor Core
--
-- School of Electrical & Computer Engineering
-- Georgia Institute of Technology
-- Atlanta, GA 30332
--
LIBRARY IEEE;
USE IEEE.STD_LOGIC_1164.ALL;
USE IEEE.STD_LOGIC_ARITH.ALL;

ENTITY MIPS IS

	PORT( 
	-- Output important signals to pins for easy display in Simulator
	   

	   reset_out, sys_clock_out				             : OUT STD_LOGIC; 
		PC_out, read_data_1_out,Instruction_out		: OUT STD_LOGIC_VECTOR( 31 DOWNTO 0 );
		ALU_result_out, read_data_2_out          : OUT STD_LOGIC_VECTOR( 31 DOWNTO 0 );
		s_ALUOut_out, write_data_out 		   	      : OUT STD_LOGIC_VECTOR( 31 DOWNTO 0 );
		Zero_out, Memwrite_out,Regwrite_out			   : OUT STD_LOGIC );
END 	MIPS;
									  
ARCHITECTURE structure OF MIPS IS

	COMPONENT my_clock
	    PORT (sys_clock, reset : OUT STD_LOGIC);
	    END COMPONENT;
	        
	COMPONENT Ifetch
   	     PORT(	-- input signals 
   	      PC_Plus_4	          	: IN 	STD_LOGIC_VECTOR( 31 DOWNTO 0 );
        	Branch_PC           	: IN 	STD_LOGIC_VECTOR( 31 DOWNTO 0 );
        	PCWrite, PCWriteCond, IRWrite : IN STD_LOGIC;
        	PCSource 			: IN STD_LOGIC_VECTOR(1 DOWNTO 0);
        	Zero, MemRead, MemWrite  : IN STD_LOGIC;
        	clock, reset, IorD	  : IN STD_LOGIC;
        	memory_data_in   	  : IN std_logic_vector(31 downto 0);
        	memory_addr_in   	  : IN std_logic_vector(31 downto 0);
        	
		-- Output signals

        	PC_out 			  : OUT STD_LOGIC_VECTOR( 31 DOWNTO 0 );
        	Memory_data_out 		  : OUT STD_LOGIC_VECTOR( 31 DOWNTO 0 );
        	Instruction 		  : OUT STD_LOGIC_VECTOR( 31 DOWNTO 0 ));
END COMPONENT Ifetch;

	COMPONENT Idecode	
	PORT(		read_data_1	: OUT STD_LOGIC_VECTOR( 31 DOWNTO 0 );
			read_data_2	: OUT STD_LOGIC_VECTOR( 31 DOWNTO 0 );
			Sign_extend : OUT STD_LOGIC_VECTOR( 31 DOWNTO 0 );

			Instruction : IN 	STD_LOGIC_VECTOR( 31 DOWNTO 0 );
			read_data 	: IN 	STD_LOGIC_VECTOR( 31 DOWNTO 0 );
			ALU_result	: IN 	STD_LOGIC_VECTOR( 31 DOWNTO 0 );
			RegWrite  	: IN 	STD_LOGIC;
			MemtoReg 	: IN 	STD_LOGIC;
			RegDst 	: IN 	STD_LOGIC;
			clock,reset	: IN 	STD_LOGIC );
END COMPONENT Idecode;


	COMPONENT control
PORT( 
	Opcode 		: IN 	STD_LOGIC_VECTOR( 5 DOWNTO 0 );
	clock, reset	: IN 	STD_LOGIC;
	 
	PCWrite     : OUT STD_LOGIC;
   	PCWriteCond : OUT STD_LOGIC;
   	IorD        : OUT STD_LOGIC;
   	MemRead 	: OUT STD_LOGIC;
   	MemWrite 	: OUT STD_LOGIC;
  	IRWrite     : OUT STD_LOGIC;
   	MemtoReg    : OUT STD_LOGIC;
   	PCSource    : OUT STD_LOGIC_VECTOR (1 DOWNTO 0);
   	ALUOp       : OUT STD_LOGIC_VECTOR(1 DOWNTO 0);
   	ALUSrcB 	: OUT STD_LOGIC_VECTOR(1 DOWNTO 0);
   	ALUSrcA     : OUT STD_LOGIC;
   	RegWrite 	: OUT STD_LOGIC;
   	RegDst      : OUT STD_LOGIC;
   	micropc : out integer);

END COMPONENT control;


	COMPONENT  Execute	
	PORT(		Read_data_1 	   : IN 	STD_LOGIC_VECTOR( 31 DOWNTO 0 );
			Read_data_2 	   : IN 	STD_LOGIC_VECTOR( 31 DOWNTO 0 );
			Sign_extend 	   : IN 	STD_LOGIC_VECTOR( 31 DOWNTO 0 );
			Function_opcode 	   : IN 	STD_LOGIC_VECTOR( 5 DOWNTO 0 );
			ALUOp 		   : IN 	STD_LOGIC_VECTOR( 1 DOWNTO 0 );
			ALUSrcB		   : IN 	STD_LOGIC_VECTOR( 1 DOWNTO 0 );
			ALUSrcA 		   : IN 	STD_LOGIC;
			PC 		         : IN 	STD_LOGIC_VECTOR( 31 DOWNTO 0 );
			clock, reset	   : IN 	STD_LOGIC;
			
			Zero 			   : OUT	STD_LOGIC;
			ALU_Result 		   : OUT	STD_LOGIC_VECTOR( 31 DOWNTO 0 );
			ALUOut 		   : OUT	STD_LOGIC_VECTOR( 31 DOWNTO 0 ));
END COMPONENT Execute;

					-- declare signals used to connect VHDL components
	SIGNAL s_PC_Out   		: STD_LOGIC_VECTOR( 31 DOWNTO 0 );
	SIGNAL s_Instruction		: STD_LOGIC_VECTOR( 31 DOWNTO 0 );
	SIGNAL s_read_data_1 		: STD_LOGIC_VECTOR( 31 DOWNTO 0 );
	SIGNAL s_read_data_2 		: STD_LOGIC_VECTOR( 31 DOWNTO 0 );
	SIGNAL s_memory_data_out 	: STD_LOGIC_VECTOR( 31 DOWNTO 0 );
	SIGNAL s_Sign_Extend 		: STD_LOGIC_VECTOR( 31 DOWNTO 0 );
	SIGNAL s_ALU_result 		: STD_LOGIC_VECTOR( 31 DOWNTO 0 );
	SIGNAL s_ALUOut 		   	: STD_LOGIC_VECTOR( 31 DOWNTO 0 );
	SIGNAL s_ALUSrcB 	   		: STD_LOGIC_vector(1 downto 0);
	SIGNAL s_ALUop 			: STD_LOGIC_VECTOR(1 DOWNTO 0 );
	SIGNAL s_PCSource    		: STD_LOGIC_VECTOR (1 DOWNTO 0);
	
   SIGNAL s_reset, s_sys_clock, s_ALUSrcA, s_RegDst 				: STD_LOGIC;
	SIGNAL s_Regwrite, s_Zero, s_MemWrite, s_MemRead, s_MemToReg	: STD_LOGIC;
	SIGNAL s_PCWrite, s_PCWriteCond, s_IRWrite, s_IorD 			: STD_LOGIC;	
	signal s_micropc 									: integer;

BEGIN
					-- copy important signals to output pins for easy 
					-- display in Simulator
   Instruction_out   <= s_Instruction;
   ALU_result_out   <= s_ALU_result;
   s_ALUOut_out    <= s_ALUOut;
   read_data_1_out 	<= s_read_data_1;
   read_data_2_out 	<= s_read_data_2;
   write_data_out  	<= s_memory_data_out WHEN s_MemtoReg = '1' ELSE s_ALUOut;
   Zero_out 		<= s_Zero;
   RegWrite_out 	   	<= s_RegWrite;
   MemWrite_out 	   	<= s_MemWrite;	
   reset_out       	<= s_reset;
   sys_clock_out    	<= s_sys_clock;	 
   PC_Out 			<= s_PC_Out;

					-- connect the MIPS components   
  MC: my_clock
  PORT MAP (sys_clock 	=> s_sys_clock,
            reset 	=> s_reset
            );
  IFE : Ifetch
PORT MAP (PC_Plus_4	 => s_ALU_Result,    
        	 Branch_PC   => s_ALUOut,    
        	PCWrite      => s_PCWrite,
        	PCWriteCond  => s_PCWriteCond,
        	IRWrite      => s_IRWrite,
        	PCSource     => s_PCSource,
        	IorD         => s_IorD,
       	 Zero        => s_Zero,
       	 MemRead     => s_MemRead,
      	 MemWrite    => s_MemWrite,    
        	clock        => s_sys_clock,
       	reset        => s_reset,
       	memory_data_in => s_read_data_2,  
        	memory_addr_in => s_ALUOut,
       	PC_out 	   => s_PC_Out,  
       	Memory_data_out 	=> s_memory_data_out,			
       	Instruction 	=> s_instruction);   

 ID : Idecode
  	PORT MAP (	
		read_data_1		=> s_read_data_1,
		read_data_2		=> s_read_data_2,
		Sign_extend 	=> s_Sign_extend,   
		Instruction       => s_instruction,
		read_data 	      => s_memory_data_out,
		ALU_result	      => s_ALUOut,
		RegWrite 	      => s_RegWrite,
		MemtoReg 	      => s_MemToReg,
		RegDst 		=> s_RegDst,
		clock             => s_sys_clock,
		reset	            => s_reset);


   CTL:   control
	PORT MAP ( 
		Opcode 	=> s_instruction(31 downto 26), 
		clock       => s_sys_clock,
		reset	      => s_reset,
		PCWrite     => s_PCWrite,  
   		PCWriteCond  => s_PCWriteCond,
   		IorD        => s_IorD, 
   		MemRead  	=> s_MemRead,  
   		MemWrite 	=> s_MemWrite, 
   		IRWrite     => s_IRWrite,
   		MemtoReg    => s_MemToReg,
   		PCSource    => s_PCSource,
   		ALUOp       => s_ALUOp,
   		ALUSrcB 	=> s_ALUSrcB,
   		ALUSrcA     => s_ALUSrcA,
   		RegWrite 	=> s_RegWrite, 
   		RegDst      => s_RegDst,
		micropc 	=> s_micropc);

  EXE:  Execute
  	PORT MAP (	Read_data_1 	=> s_read_data_1,    
			Read_data_2 	=> s_read_data_2,
			Sign_extend 	=> s_sign_extend,
			Function_opcode   => s_instruction(5 downto 0),
			ALUOp 		=> s_ALUOp,
			ALUSrcB		=> s_ALUSrcB,
			ALUSrcA 		=> s_ALUSrcA,
			PC 		      => s_PC_Out,
			clock             => s_sys_clock,
			reset	            => s_reset,
			Zero 			=> s_Zero,     
			ALU_Result        => s_ALU_Result,		 
			ALUOut 		=> s_ALUOut );

   
END structure;

