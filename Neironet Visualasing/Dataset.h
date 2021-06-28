#pragma once
#include <vector>

struct DataUnit
{
    DataUnit(std::vector<double> data_, std::vector<double> answer_)
        : data(data_),
          answer(answer_)
    {
    }

    std::vector<double> data;
    std::vector<double> answer;
};

class Dataset
{
public:
    Dataset() {}
    Dataset(std::vector<DataUnit> dataUnits_) : dataUnits(dataUnits_) {}
    DataUnit &operator[](int i) { return dataUnits[i]; }
    void add(DataUnit d) { dataUnits.push_back(d); }
    std::size_t size() { return dataUnits.size(); }

private:
    std::vector<DataUnit> dataUnits;
};
