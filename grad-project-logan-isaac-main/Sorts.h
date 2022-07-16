#ifndef SORTS_H
#define SORTS_H
#include <cstdint>
#include <string>
class Sorts
{
  protected:
    float * data;
    int32_t * idList;
    uint64_t size;
    std::string description;
  public:
    Sorts(uint64_t size, float * input, int32_t * inputIDs);
    bool match(Sorts * sptr);
    bool increasing();
    std::string getDescription();
    virtual double sort() = 0;
    virtual ~Sorts();
};
#endif
