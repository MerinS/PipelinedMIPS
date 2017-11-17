/*
 * utilityFunctions.cpp
 *
 *  Created on: Nov 6, 2017
 *      Author: merinsan
 */

#include "utilityFunctions.h"

////utilityFunctions contain functions used for parsing binary and int
int utilityFunctions::binarytoint(char* bin_array, int bit_no){
	int multiplier = 1;
	int bin = 0;
	for (int i = (bit_no-1); i >= 0; --i )
	{
	    bin += (multiplier * (bin_array[i]-'0'));
	    multiplier *= 2;
	}
	return bin;
}

int utilityFunctions::signedbinarytoint(char* bin_array, int bit_no){
  if(bin_array[0] == '1'){
      int x;
      int multiplier = 1;
      int bin = 0;
      for (int i = (bit_no-1); i >= 0; --i ){
        if(bin_array[i]-'0'==1){
          x = 0;
        }
        else{
          x = 1;
        }
        bin += (multiplier * x);
        multiplier *= 2;
      }
      return (-1*(bin+1));
  }
  else {
    return binarytoint(bin_array, bit_no);
  }
}
