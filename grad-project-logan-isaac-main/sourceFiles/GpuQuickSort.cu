#include <string.h>
#include <ParaQuickSort.h>
#include <helpers.h>

/* 
 * GPUQuickSort
 * @author: Logan Richardson & Isaac Allen
 * @date: 6/16/2022
 * GPU QuickSort parallelization for Summer 2022 - Parallel Programming Class
 * Sources:
	Source 1: Dr. Cindy Norris, Programming Assignment 7: Revisiting Sorting
 *
*/


/*
 * GPUQuickSort constructor
 * Initialize threadCt and description.
 * Use constructor in parent Sorts class to initialize size and data.
 *
 * Source: Source 1
 */
GPUQuickSort::GPUQuickSort(uint64_t size, int32_t * data, int32_t threadCt):Sorts(size, data)
{
    this->threadCt = threadCt;
    description = "Parallel sort: parallelizes quicksort by creating tasks for partitioning and recursive calls";
}



/* 
    parseArgs
    This function parses the command line arguments to get
    the dimension of the matrices, the size of the thread blocks,
    the size of the tile of elements handled by one thread, the GPU
    kernel to be executed, and whether timing results should be
    produced. If the command line argument is invalid, it prints usage 
    information and exits.
    Inputs:
    argc - count of the number of command line arguments
    argv - array of command line arguments
    matrixDimP - pointer to an int to be set to the matrix dimensions
    blkDimP - pointer to an int to be set to the block dimensions
    tileSzP - pointer to an int to be set to the size of the tile
              of elements to be handled by one thread
    whichP - which kernel to execute
    doTimeP - pointer to a bool that is set to true or false if timing
              is to be performed

    Source:  Starter code from source 1
*/
void parseArgs(int argc, char * argv[], int * matrixDimP,
               int * blkDimP, int * tileSzP, int * whichP, bool * doTimeP)
{
    int i;
    //set the parameters to their defaults
    int dimExp = MATDIM_DEFAULT;
    int blkDim = BLKDIM_DEFAULT;
    int tileSz = TILESZ_DEFAULT;
    int which = WHICH_DEFAULT;
    bool doTime = false;

    //loop through the command line arguments
    for (i = 1; i < argc; i++)
    {
       if (i < argc - 1 && strcmp(argv[i], "-n") == 0)
       {
          dimExp = atoi(argv[i+1]);
          i++;   //skip over the argument after the -n
       }
       else if (i < argc - 1 && strcmp(argv[i], "-b") == 0)
       {
          blkDim = atoi(argv[i+1]);
          i++;   //skip over the argument after the -b
       }
       else if (i < argc - 1 && strcmp(argv[i], "-t") == 0)
       {
          tileSz = atoi(argv[i+1]);
          i++;   //skip over the argument after the -t
       }
       else if (strcmp(argv[i], "-seq") == 0)
          which = SEQ;
       else if (strcmp(argv[i], "-par") == 0)
          which = PAR;
       else if (strcmp(argv[i], "-gpu") == 0)
          which = GPU;
       else if (strcmp(argv[i], "-time") == 0)
          doTime = true;
       else
          printUsage();
    }

/*
 * sort
 * Takes as input an array of int32_t values and sorts them using 
 * a quicksort.
 * Input:
 * data - array of int32_t values
 * size - number of elements in the array
 * Modifies:
 * data - elements of data array in increasing sorted order
 * 
 * Source: Source 1
 */
double GPUQuickSort::sort() {
	TIMERSTART(GPU)
	quickSort(0, size - 1, data);
	TIMERSTOP(seq)
	return GETTIME(GPU);
}


/*
 * partition
 * This function partitions the data in the array starting at index
 * sIdx and ending at eIdx into three sections.  One section contains
 * the values below a pivot. The middle section contains the single
 * pivot. The third section contains the values greater than the pivot.
 * At the end, the pivot is in the correct location in the final sorted array.
 * Inputs:
 * sIdx - starting index into the data to sort
 * eIdx - ending index into the data to sort
 * data - array of data to sort
 * Modifies:
 * data
 * Returns:
 * location of pivot
 *
 * Source: Source 1
 */
int GPUQuickSort::partition(int32_t sIdx, int32_t eIdx, int32_t * data)
{
    /* This code doesn't require modification. */

    //define a lambda expression to do a swap 
    auto swap = [](auto & a, auto & b)
    {
        auto t = a;
        a = b;
        b = t;
    };

    //define a lambda expression to do a swap
    auto choosePivotIndex = [&](auto sIdx, auto eIdx, auto data)
    {
        if (sIdx == eIdx) return sIdx;
        int32_t mid = sIdx + (eIdx - sIdx)/2;
        if (data[sIdx] < data[mid] && data[mid] < data[eIdx])
            return mid;
        else if (data[mid] < data[sIdx] &&  data[sIdx] < data[eIdx])
            return sIdx;
        else
            return eIdx;
    };

    //choose a good pivot, get the index 
    int32_t pIdx = choosePivotIndex(sIdx, eIdx, data);

    //get the value of the pivot
    int32_t pivot = data[pIdx];

    //move pivot to end
    swap(data[pIdx], data[eIdx]);
}
