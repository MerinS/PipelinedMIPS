/*
 * ALU2Unit.h
 *
 *  Created on: Nov 14, 2017
 *      Author: merinsan
 */
#include <queue>
#include "IssueUnit.h"
#include "RegisterController.h"

#ifndef ALU2UNIT_H_
#define ALU2UNIT_H_

//ALU2 unit does the effective memory address calculation for load and store instructions 
class ALU2Unit {
public:
	static queue <IssueUnit::Operation_details> postALU2buffer;
	static queue <IssueUnit::Operation_details> postALU2buffer_dummy;
	void computeALU2(IssueUnit::Operation_details var);
	void IssueALU2();
	void updateALU2buffer();
	IssueUnit::Operation_details var,var_dummy;
	private:
	int i, size;
};

#endif /* ALU2UNIT_H_ */