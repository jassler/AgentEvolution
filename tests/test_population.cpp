#include <vector>
#include <memory>
#include "population.hpp"
#include "gtest/gtest.h"

TEST(PopulationTest, Initialization) {
    Population p(1000, 500, std::vector{0.5, 0.5});

    EXPECT_EQ(1000, p.size());
}