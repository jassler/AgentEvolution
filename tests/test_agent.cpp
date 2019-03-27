#include <vector>
#include <initializer_list>
#include "../AgentEvolution/agent.hpp"
#include "gtest/gtest.h"

TEST(AgentTest, InitializationDefault) {
    Agent a({0.25, 0.25, 0.5});
    EXPECT_EQ(0.25, a[0]);
    EXPECT_EQ(0.25, a[1]);
    EXPECT_EQ(0.5, a[2]);
}