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
        std::vector <std::string> mind_map = {
            "?????",
            "?   ?",
            "?   ?",
            "?   ?",
            "?????"
        };

    public :
        Position current_position;
        Robot(){
            this->current_position = {2, 2};
        }

        void expand_x_plus(){
            this->current_position.x += 1;

            if(mind_map[current_position.y][current_position.x + 1] == '?'){
                mind_map[current_position.y][current_position.x + 1] = ' ';
                mind_map[current_position.y].resize(mind_map[current_position.y].length() + 1);
                mind_map[current_position.y][current_position.x + 2] = '?'; //tengah

                mind_map[current_position.y + 1][current_position.x + 1] = ' ';
                mind_map[current_position.y + 1].resize(mind_map[current_position.y + 1].length() + 1);
                mind_map[current_position.y + 1][current_position.x + 2] = '?'; //bawah

                mind_map[current_position.y - 1][current_position.x + 1] = ' ';
                mind_map[current_position.y - 1].resize(mind_map[current_position.y - 1].length() + 1);
                mind_map[current_position.y - 1][current_position.x + 2] = '?'; //atas

                if((mind_map[current_position.y - 1].length() - mind_map[current_position.y - 2].length()) > 1){
                    
                    mind_map[current_position.y - 2].resize(mind_map[current_position.y - 2].length() + 1);
                    mind_map[current_position.y - 2][mind_map[current_position.y - 2].length() - 1] = '?';
                }

                if((mind_map[current_position.y + 1].length() - mind_map[current_position.y + 2].length()) > 1){
                    mind_map[current_position.y + 2].resize(mind_map[current_position.y + 2].length() + 1);
                    mind_map[current_position.y + 2][mind_map[current_position.y + 2].length() - 1] = '?';
                }
            }//expand x+ wise
        }

        void print(){
            
            for(int i = 0; i < mind_map.size(); i++){
                for(int j = 0; j < mind_map[i].length(); j++){
                    if(i == current_position.y && j == current_position.x)std::cout << 'o';
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
        bot1.expand_x_plus();
        i++;
        std::this_thread::sleep_for(std::chrono::seconds(1));
        std::cout << "\x1B[H";
    }
}