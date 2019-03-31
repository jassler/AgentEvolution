#include <vector>
#include <memory>
#include <random>
#include "agent.hpp"
#include "gtest/gtest.h"

TEST(AgentTest, Initialization) {
    Agent a({0.25, 0.25, 0.5});
    EXPECT_EQ(0.25, a[0]);
    EXPECT_EQ(0.25, a[1]);
    EXPECT_EQ(0.5, a[2]);

    EXPECT_EQ(0, a.get_score());
    EXPECT_EQ(3, a.get_genome().size());
    EXPECT_EQ(3, a.get_phenotype().size());
    EXPECT_EQ(nullptr, a.get_ancestor());

    std::shared_ptr<Agent> parent = std::make_shared<Agent>(1);
    EXPECT_EQ(1, (*parent)[0]);
    EXPECT_EQ(1, parent->get_genome().size());
    EXPECT_EQ(1, parent->get_phenotype().size());

    std::shared_ptr<Agent> child = parent->make_offspring();
    EXPECT_EQ(parent, child->get_ancestor());
}

TEST(AgentTest, Normalizing) {
    Agent a({0.1, 0.1, 0.1});
    EXPECT_EQ(1.0/3.0, a[0]);
    EXPECT_EQ(1.0/3.0, a[1]);
    EXPECT_EQ(1.0/3.0, a[2]);

    a = Agent({-1, 0, 1});
    EXPECT_EQ(0, a[0]);
    EXPECT_EQ(1.0/3.0, a[1]);
    EXPECT_EQ(2.0/3.0, a[2]);

    a = Agent({-1, -1, -1, -1});
    EXPECT_EQ(0.25, a[0]);
    EXPECT_EQ(0.25, a[1]);
    EXPECT_EQ(0.25, a[2]);
    EXPECT_EQ(0.25, a[3]);
}
