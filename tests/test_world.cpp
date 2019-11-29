#include "world.hpp"
#include "gtest/gtest.h"

TEST(WorldTest, InitSingleAgent) {
    std::array<double, 4> genome{0.25, 0, 0.25, 0.5};
    Matrix<3, 4> m({{1, 2, 3, 4}, {-1, -2, -3, -4}, {0, 10, 0, -10}});
    Agent<4, 3> default_agent(genome, m);
    Matrix<3, 3> payoff({{0, 1, -1}, {2, 0, -2}, {-3, 3, 0}});

    World<500, 4, 3> world(default_agent, payoff);
    EXPECT_EQ(world.get_payoff(), payoff);

    for(size_t i = 0; i < 500; ++i) {
        auto a = world[i];
        EXPECT_EQ(a.get_score(), default_agent.get_score());

        EXPECT_EQ(a.get_genome(), genome);
        EXPECT_EQ(a.get_matrix(), m);
        EXPECT_EQ(a.get_ancestor(), nullptr);
    }
}

TEST(WorldTest, InitMultipleAgent) {
    std::array<double, 4> g1{0.25, 0, 0.25, 0.5};
    Matrix<3, 4> m1({{1, 2, 3, 4}, {-1, -2, -3, -4}, {0, 10, 0, -10}});
    Agent<4, 3> a1(g1, m1);

    std::array<double, 4> g2{7, 10, 2, 0.4};
    Matrix<3, 4> m2({{1, -0.5, 3, 0.125}, {-10, 3, 10, 499}, {0, 0, 0, 0}});
    Agent<4, 3> a2(g2, m2);

    std::array<double, 4> g3{0.9, 0.8, 0.1, 0};
    Matrix<3, 4> m3({{9, 9, 9, 9}, {0.9, 0.9, 0.9, 0.9}, {0.09, 0.09, 0.09, 0.09}});
    Agent<4, 3> a3(g3, m3);

    Matrix<3, 3> payoff({{3, 1, -2}, {2, 5, -2}, {-3, 1, 7}});

    /*
     * POPSIZE 30
     * (divisible by 3)
     */
    World<30, 4, 3> world1({a1, a2, a3}, payoff);
    EXPECT_EQ(world1.get_payoff(), payoff);

    for(size_t i = 0; i < 10; ++i) {
        auto a = world1[i];
        EXPECT_EQ(a.get_score(), a1.get_score()) << "world1: i=" << i << ", loop 1";
        EXPECT_EQ(a.get_genome(), g1) << "world1: i=" << i << ", loop 1";
        EXPECT_EQ(a.get_matrix(), m1) << "world1: i=" << i << ", loop 1";
        EXPECT_EQ(a.get_ancestor(), nullptr) << "world1: i=" << i << ", loop 1";
    }

    for(size_t i = 10; i < 20; ++i) {
        auto a = world1[i];
        EXPECT_EQ(a.get_score(), a2.get_score()) << "world1: i=" << i << ", loop 2";
        EXPECT_EQ(a.get_genome(), g2) << "world1: i=" << i << ", loop 2";
        EXPECT_EQ(a.get_matrix(), m2) << "world1: i=" << i << ", loop 2";
        EXPECT_EQ(a.get_ancestor(), nullptr) << "world1: i=" << i << ", loop 2";
    }

    for(size_t i = 20; i < 30; ++i) {
        auto a = world1[i];
        EXPECT_EQ(a.get_score(), a3.get_score()) << "world1: i=" << i << ", loop 3";
        EXPECT_EQ(a.get_genome(), g3) << "world1: i=" << i << ", loop 3";
        EXPECT_EQ(a.get_matrix(), m3) << "world1: i=" << i << ", loop 3";
        EXPECT_EQ(a.get_ancestor(), nullptr) << "world1: i=" << i << ", loop 3";
    }

    /*
     * POPSIZE 31
     */
    World<31, 4, 3> world2({a1, a2, a3}, payoff);
    EXPECT_EQ(world2.get_payoff(), payoff);

    for(size_t i = 0; i < 10; ++i) {
        auto a = world2[i];
        EXPECT_EQ(a.get_score(), a1.get_score()) << "world2: i=" << i << ", loop 1";
        EXPECT_EQ(a.get_genome(), g1) << "world2: i=" << i << ", loop 1";
        EXPECT_EQ(a.get_matrix(), m1) << "world2: i=" << i << ", loop 1";
        EXPECT_EQ(a.get_ancestor(), nullptr) << "world2: i=" << i << ", loop 1";
    }

    for(size_t i = 10; i < 21; ++i) {
        auto a = world2[i];
        EXPECT_EQ(a.get_score(), a2.get_score()) << "world2: i=" << i << ", loop 2";
        EXPECT_EQ(a.get_genome(), g2) << "world2: i=" << i << ", loop 2";
        EXPECT_EQ(a.get_matrix(), m2) << "world2: i=" << i << ", loop 2";
        EXPECT_EQ(a.get_ancestor(), nullptr) << "world2: i=" << i << ", loop 2";
    }

    for(size_t i = 21; i < 31; ++i) {
        auto a = world2[i];
        EXPECT_EQ(a.get_score(), a3.get_score()) << "world2: i=" << i << ", loop 3";
        EXPECT_EQ(a.get_genome(), g3) << "world2: i=" << i << ", loop 3";
        EXPECT_EQ(a.get_matrix(), m3) << "world2: i=" << i << ", loop 3";
        EXPECT_EQ(a.get_ancestor(), nullptr) << "world2: i=" << i << ", loop 3";
    }

    /*
     * POPSIZE 32
     */
    World<32, 4, 3> world3({a1, a2, a3}, payoff);
    EXPECT_EQ(world3.get_payoff(), payoff);

    for(size_t i = 0; i < 11; ++i) {
        auto a = world3[i];
        EXPECT_EQ(a.get_score(), a1.get_score()) << "world3: i=" << i << ", loop 1";
        EXPECT_EQ(a.get_genome(), g1) << "world3: i=" << i << ", loop 1";
        EXPECT_EQ(a.get_matrix(), m1) << "world3: i=" << i << ", loop 1";
        EXPECT_EQ(a.get_ancestor(), nullptr) << "world3: i=" << i << ", loop 1";
    }

    for(size_t i = 11; i < 21; ++i) {
        auto a = world3[i];
        EXPECT_EQ(a.get_score(), a2.get_score()) << "world3: i=" << i << ", loop 2";
        EXPECT_EQ(a.get_genome(), g2) << "world3: i=" << i << ", loop 2";
        EXPECT_EQ(a.get_matrix(), m2) << "world3: i=" << i << ", loop 2";
        EXPECT_EQ(a.get_ancestor(), nullptr) << "world3: i=" << i << ", loop 2";
    }

    for(size_t i = 21; i < 32; ++i) {
        auto a = world3[i];
        EXPECT_EQ(a.get_score(), a3.get_score()) << "world3: i=" << i << ", loop 3";
        EXPECT_EQ(a.get_genome(), g3) << "world3: i=" << i << ", loop 3";
        EXPECT_EQ(a.get_matrix(), m3) << "world3: i=" << i << ", loop 3";
        EXPECT_EQ(a.get_ancestor(), nullptr) << "world3: i=" << i << ", loop 3";
    }
}

