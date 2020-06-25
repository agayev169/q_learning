#include "game.hpp"

namespace rl {
namespace tic_tac_toe {
Game::Game() : m_size(3) {
    init();
}

Game::Game(uint64_t size, uint64_t state) : m_size(size) {
    if (m_size < 3) {
        m_size = 3;
        std::cerr << "Size of the m_board cannot be less than 3. "
                  << "Reassigning size to 3." << std::endl;
    } 
    init(state);
}

void Game::init(uint64_t state) {
    m_board = new Player*[m_size];
    for (uint64_t i = 0; i < m_size; ++i) {
        m_board[i] = new Player[m_size];
    }

    for (uint64_t i = m_size; i > 0; --i) {
        for (uint64_t j = m_size; j > 0; --j) {
            m_board[i - 1][j - 1] = static_cast<Player>(state % 3);
            state /= 3;
        }
    }
}

Game::Game(const Game &other) {
    m_size = other.get_size();
    init(other.get_state());
}

Game &Game::operator=(const Game &other) {
    if (m_board) {
        for (uint64_t i = 0; i < m_size; ++i) {
            if (m_board[i]) delete[] m_board[i];
        }
        delete[] m_board;
    }

    m_size = other.get_size();
    init(other.get_state());
    return *this;
}

Game::~Game() {
    for (uint64_t i = 0; i < m_size; ++i) {
        delete[] m_board[i];
    }
    delete[] m_board;
}

uint64_t Game::get_size() const {
    return m_size;
}

Game::Player Game::winner() const {
    bool l_diag_win = true;
    Player l_diag_val = m_board[0][0];
    if (l_diag_val != Player::EMPTY) {
        for (uint64_t i = 1; i < m_size && l_diag_win; ++i) {
            l_diag_win = (m_board[i][i] == l_diag_val);
        }
        if (l_diag_win) return l_diag_val;
    }

    bool r_diag_win = true;
    Player r_diag_val = m_board[m_size - 1][0];
    if (r_diag_val != Player::EMPTY) {
        for (uint64_t i = 1; i < m_size && r_diag_win; ++i) {
            r_diag_win = (m_board[m_size - i - 1][i] == r_diag_val);
        }
        if (r_diag_win) return r_diag_val;
    }

    for (uint64_t i = 0; i < m_size; ++i) {
        bool hor_win = true;
        Player hor_val = m_board[i][0];
        if (hor_val != Player::EMPTY) {
            for (uint64_t j = 1; j < m_size && hor_win; ++j) {
                hor_win = (m_board[i][j] == hor_val);
            }
            if (hor_win) return hor_val;
        }
    }

    for (uint64_t i = 0; i < m_size; ++i) {
        bool vert_win = true;
        Player vert_val = m_board[0][i];
        if (vert_val != Player::EMPTY) {
            for (uint64_t j = 1; j < m_size && vert_win; ++j) {
                vert_win = (m_board[j][i] == vert_val);
            }
            if (vert_win) return vert_val;
        }
    }
    return Player::EMPTY;
}

bool Game::game_over() const {
    if (winner() != EMPTY) return true;
    for (uint64_t i = 0; i < m_size; ++i) {
        for (uint64_t j = 0; j < m_size; ++j) {
            if (m_board[i][j] == EMPTY) return false;
        }
    }
    return true;
};

bool Game::play(const std::pair<uint64_t, uint64_t> &coord, Player p) {
    if (coord.first >= m_size 
            || coord.second >= m_size 
            || m_board[coord.second][coord.first] != EMPTY) {
        std::cerr << "Incorrect move" << std::endl;
        return false;
    }
    m_board[coord.second][coord.first] = p;
    return true;
}

uint64_t Game::get_state() const {
    uint64_t state = 0;
    for (uint64_t i = 0; i < m_size; ++i) {
        for (uint64_t j = 0; j < m_size; ++j) {
            state = state * 3 + (m_board[i][j] % 3);
        }
    }
    return state;
}

uint64_t Game::get_state(const std::pair<uint64_t, uint64_t> &coord, Player p) const {
    if (m_board[coord.second][coord.first] != EMPTY) {
        std::cout << "Trying to get state of the game after move " 
                  << coord.first << ", " << coord.second << " (" 
                  << ((p == X) ? "X" : "O") 
                  << "), but the position is not empty" << std::endl;
        return get_state();
    }
    
    uint64_t state = 0;
    for (uint64_t i = 0; i < m_size; ++i) {
        for (uint64_t j = 0; j < m_size; ++j) {
            if (i == coord.second && j == coord.first) 
                state = state * 3 + (p % 3);
            else 
                state = state * 3 + (m_board[i][j] % 3);
        }
    }
    return state;
}

std::vector<std::pair<uint64_t, uint64_t>> Game::get_possible_moves() const {
    std::vector<std::pair<uint64_t, uint64_t>> moves;
    for (uint64_t i = 0; i < m_size; ++i) {
        for (uint64_t j = 0; j < m_size; ++j) {
            if (m_board[i][j] == EMPTY) moves.push_back(std::make_pair(j, i));
        }
    }
    return moves;
}

std::ostream &operator<<(std::ostream &out, const rl::tic_tac_toe::Game &g) {
    for (uint64_t i = 0; i < g.m_size * 2 + 1; ++i) {
        out << "_";
    } out << std::endl;

    for (uint64_t i = 0; i < g.m_size; ++i) {
        out << "|";
        for (uint64_t j = 0; j < g.m_size; ++j) {
            if (g.m_board[i][j] == g.EMPTY) out << " |";
            else if (g.m_board[i][j] == g.X) out << "X|";
            else if (g.m_board[i][j] == g.O) out << "O|";
        }
        out << std::endl;
        for (uint64_t j = 0; j < g.m_size * 2 + 1; ++j) {
            out << "-";
        } out << std::endl;
    }
    return out;
}
} // end tic_tac_toe namespace 
} // end rl namespace