#include <bits/stdc++.h>
#include <curses.h>
using namespace std;

const int WIDTH = 10;
const int HEIGHT = 10;
class Board {
private:
    int width = 0;
    int height = 0;
    
public: 
    // default constructor
    Board() {
        height = 0;
        width = 0;
    }

    // initialize board
    Board(int h, int w) {
        height = h;
        width = w;
    }

    // make board
    void makeBoard(int h, int w) {
        Board(h, w);
        for (int i = 1; i <= w*2-1; i++) cout << "_";
        cout << '\n';
        for (int i = 1; i <= h-1; i++) {
            for (int j = 1; j <= w-1; j++) cout << "|_";
            cout << "|";
            cout << '\n';
        }
    }
};

class Snake {
private:
    // data variables come here
    
public: 
    // functions come here
};

class Food {
private:
    // data variables come here
    
public: 
    // functions come here
};

class Game {
private:
    // data variables come here

public: 
    // functions come here

};

int main() {
    Board newBoard;
    newBoard.makeBoard(HEIGHT, WIDTH);
}