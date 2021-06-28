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
    Dataset(std::vector<DataUnit> data_units_) : data_units(data_units_) {}
    DataUnit operator[](int i) { return data_units[i]; }
    void add(DataUnit d) { data_units.push_back(d); }
    std::size_t size() { return data_units.size(); }

private:
    std::vector<DataUnit> data_units;
};
