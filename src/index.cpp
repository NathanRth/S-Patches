#include "index.hpp"
#include <sstream>
#include <numeric>
#include <algorithm>
Index::Index(unsigned int size, unsigned int id, unsigned int val) :
    m_index(size,0)
{
    if(id < size)
        m_index[id] = val;
}

bool Index::operator==(const Index &lhs)
{
    if(m_index.size() == lhs.m_index.size())
    {
        for(unsigned int i = 0 ; i < m_index.size() ; i++)
        {
            if(m_index.at(i) != lhs.m_index.at(i))
            {
                return false;
            }
        }
        return true;
    }
    else
    {
        return false;
    }
}

bool Index::operator!=(const Index &lhs)
{
    return !(*(this) == lhs);
}

unsigned int &Index::operator[](std::size_t i)
{
    return m_index[i];
}

const unsigned int &Index::operator[](std::size_t i) const
{
    return m_index[i];
}

std::ostream &operator<<(std::ostream &o, Index &index)
{
    std::stringstream sstr;
    sstr << "< ";
    for(unsigned int i = 0 ; i < index.m_index.size(); i++)
    {
        sstr << index.m_index[i] << " ";
    }
    return o << sstr.str() << ">";
}

unsigned int Index::degree()
{
    return std::accumulate(m_index.begin(), m_index.end(), (unsigned int)(0));
}

unsigned int Index::size()
{
    return m_index.size();
}