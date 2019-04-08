#include <vector>
#include <memory>
#include "population.hpp"
#include "gtest/gtest.h"

TEST(PopulationTest, Initialization) {
    Population p(999, 599, Matrix(1, 1), 1);

    EXPECT_EQ(999, p.size());
}