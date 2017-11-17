/*
 * IssueUnit.h
 *
 *  Created on: Nov 10, 2017
 *      Author: merinsan
 */
#include <queue>
#include <set>
#include <cstdlib>
#include <string>
#include <stdio.h>
#include <fstream>
#include <iostream>
#include <sstream>
#include <math.h>
#include <stdlib.h>
#include <memory>
#include "fetchUnit.h"

#ifndef ISSUEUNIT_H_
#define ISSUEUNIT_H_

using namespace std;

//Evaluates ALU and MEM type instructions
//Checks for potential hazards (RAW/WAW/WAR)
//Checks for issuing stores in order
//Checks for issues loads only after earlier stores
class IssueUnit {

public:
	struct Operation_details
	{
		int type;
		//rd
		int rdes;
		//rs
		int rs1;
		int rs2;
		//offset immval shift val
		int offset_imm_shift_val;
		int val;
	};

	static queue <Operation_details> preALU1buffer;
	static queue <Operation_details> preALU2buffer;
	static queue <Operation_details> preALU1buffer_dummy;
	static queue <Operation_details> preALU2buffer_dummy;

	// empty set container
    static set <int, greater <int> > list_read;
    static set <int, greater <int> > list_write;

    //static register read list issues
    static int R[2];

	bool isbufslotPreALU1Available();
	bool isbufslotPreALU2Available();
	void IssueALU1ALU2();
	bool evalALU1ALU2(fetchUnit::InstrChar buffer, bool pointer_issue);
	Operation_details insertOperands(int type, int rdes, int rs1, int rs2, int offset_imm_shift_val);
	Operation_details instrparse(fetchUnit::InstrChar buffer);
	bool isIssuePossible_evalhazards(Operation_details var);
	void updateReg(Operation_details buffer);
	void updateLocalReg(Operation_details buffer);
	void updatelocalReg(Operation_details buffer);
	bool isinWriteLocalList(int rx);
	bool isinReadLocalList(int rx);
	void insertALUbuffer(IssueUnit::Operation_details buffer);
	void updateIssuebuffer();

private:
    map <int, fetchUnit::InstrChar>::iterator it;
	int countALU1, countALU2;
	int size;
	int i;
	int val;
	int x,y;
	int storecount;
	bool hit;
	int rs,rt,rd;
	bool status;
	bool pointer_issue;
	Operation_details var, var1, var2, var_dummy, var3;
	bool statusRAW, statusRAW1, statusRAW2, statusWAR, statusWAW, finalstatus;
};

#endif /* ISSUEUNIT_H_ */
