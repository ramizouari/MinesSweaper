#include "mines_table.h"
#include <random>
#include <cstdint>
#include <algorithm>

using namespace std;
mines_table::mines_table(uint32_t n,uint32_t m,uint32_t k)
{
    reset(n,m,k);
    //for(int i=0;i<k;i++)
      //  if(auto a=h1(p),b=h2(p);!table[a][b])
        //    table[a][b]=true;
}

mines_table::mines_table(const std::vector<std::vector<bool>> &T):table(T)
{
}

void mines_table::reset(int n,int m,int k)
{
    static random_device e;
    std::mt19937 p(e());
    table=vector<vector<bool>>(n,vector<bool>(m));
    vector<uint32_t> A(n*m);
    for(uint32_t i=0;i<m*n;i++)
        A[i]=i;
    std::shuffle(A.begin(),A.end(),p);
    for(uint32_t i=0;i<k;i++)
        table[A[i]/m][A[i]%m]=true;
}


std::vector<bool>& mines_table::operator[](std::size_t n)
{
    return table[n];
}
const std::vector<bool>& mines_table::operator[](std::size_t n) const
{
    return table[n];
}
bool mines_table::is_totally_safe(std::size_t i,std::size_t j) const
{
    return number_of_nearby_mines(i,j)==0;
}

std::uint32_t mines_table::number_of_nearby_mines(std::size_t i,std::size_t j)const
{
    std::uint32_t R(0),n(vertical_size()),m(horizontal_size());
    for(auto u=std::max<std::int32_t>(0,i-1);u<=std::min<std::int32_t>(n-1,i+1);u++)
        for(auto v=std::max<std::int32_t>(0,j-1);v<=std::min<std::int32_t>(m-1,j+1);v++)
            if(table[u][v])
                R++;
    return R;
}

bool mines_table::safe(std::size_t i,std::size_t j) const
{
    return table[i][j];
}

std::uint32_t mines_table::horizontal_size() const
{
    if(table.empty())
        return 0;
    else return table[0].size();
}
std::uint32_t mines_table::vertical_size() const
{
    return table.size();
}

std::vector<std::pair<std::size_t,std::size_t>> mines_table::nearby_safe_zones_indices(std::size_t i,std::size_t j) const
{
    std::vector<std::pair<std::size_t,std::size_t>> T;
   auto n(vertical_size()),m(horizontal_size());
    for(auto u=std::max<std::int32_t>(0,i-1);u<=std::min<std::int32_t>(n-1,i+1);u++)
        for(auto v=std::max<std::int32_t>(0,j-1);v<=std::min<std::int32_t>(m-1,j+1);v++)
            if((u==i)&&(v==j))
                continue;
            else if(!table[u][v])
                T.push_back({u,v});
    return T;
}


std::vector<std::pair<std::size_t, std::size_t>> mines_table::nearby_indices(std::size_t i, std::size_t j) const
{
    std::vector<std::pair<std::size_t, std::size_t>> T;
    auto n(vertical_size()), m(horizontal_size());
    for (auto u = std::max<std::int32_t>(0, i - 1); u <= std::min<std::int32_t>(n - 1, i + 1); u++)
        for (auto v = std::max<std::int32_t>(0, j - 1); v <= std::min<std::int32_t>(m - 1, j + 1); v++)
            if ((u == i) && (v == j))
                continue;
            else
                T.push_back({ u,v });
    return T;
}
