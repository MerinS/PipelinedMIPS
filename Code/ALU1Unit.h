/*
 * ALU1Unit.h
 *
 *  Created on: Nov 14, 2017
 *      Author: merinsan
 */
#include <queue>
#include "IssueUnit.h"
#include "RegisterController.h"

using namespace std;

#ifndef ALU1UNIT_H_
#define ALU1UNIT_H_

//ALU1 unit does the effective memory address calculation for load and store instructions 
class ALU1Unit {
public:
	static queue <IssueUnit::Operation_details> preMEMbuffer;
	static queue <IssueUnit::Operation_details> preMEMbuffer_dummy;
	void computeALU1(IssueUnit::Operation_details var);
	void IssueALU1();
	void updateALU1buffer();
	IssueUnit::Operation_details var,var_dummy;
private:
	int i, size;
};

#endif /* ALU1UNIT_H_ */

