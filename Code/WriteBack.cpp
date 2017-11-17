/*
 * WriteBack.cpp
 *
 *  Created on: Nov 14, 2017
 *      Author: merinsan
 */

#include "WriteBack.h"

void WriteBack::eval(IssueUnit::Operation_details var){
	if( var.type == 12){
		this -> setRegistersBack(var);
	}
	else{
		RegisterController::R[var.rdes].value = var.val;
		this -> setRegistersBack(var);
	}
	return;
}

void WriteBack::setRegistersBack(IssueUnit::Operation_details buff){
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

void WriteBack::WriteBackMem(){
	var1 = MemUnit::postMEMbuffer.front();
	this -> eval(var1);
	MemUnit::postMEMbuffer.pop();
	return;
}

void WriteBack::WriteBackALU2(){
	var1 = ALU2Unit::postALU2buffer.front();
	this -> eval(var1);
	ALU2Unit::postALU2buffer.pop();
	return;
}
