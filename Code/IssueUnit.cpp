/*
 * IssueUnit.cpp
 *
 *  Created on: Nov 10, 2017
 *      Author: merinsan
 */


#include "RegisterController.h"
#include "IssueUnit.h"

bool IssueUnit::isbufslotPreALU1Available(){
	if((preALU1buffer.size() + preALU1buffer_dummy.size())<2){
		return 1;
	}
	else{
		return 0;
	}
}

bool IssueUnit::isbufslotPreALU2Available(){
	if((preALU2buffer.size() + preALU2buffer_dummy.size())<2){
		return 1;
	}
	else{
		return 0;
	}
}

void IssueUnit::insertALUbuffer(IssueUnit::Operation_details buffer){
	if(buffer.type == 12 | buffer.type == 13){
		preALU1buffer_dummy.push(buffer);
	}
	else{
		preALU2buffer_dummy.push(buffer);
	}
}

IssueUnit::Operation_details IssueUnit::insertOperands(int type, int rdes, int rs1, int rs2, int offset_imm_shift_val){
	var2.type = type;
	var2.rdes = rdes;
	var2.rs1 = rs1;
	var2.rs2 = rs2;
	var2.offset_imm_shift_val = offset_imm_shift_val;
	return var2;
}

IssueUnit::Operation_details IssueUnit::instrparse(fetchUnit::InstrChar buffer){
    	x = buffer.data[0];
    	y = buffer.data[1];
    if(x==1){
		//sw
		if(y == 6){
			var1=this -> insertOperands(12, -1, buffer.data[3], buffer.data[4], buffer.data[5]);
		}
		//lw
		else if(y==7){
			var1=this -> insertOperands(13, buffer.data[5], buffer.data[3], -1, buffer.data[4]);
		}
		//sll - 14
		else if(y==8){
			var1=this -> insertOperands(14, buffer.data[4], buffer.data[3], -1, buffer.data[5]);
		}
		//srl - 15
		else if(y==9){
			var1=this -> insertOperands(15, buffer.data[4], buffer.data[3], -1, buffer.data[5]);
		}
		//sra - 16
		else if(y == 10){
			var1=this -> insertOperands(16, buffer.data[4], buffer.data[3], -1, buffer.data[5]);
		}
	}
	else if (x ==3){
		if(y==0 || y==1 ||y==2 || y==3 ||y==4 || y==5 || y==6 || y==7){
			var1=this -> insertOperands(y, buffer.data[5], buffer.data[3], buffer.data[4], -1);
		}
		else if(y==8 || y==9 ||y==10 || y==11 ){
			var1=this -> insertOperands(y, buffer.data[4], buffer.data[3], -1, buffer.data[5]);
		}
	}
	return var1;
}

bool IssueUnit::isinWriteLocalList(int rx){
	if(IssueUnit::list_write.count(rx) != 0){
		return true;
	}
	else{
		return false;
	}
}

bool IssueUnit::isinReadLocalList(int rx){
	if(IssueUnit::list_read.count(rx) != 0){
		return true;
	}
	else{
		return false;
	}
}

bool IssueUnit::isIssuePossible_evalhazards(IssueUnit::Operation_details buff){
	RegisterController Rg;
	statusRAW  = false;
	statusRAW1 = true;
	statusRAW2 = true;
	statusWAR  = true;
	statusWAW  = true;

	if(buff.rdes != -1){
		//check for WAW hazards
		if(!(RegisterController::R[buff.rdes].writestatus) && (!(this->isinWriteLocalList(buff.rdes)))){
			statusWAW  = true;
		}
		else{
			statusWAW  = false;
		}
		//check for WAR hazards
		if((buff.rdes != IssueUnit::R[0]) && (buff.rdes != IssueUnit::R[1]) && (!(this->isinReadLocalList(buff.rdes)))){
			statusWAR  = true;
		}
		else{
			statusWAR  = false;
		}
	}

	//check for RAW hazards
	if(buff.rs1 != -1){
		if(!(RegisterController::R[buff.rs1].writestatus) && (!(this->isinWriteLocalList(buff.rs1)))){
			statusRAW1 = true;
		}
		else{
			statusRAW1  = false;
		}
	}
	if(buff.rs2 != -1){
		if(!(RegisterController::R[buff.rs2].writestatus) && (!(this->isinWriteLocalList(buff.rs2)))){
			statusRAW2 = true;
		}
		else{
			statusRAW2  = false;
		}
	}
	statusRAW = statusRAW1 & statusRAW2;
	finalstatus = statusRAW & statusWAW & statusWAR;
    return finalstatus;
}

