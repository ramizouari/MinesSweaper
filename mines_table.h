#ifndef MINES_TABLE_H
#define MINES_TABLE_H
#include <vector>
#include <cstdint>
//#include "connections_graph.h"


class mines_table
{
public:
    mines_table(std::uint32_t n,std::uint32_t m,std::uint32_t k);//generate n*m grid containing k trues;
    mines_table(const std::vector<std::vector<bool>> &T);
    std::vector<bool>& operator[](std::size_t n);
    const std::vector<bool>& operator[](std::size_t n) const;
    std::uint32_t number_of_nearby_mines(std::size_t i,std::size_t j)const;
    std::vector<std::pair<std::size_t,std::size_t>> nearby_safe_zones_indices(std::size_t i,std::size_t j) const;
    std::vector<std::pair<std::size_t, std::size_t>> nearby_indices(std::size_t i, std::size_t j) const;
    std::uint32_t horizontal_size() const;
    bool safe(std::size_t i,std::size_t j) const;
    std::uint32_t vertical_size() const;
    bool is_totally_safe(std::size_t i,std::size_t j) const;
    void reset(int n,int m,int k);
private:
    std::vector<std::vector<bool>> table;
};

#endif // MINES_TABLE_H
