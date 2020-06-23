#include "model.hpp"

namespace rl {
namespace tic_tac_toe {
Model::Model(double learning_rate, double discount_factor,
             double lose_reward, double win_reward,
             double default_reward)
    : m_learning_rate(learning_rate),
    m_discount_factor(discount_factor),
    m_lose_reward(lose_reward),
    m_win_reward(win_reward),
    m_default_reward(default_reward) {}
    
Model::~Model() {}

void Model::train(unsigned int n_games=20000) {
    srand(42);
    for (unsigned int game_count = 0; game_count < n_games; ++game_count) {
        Game game;
        while (!game.game_over()) {
            // Model makes move
            unsigned int state = game.get_state();
            auto possible_moves = game.possible_moves();
            std::pair<unsigned int, unsigned int> next_move 
                = move(state, possible_moves);
            game.play(next_move, Game::X);

            // Random move from opponent
            possible_moves = game.possible_moves();
            unsigned int move_index = rand() % possible_moves.size();
            game.play(possible_moves[move_index], Game::O);

            // train the model
            unsigned int new_state = game.get_state();
            train_step(state, next_move, new_state);
        }
    }
}
} // end tic_tac_toe namespace
} // end rl namespace