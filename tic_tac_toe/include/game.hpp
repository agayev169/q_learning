#pragma once

#include <iostream>
#include <vector>

namespace rl {
namespace tic_tac_toe {
class Game {
public:
    enum Player {
        EMPTY = 0,
        X,
        O
    };
private:
    unsigned int m_size;
    Player **m_board;
private:
    void init();
public:
    Game();

    Game(unsigned int size);

    ~Game();

    Player winner() const;

    bool game_over() const;

    unsigned int get_state() const;

    /**
     * \param coord (x, y) coordinate
     */
    bool play(const std::pair<unsigned int, unsigned int> &coord, Player c);

    /**
     * \returns (x, y) coordinate
     */
    std::vector<std::pair<unsigned int, unsigned int>> possible_moves() const;

    friend std::ostream &operator<<(std::ostream &out, const Game &g);
};
} // end tic_tac_toe namespace
} // end rl namespace