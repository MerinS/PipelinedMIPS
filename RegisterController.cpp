/*
 * RegisterController.cpp
 *
 *  Created on: Nov 9, 2017
 *      Author: merinsan
 */

#include "RegisterController.h"

void RegisterController::setRegReadbusy(int r1, int r2){
	if(r1 != -1){
		RegisterController::R[r1].readstatus = true;
	}
	if(r1 != -1){
		RegisterController::R[r2].readstatus = true;
	}
	return;
}

void RegisterController::setRegWritebusy(int r1){
	if(r1 != -1){
		RegisterController::R[r1].writestatus = true;
	}
	return;
}

bool RegisterController::isRegAvailRead(int Regno){
	return !(R[Regno].writestatus);
}
