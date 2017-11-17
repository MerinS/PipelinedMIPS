/*
 * Fileprint.cpp
 *
 *  Created on: Nov 15, 2017
 *      Author: merinsan
 */

#include "Fileprint.h"
#include "instr_cat.h"
#include "utilityFunctions.h"

string Fileprint::to_string(int i)
{
    std::stringstream ss;
    ss << i;
    return ss.str();
}

string Fileprint::Printfunction(IssueUnit::Operation_details var){
	str = "";
	if(var.type == 0){
		str.append("[ADD ");
	}
	if(var.type == 1){
		str.append("[SUB ");
	}
	if(var.type == 2){
		str.append("[MUL ");
	}
	if(var.type == 3){
		str.append("[AND ");
	}
	if(var.type == 4){
		str.append("[OR ");
	}
	if(var.type == 5){
		str.append("[XOR ");
	}
	if(var.type == 6){
		str.append("[NOR ");
	}
	if(var.type == 7){
		str.append("[SLT ");
	}
	if(var.type == 8){
		str.append("[ADDI ");
	}
	if(var.type == 9){
		str.append("[ANDI ");
	}
	if(var.type == 10){
		str.append("[ORI ");
	}
	if(var.type == 11){
		str.append("[XORI ");
	}
	if(var.type == 12){
		str.append("[SW ");
	}
	if(var.type == 13){
		str.append("[LW ");
	}
	if(var.type == 14){
		str.append("[SLL ");
	}
	if(var.type == 15){
		str.append("[SRL ");
	}
	if(var.type == 16){
		str.append("[SRA ");
	}

	if(var.type>=0 && var.type<=7){
		str.append("R");
		str.append(this->to_string(var.rdes));
		str.append(", R");
		str.append(this->to_string(var.rs1));
		str.append(", R");
		str.append(this->to_string(var.rs2));
		str.append("]");
	}

	if(var.type>=8 && var.type<=11){
		str.append("R");
		str.append(this->to_string(var.rdes));
		str.append(", R");
		str.append(this->to_string(var.rs1));
		str.append(", #");
		str.append(this->to_string(var.offset_imm_shift_val));
		str.append("]");
	}
	if(var.type>=14 && var.type<=16){
		str.append("R");
		str.append(this->to_string(var.rdes));
		str.append(", R");
		str.append(this->to_string(var.rs1));
		str.append(", #");
		str.append(this->to_string(var.offset_imm_shift_val));
		str.append("]");
	}

	if(var.type == 12){
		str.append("R");
		str.append(this->to_string(var.rs1));
		str.append(", ");
		str.append(this->to_string(var.offset_imm_shift_val));
		str.append("(R");
		str.append(this->to_string(var.rs2));
		str.append(")]");
	}

	if(var.type == 13){
		str.append("R");
		str.append(this->to_string(var.rdes));
		str.append(", ");
		str.append(this->to_string(var.offset_imm_shift_val));
		str.append("(R");
		str.append(this->to_string(var.rs1));
		str.append(")]");
	}
	return str;
}

string Fileprint::printPC(int val){
	str = "";
	it = fetchUnit::address_instructions.find(val);
	fetchUnit::InstrChar ins = it->second;
	switch(ins.data[0]){
      case CATEGORY1:
        switch(ins.data[1]){
          case OP_J:
            str.append("[J #");
            str.append(this->to_string(ins.data[3]));
            str.append("]");
            break;

          case OP_JR:
        	    str.append("[JR (R");
        	  	str.append(this->to_string(ins.data[3]));
        	  	str.append(")]");
            break;

          case OP_BEQ:
        	    str.append("[BEQ R");
            str.append(this->to_string(ins.data[3]));
            str.append(", R");
            str.append(this->to_string(ins.data[4]));
            str.append(", #");
            str.append(this->to_string(ins.data[5]));
            str.append("]");
            break;

          case OP_BLTZ:
        	    str.append("[BLTZ R");
		    str.append(this->to_string(ins.data[3]));
		    str.append(", #");
		    str.append(this->to_string(ins.data[4]));
		    str.append("]");
            break;

          case OP_BGTZ:
        	    str.append("[BGTZ R");
			str.append(this->to_string(ins.data[3]));
			str.append(", #");
			str.append(this->to_string(ins.data[4]));
			str.append("]");
            break;

          case OP_BREAK:
        	    str.append("[BREAK]");
            break;

          case OP_NOP:
        	    str.append("[NOP]");
			break;
        }
	}
	str.append("\n");
	return str;
}
