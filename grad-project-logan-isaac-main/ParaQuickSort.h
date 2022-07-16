#ifndef PARAQUICKSORT_H
#define PARAQUICKSORT_H
#include "Sorts.h"
class ParaQuickSort : public Sorts
{
  private:
    int32_t threadCt;
    int partition(int32_t sIdx, int32_t eIdx, float * data, int32_t * idList);
    void quickSort(int32_t sIdx, int32_t eIdx, float * data, int32_t * idList);
  public:
    ParaQuickSort(uint64_t size, float * data, int32_t * idList, int32_t threadCt);
    double sort();
};
#endif
