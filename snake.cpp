#include <bits/stdc++.h>
#include <curses.h>
using namespace std;

const int WIDTH = 30;
const int HEIGHT = 30;
const int HEAD_X = WIDTH/2;
const int HEAD_Y = HEIGHT/2;
const char BODY_CHAR = '*';
const char HEAD_CHAR = '@';

class Board {
private:
    int height, width;
    
public: 
    // default constructor
    Board() {
        height = HEIGHT;
        width = WIDTH;
    }

    // make board
    void makeBoard() {
        for (int j = 0; j < WIDTH-1; j++) {
            mvprintw(0, j*2+1, "_");
        }
        for (int i = 1; i < HEIGHT; i++) {
            for (int j = 0; j < WIDTH; j++) {
                if (j == WIDTH-1) {
                    mvprintw(i, j*2, "|");
                }
                else mvprintw(i, j * 2, "|_"); 
            }
        }
        refresh();
    }
};

class Snake {
private:
    int length;
    char bodyChar, headChar;
    deque<pair<int, int>> snakeBody;
    
public: 
    // default constructor
    Snake() {
        length = 2;
        bodyChar = BODY_CHAR;
        headChar = HEAD_CHAR;
        snakeBody = {{HEAD_Y, HEAD_X}, {HEAD_Y, HEAD_X-1}};
    }

    // get the length of the snake
    const int getLength() const {
        return length;
    }

    // get the coordinates of the body
    const deque<pair<int, int>>& getSnakeBody() const {
        return snakeBody;
    }

    // get the head
    const pair<int, int>& getHead() const {
        return snakeBody[0];
    }

    // get the tail
    const pair<int, int>& getTail() const {
        return snakeBody.back();
    }

    // remove last element from snake's body list
    void removeTail() {
        snakeBody.pop_back();
    }

    // extend snake by adding new tail element
    void addTail(pair<int, int> &newTail) {
        snakeBody.push_back(newTail);
    }

    // replace head by adding new head
    void addHead(pair<int, int> &newHead) {
        snakeBody.push_front(newHead);
    }

    // print the snake on the specified coordinates
    void printSnake() {
        for (int i = 0; i < length; i++) {
            int y = snakeBody[i].first;
            int x = snakeBody[i].second;
            if (i == 0) mvprintw(y, x*2, "|%c", headChar);
            else mvprintw(y, x*2, "|%c", bodyChar);
            refresh();
        }
    }
    // get the direction in which snake's head is facing
    char headDirection() {
        if (snakeBody[0].first == snakeBody[1].first) {
            return (snakeBody[0].second < snakeBody[1].second)? 'L' : 'R';
        }
        else {
            return (snakeBody[0].first < snakeBody[1].first)? 'U' : 'D';
        }
    }
    
    // get the direction in which snake's tail is moving
    char tailDirection() {
        if (snakeBody[length-1].first == snakeBody[length-2].first) {
            return (snakeBody[length-1].second < snakeBody[length-2].second)? 'R' : 'L';
        }
        else {
            return (snakeBody[length-1].first < snakeBody[length-2].first)? 'D' : 'U';
        }
    }
    
    // move the snake forward by one unit by changing its body list
    void moveForward() {
        char headDir = headDirection();
        auto curHead = snakeBody[0];
        pair<int, int> newHead = curHead;
        
        if (headDir == 'R') newHead.second++;
        else if (headDir == 'L') newHead.second--;
        else if (headDir == 'U') newHead.first--;
        else newHead.first++;
    
        addHead(newHead);
        removeTail();
    }
    
    // move the snake right by changing its body list
    void moveRight() {
        char headDir = headDirection();
        auto curHead = snakeBody[0];
        pair<int, int> newHead = curHead;
        
        if (headDir == 'R') newHead.first++;
        else if (headDir == 'L') newHead.first--;
        else if (headDir == 'U') newHead.second++;
        else newHead.second--;
    
        addHead(newHead);
        removeTail();
    }
    
    // move the snake forward by changing its body list
    void moveLeft() {
        char headDir = headDirection();
        auto curHead = snakeBody[0];
        pair<int, int> newHead = curHead;
        
        if (headDir == 'R') newHead.first--;
        else if (headDir == 'L') newHead.first++;
        else if (headDir == 'U') newHead.second--;
        else newHead.second++;
    
        addHead(newHead);
        removeTail();
    }
    
    // extend the snake by adding a tail
    void extendSnake() {
        char tailDir = tailDirection();
        auto curTail = snakeBody.back();
        pair<int, int> newTail = curTail;
    
        if (tailDir == 'R') newTail.second--;
        else if (tailDir == 'L') newTail.second++;
        else if (tailDir == 'U') newTail.first++;
        else newTail.first--;
    
        addTail(newTail);
    }
};

class Food {
private:
    // data variables come here
    
public: 
    // functions come here
};

class Game {
private:
    Board board;
    Snake snake;

public: 
    // default constructor
    Game() {
        snake = Snake();
        board = Board();
    }

};

int main() {
    initscr();
    // Board newBoard;
    // newBoard.makeBoard();
    // Snake newSnake;
    // newSnake.printSnake();
    getch();
    endwin();
    return 0;
}