TEST(WorldTest, Simulation) {
    Agent<3, 3> a({0.1, 0.5, 0.9});

    World<100, 3, 3> world(a, {{0, 1, 1}, {1, 0, 1}, {0, 1, 1}});

    world.simulate_generation();
    world.simulate_generation();
    world.simulate_generation();
    world.simulate_generation();
    
    auto ancestor = world[0].get_ancestor()->get_ancestor()->get_ancestor()->get_ancestor();
    EXPECT_EQ(ancestor->get_genome(), a.get_genome());
    EXPECT_EQ(ancestor->get_matrix(), a.get_matrix());
    EXPECT_EQ(ancestor->get_ancestor(), a.get_ancestor());
    EXPECT_NE(ancestor->get_children_amount(), 0);
}

TEST(WorldTest, Average) {
    Agent<3, 3> a({2, 2, 4});
    World<2, 3, 3> world(a, {{0, 1, 1}, {1, 0, 1}, {0, 1, 1}});

    std::array<double, 3> avg{0.25, 0.25, 0.5};
    std::array<double, 3> result;
    world.calculate_average_phenotype(result);
    EXPECT_EQ(result, avg);

    world[0] = Agent<3, 3>({4, 4, 2});
    avg = { (0.25 + 0.4) / 2, (0.25 + 0.4) / 2, (0.5 + 0.2) / 2 };
    world.calculate_average_phenotype(result);
    EXPECT_EQ(result, avg);
}
