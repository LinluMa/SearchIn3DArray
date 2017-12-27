# SearchIn3DArray
Given a 3D array of size 14*32400*282 and a 2D array created from a particular plane of the 3D array(10*282), this algorithm perform the search and return the row number of the plane where 2D array are created from.

-Run database.cpp to create to 14*32400*282 array
-Run solution.cpp to generate the 2D array and return the result

-How the 2D array is created from 3D one?
Select a row from the 3D array
Randomly select 10 columns from 32400 columns
And copy the this 10 arrays to the 2D array, with a small random offset of around 0.00002 of the actual value 

-Algorithm
 1. Pre-processing (on database of 14 * 32400 * 282 values)

1.1 Standardize values
  For each depth, compute mean and std. dev. 
  Replace the values in the database with the 
  standardized values.

1.2 Sort
  For each row, 
    For each depth,
      Sort the columns based on their standardized 
      value in the particular depth. 
      Retain the corresponding column index after 
      the sort.


2. Searching (given an input of 10x282 values)

2.1 For each row, for each 1x282 of the input, 
      find the closest column:

2.1.1 Initialize number of "fully ranked" columns, fColCount=1

2.1.2 Initialize rank range, rRange = 1

2.1.3 For each depth:

2.1.3.1 Based on standardized value difference, 
             find the rRange closest columns.
2.1.3.2 Store the standardized value differences 
             of the rRange closest columns.
2.1.3.3 Check the number of columns that have 
             value differences in all depths computed.
             If this is less than fColCount,
             increment rRange and repeat 2.1.3.

2.1.4 Compare the "fully ranked" columns
          found in 2.1.3 to get the one with minimum 
          difference. Store this column index.

2.1.5 Set fColCount = fColCount * 2.
          If 2.1.4 has returned the same column
          index 3 times consecutively, the column
          is the closest to the 1x282 input. Exclude column
          from the next 1x282 input.

2.2 Find the row with the minimum standard diff for all 10
      columns.
