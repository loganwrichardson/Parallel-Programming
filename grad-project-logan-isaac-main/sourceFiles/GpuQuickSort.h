#ifndef GPUQUICKSORT_H
#define GPUQUICKSORT_H
#include "Sorts.h"
class GpuQuickSort : public Sorts
{
  private:
    int partition(int32_t sIdx, int32_t eIdx, int32_t * data);
    void quickSort(int32_t sIdx, int32_t eIdx, int32_t * data);
  public:
    GpuQuickSort(uint64_t size, int32_t * data);
    double sort();
};
#endif
