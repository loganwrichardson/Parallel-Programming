#include <iostream>
#include <unistd.h>
#include <cstdint>
#include <chrono>
#include <time.h>
#include <functional>
#include <omp.h>
#include "sorter.h"
#include "SeqMergeSort.h"
#include "SeqQuickSort.h"
#include "ParaMergeSort.h"
#include "ParaQuickSort.h"
#include "DataSet.h"
#include "bmsData.h"

/* headers for functions in this file */
static void parseArgs(int32_t argc, char * argv[], uint64_t & size, 
	int32_t & threadCt, bool & runSeq, bool & runQuick);
static void usage();
static float * createSortData(int64_t size);
static void saveSortedData(int32_t * idList, int64_t size);
static int32_t * createIDList(int64_t size);
static void runSort(int32_t which, std::string errMsg, float * data, 
	int32_t * idList, int32_t size, int32_t threadCt);

#define MERGESORT 0
#define QUICKSORT 1
#define NUMSORTS 2

/* To run code:
 * ./sorter -n <n> -t <t> [-m] [-q] 
 * size of the array: 1 << <n>; e.g. 2 to the power of <n>
 * number of threads: <t>
 * if -m option is provided, sequential and parallel mergesorts are performed
 * if -q option is provided, sequential and parallel quicksorts are performed
 */
int32_t main(int32_t argc, char * argv[])
{
    uint64_t size = 0;                        //amount of data to sort
    int32_t threadCt = 8;                     //default number of threads
    bool runMerge = false;                    //perform merge sort
    bool runQuick = false;                    //perform quick sort
    float * data;                             //pointer to data to sort
    int32_t * idList;			      //pointer to list of IDs	
    /* parse command line arguments to get array size, thread count */
    /* and which sorts to run */
    parseArgs(argc, argv, size, threadCt, runMerge, runQuick);

    printf("Sorting an array of size %ld.\n", size);
    printf("Parallel versions use %d threads.\n", threadCt);

    /* create data to sort */
    data = createSortData(size);
    
    /* create list of IDs */
    idList = createIDList(size);

    /* run one or both sorts */
    if (runMerge)
    {
	runSort(MERGESORT, "Parallel mergesort failed.", data, idList, size, threadCt);
    }
    if (runQuick)
    {
	runSort(QUICKSORT, "Parallel quicksort failed.", data, idList,  size, threadCt);
    }

    /* save the sorted data */
    saveSortedData(idList, size);

    delete [] data;
}

/*
 * runSort
 * Runs the sort indicated by which, both the sequential and parallel
 * versions.
 * Inputs:
 * which - indicates the sort, either MERGESORT or QUICKSORT
 * errMsg - error message to output if the parallel result doesn't match the 
 *          sequential result
 * size - size of array to sort
 * data - array to sort
 * threadCt - number of threads to be used by parallel version
 */
void runSort(int32_t which, std::string errMsg,
	float * data, int32_t * idList, int32_t size, int32_t threadCt)
{
    //create an array of pointers to functions that create sequential sort objects
    auto makeSeqMergeSort = [&] (){ return new SeqMergeSort(size, data, idList); };
    auto makeSeqQuickSort = [&] (){ return new SeqQuickSort(size, data, idList); };
    std::function<Sorts *()> makeSeqSort[NUMSORTS] = {makeSeqMergeSort, makeSeqQuickSort};

    //create an array of pointers to functions that create parallel sort objects
    auto makeParaMergeSort = [&] (){ return new ParaMergeSort(size, data, idList, threadCt); };
    auto makeParaQuickSort = [&] (){ return new ParaQuickSort(size, data, idList, threadCt); };
    std::function<Sorts *()> makeParaSort[NUMSORTS] = {makeParaMergeSort, makeParaQuickSort};

    Sorts * seqPtr, * paraPtr;

    /* create the sequential sort object and perform sort */ 
    seqPtr = makeSeqSort[which]();
    double seqTime = seqPtr->sort();

    /* create the parallel sort object and perform sort */ 
    paraPtr = makeParaSort[which]();
    double paraTime = paraPtr->sort();
    /* check to see if the sequential result matches the parallel result */
    if (!paraPtr->match(seqPtr)) 
    { 
	printf("%s\n", errMsg.c_str());
	exit(1);
    }
    /* output the times and speedup */
    printf("\n%s\n", seqPtr->getDescription().c_str());
    printf("Time: %2.6f\n", seqTime);

    printf("\n%s\n", paraPtr->getDescription().c_str());
    printf("Time: %2.6f\n", paraTime);
    printf("Speedup: %2.6f\n", seqTime/paraTime);
    delete seqPtr;
    delete paraPtr;
}

