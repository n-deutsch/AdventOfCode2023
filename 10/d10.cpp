#include <iostream>
#include <fstream>
#include <vector>
#include <unordered_set>


/*
Guide: 
| is a vertical pipe connecting north and south.
- is a horizontal pipe connecting east and west.
L is a 90-degree bend connecting north and east.
J is a 90-degree bend connecting north and west.
7 is a 90-degree bend connecting south and west.
F is a 90-degree bend connecting south and east.
. is ground; there is no pipe in this tile.
S is the starting position of the animal; there is a pipe on this tile, but your sketch doesn't show what shape the pipe has.
*/
enum direction {
    NONE,
    UP,
    DOWN,
    RIGHT,
    LEFT
};

class coordinates {
    private:
        int x;
        int y;
        direction d;

    public:
        coordinates(int a, int b, direction c) {
            x = a;
            y = b;
            d = c;
        }

        int get_x() {
            return x;
        }

        int get_y() {
            return y;
        }

        direction get_direction() {
            return d;
        }
};

std::unordered_set<char> up_facing_pipes;
std::unordered_set<char> down_facing_pipes;
std::unordered_set<char> left_facing_pipes;
std::unordered_set<char> right_facing_pipes;

const int X_LIM = 10;
const int Y_LIM = 9;
const std::string FILE_NAME = "/Users/deutsch/sft/AdventOfCode2023/10/in5.txt";

void initialize_pipes() {
    up_facing_pipes.insert('S');
    up_facing_pipes.insert('|');
    up_facing_pipes.insert('L');
    up_facing_pipes.insert('J');

    down_facing_pipes.insert('S');
    down_facing_pipes.insert('|');
    down_facing_pipes.insert('F');
    down_facing_pipes.insert('7');

    right_facing_pipes.insert('S');
    right_facing_pipes.insert('-');
    right_facing_pipes.insert('L');
    right_facing_pipes.insert('F');

    left_facing_pipes.insert('S');
    left_facing_pipes.insert('-');
    left_facing_pipes.insert('J');
    left_facing_pipes.insert('7');
}

bool can_move_up(char** pipes, int x, int y) {
    char current_pipe = pipes[y][x];
    char next_pipe = pipes[y-1][x];
    bool b1 = up_facing_pipes.find(current_pipe) != up_facing_pipes.end();
    bool b2 = down_facing_pipes.find(next_pipe) != down_facing_pipes.end();
    return b1 && b2;
}

bool can_move_down(char** pipes, int x, int y) {
    char current_pipe = pipes[y][x];
    char next_pipe = pipes[y+1][x];
    bool b1 = down_facing_pipes.find(current_pipe) != down_facing_pipes.end();
    bool b2 = up_facing_pipes.find(next_pipe) != up_facing_pipes.end();
    return b1 && b2;
}

bool can_move_left(char** pipes, int x, int y) {
    char current_pipe = pipes[y][x];
    char next_pipe = pipes[y][x-1];
    bool b1 = left_facing_pipes.find(current_pipe) != left_facing_pipes.end();
    bool b2 = right_facing_pipes.find(next_pipe) != right_facing_pipes.end();
    return b1 && b2;
}

bool can_move_right(char** pipes, int x, int y) {
    char current_pipe = pipes[y][x];
    char next_pipe = pipes[y][x+1];
    bool b1 = right_facing_pipes.find(current_pipe) != right_facing_pipes.end();
    bool b2 = left_facing_pipes.find(next_pipe) != left_facing_pipes.end();
    return b1 && b2;
}

std::vector<coordinates> find_next(char** pipes, coordinates cords) {
    std::vector<coordinates> next_steps;
    int x = cords.get_x();
    int y = cords.get_y();
    direction d = cords.get_direction();

    // can we move UP?
    if(y - 1 >= 0 && d != DOWN && can_move_up(pipes, x, y)) {
        std::cout << "Going UP" << std::endl;
        next_steps.push_back(coordinates(x, y-1, UP));
    }
    if(y + 1 < Y_LIM && d != UP && can_move_down(pipes, x, y)) {
        std::cout << "Going DOWN" << std::endl;
        next_steps.push_back(coordinates(x, y+1, DOWN));
    }
    if(x + 1 < X_LIM && d != LEFT && can_move_right(pipes, x, y)) {
        std::cout << "Going RIGHT" << std::endl;
        next_steps.push_back(coordinates(x+1, y, RIGHT));
    }
    if(x - 1 >= 0 && d != RIGHT && can_move_left(pipes, x, y)) {
        std::cout << "Going LEFT" << std::endl;
        next_steps.push_back(coordinates(x-1, y, LEFT));
    }

    // can we move 
    return next_steps;
}

