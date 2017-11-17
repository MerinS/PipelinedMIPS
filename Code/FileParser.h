/*
 * FileParser.h
 *
 *  Created on: Nov 6, 2017
 *      Author: merinsan
 */
#include <cstdlib>
#include <cstring>
#include <string>
#include <stdio.h>
#include <map>
#include <fstream>
#include <iostream>
#include <sstream>
#include <math.h>
#include <stdlib.h>
#include <memory>
#include "utilityFunctions.h"

using namespace std;

const int CAT_START          = 0;
const int INST_START         = 2;
const int INFO_START         = 6;
const int INFO_END           = 32;
const int PC_START           = 256;

#ifndef FILEPARSER_H_
#define FILEPARSER_H_

class FileParser {
	public:
	void insertval(int val_array[5], int val1, int val2, int val3, int val4);
	void readMIPSFile(const string &MIPSFilename);
	int categorize(int a, int b);

	private:
	string buffer;
    int rs, rd, rt, offset, sa, imm_val, jump_mem,mem_eff, jr_mem, output,base;
};

#endif /* FILEPARSER_H_ */
