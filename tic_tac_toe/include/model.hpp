#pragma once

#include <unordered_map>
#include <cstdlib>
#include <assert.h>

#include "game.hpp"

namespace rl {
namespace tic_tac_toe {
class Model {
private:
    Game::Player m_player;
    double m_learning_rate;
    double m_discount_factor;
    double m_lose_reward;
    double m_win_reward;
    double m_default_reward;
    double m_exploration_rate;
    double m_exploration_decay;
    
    std::unordered_map<uint64_t, double> q_vals;
public:
    Model(Game::Player player=Game::X, double learning_rate=0.01, 
          double discount_factor=0.9, double lose_reward=-1.0, 
          double win_reward=1.0, double default_reward=0.0);

    ~Model();

    void train(uint64_t n_games=200000, uint64_t print_each=10000);

    std::pair<uint64_t, uint64_t> move(Game &game);

    std::pair<uint64_t, uint64_t> get_best_move(const Game &game) const;

    void train_step(uint64_t old_state,
                    const std::pair<uint64_t, uint64_t> &last_move,
                    const Game &game);

    double get_q_value(const Game &game,
                       const std::pair<uint64_t, uint64_t> &move) const;

    double get_q_value(uint64_t state) const;

    void set_q_value(uint64_t state, double value);
};
} // end tic_tac_toe namespace 
} // end rl namespace 