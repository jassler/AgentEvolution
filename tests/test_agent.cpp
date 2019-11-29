#include <vector>
#include <memory>
#include "agent.hpp"
#include "gtest/gtest.h"

TEST(AgentTest, Initialization) {
    Agent<3, 3> a({0.25, 0.25, 0.5});
    
    std::array<double, 3> expected{0.25, 0.25, 0.5};
    EXPECT_EQ(expected, a.get_genome());
    EXPECT_EQ(expected, a.get_phenotype());

//    EXPECT_EQ(0, a.get_score());
    EXPECT_EQ(3, a.get_genome().size());
    EXPECT_EQ(3, a.get_phenotype().size());
    EXPECT_EQ(nullptr, a.get_ancestor());

    Agent<4, 4> b;
    std::array<double, 4> expected_2{0.25, 0.25, 0.25, 0.25};
    EXPECT_EQ(expected_2, b.get_genome());
    EXPECT_EQ(expected_2, b.get_phenotype());
}

TEST(AgentTest, Offspring) {
    Agent<3, 3> *a = new Agent<3, 3>({0.25, 0.25, 0.5});
    EXPECT_EQ(nullptr, a->get_ancestor());

    Agent<3, 3> child1 = a->make_offspring();
    Agent<3, 3> child2 = a->make_offspring();
    EXPECT_EQ(a, child1.get_ancestor());
    EXPECT_EQ(a, child2.get_ancestor());
    EXPECT_NE(&child1, &child2);

    Agent<3, 3> child11 = child1.make_offspring();
    Agent<3, 3> child21 = child2.make_offspring();

    EXPECT_EQ(&child1, child11.get_ancestor());
    EXPECT_EQ(&child2, child21.get_ancestor());
    EXPECT_EQ(child11.get_ancestor()->get_ancestor(), child21.get_ancestor()->get_ancestor());
    
    delete a;
}

TEST(AgentTest, Normalizing) {
    // a thirds
    Agent<3, 3> a({0.1, 0.1, 0.1});
    std::array<double, 3> expected{1.0/3, 1.0/3, 1.0/3};
    EXPECT_EQ(expected, a.get_phenotype());

    expected = {.1, .1, .1};
    EXPECT_EQ(expected, a.get_genome());

    // one negative
    a = Agent<3, 3>({-1, 0, 1});
    expected = {0, 1.0/3, 2.0/3};
    EXPECT_EQ(expected, a.get_phenotype());

    expected = {-1, 0, 1};
    EXPECT_EQ(expected, a.get_genome());

    // only negatives
    Agent<4, 4> b({-1, -1, -1, -1});
    std::array<double, 4> expected_b{.25, .25, .25, .25};
    EXPECT_EQ(expected_b, b.get_phenotype());

    expected_b = {-1, -1, -1, -1};
    EXPECT_EQ(expected_b, b.get_genome());
}

TEST(AgentTest, Play) {
    Agent<3, 3> a({0.25, 0.25, 0.5});

    EXPECT_EQ(0, a.play(0));
    EXPECT_EQ(0, a.play(0.1));
    EXPECT_EQ(0, a.play(0.25));
    EXPECT_EQ(1, a.play(0.25001));
    EXPECT_EQ(1, a.play(0.499));
    EXPECT_EQ(2, a.play(0.5001));
    EXPECT_EQ(2, a.play(0.999));
    EXPECT_EQ(2, a.play(1));

    std::array<double, 8> vec;
    vec.fill(1.0/8);
    Agent<8, 8> b(vec);
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
    Agent<1, 1> a, b;
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
