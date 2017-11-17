/*
 * MemUnit.cpp
 *
 *  Created on: Nov 14, 2017
 *      Author: merinsan
 */

#include "MemUnit.h"
#include "ALU1Unit.h"
#include "DataController.h"

void MemUnit::setRegistersBacksw(IssueUnit::Operation_details buff){
	if(buff.rdes != -1){
		RegisterController::R[buff.rdes].writestatus = false;
	}
	if(buff.rs1 != -1){
		RegisterController::R[buff.rs1].readstatus = false;
	}
	if(buff.rs2 != -1){
		RegisterController::R[buff.rs2].readstatus = false;
	}
	return;
}

void  MemUnit::computeMEM(IssueUnit::Operation_details var){
	//sw execute
	if(var.type == 12){
		DataController::data_instructions[var.val] = RegisterController::R[var.rs1].value;
		this -> setRegistersBacksw(var);
	}
	//lw execute
	else if(var.type == 13){
		var.val = DataController::data_instructions[var.val];\
		postMEMbuffer_dummy.push(var);
	}
}

void MemUnit::IssueMEM(){
	//printf("\n IssueMEM");
	var = ALU1Unit::preMEMbuffer.front();
	ALU1Unit::preMEMbuffer.pop();
	this -> computeMEM(var);
}


void MemUnit::updateMEMbuffer(){
	i = 0;
	size = MemUnit::postMEMbuffer_dummy.size();
	//printf("size of preMEM dummy buffer = %d \n", size);
	while(i< size){
		var_dummy = MemUnit::postMEMbuffer_dummy.front();
		MemUnit::postMEMbuffer_dummy.pop();
		i+=1;
		MemUnit::postMEMbuffer.push(var_dummy);
	}
}
