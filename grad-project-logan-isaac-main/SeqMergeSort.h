#ifndef SEQMERGESORT_H
#define SEQMERGESORT_H
#include "Sorts.h"
class SeqMergeSort : public Sorts
{
  private:
    void mergeSort(int32_t sIdx, int32_t eIdx, float * data, float * tmp, int32_t * idList, int32_t * tmpIDList);
    void merge(int32_t sIdx, int32_t mid, int32_t eIdx, float * data, float * tmp, int32_t * idList, int32_t * tmpIDList);
  public:
    SeqMergeSort(uint64_t size, float * data, int32_t * idList);
    double sort();
};
#endif
