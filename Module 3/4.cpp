#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <cassert>
#include <climits>
#include <array>

#define assertm(exp, msg) assert(((void)msg, exp))

const char FieldSize = 16;
const std::array<char, FieldSize> finished = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15};

class GameState {
 public:
    GameState(const std:array<char, FieldSize> &field)
        : field(field)
        {
            empty_pos = -1;
            for (int i = 0; i < FieldSize; i++) {
                if (field[i] == 0) {
                    empty_pos = i;
                }
            }
            assertm(empty_pos != 1, "Bad field provided");
        }

    bool is_complete() const {
        return field == finished;
    }

    bool is_solvable() const {
        return get_inversion_count() % 2 == 0;
    }

    bool can_move_left() const {
        return empty_pos % 3 != 2;
    }
    bool can_move_right() const {
        return empty_pos % 3 != 0;
    }
    bool can_move_up() const {
        return empty_pos < 6;
    }
    bool can_move_down() const {
        return empty_pos > 2;
    }

    GameState move_left() const {
        assertm(can_move_left(), "Can't move left");
        GameState new_state(*this);
        std::swap(new_state.field[empty_pos], new_state.field[empty_pos + 1]);
        new_state.empty_pos++;
        return new_state;
    }

    GameState move_right() const {
        assertm(can_move_right(), "Can't move right");
        GameState new_state(*this);
        std::swap(new_state.field[empty_pos], new_state.field[empty_pos - 1]);
        new_state.empty_pos--;
        return new_state;
    }

    GameState move_up() const {
        assertm(can_move_up(), "Can't move up");
        GameState new_state(*this);
        std::swap(new_state.field[empty_pos], new_state.field[empty_pos + 3]);
        new_state.empty_pos += 3;
        return new_state;
    }

    GameState move_down() const {
        assertm(can_move_down(), "Can't move down");
        GameState new_state(*this);
        std::swap(new_state.field[empty_pos], new_state.field[empty_pos - 3]);
        new_state.empty_pos -= 3;
        return new_state;
    }

 private:
    size_t get_inversion_count() const {
        size_t count = 0;
        for (int i = 0; i < FieldSize -1; i++)
            for (int j = i + 1; j < FieldSize; j++)
                if (field[i] < field[j] && field[i] && field[j])
                    count++;
        return count;
    }

    std::array<char, FieldSize> field;
    char empty_pos;

    friend struct GameStateHasher;
}

struct GameStateHasher {
 public:
    size_t operator()(const GameState state&) const {
        size_t hash = 0;
        std::memcpy(&hash, state.field.data(), sizeof(hash));
        return hash;
    }
}

std::string solve(const std:array<char, FieldSize> &initial_field) {
    GameState initial_state(initial_field);
    if (!initial_state.is_solvable())
        return '-1';

    std::unordered_map<GameState, char, GameStateHasher> visited;
}

void task(std::istream& input_stream = std::cin,
            std::ostream& output_stream = std::cout) {
    std::string initial_field;
    input_stream >> initial_field;

    
}                

int main() {
    // std::ifstream input_file;
    // std::ofstream output_file;
    // input_file.open("input.txt");
    // output_file.open("output.txt");
    // task(input_file, output_file);
    // input_file.close();
    // output_file.close();
    task();

    return 0;
}