void IssueUnit::updateReg(IssueUnit::Operation_details buff){
	RegisterController Rg;
	if(buff.rdes != -1){
		Rg.setRegWritebusy(buff.rdes);
	}
	if(buff.rs1 != -1){
		Rg.setRegReadbusy(buff.rs1, -1);
	}
	if(buff.rs2 != -1){
		Rg.setRegReadbusy(buff.rs2, -1);
	}
}

void IssueUnit::updatelocalReg(IssueUnit::Operation_details buff){
	if(buff.rdes != -1){
		list_write.insert(buff.rdes);
	}
	if(buff.rs1 != -1){
		list_read.insert(buff.rs1);
	}
	if(buff.rs2 != -1){
		list_read.insert(buff.rs2);
	}
}

bool IssueUnit::evalALU1ALU2(fetchUnit::InstrChar buffer, bool pointer_issue){
	var = this -> instrparse(buffer);
	status = isIssuePossible_evalhazards(var);
	if(pointer_issue & status){
		this -> updateReg(var);
		this -> insertALUbuffer(var);
		if(var.rs1 != -1){
			IssueUnit::R[0] = var.rs1;
		}
		if(var.rs2 != -1){
			IssueUnit::R[1] = var.rs2;
		}
		return true;
	}
	else{
		this -> updatelocalReg(var);
		return false;
	}
}

void IssueUnit::IssueALU1ALU2(){
	size = fetchUnit::preIssue.size();
	i = 0;
	storecount = 0;
	countALU1 = 0;
	countALU2 = 0;
	while(i< size){
		val = fetchUnit::preIssue.front();
		fetchUnit::preIssue.pop();
		it = fetchUnit::address_instructions.find(val);
		fetchUnit::InstrChar buffer = it->second;
		x = buffer.data[0];
		y = buffer.data[1];
		hit = false;
		pointer_issue = false;
		if((x==1 && y==6) || (x==1 && y==7)){
			if(storecount ==0 && countALU1==0 && (isbufslotPreALU1Available())){
				pointer_issue = true;
			}
			hit = this->evalALU1ALU2(buffer,pointer_issue);
			if(y==6){
				storecount +=1;
			}
			//if the MEM type instruction was evaluated and found fit to be issued
			if(hit){
				countALU1+=1;
				}
			}

		else{
			if(countALU2==0 && (isbufslotPreALU2Available())){
				pointer_issue = true;
			}
			hit = this->evalALU1ALU2(buffer, pointer_issue);
			//if the ALU type instruction was evaluated and found fit to be issued
			if(hit){
				countALU2+=1;
			}
		}
		if(!hit){
			fetchUnit::preIssue.push(val);
		}
		i+=1;
	}
}

void IssueUnit::updateIssuebuffer(){
	i = 0;
	size = IssueUnit::preALU1buffer_dummy.size();
	while(i< size){
		var_dummy = IssueUnit::preALU1buffer_dummy.front();
		IssueUnit::preALU1buffer_dummy.pop();
		i+=1;
		IssueUnit::preALU1buffer.push(var_dummy);
	}

	i = 0;
	size = IssueUnit::preALU2buffer_dummy.size();
	while(i< size){
		var_dummy = IssueUnit::preALU2buffer_dummy.front();
		IssueUnit::preALU2buffer_dummy.pop();
		i+=1;
		IssueUnit::preALU2buffer.push(var_dummy);
	}

}
