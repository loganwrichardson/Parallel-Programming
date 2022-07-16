#ifndef SEQQUICKSORT_H
#define SEQQUICKSORT_H
#include "Sorts.h"
class SeqQuickSort : public Sorts
{
  private:
    void quickSort(int32_t sIdx, int32_t eIdx, float * data, int32_t * idList);
    int partition(int32_t sIdx, int32_t eIdx, float * data, int32_t * idList);
  public:
    SeqQuickSort(uint64_t size, float * data, int32_t * idList);
    double sort();
};
#endif
