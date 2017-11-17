/*
 * controller.h
 *
 *  Created on: Nov 7, 2017
 *      Author: merinsan
 */
#include <cstdlib>
#include <string>
#include <stdio.h>
#include <fstream>
#include <iostream>
#include <sstream>
#include "ALU1Unit.h"
#include "fetchUnit.h"
#include "MemUnit.h"
#include "ALU2Unit.h"
#include "WriteBack.h"
#include "RegisterController.h"

using namespace std;

#ifndef CONTROLLER_H_
#define CONTROLLER_H_

//This class controls the general data flows and evaluates corresponding buffers before moving into execute any instruction
//This is like a gateway to the other units
//It controls the flow of code from fetch --> issue --> ALU1(MEM) or ALU2 type --> write back
class controller {
public:

	fetchUnit::Instype startFetching(int ALU2dummy_size, int ALU1dummy_size);
	void evalPreIssueBuffer();
	void updatebuffers();
	void startALU1();
	void startMEM();
	void startALU2();
	void startWriteBack();

	static const int maxInstr = 2;

	set <int, greater <int> > empty;
private:
	int noInstr;
	int instr_details;
	int sum;

};

#endif /* CONTROLLER_H_ */
