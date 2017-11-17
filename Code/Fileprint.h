/*
 * Fileprint.h
 *
 *  Created on: Nov 15, 2017
 *      Author: merinsan
 */
#include "fetchUnit.h"
#include "IssueUnit.h"
#include <string>

#ifndef FILEPRINT_H_
#define FILEPRINT_H_

//Class to control the printing of simulation output into a file
class Fileprint {
public:
	string printPC(int val);
	IssueUnit::Operation_details info;
	string to_string(int i);
	string Printfunction(IssueUnit::Operation_details info);
private:
	map <int, fetchUnit::InstrChar>::iterator it;
	int rs, rd, rt, offset, sa, imm_val, jump_mem,mem_eff, jr_mem, output,base;
	string str;
};

#endif /* FILEPRINT_H_ */
