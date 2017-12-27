/*!
 * This file generates a binary file containing [14][32400][282] (~500Mb) float values and
 * another binary file containing [10][282] float values chosen from the above set.
 * The challenge is to find the best matching x out of 14 that the [10][282] values were chosen from.
 */
#include <fstream>
#include <iostream>
#include <stdlib.h>
#include <math.h>
#include <limits>
#include <unordered_map>

using namespace std;

float*** dat;
float** A;
float** B;
float** input;

//Sort
int partition_(float *arr, int left,int right,int n){
    //Partition
    float pivot = arr[right];
    int pIndex = left;
    for(int i=left;i<right;i++){
        if (arr[i]<pivot){
         swap(arr[i],arr[pIndex]);
         swap(A[i][n],A[pIndex][n]);
         pIndex++;
        }
    }
    swap(arr[pIndex],arr[right]);
    swap(A[pIndex][n],A[right][n]);
    return pIndex;
}

void quickSort(float *arr, int left,int right,int n){
    if(left>=right) return;
    int pIndex = partition_(arr,left,right,n);
    //Recursion
    quickSort(arr, left, pIndex-1,n);
    quickSort(arr, pIndex+1, right,n);
}

//Search Function, return closet column index
int binarySearch(float search_key,float *arr,int first,int last, int maxIndex)
{
    int mid = first+(last-first)/2;
    int index=mid;

    if (first >= last){
      if (index==0||index==maxIndex) return index;
      else if (arr[index]<search_key) return ((search_key-arr[index])<(arr[index+1]-search_key))? index:index+1;
      else return((arr[index]-search_key)<(search_key-arr[index-1]))? index:index-1;
    }
    else{
      //Recursion
      if (search_key < arr[mid])
        index = binarySearch(search_key,arr,first,mid, maxIndex);
      else
        index = binarySearch(search_key,arr, mid+1,last, maxIndex);
    }
 }