int recursive_longest_loop(char **pipes, char **visited, coordinates cords, int length, std::string route) {
    int x = cords.get_x();
    int y = cords.get_y();
    char pipe = pipes[y][x];

    std::string x_str = std::to_string(x);
    std::string y_str = std::to_string(y);
    if(!route.empty()) {
        route += "->";
    }
    std::string new_route = route + "(" + x_str + "," + y_str + ")";

    if(pipes[y][x] == 'S' && visited[y][x] == 'X') {
        std::cout << "Found loop of length=[" << length << "]" << std::endl;
        std::cout << "Route=[" << new_route << "]" << std::endl;
        return length;
    } else if(visited[y][x] == 'X') {
        std::cout << "Already been to [" << x << "," << y << "]!" << std::endl;
        return -1;
    }

    std::cout << "Visiting [" << x << "," << y << "] current pipe [" << pipe << "] current length [" << length << "]..." << std::endl;
    visited[y][x] = 'X';

    std::vector<coordinates> next_steps = find_next(pipes, cords);
    int max_len = 0;
    for(int i = 0; i < next_steps.size(); i++) {
        int l = recursive_longest_loop(pipes, visited, next_steps[i], length+1, new_route);
        if(l > max_len) {
            max_len = l;
            // std::cout << "New longest path: " << max_len << std::endl;
        }
    }

    return max_len;
}

void draw_visited(char **visited) {
    std::cout << "Printing visited grid..." << std::endl;
    for(int i = 0; i < Y_LIM; i++) {
        std::string line;
        for(int j = 0; j < X_LIM; j++) {
            char c = visited[i][j];
            line += std::string(c);
        }
        std::cout << line << std::endl;
    }
    std::cout << "Done printing visited grid." << std::endl;
}

int recursive_contained_tiles(char **pipes, char **visited, int x, int y, bool contained) {
    if (visited[y][x] == 'O' || visited[y][x] == 'I') {
        // std::cout << "Already been to (" << x << "," << y << ")" << std::endl;
        return 0;
    }

    std::cout << "Visiting (" << x << "," << y << ")" << std::endl;
    if (x == 0 || x == X_LIM - 1 || y == 0 || y == Y_LIM - 1) {
        std::cout << "(" << x << "," << y << ") is at the map border. We're free!" << std::endl;
        contained = false;
    }

    if(contained) {
        visited[y][x] = 'I';
    }

    int tiles = 1;
    // haven't visited this square, keep exploring
    std::vector<coordinates> next_steps;
    if (y != 0) {
        next_steps.push_back(coordinates(x, y-1, NONE)); // up
    }
    if (y != Y_LIM - 1) {
        next_steps.push_back(coordinates(x, y+1, NONE)); // down
    }
    if (x != 0) {
    next_steps.push_back(coordinates(x-1, y, NONE)); // left
    }
    if(x != X_LIM -1) {
        next_steps.push_back(coordinates(x+1, y, NONE)); // right
    }

    for(int i = 0; i < next_steps.size(); i++) {
        coordinates c = next_steps[i];
        int tile_count = recursive_contained_tiles(pipes, visited, c.get_x(), c.get_y(), contained);
        if(tile_count < 0) {
            contained = false;
        } else {
            tiles += tile_count;
        }
    }

    if(contained) {
        std::cout << "Discovered [" << tiles << "] contained tiles" << std::endl;
        return tiles;
    }
    else {
        std::cout << "Able to escape, recursively returning [-1]" << std::endl;
        return -1;
    }
}

int count_contained_tiles(char **pipes, char **visited) {
    int contained_tiles = 0;
    for(int row_index = 0; row_index < Y_LIM; row_index++) {
        for(int col_index = 0; col_index < X_LIM; col_index++) {
            bool visit = visited[row_index][col_index];
            if(!visit) {
                std::cout << "Beginning recursive tile search at (" << col_index << "," << row_index << ")" << std::endl;
                int count = recursive_contained_tiles(pipes, visited, col_index, row_index, true);
                if(count > 0) {
                    contained_tiles+=count;
                }
            } else {
                // std::cout << "Skipping (" << row_index << "," << col_index << ")..." << std::endl;
            }
        }
    }
    std::cout << "Found [" << contained_tiles << "] TOTAL contained tiles" << std::endl;
    return contained_tiles;
}


int longest_loop(std::string filename) {
    initialize_pipes();
    char **pipes;
    char **visited;

    pipes = new char*[X_LIM];
    visited = new char*[Y_LIM];

    int start_x = 0;
    int start_y = 0;

    std::string line;
    std::ifstream file(filename);
    if(file.is_open()) {
        int row_count = 0;
        while(std::getline(file,line)) {
            pipes[row_count] = new char[X_LIM];
            visited[row_count] = new char[Y_LIM];
            std::cout << line << std::endl;
            for(int i = 0; i < line.size(); i++) {
                char c = line[i];
                pipes[row_count][i] = c;
                visited[row_count][i] = '*';

                if(c == 'S') {
                    start_y = row_count;
                    start_x = i;
                }
            }
            row_count++;
        }
    }
    file.close();

    int longest = recursive_longest_loop(pipes, visited, coordinates(start_x, start_y, NONE), 0, "");
    draw_visited(visited);
    // char**big_map = create_big_map(longest);
    count_contained_tiles(pipes, visited);
    draw_visited(visited);
    return longest;
}

int main() {
    std::cout << "Startup" << std::endl;
    int longest = longest_loop(FILE_NAME);
    std::cout << "Done! Longest=[" << longest << "]" << std::endl;
    std::cout << "Answer=[" << longest/2 << "]" << std::endl;
    return 0;
}