#include <iterator>
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include "bmsData.h"
/**
 * Helper Class to read CSV files as a stream
 * Source: 5
 **/
class CSVRow
{
    public:
	std::string operator[](std::size_t index) const
	{
	    return std::string(&m_line[m_data[index] + 1], m_data[index + 1] -  (m_data[index] + 1));
	}
	std::size_t size() const
	{
	    return m_data.size() - 1;
	}
	void readNextRow(std::istream& str)
	{
	    std::getline(str, m_line);

	    m_data.clear();
	    m_data.emplace_back(-1);
	    std::string::size_type pos = 0;
	    while((pos = m_line.find(',', pos)) != std::string::npos)
	    {
		m_data.emplace_back(pos);
		++pos;
	    }
	    // This checks for a trailing comma with no data after it.
	    pos   = m_line.size();
	    m_data.emplace_back(pos);
	}
    private:
	std::string         m_line;
	std::vector<int>    m_data;
};

std::istream& operator>>(std::istream& str, CSVRow& data)
{
    data.readNextRow(str);
    return str;
}

class DataSet
{
    public:
	/*
	 * createSortData
	 * Dynamically allocates space for size int32_t values and initializes those
	 * values
	 * Input:
	 * size - number of elements to be allocated 
	 * Output:
	 * pointer to the allocated data
	 * Source: 5
	 */          
	bmsData * createSortData(int64_t size)
	{
	    bmsData * data = new bmsData[size];
	    std::ifstream file("bms.csv");
	    CSVRow row;
	    int i = 0;
	    bool firstIteration = true;
	    while(file>>row && i < size)	
	    {
		if (!firstIteration) {
		    data[i].packMetrics.soc = std::stof(row[1]);
		    //std::cout << data[i].packMetrics.soc << "\n";
		    data[i].packMetrics.packVolt = std::stof(std::string{row[2]});
		    //std::cout << data[i].packMetrics.packVolt << "\n";
		    data[i].packMetrics.packCurrent = std::stof(std::string{row[3]});
		    //std::cout << data[i].packMetrics.packCurrent << "\n";
		    data[i].packMetrics.packCCL = std::stoi(std::string{row[4]});
		    //std::cout << data[i].packMetrics.packCCL << "\n";
		    data[i].packMetrics.packDCL = std::stoi(std::string{row[5]});
		    //std::cout << data[i].packMetrics.packDCL << "\n";
		    data[i].hiLoMetrics.highestID = std::stoi(std::string{row[6]});
		    //std::cout << data[i].hiLoMetrics.highestID << "\n";
		    data[i].hiLoMetrics.highestCellVolt = std::stof(std::string{row[7]});
		    //std::cout << data[i].hiLoMetrics.highestCellVolt << "\n";
		    data[i].hiLoMetrics.lowestID = std::stoi(std::string{row[8]});
		    //std::cout << data[i].hiLoMetrics.lowestID << "\n";
		    data[i].hiLoMetrics.lowestCellVolt = std::stof(std::string{row[9]});
		    //std::cout << data[i].hiLoMetrics.lowestCellVolt << "\n";
		    data[i].packMetrics.chargeSafetyStat = (bool)std::stoi(std::string{row[10]});
		    //std::cout << data[i].packMetrics.chargeSafetyStat << "\n";
		    data[i].packMetrics.chargeEnableStat = (bool)std::stoi(std::string{row[11]});
		    //std::cout << data[i].packMetrics.chargeEnableStat << "\n";
		    data[i].packMetrics.dischargeEnableStat = (bool)std::stoi(std::string{row[12]});
		    //std::cout << data[i].packMetrics.dischargeEnableStat << "\n";
		    data[i].hiLoMetrics.highTemp = std::stoi(std::string{row[13]});
		    //std::cout << data[i].hiLoMetrics.highTemp << "\n";
		    data[i].hiLoMetrics.lowTemp = std::stoi(std::string{row[14]});
		    //std::cout << data[i].hiLoMetrics.lowTemp << "\n";
		    int offsetVolt = 15;
		    int offsetRes = 45;
		    int offsetOpenVolt = 75;

		    for(int j = 1; j < 31; ++j) {
			data[i].individualCells[j].volt = std::stof(std::string{row[offsetVolt++]});
			//std::cout << data[i].individualCells[j].volt << "\n";
			data[i].individualCells[j].cellRes = std::stof(std::string{row[offsetRes++]});
			//std::cout << data[i].individualCells[j].cellRes << "\n";
			data[i].individualCells[j].openCellVolt = std::stof(std::string{row[offsetOpenVolt++]});		
			//std::cout << "Open Cell Volt at " << i << ", " << j << " = \n";
			//std::cout << data[i].individualCells[j].openCellVolt << "\n";
		    }
		    i++;
		}
		else
		{
		    firstIteration = false;
		}
	    }
	    return data;
	}
	void saveSortData(bmsData * data, int32_t * idList, uint64_t size) 
	{
	    std::ofstream out("bmsOut.csv");

	    out << "State of Charge" << ',';
	    out << "Pack Voltage" << ',';
	    out << "Pack Current" << ',';
	    out << "Pack CCL" << ',';
	    out << "Pack DCL" << ',';
	    out << "Highest Cell ID" << ',';
	    out << "Highest Cell Volt" << ',';
	    out << "Lowest Cell ID" << ',';
	    out << "Lowest Cell Volt" << ',';
	    out << "Charger Safety Status" << ',';
	    out << "Charge Enable Status" << ',';
	    out << "Discharge Enable Status" << ',';
	    out << "High Temperature" << ',';
	    out << "Low Temperature" << ',';
	    for (int i = 1; i < 31; ++i)
	    {
		out << "Cell Voltage " << i << ',';
	    }
	    for (int i = 1; i < 31; ++i)
	    {
		out << "Cell Resistance " << i << ',';
	    }
	    for (int i = 1; i < 31; ++i)
	    {
		out << "Open Cell Voltage " << i << ',';
	    }
	    out << '\n';
	    for (uint64_t i = 0; i < size; i++) 
	    {
		    bmsData d = data[idList[i]];
		    out << d.packMetrics.soc << ',';
		    out << d.packMetrics.packVolt << ',';
		    out << d.packMetrics.packCurrent << ',';
		    out << d.packMetrics.packCCL << ',';
		    out << d.packMetrics.packDCL << ',';
		    out << d.hiLoMetrics.highestID << ',';
		    out << d.hiLoMetrics.highestCellVolt << ',';
		    out << d.hiLoMetrics.lowestID << ',';
		    out << d.hiLoMetrics.lowestCellVolt << ',';
		    out << d.packMetrics.chargeSafetyStat << ',';
		    out << d.packMetrics.chargeEnableStat << ',';
		    out << d.packMetrics.dischargeEnableStat << ',';
		    out << d.hiLoMetrics.highTemp << ',';
		    out << d.hiLoMetrics.lowTemp << ',';

		    for (int j = 1; j < 31; ++j)
		    {
			out << d.individualCells[j].volt << ',';
			out << d.individualCells[j].cellRes << ',';
			out << d.individualCells[j].openCellVolt << ',';
		    }
		    out << '\n';
		}
	    out.close();
	}
};
