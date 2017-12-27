/*!
 * This file generates a binary file containing [14][32400][282] (~500Mb) float values and
 * another binary file containing [10][282] float values chosen from the above set.
 * The challenge is to find the best matching x out of 14 that the [10][282] values were chosen from.
 */
#include <fstream>
#include <iostream>
#include <stdlib.h>

using namespace std;

int main(void) {

  const unsigned int x = 14;
  const unsigned int y = 32400;
  const unsigned int z = 282;

  ofstream outFile;
  outFile.open("database.dat", ios::out|ios::binary);
  for(unsigned int i=0; i<x; ++i) {
    for(unsigned int j=0; j<y; ++j) {
      for(unsigned int k=0; k<z; ++k) {
        float val = static_cast<float>(rand())/static_cast<float>(RAND_MAX);
	outFile.write((char*)&val, sizeof(float));
      }
    }
  }
  outFile.close();

  return 0;
}
