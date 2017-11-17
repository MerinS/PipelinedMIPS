/*
 * MemUnit.h
 *
 *  Created on: Nov 14, 2017
 *      Author: merinsan
 */
#include "IssueUnit.h"

#ifndef MEMUNIT_H_
#define MEMUNIT_H_


//Performs the memory operation (LW  and SW)
//SW is done executing at this stage
class MemUnit {
public:
	static queue <IssueUnit::Operation_details> postMEMbuffer;
	static queue <IssueUnit::Operation_details> postMEMbuffer_dummy;

	void computeMEM(IssueUnit::Operation_details var);
	void IssueMEM();
	void updateMEMbuffer();
	void setRegistersBacksw(IssueUnit::Operation_details buff);

	IssueUnit::Operation_details var,var_dummy;

private:
	int i,size;
};

#endif /* MEMUNIT_H_ */
