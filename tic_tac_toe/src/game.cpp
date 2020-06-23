#include "game.hpp"

namespace rl {
namespace tic_tac_toe {
Game::Game() : m_size(3) {
    init();
}

Game::Game(unsigned int size) : m_size(size) {
    if (m_size < 3) {
        m_size = 3;
        std::cerr << "Size of the m_board cannot be less than 3. "
                  << "Reassigning size to 3." << std::endl;
    } 
    init();
}

void Game::init() {
    m_board = new Player*[m_size];
    for (unsigned int i = 0; i < m_size; ++i) {
        m_board[i] = new Player[m_size];
        for (unsigned int j = 0; j < m_size; ++j) {
            m_board[i][j] = EMPTY;
        }
    }
}

Game::~Game() {
    for (unsigned int i = 0; i < m_size; ++i) {
        delete[] m_board[i];
    }
    delete[] m_board;
}

Game::Player Game::winner() const {
    bool l_diag_win = true;
    Player l_diag_val = m_board[0][0];
    if (l_diag_val != Player::EMPTY) {
        for (unsigned int i = 1; i < m_size && l_diag_win; ++i) {
            l_diag_win = (m_board[i][i] == l_diag_val);
        }
        if (l_diag_win) return l_diag_val;
    }

    bool r_diag_win = true;
    Player r_diag_val = m_board[m_size - 1][0];
    if (r_diag_val != Player::EMPTY) {
        for (unsigned int i = 1; i < m_size && r_diag_win; ++i) {
            r_diag_win = (m_board[m_size - i - 1][i] == r_diag_val);
        }
        if (r_diag_win) return r_diag_val;
    }

    for (unsigned int i = 0; i < m_size; ++i) {
        bool hor_win = true;
        Player hor_val = m_board[i][0];
        if (hor_val != Player::EMPTY) {
            for (unsigned int j = 1; j < m_size && hor_win; ++j) {
                hor_win = (m_board[i][j] == hor_val);
            }
            if (hor_win) return hor_val;
        }
    }

    for (unsigned int i = 0; i < m_size; ++i) {
        bool vert_win = true;
        Player vert_val = m_board[0][i];
        if (vert_val != Player::EMPTY) {
            for (unsigned int j = 1; j < m_size && vert_win; ++j) {
                vert_win = (m_board[j][i] == vert_val);
            }
            if (vert_win) return vert_val;
        }
    }
    return Player::EMPTY;
}

bool Game::game_over() const {
    if (winner() != EMPTY) return true;
    for (unsigned int i = 0; i < m_size; ++i) {
        for (unsigned int j = 0; j < m_size; ++j) {
            if (m_board[i][j] == EMPTY) return false;
        }
    }
    return true;
};

bool Game::play(const std::pair<unsigned int, unsigned int> &coord, Player p) {
    if (coord.first >= m_size 
            || coord.second >= m_size 
            || m_board[coord.second][coord.first] != EMPTY) {
        std::cerr << "Incorrect move" << std::endl;
        return false;
    }
    m_board[coord.second][coord.first] = p;
    return true;
}

unsigned int Game::get_state() const {
    unsigned int state = 0;
    for (unsigned int i = 0; i < m_size; ++i) {
        for (unsigned int j = 0; j < m_size; ++j) {
            state = state * 3 + (m_board[i][j] % 3);
        }
    }
    return state;
}

std::vector<std::pair<unsigned int, unsigned int>> Game::possible_moves() const {
    std::vector<std::pair<unsigned int, unsigned int>> moves;
    for (unsigned int i = 0; i < m_size; ++i) {
        for (unsigned int j = 0; j < m_size; ++j) {
            if (m_board[i][j] != EMPTY) moves.push_back(std::make_pair(j, i));
        }
    }
    return moves;
}

std::ostream &operator<<(std::ostream &out, const rl::tic_tac_toe::Game &g) {
    for (unsigned int i = 0; i < g.m_size * 2 + 1; ++i) {
        out << "_";
    } out << std::endl;

    for (unsigned int i = 0; i < g.m_size; ++i) {
        out << "|";
        for (unsigned int j = 0; j < g.m_size; ++j) {
            if (g.m_board[i][j] == g.EMPTY) out << " |";
            else if (g.m_board[i][j] == g.X) out << "X|";
            else if (g.m_board[i][j] == g.O) out << "O|";
        }
        out << std::endl;
        for (unsigned int j = 0; j < g.m_size * 2 + 1; ++j) {
            out << "-";
        } out << std::endl;
    }
    return out;
}
} // end tic_tac_toe namespace 
} // end rl namespace