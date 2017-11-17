/*
 * fetchUnit.cpp
 *
 *  Created on: Nov 7, 2017
 *      Author: merinsan
 */

#include "fetchUnit.h"
#include "instr_cat.h"
#include "RegisterController.h"


int fetchUnit::isStalled(){
	return mstallFetchUnit;
}


int fetchUnit::isbufslotAvailable(){
	    sum = preIssue.size() + preIssue_dummy.size();
		return sum;
}


void fetchUnit::stallFetchUnit(int r1, int r2){
	Rstall[0]  = r1;
	Rstall[1]  = r2;
	if(!mstallFetchUnit){
		mstallFetchUnit = true;
	}
}


bool fetchUnit::checkisstalled(){
	i = 0;
	RegisterController Rg;
	state = false;
	while(i<2){
		if(Rstall[i] != -1){
			if(!Rg.isRegAvailRead(Rstall[0])){
				state = true;
			}
			else{
				Rstall[i] = -1;
			}
		}
		i+=1;
	}
	if(!state){
		mstallFetchUnit = false;
	}
	return state;
}

fetchUnit::Instype fetchUnit::initInsttype(int type, int PC1, int state){
	ret_val.type = type;
	ret_val.PC1 = PC;
	ret_val.state = state;
	return ret_val;
}

fetchUnit::Instype fetchUnit::fetchandEval(){
	it = address_instructions.find(PC);
	InstrChar buf1 = it->second;
	if(buf1.data[2] == 1){
		// branch type instructions are executed immediately if the associated registers are ready
		ret_val1 = this->evaljump(buf1,PC);
	}
	//write alu and mem type into preIssue buffer
	else if(buf1.data[2] == 2 ||buf1.data[2] == 3){
		preIssue_dummy.push(PC);
		ret_val1 = initInsttype(buf1.data[2], PC, 1);
		PC +=4;
	}
	else if(buf1.data[2] == 0 ||buf1.data[2] == 4){
		ret_val1 = initInsttype(buf1.data[2], PC, 1);
	}
	return ret_val1;
}


fetchUnit::Instype fetchUnit::evaljump(InstrChar buf1, int PC1){
	int x = buf1.data[0];
	int y = buf1.data[1];
	RegisterController Rg;
	switch(x){
		case CATEGORY1:
		switch(y){
		   case OP_J:
			 jump_mem = buf1.data[3];
			 mem_eff  = ((PC+4)& 0xF0000000)|(jump_mem);
			 ret_branch = initInsttype(1, PC, 1);
			 PC       = mem_eff;
			 break;

		   case OP_JR:
			 jump_mem = buf1.data[3];
			 if(Rg.isRegAvailRead(jump_mem)){
				 ret_branch = initInsttype(1, PC, 1);
				 mem_eff  = RegisterController::R[jump_mem].value;
				 PC       = mem_eff;
				 Rg.setRegReadbusy(jump_mem, -1);
			 }
			 else{
				 stallFetchUnit(jump_mem, -1);
				 ret_branch = initInsttype(1, PC, 0);
			 }

			 break;

		   case OP_BEQ:
			 rs     = buf1.data[3];
			 rt     = buf1.data[4];
			 offset = buf1.data[5];
			 mem_eff= PC+4;
			 if(Rg.isRegAvailRead(rs) & Rg.isRegAvailRead(rt)){
				 ret_branch = initInsttype(1, PC, 1);
				 if(RegisterController::R[rs].value == RegisterController::R[rt].value){
					 mem_eff  = (PC+4+offset);
					 PC = mem_eff;
					 Rg.setRegReadbusy(rs, rt);
				 }
				 else{
					 PC = PC + 4;
				 }
			 }
			 else{
				 ret_branch = initInsttype(1, PC, 0);
				 stallFetchUnit(rs, rt);
			 }
			 break;

		   case OP_BLTZ:
			 //ADD THIS OFFSET TO PC+4 if rs < 0 then branch
			 rs     = buf1.data[3];
			 offset = buf1.data[4];
			 mem_eff= PC+4;
			 if(Rg.isRegAvailRead(rs)){
				 ret_branch = initInsttype(1, PC, 1);
				 if(RegisterController::R[rs].value<0){
					 mem_eff  = (PC+4+offset);
					 PC = mem_eff;
					 Rg.setRegReadbusy(rs, rt);
				 }
				 else{
					 PC = PC + 4;
				 }
			 }
			 else{
				 ret_branch = initInsttype(1, PC, 0);
				 stallFetchUnit(rs, -1);
			 }

			 break;

		   case OP_BGTZ:
			 rs     = buf1.data[3];
			 offset = buf1.data[4];
			 mem_eff= PC+4;
			 if(Rg.isRegAvailRead(rs)){
				 ret_branch = initInsttype(1, PC, 1);
				 if(RegisterController::R[rs].value>0){
					 mem_eff  = (PC+4+offset);
					 PC = mem_eff;
					 Rg.setRegReadbusy(rs, rt);
				 }
				 else{
					 PC = PC + 4;
				 }
			 }
			 else{
				 ret_branch = initInsttype(1, PC, 0);
				 stallFetchUnit(rs, -1);
			 }
			 break;
		}
	}
	return ret_branch;
}

void fetchUnit::updateFetchbuffer(){
	i = 0;
	size = fetchUnit::preIssue_dummy.size();
	while(i< size){
		val = fetchUnit::preIssue_dummy.front();
		fetchUnit::preIssue_dummy.pop();
		i+=1;
		fetchUnit::preIssue.push(val);
	}
}