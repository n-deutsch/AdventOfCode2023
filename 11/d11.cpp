#include <iostream>
#include <vector>
#include <fstream>

class galaxy {
    int identifier = 0;
    int x = 0;
    int y = 0;

    public:
        galaxy(int i, int xArg, int yArg) {
            identifier = i;
            x = xArg;
            y = yArg;
        }

        void setX(int xArg) {
            x = xArg;
        }

        void setY(int yArg) {
            y = yArg;
        }

        int get_identifier() {
            return identifier;
        }

        int getX() {
            return x;
        }

        int getY() {
            return y;
        }
};

const int TRANSFORM_FACTOR = 999999;
const int ONE_MILLION = 1000000;

std::vector<galaxy> expand_universe(std::vector<galaxy> galaxies, std::vector<bool> columns_with_galaxies, std::vector<bool> rows_with_galaxies) {
    std::vector<galaxy> expanded_galaxies;
    for(int i = 0; i < galaxies.size(); i++) {
        galaxy g = galaxies[i];
        int old_x = g.getX();
        int old_y = g.getY();

        int horizontal_transform = 0;
        for(int j = 0; j < old_x; j++) {
            if(!columns_with_galaxies[j])
                horizontal_transform++;
        }
        int vertical_transform = 0;
        for(int j = 0; j < old_y; j++) {
            if(!rows_with_galaxies[j])
                vertical_transform++;
        }

        // part 2, expand the universe by a MILLION?!
        int new_x = old_x + (horizontal_transform * TRANSFORM_FACTOR);
        int new_y = old_y + (vertical_transform * TRANSFORM_FACTOR);
        std::cout << "Galaxy #" << g.get_identifier() << " moved from [" << old_x << "," << old_y << "] to [" << new_x << "," << new_y << "]" << std::endl;

        g.setX(new_x);
        g.setY(new_y);
        expanded_galaxies.push_back(g);
    }
    return expanded_galaxies;
}

std::vector<galaxy> galaxy_expansion(std::string filename) {
    std::vector<galaxy> galaxies = {};

    std::string line;
    std::ifstream file(filename);

    if(file.is_open()) {
        int x = 0;
        int y = 0;
        int galaxy_count = 1;
        while(std::getline(file,line)) {
            std::cout << "Line:[" << line << "]" << std::endl;
            for(x = 0; x < line.size(); x++) {
                char c = line[x];
                if(c == '#') {
                    std::cout << "Found galaxy at x[" << x << "] y[" << y << "]" << std::endl;
                    galaxies.push_back(galaxy(galaxy_count,x,y));
                    galaxy_count++;
                }
            }
            y++;
        }

        std::vector<bool> columns_with_galaxies;
        columns_with_galaxies.assign(x, false);
        std::vector<bool> rows_with_galaxies;
        rows_with_galaxies.assign(y, false);

        // iterate over each galaxy, identify rows and columns that contain galaxies
        for(int i = 0; i < galaxies.size(); i++) {
            galaxy g = galaxies[i];
            columns_with_galaxies[g.getX()] = true;
            rows_with_galaxies[g.getY()] = true;
        }

        galaxies = expand_universe(galaxies, columns_with_galaxies, rows_with_galaxies);
    }

    file.close();
    return galaxies;
}

double sum_total_distance(std::vector<galaxy> galaxies) {
    int total = 0;
    int millions = 0;
    for(int i = 0; i < galaxies.size(); i++) {
        for(int j = i+1; j < galaxies.size(); j++) {
            galaxy start = galaxies[i];
            galaxy destination = galaxies[j];
            int start_x = start.getX();
            int start_y = start.getY();
            int dest_x = destination.getX();
            int dest_y = destination.getY();
            
            std::cout << "Calculating distance between Galaxy #" << start.get_identifier() << " [" << start_x << "," << start_y << "] and Galaxy #" << destination.get_identifier() << " [" << dest_x << "," << dest_y << "]" << std::endl;
            int x_dist = dest_x - start_x;
            if(x_dist < 0)
                x_dist = x_dist * -1;

            int y_dist = dest_y - start_y;
            if(y_dist < 0)
                y_dist = y_dist * -1;

            int distance = x_dist + y_dist;
            std::cout << "Distance: " << distance << std::endl;
            total += distance;
            if(total > ONE_MILLION) {
                millions = millions + total / ONE_MILLION;
                total = total % ONE_MILLION;
            }

            std::cout << "Running total: [" << total  << "] and [" << millions << "] million" << std::endl;
        }
    }

    return total;
}

int main() {
    std::cout << "Startup" << std::endl;
    std::vector<galaxy> galaxies = galaxy_expansion("infile2.txt");
    double total = sum_total_distance(galaxies);
    std::cout << "TOTAL=" << total << std::endl;
    std::cout << "Done" << std::endl;
    return 0;
}
