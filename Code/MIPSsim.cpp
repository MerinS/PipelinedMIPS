/* On my honor, I have neither given nor received unauthorized aid on this assignment */
/* Author - Merin Santhosh Date - Nov 6,2017*/

#include <iostream>
#include <sstream>
#include <fstream>
#include <map>
#include <math.h>
#include <stdlib.h>
#include <string>
#include <memory>
#include <queue>
#include "Fileprint.h"
#include "FileParser.h"
#include "controller.h"
#include "DataController.h"
#include "RegisterController.h"
#include "IssueUnit.h"
#include "ALU1Unit.h"
#include "MemUnit.h"
#include "ALU2Unit.h"

using namespace std;

//declaring the static variables of fetch class in the global space
queue<int> empty;
bool fetchUnit::mstallFetchUnit  = false;
queue <int> fetchUnit::preIssue = empty;
queue <int> fetchUnit::preIssue_dummy = empty;
int fetchUnit::PC               = PC_START;
int fetchUnit::Rstall[]        = {-1,-1};

//declaring the static variables of issue class in the global space
queue<IssueUnit::Operation_details> empty1;
set <int, greater <int> > empty2;
queue <IssueUnit::Operation_details> IssueUnit::preALU1buffer = empty1;
queue <IssueUnit::Operation_details> IssueUnit::preALU2buffer = empty1;
queue <IssueUnit::Operation_details> IssueUnit::preALU1buffer_dummy = empty1;
queue <IssueUnit::Operation_details> IssueUnit::preALU2buffer_dummy = empty1;
set <int, greater <int> > IssueUnit::list_read = empty2;
set <int, greater <int> > IssueUnit::list_write = empty2;
int IssueUnit::R[] = {-1,-1};

//declaring the static variables that controls Registers in the RegisterController class in the global space
RegisterController::Register RegisterController::R[] = {{0,false,false},{0,false,false},{0,false,false},{0,false,false},{0,false,false},{0,false,false},{0,false,false},{0,false,false},{0,false,false},{0,false,false},{0,false,false},{0,false,false},{0,false,false},{0,false,false},{0,false,false},{0,false,false},{0,false,false},{0,false,false},{0,false,false},{0,false,false},{0,false,false},{0,false,false},{0,false,false},{0,false,false},{0,false,false},{0,false,false},{0,false,false},{0,false,false},{0,false,false},{0,false,false},{0,false,false},{0,false,false}};

//declaring the static variables that controls Data in the DataController class in the global space
pair<int, int> empty3;
map<int, int> DataController::data_instructions;

//declaring the static variables of ALU1 unit class in the global space
queue <IssueUnit::Operation_details> ALU1Unit::preMEMbuffer = empty1;
queue <IssueUnit::Operation_details> ALU1Unit::preMEMbuffer_dummy = empty1;

//declaring the static variables of MEM unit class in the global space
queue <IssueUnit::Operation_details> MemUnit::postMEMbuffer = empty1;
queue <IssueUnit::Operation_details> MemUnit::postMEMbuffer_dummy = empty1;

//declaring the static variables of ALU1 unit class in the global space
queue <IssueUnit::Operation_details> ALU2Unit::postALU2buffer = empty1;
queue <IssueUnit::Operation_details> ALU2Unit::postALU2buffer_dummy = empty1;

