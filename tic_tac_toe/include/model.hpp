#pragma once

#include <unordered_map>
#include <cstdlib>

#include "game.hpp"

namespace rl {
namespace tic_tac_toe {
class Model {
private:
    double m_learning_rate;
    double m_discount_factor;
    double m_lose_reward;
    double m_win_reward;
    double m_default_reward;
    
    std::unordered_map<unsigned int, double> q_vals;
public:
    Model(double learning_rate=0.01, double discount_factor=0.9,
          double lose_reward=-1.0, double win_reward=1.0,
          double default_reward=0.0);

    ~Model();

    void train(unsigned int n_games=20000);
};
} // end tic_tac_toe namespace 
} // end rl namespace 