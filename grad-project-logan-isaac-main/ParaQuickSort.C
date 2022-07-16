#include <string.h>
#include "ParaQuickSort.h"
#include "helpers.h"
#include <atomic>

/*
 * ParaQuickSort constructor
 * Initialize threadCt and description.
 * Use constructor in parent Sorts class to initialize size and data.
 */
ParaQuickSort::ParaQuickSort(uint64_t size, float * data, int32_t * idList, int32_t threadCt):Sorts(size, data, idList)
{
    this->threadCt = threadCt;
    description = "Parallel sort: parallelizes quicksort by creating tasks for partitioning and recursive calls";
}

/*
 * sort
 * Sorts an array of int32_t values using OpenMP tasks. 
 * Input:
 * data - array of int32_t values
 * size - number of elements in the array
 * threadCt - number of threads to use to perform the sort
 * Modifies:
 * data - elements of data array in increasing sorted order
 */
double ParaQuickSort::sort()
{
    /* This code can be parallelized much the same way as 
     * the mergesort. 
     */
    TIMERSTART(para)
    #pragma omp parallel num_threads(threadCt)
    {
	#pragma omp single 
	{
	    quickSort(0, size - 1, data, idList);
	}
    }
    TIMERSTOP(para)
    /* return the amount of time taken */
    return GETTIME(para);
}

/*
 * quickSort
 * Performs a parallel quicksort by executing the two recursive
 * calls to quicksort in parallel.
 * Inputs:
 * sIdx - starting index into the data to sort
 * eIdx - ending index into the data to sort
 * data - array of data to sort
 * Modifies:
 * data
 */
void ParaQuickSort::quickSort(int32_t sIdx, int32_t eIdx, float * data, int32_t * idList)
{
    float mid;
    if (sIdx >= eIdx) return;

    //1. Like mergeSort, you'll need a taskCt to limit the number of tasks that
    //   are created. 
    //2. Don't create a task to handle partition since that needs to be completed
    //   before the calls to quickSort
    //3. Do create a task for each of the calls to quickSort
    //4. Multiple threads will be trying to update taskCt; avoid a race
    //   Add one to it for each tasks that is created (add 2 for two tasks)

    //You'll need an if-else based upon the number of tasks.
    //The code below will be in both the if and the else parts, but one part will create
    //tasks.
    static std::atomic<uint16_t> taskCt;  //Static will make it shared among all threads (uncomment declaration when ready)
       
    mid = partition(sIdx, eIdx, data, idList);
    if (taskCt < 2 * threadCt) {
	taskCt += 2;
	    #pragma omp task
	    {
		quickSort(sIdx, mid - 1, data, idList);
	    }

	    #pragma omp task
	    {
		quickSort(mid + 1, eIdx, data, idList);
	    }
    } else 
    {
	quickSort(sIdx, mid - 1, data, idList);
	quickSort(mid + 1, eIdx, data, idList);
    }
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
 * idList - array of IDs to sort
 * Modifies:
 * data
 * idList
 * Returns:
 * location of pivot
 */
int ParaQuickSort::partition(int32_t sIdx, int32_t eIdx, float * data, int32_t * idList)
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
    float pivot = data[pIdx];

    //move pivot to end
    swap(idList[pIdx], idList[eIdx]);
    swap(data[pIdx], data[eIdx]);

    int32_t end = eIdx - 1;
    while (sIdx < end)
    {
	//from beginning skip over the values less than the pivot
	while (data[sIdx] < pivot && sIdx < end) sIdx++;
	//from end skip over the values greater than the pivot
	while (data[end] > pivot && sIdx < end) end--;
	//sIdx is pointing to a value greater than the pivot and
	//end is pointing to a value less than the pivot,
	//switch them
	if (sIdx < end)
	{
	    swap(idList[sIdx], idList[end]);
	    swap(data[sIdx], data[end]);
	    sIdx++;
	    end--;
	}
    }
    //put the pivot in the right spot
    if (data[end] < pivot) end++;
    swap(idList[end], idList[eIdx]);
    swap(data[end], data[eIdx]);
    return end;  //return location of pivot
}

