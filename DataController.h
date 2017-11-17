/*
 * DataController.h
 *
 *  Created on: Nov 13, 2017
 *      Author: merinsan
 */
#include <cstdlib>
#include <string>
#include <stdio.h>
#include <map>
#include <fstream>
#include <iostream>
#include <sstream>
#include <math.h>
#include <stdlib.h>
#include <memory>

using namespace std;

#ifndef DATACONTROLLER_H_
#define DATACONTROLLER_H_

//Class that one static map that controls the Data
class DataController {
public:
	static map <int, int> data_instructions;
};

#endif /* DATACONTROLLER_H_ */
