#include <vector>
#include <memory>
#include "agent.hpp"
#include "gtest/gtest.h"

#define VEC(...) std::vector<double>{ __VA_ARGS__ }

TEST(AgentTest, Initialization) {
    Agent a({0.25, 0.25, 0.5});
    
    std::vector<double> expected{0.25, 0.25, 0.5};
    EXPECT_EQ(expected, a.get_genome());
    EXPECT_EQ(expected, a.get_phenotype());

    EXPECT_EQ(0, a.get_score());
    EXPECT_EQ(3, a.get_genome().size());
    EXPECT_EQ(3, a.get_phenotype().size());
    EXPECT_EQ(nullptr, a.get_ancestor());

    std::shared_ptr<Agent> parent = std::make_shared<Agent>(1);
    EXPECT_EQ(VEC(1), parent->get_phenotype());
    EXPECT_EQ(1, parent->get_genome().size());
    EXPECT_EQ(1, parent->get_phenotype().size());

    std::shared_ptr<Agent> child = parent->make_offspring();
    EXPECT_EQ(parent, child->get_ancestor());
}

TEST(AgentTest, Normalizing) {
    Agent a({0.1, 0.1, 0.1});
    std::vector<double> expected{1.0/3, 1.0/3, 1.0/3};
    EXPECT_EQ(expected, a.get_phenotype());

    expected = {.1, .1, .1};
    EXPECT_EQ(expected, a.get_genome());

    a = Agent({-1, 0, 1});
    expected = {0, 1.0/3, 2.0/3};
    EXPECT_EQ(expected, a.get_phenotype());

    expected = {-1, 0, 1};
    EXPECT_EQ(expected, a.get_genome());
    // EXPECT_EQ(0, a[0]);
    // EXPECT_EQ(1.0/3.0, a[1]);
    // EXPECT_EQ(2.0/3.0, a[2]);

    a = Agent({-1, -1, -1, -1});
    expected = {.25, .25, .25, .25};
    EXPECT_EQ(expected, a.get_phenotype());

    expected = {-1, -1, -1, -1};
    EXPECT_EQ(expected, a.get_genome());
    // EXPECT_EQ(0.25, a[0]);
    // EXPECT_EQ(0.25, a[1]);
    // EXPECT_EQ(0.25, a[2]);
    // EXPECT_EQ(0.25, a[3]);
}

TEST(AgentTest, Play) {
    Agent a({0.25, 0.25, 0.5});

    EXPECT_EQ(0, a.play(0));
    EXPECT_EQ(0, a.play(0.1));
    EXPECT_EQ(0, a.play(0.25));
    EXPECT_EQ(1, a.play(0.25001));
    EXPECT_EQ(1, a.play(0.499));
    EXPECT_EQ(2, a.play(0.5001));
    EXPECT_EQ(2, a.play(0.999));
    EXPECT_EQ(2, a.play(1));

    Agent b({0.125, 0.125, 0.125, 0125, 0.125, 0.125, 0.125, 0.125});
    EXPECT_EQ(0, b.play(0));
    EXPECT_EQ(0, b.play(0.125));
    EXPECT_EQ(1, b.play(0.125001));
    EXPECT_EQ(1, b.play(0.2));
    EXPECT_EQ(1, b.play(0.25));
    EXPECT_EQ(2, b.play(0.2500001));
    EXPECT_EQ(2, b.play(0.35));
    EXPECT_EQ(3, b.play(0.3751));
    EXPECT_EQ(3, b.play(0.4));
    EXPECT_EQ(3, b.play(0.49));
    EXPECT_EQ(3, b.play(0.5));
    EXPECT_EQ(4, b.play(0.6));
    EXPECT_EQ(4, b.play(0.6249));
    EXPECT_EQ(4, b.play(0.625));
    EXPECT_EQ(5, b.play(0.625001));
    EXPECT_EQ(5, b.play(0.74));
    EXPECT_EQ(5, b.play(0.75));
    EXPECT_EQ(6, b.play(0.8));
    EXPECT_EQ(6, b.play(0.875));
    EXPECT_EQ(7, b.play(0.8750001));
    EXPECT_EQ(7, b.play(0.9));
    EXPECT_EQ(7, b.play(0.9999));
    EXPECT_EQ(7, b.play(1));
}

TEST(AgentTest, OperatorCmp) {
    Agent a(1), b(1);
    EXPECT_FALSE(a > b);
    EXPECT_FALSE(a < b);

    a.play_result(1);
    EXPECT_TRUE(a > b);
    EXPECT_FALSE(a < b);

    b.play_result(1);
    EXPECT_FALSE(a > b);
    EXPECT_FALSE(a < b);

    a.play_result(-1);
    EXPECT_FALSE(a > b);
    EXPECT_TRUE(a < b);
}
