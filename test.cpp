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

        void expand_x_plus(){
            this->virtual_position.x += 1;
            this->current_position.x += 1;

            if(mind_map[virtual_position.y][virtual_position.x + 1] == '?'){
                mind_map[virtual_position.y][virtual_position.x + 1] = ' ';
                mind_map[virtual_position.y].resize(mind_map[virtual_position.y].size() + 1);
                mind_map[virtual_position.y][virtual_position.x + 2] = '?'; //tengah

                mind_map[virtual_position.y + 1][virtual_position.x + 1] = ' ';
                mind_map[virtual_position.y + 1].resize(mind_map[virtual_position.y + 1].size() + 1);
                mind_map[virtual_position.y + 1][virtual_position.x + 2] = '?'; //bawah

                mind_map[virtual_position.y - 1][virtual_position.x + 1] = ' ';
                mind_map[virtual_position.y - 1].resize(mind_map[virtual_position.y - 1].size() + 1);
                mind_map[virtual_position.y - 1][virtual_position.x + 2] = '?'; //atas

                std::cout << mind_map[virtual_position.y - 1].size() - mind_map[virtual_position.y - 2].size() << '\n';
                std::cout << mind_map[virtual_position.y + 1].size() - mind_map[virtual_position.y + 2].size() << '\n';

                if((mind_map[virtual_position.y - 1].size() - mind_map[virtual_position.y - 2].size()) > 0){
                    mind_map[virtual_position.y - 2].insert(mind_map[virtual_position.y - 2].begin() + mind_map[virtual_position.y - 2].size() - 2, '?');
                }

                if((mind_map[virtual_position.y + 1].size() - mind_map[virtual_position.y + 2].size()) > 0){
                    mind_map[virtual_position.y + 2].insert(mind_map[virtual_position.y + 2].begin() + mind_map[virtual_position.y + 2].size() - 2, '?');
                }
            }//expand x+ wise
        }

        void expand_x_minus(){
            this->virtual_position.x -= 1;
            this->current_position.x -= 1;
            
            static int corner = 0;

            if(mind_map[virtual_position.y][virtual_position.x - 1] == '?'){
                corner++;
                mind_map[virtual_position.y].resize(mind_map[virtual_position.y].size() + 1);
                mind_map[virtual_position.y].insert(mind_map[virtual_position.y].begin(), '?');
                this->virtual_position.x += 1;
                mind_map[virtual_position.y][virtual_position.x - 1] = ' ';//tengah

                mind_map[virtual_position.y + 1].resize(mind_map[virtual_position.y + 1].size() + 1);
                mind_map[virtual_position.y + 1].insert(mind_map[virtual_position.y + 1].begin(), '?');
                mind_map[virtual_position.y + 1][virtual_position.x - 1] = ' ';

                mind_map[virtual_position.y - 1].resize(mind_map[virtual_position.y - 1].size() + 1);
                mind_map[virtual_position.y - 1].insert(mind_map[virtual_position.y - 1].begin(), '?');
                mind_map[virtual_position.y - 1][virtual_position.x - 1] = ' ';

                if(corner < 1);
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
    //bot1.print();
    while(i < 5){
        bot1.expand_x_plus();
        bot1.print();
        i++;
        std::this_thread::sleep_for(std::chrono::seconds(1));
        //std::cout << "\x1B[H";
        std::cout << "\n\n";
    }
}