int main(void) {

  const unsigned int x = 14;  //length,3D
  const unsigned int y = 32400;  //width,3D
  const unsigned int z = 282;   //depth
  const unsigned int w = 10;    //length,2D


  dat = new float**[x];
    //3D array
  for(unsigned int i=0; i<x; ++i){
        dat[i] = new float*[y];
        for(unsigned int j=0; j<y; ++j)
            dat[i][j] = new float[z];
  }
    //Array to retain original index for each row
    A = new float*[y];
    for(unsigned int j=0; j<y; ++j){
      A[j] = new float[z];
    }
    //Array to collect values aft sort for each row
    B = new float*[z];
    for(unsigned int k=0; k<z; ++k){
      B[k] = new float[y];
    }

    //2D array to be searched
    input = new float*[w];
    for(unsigned int i=0; i<w; ++i){
    input[i] = new float [z];
    }

  //Read dat
  ifstream inFile;
  inFile.open("database.dat", ios::in|ios::binary);
  for(unsigned int i=0; i<x; ++i){
    for(unsigned int j=0; j<y; ++j){
      for(unsigned int k=0; k<z; ++k){
        inFile.read((char*)&dat[i][j][k],sizeof(float));
      }
    }
  }
  inFile.close();

  //Change a row
  ofstream outFile2;
  outFile2.open("input.dat",ios::out|ios::binary);
  unsigned int c = 11;
  unsigned int vecs[10];
  for(unsigned int i=0; i<10; ++i) {
    float val = static_cast<float>(rand())/static_cast<float>(RAND_MAX);
    unsigned int vecIndex = val * y;
    for(unsigned int k=0; k<z; ++k){
      float valAlter = dat[c][vecIndex][k]+0.00002*(static_cast<float>(rand())/static_cast<float>(RAND_MAX));
      outFile2.write((char*)&valAlter,sizeof(float));
      //cout << dat[c][vecIndex][k] << " : " << valAlter << endl;
    }
  }
  outFile2.close();

  //Read from file stream

  ifstream inFile2;
  inFile2.open("input.dat", ios::in|ios::binary);
    for(unsigned int i=0; i<w; ++i){
      for(unsigned int j=0; j<z; ++j){
        inFile2.read((char*)&input[i][j],sizeof(float));
      }
    }
  inFile2.close();

  /*--Solution--*/

  //Standardized database&input
  for(unsigned int k=0; k<z; ++k){
        float sum = 0.0;
        float mean = 0.0;
        float stdDev = 0.0;
      for(unsigned int i=0; i<x; ++i){
        for(unsigned int j=0; j<y; ++j){
            sum +=dat[i][j][k];
        }
      }
        mean = sum/(x*y);                   //mean for each depth
       for(unsigned int i=0; i<x; ++i){
         for(unsigned int j=0; j<y; ++j){
            stdDev += pow(dat[i][j][k] - mean, 2);
        }
       }
        stdDev = sqrt(stdDev / (x*y));     //standard dev for each depth
        for(unsigned int i=0; i<x; ++i){
         for(unsigned int j=0; j<y; ++j){
            dat[i][j][k]=(dat[i][j][k]-mean)/stdDev;     //standardize database
         }
        }
        for(unsigned int j=0; j<w; ++j){
            input[j][k]=(input[j][k]-mean)/stdDev;      //standardize input
        }
  }

   int ans = -1;
   float sum=std::numeric_limits<float>::max();  //min diff of sum of 10 depths
   for(unsigned int i=0;i<x;++i){
        //Sort
       for(unsigned int k=0;k<z;++k){
         for (unsigned int j=0;j<y;++j){
            A[j][k]=j;
            B[k][j]= dat[i][j][k];
         }
         quickSort(B[k],0,y-1,k);
       }

       float totSum = 0.0;  //tot diff of 10 depths
       unsigned int j=0;
       for(j;j<w;++j){
            int counter =0;
            int closeCol = -1;
            int rRange = 1;
            int C[y]={0};   //To store closet column
            float minDiff = std::numeric_limits<float>::max();   //min diff of a depth
            int closetCol[z] = {-1};
            int tmp=0;
            for(unsigned int k=0;k<z;++k){  //find rRange closet columns
                 closetCol[k] = binarySearch(input[j][k],B[k],0,y-1, y-1);
                 tmp = A[closetCol[k]][k];    //map to original column
                 C[tmp]++;
            }
            int previousRange=1;
            while(counter<3){
                 int fColCount=pow(2,counter); //fColCount = fColCount*2
                 int colCount = 0;   //number of fully ranked column
                 int curCloseCol = -1;    //the column with min diff

                 while (colCount<fColCount){
                     for(unsigned int k=0;k<z;++k){
                         if(rRange != previousRange) {

                           int left=closetCol[k];    //head moving left
                           int right=closetCol[k];   //head moving right

                           if((left-1)>=0&&((right==y-1)||fabs(input[j][k]-B[k][left-1])<fabs(B[k][right+1]-input[j][k]))){       //move left
                                    tmp = A[left-1][k];       //count column
                                    C[tmp]++;
                                    left--;
                                }
                            else {                             //move right
                                 tmp = A[right+1][k];          //count column
                                 C[tmp]++;
                                 right++;
                            }
                          }
                        }//end of for

                     colCount = 0;
                     float diff = 0.0;    //actual diff of a depth along fully ranked column
                     for(unsigned int m=0;m<y;++m ){
                         if (C[m]>=z) {  //check if any fully ranked
                            colCount++;
                            for(unsigned int k=0;k<z;++k){           //calculate diff along depth
                                diff=diff+fabs(dat[i][m][k]-input[j][k]);
                            }
                            if (diff!=0&&minDiff>=diff) {  //update min diff of a depth
                            curCloseCol=m;  //store closet fully ranked column
                            minDiff= diff;
                            }
                         }
                     }
                    previousRange = rRange;
                    if (colCount>=fColCount) break;
                    rRange++;
                }

                if(closeCol==-1) { closeCol=curCloseCol; counter++;}       //update counter
                else if (closeCol==curCloseCol) counter++;
                else {counter=1;closeCol = curCloseCol;}
            }
                totSum += minDiff;       //update diff of 10 depth
                if (totSum>sum) break;   //pruning
        }
        if (j==w&&sum>totSum) {
            sum = totSum;
            ans = i;
        }
   }

   cout<<"ans"<<ans<<endl;
 //deallocate
  for(unsigned int i=0; i<y; ++i){
    delete[] A[i];
  }
  delete[] A;

 for(unsigned int i=0; i<z; ++i){
    delete[] B[i];
  }
  delete[] B;

  for(unsigned int i=0; i<w; ++i){
    delete[] input[i];
  }
  delete[] input;

  for(unsigned int i=0; i<x; ++i){
    for(unsigned int j=0; j<y; ++j){
      delete[] dat[i][j];
    }
    delete[] dat[i];
  }
  delete[] dat;


  return 0;
}
