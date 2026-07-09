#include <vector>
#include <string>
#include <iostream>
#include <thread>
#include <chrono>
#include <random>
#include <unordered_map>
#include <array>
#include <utility>

/*const std::vector <std::vector<char>> map = {
    {'#','#','#','#','#','#','#','#','#','#','#','#','#','#','#','#','#','#','#','#'},
    {'#',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ','#',' ',' ',' ',' ','#'},
    {'#',' ',' ',' ','#','#','#',' ',' ',' ',' ',' ',' ',' ','#',' ',' ',' ',' ','#'},
    {'#',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ','#'},
    {'#',' ',' ',' ',' ',' ','#','#','#',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ','#'},
    {'#',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ','#','#',' ','#'},
    {'#',' ',' ','#','#',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ','#'},
    {'#',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ','#',' ',' ',' ',' ',' ',' ',' ','#'},
    {'#',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ','#',' ',' ',' ',' ',' ',' ',' ','#'},
    {'#','#','#','#','#','#','#','#','#','#','#','#','#','#','#','#','#','#','#','#'}
};*/

const std::vector <std::vector<char>> map = {
    {'#','#','#','#','#','#','#','#','#','#','#','#','#','#','#','#','#','#','#','#'},
    {'#',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ','#'},
    {'#',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ','#'},
    {'#',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ','#'},
    {'#',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ','#'},
    {'#',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ','#'},
    {'#',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ','#'},
    {'#',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ','#'},
    {'#',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ','#'},
    {'#','#','#','#','#','#','#','#','#','#','#','#','#','#','#','#','#','#','#','#'}
};


struct Position{
    int y;
    int x;

    bool operator==(const Position& other) const{
        return x == other.x && y == other.y;
    }
};

struct PositionHash{
    std::size_t operator()(const Position& other) const{
        return (std::hash<int>{}(other.x) ^ std::hash<int>{}(other.y) << 1);
    }
};

enum class TileState{
    EMPTY, 
    WALL
};

Position random_pos_picker(){
    std::random_device seed;
    std::mt19937 generator(seed());
    std::uniform_int_distribution<int> random_x(1, map[0].size() - 1);
    std::uniform_int_distribution<int> random_y(1, map.size() - 1);

    int y, x;
    do{
        y = random_y(generator);
        x = random_x(generator);
    }while(map[y][x] != ' ');

    return {y, x};
}

class Robot{
    private :
        std::vector <std::vector<char>> mind_map = {
            {' ', '?', '?', '?', ' '},
            {'?', ' ', ' ', ' ', '?'},
            {'?', ' ', ' ', ' ', '?'},
            {'?', ' ', ' ', ' ', '?'},
            {' ', '?', '?', '?', ' '}
        };

        const std::array<Position, 9> offsets{{
            { 0,  0},//self
            { 0, -1},//kiri
            { 0,  1},//kanan
            {-1,  0},//atas
            { 1,  0},//bawah
            {-1, -1},//kiri atas
            { 1, -1},//kiri bawah
            {-1,  1},//kanan atas
            { 1,  1}//kanan bawah
        }};

        void discover_tile(const Position& pos){
            map_memo.insert({pos, 
                (map[pos.y][pos.x] == ' ') ? TileState::EMPTY : TileState::WALL});

            mind_map[pos.y][pos.x] = ((map[pos.y][pos.x] == ' ') ? ' ' : '#');
        }

        std::unordered_map<Position, TileState, PositionHash> map_memo;

    public :
        Position virtual_position;
        Position current_position;

        Robot(){
            //this->virtual_position = this->current_position = random_pos_picker();
            this->virtual_position = {2, 2};
            this->current_position = {2, 2};

            //insert sekeliling ke map memo dulu. biar ga error PENTINGGG!!!!!!!!!!!!!!!
            for(const Position& offset : offsets){
                Position p{
                    virtual_position.y + offset.y,
                    virtual_position.x + offset.x
                };

                map_memo.insert({p, map[p.y][p.x] == ' ' ? TileState::EMPTY : TileState::WALL});
            }
        }

        void expand_x_plus(){//right
            auto iterator = map_memo.find({virtual_position.y, virtual_position.x + 1});
            if(iterator->second != TileState::WALL){
                //std::cout << "\na";
                this->virtual_position.x += 1;
                this->current_position.x += 1;

                if(map_memo.find(Position{virtual_position.y, virtual_position.x + 1}) == map_memo.end()){
                    if(virtual_position.x + 1 == mind_map[virtual_position.y].size() - 1) {for(int i = 0; i < mind_map.size(); i++)mind_map[i].push_back(' ');}

                    discover_tile({virtual_position.y, virtual_position.x + 1});

                    for(const Position& d : std::array<Position, 2>{{{-1, 1}, {1, 1}}}){
                        if(map_memo.find({virtual_position.y + d.y, virtual_position.x + d.x}) == map_memo.end()){
                            discover_tile({virtual_position.y + d.y, virtual_position.x + d.x});
                        }
                    }
                }
            }
            else return;
        }

        void expand_x_minus(){//left
            auto iterator = map_memo.find({virtual_position.y, virtual_position.x - 1});
            if(iterator->second != TileState::WALL){
                this->virtual_position.x -= 1;
                this->current_position.x -= 1;
                //std::cout << "\nmoving left\n";
                if(map_memo.find(Position{virtual_position.y, virtual_position.x - 1}) == map_memo.end()){
                    if(virtual_position.x - 1 == 0){
                        for (int i = 0; i < mind_map.size(); i++)mind_map[i].insert(mind_map[i].begin(), ' ');
                        this->virtual_position.x += 1;

                        std::unordered_map<Position, TileState, PositionHash> new_map;

                        for(const auto &pair : map_memo){
                            Position new_pos{pair.first.y, pair.first.x + 1};

                            new_map.insert({new_pos, pair.second});
                        }

                        map_memo = std::move(new_map);
                    }

                    discover_tile({virtual_position.y, virtual_position.x - 1});

                    for(const Position& d : std::array<Position, 2>{{{-1, -1}, {1, -1}}}){
                        if(map_memo.find({virtual_position.y + d.y, virtual_position.x + d.x}) == map_memo.end()){
                            discover_tile({virtual_position.y + d.y, virtual_position.x + d.x});
                        }
                    }
                }
            }
            else return;
        }

        void expand_y_minus(){//up
            auto iterator = map_memo.find({virtual_position.y - 1, virtual_position.x});
            if(iterator->second != TileState::WALL){
                this->virtual_position.y -= 1;
                this->current_position.y -= 1;

                if(map_memo.find(Position{virtual_position.y - 1, virtual_position.x}) == map_memo.end()){
                    if(virtual_position.y - 1 == 0){
                        mind_map.insert(mind_map.begin(), std::vector <char>(mind_map[virtual_position.y].size(), ' '));
                        virtual_position.y += 1;
                        std::unordered_map<Position, TileState, PositionHash> new_map;
    
                        for(const auto &pair : map_memo){
                            Position new_pos{pair.first.y + 1, pair.first.x};

                            new_map.insert({new_pos, pair.second});
                        }
                        map_memo = std::move(new_map);
                    }
                    discover_tile({virtual_position.y - 1, virtual_position.x});
                    for(const Position& d : std::array<Position, 2>{{{-1, -1}, {-1, 1}}}){
                        if(map_memo.find({virtual_position.y + d.y, virtual_position.x + d.x}) == map_memo.end()){
                            discover_tile({virtual_position.y + d.y, virtual_position.x + d.x});
                        }
                    }
                }
            }
            else return;
        }

        void expand_y_plus(){
            auto iterator = map_memo.find({virtual_position.y + 1, virtual_position.x});
            if(iterator->second != TileState::WALL){
                //std::cout << "\na";
                this->virtual_position.y += 1;
                this->current_position.y += 1;

                if(map_memo.find(Position{virtual_position.y + 1, virtual_position.x}) == map_memo.end()){
                    if(virtual_position.y + 1 == mind_map.size() - 1)mind_map.push_back(std::vector <char>(mind_map[virtual_position.y].size(), ' '));

                    discover_tile({virtual_position.y + 1, virtual_position.x});

                    for(const Position& d : std::array<Position, 2>{{{1, 1}, {1, -1}}}){
                        if(map_memo.find({virtual_position.y + d.y, virtual_position.x + d.x}) == map_memo.end()){
                            discover_tile({virtual_position.y + d.y, virtual_position.x + d.x});
                        }
                    }
                }
            }
            else return;
        }

        void print(){
            
            for(int i = 0; i < mind_map.size(); i++){
                for(int j = 0; j < mind_map[i].size(); j++){
                    auto iterator = map_memo.find(Position{i, j});
                    if(iterator == map_memo.end()) std::cout << '?';
                    else if(i == virtual_position.y && j == virtual_position.x)std::cout << 'o';
                    else{
                        TileState state = iterator->second;
                        std::cout << ((state == TileState::WALL) ? '#' : ' ');
                    }
                    //else std::cout << mind_map[i][j];
                }
                std::cout << "                                      ";
                std::cout << '\n';
            }
            std::cout << "x : " << mind_map[virtual_position.y].size();
            std::cout << "\ny : " << mind_map.size();
            //std::cout << "\npos x : " << this->current_position.x;
            //std::cout << "\npos y : " << this->current_position.y;
            std::cout << "\npos x virt : " << this->virtual_position.x;
            std::cout << "\npos y virt : " << this->virtual_position.y << '\n';
        }
};



int main(){
    Robot bot1;
    int i = 0;
    //bot1.print();
    while(i < 10){
        std::cout << '\n' << i << '\n';
        bot1.print();
        bot1.expand_y_plus();
        i++;
        std::this_thread::sleep_for(std::chrono::seconds(1));
        std::cout << "\x1B[H";
    }

}