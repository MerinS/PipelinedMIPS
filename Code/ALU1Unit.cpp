/*
 * ALU1Unit.cpp
 *
 *  Created on: Nov 14, 2017
 *      Author: merinsan
 */

#include "ALU1Unit.h"

void ALU1Unit::computeALU1(IssueUnit::Operation_details var){
	//sw
	if(var.type == 12){
		var.val = var.offset_imm_shift_val +  (RegisterController::R[var.rs2].value);
	}
	//lw
	else if(var.type == 13){
		var.val = var.offset_imm_shift_val +  (RegisterController::R[var.rs1].value);
	}
	preMEMbuffer_dummy.push(var);
	return;
}

void ALU1Unit::IssueALU1(){
	var = IssueUnit::preALU1buffer.front();
	IssueUnit::preALU1buffer.pop();
	this -> computeALU1(var);
	return;
}

void ALU1Unit::updateALU1buffer(){
	i = 0;
	size = ALU1Unit::preMEMbuffer_dummy.size();
	while(i< size){
		var_dummy = ALU1Unit::preMEMbuffer_dummy.front();
		ALU1Unit::preMEMbuffer_dummy.pop();
		i+=1;
		ALU1Unit::preMEMbuffer.push(var_dummy);
	}
	return;
}
