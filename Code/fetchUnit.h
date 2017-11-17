/*
 * fetchUnit.h
 *
 *  Created on: Nov 7, 2017
 *      Author: merinsan
 */
#include <cstdlib>
#include <string>
#include <stdio.h>
#include <map>
#include <fstream>
#include <iostream>
#include <sstream>
#include <queue>
#include <math.h>

using namespace std;

#ifndef FETCHUNIT_H_
#define FETCHUNIT_H_

const int MAX_DATA_PER_INSTR = 6;

//Controls the entire stage of fetching instructions, evaluating them and when ideal, pushing them to the PreIssue buffer
//Branch instructions are evaluated in the fetch stage itself
class fetchUnit {
	public:
		struct InstrChar
		{
		  InstrChar(int in[MAX_DATA_PER_INSTR]) {
			copy(in, in+MAX_DATA_PER_INSTR, data);
		  }
		  int data[MAX_DATA_PER_INSTR];
		};

		struct Instype
		{
		  int type;
		  int PC1;
		  //in the case of branch
		  int state;
		};

		int isStalled();
		bool checkisstalled();
		void stallFetchUnit(int R1,int R2);

		int isbufslotAvailable();
		void updateFetchbuffer();

		Instype fetchandEval();
		Instype initInsttype(int type, int PC1, int state);
		Instype evaljump(InstrChar buffer, int PC);

		static bool mstallFetchUnit;
		static queue <int> preIssue_dummy;
		static queue <int> preIssue;
		static map <int, InstrChar> address_instructions;
		int noInstr;
		static int PC;
		static int Rstall[2];

	private:
		Instype ret_val, ret_val1, ret_branch;
		map <int, InstrChar>::iterator it;
		int rs, rt, offset, imm_val, jump_mem, mem_eff;
		int sum;
		int i;
        int size;
		bool state;
		int val;
};

#endif /* FETCHUNIT_H_ */