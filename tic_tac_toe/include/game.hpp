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
    uint64_t m_size;
    Player **m_board;
private:
    void init(uint64_t state=0);
public:
    Game();

    Game(uint64_t size, uint64_t state=0);

    Game(const Game &other);

    ~Game();

    Game &operator=(const Game &other);

    uint64_t get_size() const;

    Player winner() const;

    bool game_over() const;

    uint64_t get_state() const;

    uint64_t get_state(const std::pair<uint64_t, uint64_t> &coord, Player p) const;

    /**
     * \param coord (x, y) coordinate
     */
    bool play(const std::pair<uint64_t, uint64_t> &coord, Player p);

    /**
     * \returns (x, y) coordinate
     */
    std::vector<std::pair<uint64_t, uint64_t>> get_possible_moves() const;

    friend std::ostream &operator<<(std::ostream &out, const Game &g);
};
} // end tic_tac_toe namespace
} // end rl namespace