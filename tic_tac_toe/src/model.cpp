#include "model.hpp"

namespace rl {
namespace tic_tac_toe {
Model::Model(Game::Player player, double learning_rate, 
             double discount_factor, double lose_reward, 
             double win_reward, double default_reward)
    : m_player(player),
    m_learning_rate(learning_rate),
    m_discount_factor(discount_factor),
    m_lose_reward(lose_reward),
    m_win_reward(win_reward),
    m_default_reward(default_reward) {
    assert(m_player == Game::X || m_player == Game::O);
}
    
Model::~Model() {}

void Model::train(uint64_t n_games, uint64_t print_each) {
    srand(42);

    Game::Player opponent_player = (m_player == Game::X) ? Game::O : Game::X;

    uint64_t win_count = 0;
    uint64_t lose_count = 0;
    uint64_t draw_count = 0;
    for (uint64_t game_count = 0; game_count < n_games; ++game_count) {
        Game game;
        while (!game.game_over()) {
            // Model makes move
            uint64_t state = game.get_state();
            std::pair<uint64_t, uint64_t> last_move = move(game);

            // Random move from opponent
            auto possible_moves = game.get_possible_moves();
            if (possible_moves.size() > 0) {
                uint64_t move_index = rand() % possible_moves.size();
                game.play(possible_moves[move_index], opponent_player);
            }

            // train the model
            train_step(state, last_move, game);
        }
        if (game.winner() == m_player)
            ++win_count;
        else if (game.winner() == opponent_player)
            ++lose_count;
        else
            ++draw_count;
        
        if ((game_count + 1) % print_each == 0) 
            std::cout << "win_ratio: " << (double) win_count / game_count << std::endl
                      << "lose_ratio: " << (double) lose_count / game_count << std::endl
                      << "draw_ratio: " << (double) draw_count / game_count << std::endl
                      <<  std::endl;
    }
}

std::pair<uint64_t, uint64_t> Model::move(Game &game) {
    auto best_move = get_best_move(game);
    game.play(best_move, m_player);
    return best_move;
}

std::pair<uint64_t, uint64_t> Model::get_best_move(const Game &game) const {
    auto possible_moves = game.get_possible_moves();
    const std::pair<uint64_t, uint64_t> size 
        = std::make_pair(game.get_size(), game.get_size());
    std::pair<uint64_t, uint64_t> best_move = size;
    double best_move_val = INT32_MAX;
    for (auto &move : possible_moves) {
        double cur_move_val = get_q_value(game, move);
        if (best_move == size || cur_move_val > best_move_val) {
            best_move = move;
            best_move_val = cur_move_val;
        }
    }
    return best_move;
}

void Model::train_step(uint64_t old_state,
                       const std::pair<uint64_t, uint64_t> &last_move,
                       const Game &game) {
    double reward = m_default_reward;
    if (game.game_over() && game.winner() != m_player)
        reward = m_lose_reward;
    else if (game.game_over() && game.winner() == m_player) 
        reward = m_win_reward;

    double diff = m_learning_rate 
                * (reward + m_discount_factor * get_q_value(game.get_state()) 
                   - get_q_value(old_state));
    set_q_value(old_state, get_q_value(old_state) + diff);
    Game new_game(game.get_size(), old_state);
    uint64_t intermediate_state = new_game.get_state(last_move, m_player);
    set_q_value(intermediate_state, get_q_value(game.get_state()));
}

double Model::get_q_value(const Game &game, 
                          const std::pair<uint64_t, uint64_t> &move) const {
    uint64_t state = game.get_state(move, m_player);
    return get_q_value(state);
}

double Model::get_q_value(uint64_t state) const {
    return q_vals.find(state) != q_vals.end()
           ? q_vals.find(state)->second 
           : m_default_reward;
}

void Model::set_q_value(uint64_t state, double value) {
    q_vals[state] = value;
}
} // end tic_tac_toe namespace
} // end rl namespace