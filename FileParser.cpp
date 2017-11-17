/*
 * FileParser.cpp
 *
 *  Created on: Nov 6, 2017
 *      Author: merinsan
 */
#include "FileParser.h"
#include "fetchUnit.h"
#include "instr_cat.h"

map <int, fetchUnit::InstrChar> fetchUnit::address_instructions;

void FileParser::insertval(int *val_array, int val1, int val2, int val3, int val4){
  val_array[2] = val1;
  val_array[3] = val2;
  val_array[4] = val3;
  val_array[5] = val4;
}

void FileParser::readMIPSFile(const std::string &MIPSFilename){
	// create a file-reading object and open the MIPS binary file for input
	ifstream fin(MIPSFilename.c_str());
	if (!fin.good()){
		printf("File has issues \n");
		return;
	}


	int count = 0;
	int PC    = PC_START;

	while (fin && getline(fin, buffer)){
		count +=1;
		if (buffer[buffer.length()-1] == '\r' || buffer[buffer.length()-1] == '\n' ) {
		  buffer.erase(buffer.length()-1);
		}

		if (buffer.length() == 0)continue;
		int ins[MAX_DATA_PER_INSTR] = {-1,-1,-1,-1,-1};

		char *buf = new char[buffer.length() + 1];
		strcpy(buf, buffer.c_str());

		utilityFunctions util;
		ins[0]   = util.binarytoint(buf, (INST_START-CAT_START));
		ins[1]   = util.binarytoint(&buf[INST_START], (INFO_START- INST_START));

		switch(ins[0]){
			case CATEGORY1:
				switch(ins[1]){
				  case OP_J:
					jump_mem = util.binarytoint(&buf[INFO_START], 26);
					mem_eff  = ((PC+4)& 0xF0000000)|(jump_mem<<2);
					insertval(ins,1,jump_mem<<2,0,0);
					break;

				  case OP_JR:
					jr_mem = util.binarytoint(&buf[INFO_START], 5);
					insertval(ins,1,jr_mem,0,0);
					break;

				  case OP_BEQ:
					//rs rt
					rs = util.binarytoint(&buf[INFO_START], 5);
					rt = util.binarytoint(&buf[INFO_START+5], 5);
					offset = util.signedbinarytoint(&buf[INFO_START+10], 16);
					offset = offset*pow(2,2);
					insertval(ins,1,rs,rt,offset);
					break;

				  case OP_BLTZ:
					//ADD THIS OFFSET TO PC+4 if rs < 0 then branch
					rs = util.binarytoint(&buf[INFO_START], 5);
					offset = util.signedbinarytoint(&buf[INFO_START+10], 16);
					offset = offset*pow(2,2);
					insertval(ins,1,rs,offset,0);
					break;

				  case OP_BGTZ:
					//ADD THIS OFFSET TO PC+4 if rs > 0 then branch
					rs = util.binarytoint(&buf[INFO_START], 5);
					offset = util.signedbinarytoint(&buf[INFO_START+10], 16);
					offset = offset*pow(2,2);
					insertval(ins,1,rs,offset,0);
					break;

				  case OP_BREAK:
					insertval(ins,0,0,0,0);
					break;

				  //The 16-bit signed offset is added to the contents of GPR base to form the effective address.
				  //the value at rs is stored into that memory
				  case OP_SW:
					base   = util.binarytoint(&buf[INFO_START], 5);
					rs     = util.binarytoint(&buf[INFO_START+5], 5);
					offset = util.signedbinarytoint(&buf[INFO_START+10], 16);
					insertval(ins,2,rs,base,offset);
					break;

				  // The 16-bit signed offset is added to the contents of GPR base to form the effective address.
				  // The contents of the effective address are placed in rt
				  case OP_LW:
					base   = util.binarytoint(&buf[INFO_START], 5);
					rt     = util.binarytoint(&buf[INFO_START+5], 5);
					offset = util.signedbinarytoint(&buf[INFO_START+10], 16);
					insertval(ins,2,base,offset,rt);
					break;

				  //the contents of the register are shifted Left
				  // rd =rt << sa
				  case OP_SLL:
					rt     = util.binarytoint(&buf[INFO_START+5], 5);
					rd     = util.binarytoint(&buf[INFO_START+10], 5);
					sa     = util.binarytoint(&buf[INFO_START+15], 5);
					insertval(ins,3,rt,rd,sa);
					break;

				  case OP_SRL:
					rt     = util.binarytoint(&buf[INFO_START+5], 5);
					rd     = util.binarytoint(&buf[INFO_START+10], 5);
					sa     = util.binarytoint(&buf[INFO_START+15], 5);
					insertval(ins,3,rt,rd,sa);
					break;

				  case OP_SRA:
					rt     = util.binarytoint(&buf[INFO_START+5], 5);
					rd     = util.binarytoint(&buf[INFO_START+10], 5);
					sa     = util.binarytoint(&buf[INFO_START+15], 5);
					insertval(ins,3,rt,rd,sa);
					break;

				  case OP_NOP:
				    insertval(ins,4,0,0,0);
					break;

				  default:
					output = util.signedbinarytoint(buf, INFO_END);
				}
				break;

			case CATEGORY2:
				switch(ins[1]){
				  case OP_ADD:
					//ADD rd, rs, rt
					rs     = util.binarytoint(&buf[INFO_START], 5);
					rt     = util.binarytoint(&buf[INFO_START+5], 5);
					rd     = util.binarytoint(&buf[INFO_START+10], 5);
					insertval(ins,3,rs,rt,rd);
					break;

				  case OP_SUB:
					rs     = util.binarytoint(&buf[INFO_START], 5);
					rt     = util.binarytoint(&buf[INFO_START+5], 5);
					rd     = util.binarytoint(&buf[INFO_START+10], 5);
					insertval(ins,3,rs,rt,rd);
					break;

				  case OP_MUL:
					rs     = util.binarytoint(&buf[INFO_START], 5);
					rt     = util.binarytoint(&buf[INFO_START+5], 5);
					rd     = util.binarytoint(&buf[INFO_START+10], 5);
					insertval(ins,3,rs,rt,rd);
					break;

				  case OP_AND:
					rs     = util.binarytoint(&buf[INFO_START], 5);
					rt     = util.binarytoint(&buf[INFO_START+5], 5);
					rd     = util.binarytoint(&buf[INFO_START+10], 5);
					insertval(ins,3,rs,rt,rd);
					break;

				  case OP_OR:
					rs     = util.binarytoint(&buf[INFO_START], 5);
					rt     = util.binarytoint(&buf[INFO_START+5], 5);
					rd     = util.binarytoint(&buf[INFO_START+10], 5);
					insertval(ins,3,rs,rt,rd);
					break;

				  //xor = (a^b);
				  case OP_XOR:
					rs     = util.binarytoint(&buf[INFO_START], 5);
					rt     = util.binarytoint(&buf[INFO_START+5], 5);
					rd     = util.binarytoint(&buf[INFO_START+10], 5);
					insertval(ins,3,rs,rt,rd);
					break;

				  //nor = ~(a|b);
				  case OP_NOR:
					rs     = util.binarytoint(&buf[INFO_START], 5);
					rt     = util.binarytoint(&buf[INFO_START+5], 5);
					rd     = util.binarytoint(&buf[INFO_START+10], 5);
					insertval(ins,3,rs,rt,rd);
					break;


				  case OP_SLT:
					//rd <- (rs < rt) - boolean output
					rs     = util.binarytoint(&buf[INFO_START], 5);
					rt     = util.binarytoint(&buf[INFO_START+5], 5);
					rd     = util.binarytoint(&buf[INFO_START+10], 5);
					insertval(ins,3,rs,rt,rd);
					break;

				  //overflow provisions
				  case OP_ADDI:
					//rt <--rs + immediate
					rs     = util.binarytoint(&buf[INFO_START], 5);
					rt     = util.binarytoint(&buf[INFO_START+5], 5);
					imm_val= util.signedbinarytoint(&buf[INFO_START+10], 16);
					insertval(ins,3,rs,rt,imm_val);
					break;

					//rt, rs and immediate
				  case OP_ANDI:
					rs     = util.binarytoint(&buf[INFO_START], 5);
					rt     = util.binarytoint(&buf[INFO_START+5], 5);
					imm_val= util.binarytoint(&buf[INFO_START+10], 16);
					insertval(ins,3,rs,rt,imm_val);
					break;

				  case OP_ORI:
					rs     = util.binarytoint(&buf[INFO_START], 5);
					rt     = util.binarytoint(&buf[INFO_START+5], 5);
					imm_val= util.binarytoint(&buf[INFO_START+10], 16);
					insertval(ins,3,rs,rt,imm_val);
					break;

				  case OP_XORI:
					rs     = util.binarytoint(&buf[INFO_START], 5);
					rt     = util.binarytoint(&buf[INFO_START+5], 5);
					imm_val= util.binarytoint(&buf[INFO_START+10], 16);
					insertval(ins,3,rs,rt,imm_val);
					break;

				  default:
					output = util.signedbinarytoint(buf, INFO_END);
				}
				break;

			  case SIGN_INT1:
				break;

			  case SIGN_INT2:
				break;

	    }
		fetchUnit::address_instructions.insert(std::pair<int, fetchUnit::InstrChar>(PC, ins));
	    PC+=4;
	    delete [] buf;
	}
	return;
}