/*
 * createSortData
 * Dynamically allocates space for size float values and initializes those
 * values to the highestCellVolt column of size, size.
 * Input:
 * size - number of elements to be allocated 
 * Output:
 * pointer to the allocated data
 */
float * createSortData(int64_t size)
{
    DataSet * dataSet = new DataSet;
    bms_data = dataSet -> createSortData(size);  
    float * data = new float[size];
    for (int64_t i = 0; i < size; i++)
    {
	*(data + i) = bms_data[i].hiLoMetrics.highestCellVolt;
    }
    return data;
}

/*
 * saveSortData
 * Saves the sorted bmsData back to csv form
 * Input:
 * idList - identifies the ordering of locations of bms_data to save
 * size - number of elements to be saved
 * Output:
 * whether the save was successful
 */
void saveSortedData(int32_t * idList, int64_t size)
{
    DataSet * dataSet = new DataSet;
    dataSet -> saveSortData(bms_data, idList, size);  
}

/*
 * createIDList
 * Dynamically allocates space for size int32_t values and initializes those
 * values to ID elements for rearrangement identification.
 * Input:
 * size - number of elements to be allocated 
 * Output:
 * pointer to the allocated data
 */
int32_t * createIDList(int64_t size)
{
    int32_t * idList = new int32_t[size];
    for (int32_t i = 0; i < size; i++)
    {
	*(idList + i) = i;
    }
    return idList;
}

/*
 * parseArgs
 * Takes as input the command line arguments, parses them,
 * and sets size, threadCt, runMerge, and runQuick
 * Inputs: 
 * argc is count of command line arguments
 * argv[1] ... argv[argc - 1] are actual command line arguments
 * Returns:
 * size - size of the array to be generated 
 * threadCt - number of threads to use in the parallel sort
 * runMerge - set to true if the mergesort is to be executed
 * runQuick - set to true if the quicksort is to be executed
 */
void parseArgs(int32_t argc, char * argv[], uint64_t & size, int32_t & threadCt, 
	bool & runMerge, bool & runQuick)
{
    if (argc < 4) usage();  //must include: sort -n <n> and at least one of -m, -q
    int32_t opt;
    while((opt = getopt(argc, argv, "n:t:mq")) != -1)
    {
	switch(opt)
	{
	    case 'n':
		if (atoi(optarg) <= 3) //in case a negative value is entered
		    size = 0;
		else
		    size = (1 << atoi(optarg));
		break;
	    case 't':
		threadCt = atoi(optarg);
		break;
	    case 'm':
		runMerge = true;
		break;
	    case 'q':
		runQuick = true;
		break;
	    default:
		usage();
	}
    }
    /* make sure size is big enough */
    if (size <= 8) 
    {
	printf("-n argument must be greater than 5.\n"); 
	usage();
    }  
    /* use at least two threads and not more than the number of cores */
    if (threadCt < 2 || threadCt > sysconf(_SC_NPROCESSORS_ONLN)) 
    {
	printf("-t argument must be greater than 1 and less than %ld.\n", 
		sysconf(_SC_NPROCESSORS_ONLN) + 1); 
	usage();
    }  
    if (!runMerge && !runQuick)
    {
	printf("At least one sort needs to be performed (-m and/or -q).\n");
	usage();
    }
}

/*
 * usage
 * Prints usage information and exits.
 */
void usage()
{
    printf("usage: sort  -n <n> -t <t> [-m] [-q]\n\n");
    printf("\tRandomly generates an array of size (1 << <n>) integers and sorts the\n");
    printf("\tarray using up to four different techniques. If the -m option is provided,\n");
    printf("\tsequential and parallel mergesorts are performed. If the -q option is\n");
    printf("\tprovided, sequential and parallel quicksorts are performed.\n");
    printf("\tParallel speedups are provided using the results from the corresponding\n");
    printf("\tsequential sort. \n\n");
    printf("\t<n> must be greater than 5. The size of the array to sort will be \n");
    printf("\t2 to the power of <n> (1 << <n>).\n\n");
    printf("\t<t> must be greater than 1 and less than %ld.\n\n", 
	    sysconf(_SC_NPROCESSORS_ONLN) + 1);
    printf("\t-m causes sequential and parallel mergesorts to be performed and the parallel\n"); 
    printf("\tresult to be compared to the sequential result.\n\n");
    printf("\t-q causes sequential and parallel quicksorts to be performed and the parallel\n"); 
    printf("\tresult to be compared to the sequential result.\n\n");
    exit(0);
}

