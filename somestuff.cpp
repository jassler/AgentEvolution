#include <range/v3/all.hpp>
#include <vector>
#include <iostream>
#include <algorithm>
#include <random>
using namespace ranges;
using namespace std;

struct Agent {
    std::string name;
    int score;

    // for std::minmax
    bool operator<(const Agent& other) const { return score < other.score; }
    bool operator<=(const Agent& other) const { return score <= other.score; }
    bool operator>(const Agent& other) const { return score > other.score; }
    bool operator>=(const Agent& other) const { return score >= other.score; }

    // for action::unique
    bool operator==(const Agent& other) const { return name == other.name; }
    bool operator!=(const Agent& other) const { return name != other.name; }

    friend std::ostream& operator<<(std::ostream& os, const Agent& a) {
        os << "Agent " << a.name << " with " << a.score << " points";
        return os;
    }
};

vector<double> calculate_avg(vector<vector<double>> lists) {
    auto it = lists.begin();
    vector<double> avg = *it;

    while(++it != lists.end()) {
        for(int i = 0; i < it->size(); ++i) {
            avg[i] += (*it)[i];
        }
    }

    for(auto& v : avg) {
        v /= avg.size();
    }

    return avg;
}

int main() {
    vector<vector<double>> v{{1, 2, 3}, {2, 3, 4}, {3, 7, -1}};
    auto result = calculate_avg(v);

    for(const auto& r : result) {
        cout << r << ", ";
    }
    cout << "\b\b \n";

    // size_t seed = 43;
    // std::mt19937 wheel(seed);

    // // 10 agents with "random" scores
    std::vector<Agent> agents{
        { "Aura",  1 },
        { "Chai", 15 },
        { "Flix", 12 },
        { "Jeff", 10 },
        { "Jory",  9 },
        { "Leya",  0 },
        { "Mark",  5 },
        { "Nick", 13 },
        { "Roya", 20 },
        { "Stan", 19 }
    };
    
    auto result = ranges::accumulate(agents, &Agent::score);
    // auto [min,max] = std::minmax_element(agents.begin(), agents.end());
    // std::uniform_int_distribution<int> spin(min->score, max->score);
    
    // // auto b = agents
    // //         | view::cycle
    // //         | view::filter([&](auto agent){
    // //             spin(wheel) < agent.score; })
    // //         // | action::sort
    // //         // | action::unique
    // //         | view::take(5);
    // auto b = agents
    //         | view::cycle
    //         | view::filter([&](auto& a){return spin(wheel) < a.score;})
    //         | view::take(5);
    
    // auto result = ranges::find(b, Agent{"Chai"});
    // if(result == agents.begin())
    //     std::cout << "found him at " << typeid(result).name() << "\n";
    // else
    //    std::cout << "didn't find him\n";
    
    // // std::cout << "found it: " << *meta::in(b, Agent{"Chai", 1}) << "\n";
    // //std::cout << in(agents, Agent{"Chai", 1}) << "\n";

    // for(auto& c : b) {
    //     std::cout << c << "\n";
    // }
    // std::cout << std::endl;
}