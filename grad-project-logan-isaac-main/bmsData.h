#ifndef BMSDATA_H
#define BMSDATA_H
typedef struct hiLo {
    float highestCellVolt, lowestCellVolt;
    int32_t highestID, lowestID, highTemp, lowTemp;
} hiLo;

typedef struct pack {
    float soc, packVolt, packCurrent;
    int32_t packCCL, packDCL;
    bool chargeSafetyStat, chargeEnableStat, dischargeEnableStat;
} pack;

/* 	volt = voltage
	cellRes = cell resistance
	openCellVolt = open cell voltage
*/
typedef struct cell {
    float volt, cellRes, openCellVolt;
} cell;

typedef struct bmsData {
    pack  packMetrics;
    hiLo  hiLoMetrics;
    // 1-based indexing
    cell individualCells[31];
} bmsData;
#endif
