#ifndef INDEX_H
#define INDEX_H

#include <vector>
#include <iostream>

class Index
{
  public:
    Index(unsigned int size, unsigned int id = 0, unsigned int val = 0);

    bool operator==(const Index &lhs);
    bool operator!=(const Index &lhs);

    unsigned int &operator[](std::size_t i);

    const unsigned int &operator[](std::size_t i) const;

    friend std::ostream &operator<<(std::ostream &o, Index &index);

    unsigned int degree();

    unsigned int size();

  private:
    std::vector<unsigned int> m_index;
};

#endif