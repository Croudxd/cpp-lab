#include <gtest/gtest.h>
#include "ben/map.hpp"

TEST(map_insert, map_insert_test)
{
    ben::map<int, int> m;
    m.insert(10,  100001);
    m.insert(11,  1);
    m.insert(12,  984);
}
