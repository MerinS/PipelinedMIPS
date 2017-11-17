/*
 * ALU2Unit.cpp
 *
 *  Created on: Nov 14, 2017
 *      Author: merinsan
 */

#include "ALU2Unit.h"

void ALU2Unit::computeALU2(IssueUnit::Operation_details var){
	if(var.type == 0){
		var.val =  ((RegisterController::R[var.rs1].value) + (RegisterController::R[var.rs2].value));
	}
	if(var.type == 1){
		var.val =  ((RegisterController::R[var.rs1].value) - (RegisterController::R[var.rs2].value));
	}
	if(var.type == 2){
		var.val =  ((RegisterController::R[var.rs1].value) * (RegisterController::R[var.rs2].value));
	}
	if(var.type == 3){
		var.val =  ((RegisterController::R[var.rs1].value) &  (RegisterController::R[var.rs2].value));
	}
	if(var.type == 4){
		var.val =  ((RegisterController::R[var.rs1].value) |  (RegisterController::R[var.rs2].value));
	}
	if(var.type == 5){
		var.val =  ((RegisterController::R[var.rs1].value) ^  (RegisterController::R[var.rs2].value));
	}
	if(var.type == 6){
		var.val =  ~((RegisterController::R[var.rs1].value) |  (RegisterController::R[var.rs2].value));
	}
	if(var.type == 7){
		var.val =  ((RegisterController::R[var.rs1].value) < (RegisterController::R[var.rs2].value))?1:0;
	}
	if(var.type == 8){
		var.val =  ((RegisterController::R[var.rs1].value) + (var.offset_imm_shift_val));
	}
	if(var.type == 9){
		var.val =  ((RegisterController::R[var.rs1].value) & (var.offset_imm_shift_val));
	}
	if(var.type == 10){
		var.val =  ((RegisterController::R[var.rs1].value) | (var.offset_imm_shift_val));
	}
	if(var.type == 11){
		var.val =  ((RegisterController::R[var.rs1].value) ^ (var.offset_imm_shift_val));
	}
	if(var.type == 14){
		var.val =  ((RegisterController::R[var.rs1].value) << (var.offset_imm_shift_val));
	}
	if(var.type == 15){
		var.val =  ((RegisterController::R[var.rs1].value) >> (var.offset_imm_shift_val));
	}
	if(var.type == 16){
		var.val =  ((RegisterController::R[var.rs1].value) >> (var.offset_imm_shift_val));
	}
	postALU2buffer_dummy.push(var);
	return;
}


void ALU2Unit::IssueALU2(){
	var = IssueUnit::preALU2buffer.front();
	IssueUnit::preALU2buffer.pop();
	this -> computeALU2(var);
	return;
}

void ALU2Unit::updateALU2buffer(){
	i = 0;
	size = ALU2Unit::postALU2buffer_dummy.size();
	while(i< size){
		var_dummy = ALU2Unit::postALU2buffer_dummy.front();
		ALU2Unit::postALU2buffer_dummy.pop();
		i+=1;
		ALU2Unit::postALU2buffer.push(var_dummy);
	}
	return;
}
