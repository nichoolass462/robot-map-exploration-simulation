#include <vector>
#include <string>
#include <iostream>
#include <thread>
#include <chrono>

struct Position{
    int y;
    int x;
};

class Robot{
    private :
        std::vector <std::vector<char>> mind_map = {
            {' ', '?', '?', '?', ' '},
            {'?', ' ', ' ', ' ', '?'},
            {'?', ' ', ' ', ' ', '?'},
            {'?', ' ', ' ', ' ', '?'},
            {' ', '?', '?', '?', ' '}
        };

    public :
        Position virtual_position;
        Position current_position;
        Robot(){
            this->virtual_position = {2, 2};
            this->current_position = {2, 2};
        }

        void expand_x_plus(){//right
            this->virtual_position.x += 1;
            this->current_position.x += 1;

            if(mind_map[virtual_position.y][virtual_position.x + 1] == '?'){
                for(int i = 0; i < mind_map.size(); i++)mind_map[i].push_back(' ');
                //this->virtual_position.x += 1;
                mind_map[virtual_position.y][virtual_position.x + 2] = '?';
                mind_map[virtual_position.y + 1][virtual_position.x + 2] = '?';
                mind_map[virtual_position.y - 1][virtual_position.x + 2] = '?';

                mind_map[virtual_position.y][virtual_position.x + 1] = ' ';//tengah
                mind_map[virtual_position.y + 1][virtual_position.x + 1] = ' ';
                mind_map[virtual_position.y - 1][virtual_position.x + 1] = ' ';

                mind_map[virtual_position.y + 2][virtual_position.x + 1] = '?';
                mind_map[virtual_position.y - 2][virtual_position.x + 1] = '?';
            }//expand x+ wise
        }

        void expand_x_minus(){//left
            this->virtual_position.x -= 1;
            this->current_position.x -= 1;

            if(mind_map[virtual_position.y][virtual_position.x - 1] == '?'){
                for(int i = 0; i < mind_map.size(); i++)mind_map[i].insert(mind_map[i].begin(), ' ');
                this->virtual_position.x += 1;
                mind_map[virtual_position.y][virtual_position.x - 2] = '?';
                mind_map[virtual_position.y + 1][virtual_position.x - 2] = '?';
                mind_map[virtual_position.y - 1][virtual_position.x - 2] = '?';

                mind_map[virtual_position.y][virtual_position.x - 1] = ' ';//tengah
                mind_map[virtual_position.y + 1][virtual_position.x - 1] = ' ';
                mind_map[virtual_position.y - 1][virtual_position.x - 1] = ' ';

                mind_map[virtual_position.y + 2][virtual_position.x - 1] = '?';
                mind_map[virtual_position.y - 2][virtual_position.x - 1] = '?';
            }
        }

        void expand_y_minus(){//up
            this->virtual_position.y -= 1;
            this->current_position.y -= 1;
            if(mind_map[virtual_position.y - 1][virtual_position.x] == '?'){
                mind_map[virtual_position.y - 1][virtual_position.x] = ' ';
                mind_map[virtual_position.y - 1][virtual_position.x - 1] = ' ';
                mind_map[virtual_position.y - 1][virtual_position.x + 1] = ' ';

                mind_map.insert(mind_map.begin(), std::vector <char>(mind_map[virtual_position.y].size(), ' '));
                virtual_position.y += 1;
                mind_map[virtual_position.y - 2][virtual_position.x] = '?';
                mind_map[virtual_position.y - 2][virtual_position.x - 1] = '?';
                mind_map[virtual_position.y - 2][virtual_position.x + 1] = '?';

                mind_map[virtual_position.y - 1][virtual_position.x - 2] = '?';
                mind_map[virtual_position.y - 1][virtual_position.x + 2] = '?';
            }
        }

        void expand_y_plus(){
            this->virtual_position.y += 1;
            this->current_position.y += 1;
            if(mind_map[virtual_position.y + 1][virtual_position.x] == '?'){
                mind_map[virtual_position.y + 1][virtual_position.x] = ' ';
                mind_map[virtual_position.y + 1][virtual_position.x - 1] = ' ';
                mind_map[virtual_position.y + 1][virtual_position.x + 1] = ' ';

                mind_map.push_back(std::vector <char>(mind_map[virtual_position.y].size(), ' '));
                //virtual_position.y += 1;
                mind_map[virtual_position.y + 2][virtual_position.x] = '?';
                mind_map[virtual_position.y + 2][virtual_position.x - 1] = '?';
                mind_map[virtual_position.y + 2][virtual_position.x + 1] = '?';

                mind_map[virtual_position.y + 1][virtual_position.x - 2] = '?';
                mind_map[virtual_position.y + 1][virtual_position.x + 2] = '?';
            }
        }

        void print(){
            
            for(int i = 0; i < mind_map.size(); i++){
                for(int j = 0; j < mind_map[i].size(); j++){
                    if(i == virtual_position.y && j == virtual_position.x)std::cout << 'o';
                    else std::cout << mind_map[i][j];
                }
                std::cout << '\n';
            }
        }
};



int main(){
    Robot bot1;
    int i = 0;
    while(i < 5){
        bot1.print();
        bot1.expand_x_minus();
        i++;
        std::this_thread::sleep_for(std::chrono::seconds(1));
        std::cout << "\x1B[H";
    }

    while(i < 10){
        bot1.print();
        bot1.expand_y_minus();
        i++;
        std::this_thread::sleep_for(std::chrono::seconds(1));
        std::cout << "\x1B[H";
    }

    while(i < 15){
        bot1.print();
        bot1.expand_x_plus();
        i++;
        std::this_thread::sleep_for(std::chrono::seconds(1));
        std::cout << "\x1B[H";
    }

    while(i < 18){
        bot1.print();
        bot1.expand_y_plus();
        i++;
        std::this_thread::sleep_for(std::chrono::seconds(1));
        std::cout << "\x1B[H";
    }

}