/*
 * WriteBack.h
 *
 *  Created on: Nov 14, 2017
 *      Author: merinsan
 */
#include "IssueUnit.h"
#include "ALU2Unit.h"
#include "MemUnit.h"
#include "RegisterController.h"

#ifndef WRITEBACK_H_
#define WRITEBACK_H_

//Class that handles writeback and updates the read and write status of registers to available
class WriteBack {
public:
	void WriteBackMem();
	void WriteBackALU2();
	void setRegistersBack(IssueUnit::Operation_details);
	void eval(IssueUnit::Operation_details);
private:
	IssueUnit::Operation_details var1;
};

#endif /* WRITEBACK_H_ */