int main(int argc, char const *argv[]) {
	if (argc != 2) {
	    cerr
	      << "Error in Usage. Correct usage: ./MIPSsim sample.txt"
	      << endl;
    	return -1;
	}

    //File name is parsed in 
	string filename = argv[1];
	FileParser parser;
	//Calling the function to parse the file using the FileParser class
	parser.readMIPSFile(filename);

	//init Data memory
	DataController::data_instructions.insert( pair<int, int>(300, -1));
	DataController::data_instructions.insert( pair<int, int>(304, -2));
	DataController::data_instructions.insert( pair<int, int>(308, -3));
	DataController::data_instructions.insert( pair<int, int>(312, 1));
	DataController::data_instructions.insert( pair<int, int>(316, 2));
	DataController::data_instructions.insert( pair<int, int>(320, 4));
	DataController::data_instructions.insert( pair<int, int>(324, -4));
	DataController::data_instructions.insert( pair<int, int>(328, 10));
	DataController::data_instructions.insert( pair<int, int>(332, 7));
	DataController::data_instructions.insert( pair<int, int>(336, 9));
	DataController::data_instructions.insert( pair<int, int>(340, 1));
	DataController::data_instructions.insert( pair<int, int>(344, 0));
	DataController::data_instructions.insert( pair<int, int>(348, -1));
	DataController::data_instructions.insert( pair<int, int>(352, 1));
	DataController::data_instructions.insert( pair<int, int>(356, -1));
	DataController::data_instructions.insert( pair<int, int>(360, 0));

	controller controlseq;

	fetchUnit::Instype instr_details;
	instr_details.type = -1;

	IssueUnit iu;
	IssueUnit::Operation_details info;

    //initialisation of classes and associated variables for writing out into the simulation
	stringstream ss;
	ofstream outputfile;
	outputfile.open ("simulation.txt");
	Fileprint filep;
	string str;

	map <int, fetchUnit::InstrChar>::iterator it;
	int count = 1;
	while(1){
		//PreIssue Buffer is evaluated prior to fetching as ALU and MEM type instructions in the preissue buffer are given
		//preference for  exection over branch type instructions in that are fetched in the current cycle
		controlseq.evalPreIssueBuffer();

        //if a break is encountered
		if (instr_details.type == 0){
			printf("breaking out of the MIPSIM main \n");
			break;
		}
		else{
			instr_details.type = -1;
			if(fetchUnit::preIssue.size() + fetchUnit::preIssue_dummy.size()< 4){
				//Start fectching instructions into the PreIssue buffer
				instr_details = controlseq.startFetching(IssueUnit::preALU2buffer_dummy.size(), IssueUnit::preALU1buffer_dummy.size());
			}
		}

		//evalPreALU1 buffer and start execute ALU1
		controlseq.startALU1();
		//evalPreMEM buffer and start execute MEM
		controlseq.startMEM();
		//evalPreALU2 buffer and start execute ALU2
		controlseq.startALU2();
		//start Writeback 
		controlseq.startWriteBack();

		//in all the stages, the instructions are added to the dummy buffers
		//only at the end the actual buffers are updated
		//this is to prevent the instructions pushed in the same cycle from getting executed in the same one
		controlseq.updatebuffers();



		/* ***************************************************************************************************
           START OF THE BLOCK THAT DOES PRINTING OF CURRENT STATUS OF VARIOUS UNITS, REGISTERS AND DATA MEMORY        
           ***************************************************************************************************
        */
		printf("\n\nCycle = %d \n", count);
		outputfile<<"--------------------"<<"\n";
	    outputfile<<"Cycle:"<<count<<"\n";

		outputfile<<"IF Unit:"<<"\n";
		str =filep.printPC(instr_details.PC1);
		if(instr_details.type == 1){
			if(instr_details.state == 1){
				outputfile<<"\t\t"<<"Waiting Instruction: \n";
				outputfile<<"\t\t"<<"Executed Instruction: "<<str;
			}
			else{
				outputfile<<"\t\t"<<"Waiting Instruction: "<<str;
				outputfile<<"\t\t"<<"Executed Instruction: \n";
			}
		}
		else if(instr_details.type == 0 || instr_details.type == 4 ){
			outputfile<<"\t\t"<<"Waiting Instruction: \n";
			outputfile<<"\t\t"<<"Executed Instruction: "<<str;
		}
		else{
			outputfile<<"\t\t"<<"Waiting Instruction: \n";
			outputfile<<"\t\t"<<"Executed Instruction: \n";
		}


		int i = 0;
		int size = fetchUnit::preIssue.size();
		outputfile<<"Pre-Issue Queue: \n";
		while(i< 4){
			outputfile<<"Entry "<<i<<":";
			if(i<size){
				int val1 = fetchUnit::preIssue.front();
				it = fetchUnit::address_instructions.find(val1);
				fetchUnit::InstrChar buffer = it->second;
				info = iu.instrparse(buffer);
				str = filep.Printfunction(info);
				outputfile<<str;
				fetchUnit::preIssue.pop();
				fetchUnit::preIssue.push(val1);
			}
			outputfile<<"\n";
			i+=1;
		}

		i = 0;
		IssueUnit::Operation_details var;
		size = IssueUnit::preALU1buffer.size();
		outputfile<<"Pre-ALU1 Queue: \n";
		while(i< 2){
			outputfile<<"\t"<<"Entry "<<i<<":";
			if(i<size){
				var = IssueUnit::preALU1buffer.front();
				str = filep.Printfunction(var);
				outputfile<<str;
				IssueUnit::preALU1buffer.pop();
				IssueUnit::preALU1buffer.push(var);
			}
			outputfile<<"\n";
			i+=1;
		}

		i = 0;
		size = ALU1Unit::preMEMbuffer.size();
		outputfile<<"Pre-MEM Queue: ";
		if(size == 1){
			var = ALU1Unit::preMEMbuffer.front();
			str = filep.Printfunction(var);
			outputfile<<str;
			ALU1Unit::preMEMbuffer.pop();
			ALU1Unit::preMEMbuffer.push(var);
		}
		outputfile<<"\n";


		i = 0;
		size = MemUnit::postMEMbuffer.size();
		outputfile<<"Post-MEM Queue: ";
		if(size == 1){
			var = MemUnit::postMEMbuffer.front();
			str = filep.Printfunction(var);
			outputfile<<str;
			MemUnit::postMEMbuffer.pop();
			MemUnit::postMEMbuffer.push(var);
			i+=1;
		}
		outputfile<<"\n";

		i = 0;
		size = IssueUnit::preALU2buffer.size();
		outputfile<<"Pre-ALU2 Queue: \n";
		while(i< 2){
			outputfile<<"\t"<<"Entry "<<i<<":";
			if(i< size){
				var = IssueUnit::preALU2buffer.front();
				str = filep.Printfunction(var);
				outputfile<<str;
				IssueUnit::preALU2buffer.pop();
				IssueUnit::preALU2buffer.push(var);
			}
			outputfile<<"\n";
			i+=1;
		}


		outputfile<<"Post-ALU2 Queue: ";
		i = 0;
		size = ALU2Unit::postALU2buffer.size();
		if(size == 1){
			var = ALU2Unit::postALU2buffer.front();
			str = filep.Printfunction(var);
			outputfile<<str;
			ALU2Unit::postALU2buffer.pop();
			ALU2Unit::postALU2buffer.push(var);
			i+=1;
		}
		outputfile<<"\n";

		outputfile<<"Registers"<<"\n";
		outputfile<<"R00:"<<"\t"<<RegisterController::R[0].value<<"\t"<<RegisterController::R[1].value<<"\t"<<RegisterController::R[2].value<<"\t"<<RegisterController::R[3].value<<"\t"<<RegisterController::R[4].value<<"\t"<<RegisterController::R[5].value<<"\t"<<RegisterController::R[6].value<<"\t"<<RegisterController::R[7].value<<"\n";
		outputfile<<"R08:"<<"\t"<<RegisterController::R[8].value<<"\t"<<RegisterController::R[9].value<<"\t"<<RegisterController::R[10].value<<"\t"<<RegisterController::R[11].value<<"\t"<<RegisterController::R[12].value<<"\t"<<RegisterController::R[13].value<<"\t"<<RegisterController::R[14].value<<"\t"<<RegisterController::R[15].value<<"\n";
		outputfile<<"R16:"<<"\t"<<RegisterController::R[16].value<<"\t"<<RegisterController::R[17].value<<"\t"<<RegisterController::R[18].value<<"\t"<<RegisterController::R[19].value<<"\t"<<RegisterController::R[20].value<<"\t"<<RegisterController::R[21].value<<"\t"<<RegisterController::R[22].value<<"\t"<<RegisterController::R[23].value<<"\n";
		outputfile<<"R24:"<<"\t"<<RegisterController::R[24].value<<"\t"<<RegisterController::R[25].value<<"\t"<<RegisterController::R[26].value<<"\t"<<RegisterController::R[27].value<<"\t"<<RegisterController::R[28].value<<"\t"<<RegisterController::R[29].value<<"\t"<<RegisterController::R[30].value<<"\t"<<RegisterController::R[31].value<<"\n\n";

		outputfile<<"Data"<<"\n";
		int data_start  = 300;
		for (int i = 0; i < 2; ++i)
		{
		  outputfile<<data_start<<":\t";
		  for (int j = 0; j < 8; ++j)
		  {
			outputfile<<DataController::data_instructions[data_start]<<"\t";
			data_start +=4;
		  }
		  outputfile<<"\n";
		}
		outputfile<<"\n";
        /* *************************************************************************************************
           END OF THE BLOCK THAT DOES PRINTING OF CURRENT STATUS OF VARIOUS UNITS, REGISTERS AND DATA MEMORY       
           ************************************************************************************************* 
        */

		count +=1;
	}
	outputfile.close();
}
