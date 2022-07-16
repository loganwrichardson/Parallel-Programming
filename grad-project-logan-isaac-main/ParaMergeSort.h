#ifndef PARAMERGESORT_H
#define PARAMERGESORT_H
#include "Sorts.h"
class ParaMergeSort : public Sorts
{
  private:
    int32_t threadCt;
    void mergeSort(int32_t sIdx, int32_t eIdx, float * data, float * tmp, int32_t * idList, int32_t * tmpIDList);
    void merge(int32_t sIdx, int32_t mid, int32_t eIdx, float * data, float * tmp, int32_t * idList, int32_t * tmpIDList);
  public:
    ParaMergeSort(uint64_t size, float * data, int32_t * idList, int32_t threadCt);
    double sort();
};
#endif

