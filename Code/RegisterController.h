/*
 * RegisterController.h
 *
 *  Created on: Nov 9, 2017
 *      Author: merinsan
 */

#ifndef REGISTERCONTROLLER_H_
#define REGISTERCONTROLLER_H_


//Class that controls the Registers in the system
//monitors the read and write status of the registers
class RegisterController {
public:
	struct Register{
		int value;
		//true when it is being read from false when it is not being read from
		bool readstatus;
		//true when it is being written to false when it is not being written to
		bool writestatus;
	};

	void setRegReadbusy(int r1, int r2);
	void setRegWritebusy(int r1);
	bool isRegAvailRead(int Regno);
	static Register R[32];
};

#endif /* REGISTERCONTROLLER_H_ */
