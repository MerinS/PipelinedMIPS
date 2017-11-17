/*
 * controller.cpp
 *
 *  Created on: Nov 7, 2017
 *      Author: merinsan
 */

#include "controller.h"

fetchUnit::Instype controller::startFetching(int ALU2dummy_size,  int ALU1dummy_size){
	fetchUnit::Instype instr_details;
	instr_details.type = -1;
	fetchUnit fu;
	noInstr  = 0;

	//updating the register stall array and confirming that there is no Fetch unit stall
	if(fu.checkisstalled()){
		instr_details.type = 1;
		instr_details.PC1 = fetchUnit::PC;
		instr_details.state = 0;
	}
	while (noInstr < maxInstr){
		sum = fu.isbufslotAvailable() + ALU2dummy_size + ALU1dummy_size;
		if(!fu.isStalled() && (sum < 4.0)){
			instr_details = fu.fetchandEval();
			//if it is a  break type or branch type instruction you break out of the loop as no more instructions are fetched
			if(instr_details.type ==  0 || instr_details.type == 1 ){
				break;
			}
		}
		else{
			break;
		}
		noInstr+=1;
	}
	return instr_details;
}

void controller::evalPreIssueBuffer(){
	IssueUnit iu;
	if(fetchUnit::preIssue.size()>0){
		//initialisation of the associated variables
		//set-map list_read is empty
		IssueUnit::list_read.clear();
		//set-map list_write is empty
		IssueUnit::list_write.clear();
		IssueUnit::R[0] = -1;
		IssueUnit::R[1] = -1;
		iu.IssueALU1ALU2();
	}
	return;
}

void controller::startALU1(){
	ALU1Unit a1u;
	if(IssueUnit::preALU1buffer.size()>0){
			a1u.IssueALU1();
	}
	return;
}

void controller::startMEM(){
	MemUnit mu;
	if(ALU1Unit::preMEMbuffer.size()>0){
		mu.IssueMEM();
	}
	return;
}

void controller::startALU2(){
	ALU2Unit a2u;
	if(IssueUnit::preALU2buffer.size()>0){
			a2u.IssueALU2();
	}
	return;
}

void controller::startWriteBack(){
	WriteBack wb;
	if(MemUnit::postMEMbuffer.size()>0){
		wb.WriteBackMem();
	}
	if(ALU2Unit::postALU2buffer.size() >0){
		wb.WriteBackALU2();
	}
	return;
}

void controller::updatebuffers(){
	fetchUnit fu;
	IssueUnit iu;
    ALU1Unit a1u;
    ALU2Unit a2u;
    MemUnit mu;
	fu.updateFetchbuffer();
	iu.updateIssuebuffer();
	a1u.updateALU1buffer();
	mu.updateMEMbuffer();
	a2u.updateALU2buffer();
	return;
}
