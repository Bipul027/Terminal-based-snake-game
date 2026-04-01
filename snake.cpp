#include <bits/stdc++.h>
#include <curses.h>
#include <random>
#include <stdexcept>
using namespace std;

const int WIDTH = 30;
const int HEIGHT = 30;
const int HEAD_X = WIDTH/3;
const int HEAD_Y = HEIGHT/2;
const char BODY_CHAR = '*';
const char HEAD_CHAR = '@';
const int FOOD_AMOUNT = (WIDTH*HEIGHT)/100;
int TIME_DELAY = 100000;


class Board {
private:
    int height, width;
    
public: 
    // default constructor
    Board() {
        height = HEIGHT;
        width = WIDTH;
    }
    
    // get board height
    const int getHeight() const {
        return height;
    }
    
    // get board width
    const int getWidth() const {
        return width;
    }
    // make board
    void makeBoard() {
        for (int j = 0; j <= width-1; j++) {
            mvprintw(0, j*2+1, "_");
        }
        for (int i = 1; i <= height; i++) {
            for (int j = 0; j <= width; j++) {
                if (j == width) {
                    mvprintw(i, j * 2, "|");
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
        length++;
    }
    
    // print the snake on the specified coordinates
    void printSnake() {
        for (int i = 0; i < length; i++) {
            int y = snakeBody[i].first;
            int x = snakeBody[i].second;
            if (i == 0) mvprintw(y, x*2, "|%c", headChar);
            else mvprintw(y, x*2, "|%c", bodyChar);
        }
        refresh();
    }
};

// random number generator in a 2D vector (for generating random food)
pair<int, int> getRandom2D(const std::vector<std::vector<int>>& grid) {
    std::vector<std::pair<int,int>> cells;

    for (int i = 0; i < grid.size(); i++) {
        for (int j = 0; j < grid[i].size(); j++) {
            cells.push_back({i, j});
        }
    }

    if (cells.empty()) {
        throw std::runtime_error("2D vector is empty");
    }

    static std::random_device rd;
    static std::mt19937 gen(rd());

    std::uniform_int_distribution<> dis(0, cells.size() - 1);
    auto [i, j] = cells[dis(gen)];

    return {i, j};
}
class Food {
private:
    int x, y;
    
public: 
    Food() {
        x = 0; y = 1;
    }
    
    // take input as cells not occupied by the snake and initialize food with random coordinates
    Food(const vector<vector<bool>> emptyCells) {
        int ht = emptyCells.size()-1, wt = emptyCells[0].size();
        vector<vector<int>> emptyYtoX(ht+1);

        for (int i = 1; i <= ht; i++) {
            for (int j = 0; j < wt; j++) {
                if (emptyCells[i][j]) emptyYtoX[i].push_back(j);
            }
        }
        auto randCoord = getRandom2D(emptyYtoX);
        y = randCoord.first;
        x = randCoord.second;
    }

    // get x coordinate of food
    int getx() {
        return x;
    }

    // get y coordinate of food
    int gety() {
        return y;
    }

    void printFood() {
        mvprintw(y, x * 2, "|%c", '#');
    }

};

class Game {
private:
    Board board;
    Snake snake;
    int foodAmount;
    vector<Food> foodList;
    int score;

public: 
    // generating 2D array of cells which are not occupied by the cells
    const vector<vector<bool>> emptyCells(const Snake &newSnake, const Board &newBoard) const {
        vector<vector<bool>> empty(1+newBoard.getHeight(), vector<bool>(newBoard.getWidth(), true));
        deque<pair<int, int>> body = newSnake.getSnakeBody();
        for (int j = 0; j < newBoard.getWidth(); j++) empty[0][j] = false;
        for (auto &[x, y] : body) {
            empty[x][y] = false;
        }
        return empty;
    }

    // default constructor
    Game() {
        snake = Snake();
        board = Board();
        board.makeBoard();
        snake.printSnake();
        // generating foodAmount number of random food coordinates in foodList
        auto empty = emptyCells(snake, board);
        foodAmount = FOOD_AMOUNT;
        for (int i = 0; i < foodAmount; i++) {
            Food newfood(empty);
            foodList.push_back(newfood);
        }
        score = 0;
    }


    // construct game using existing components
    Game(const Snake &newSnake, const Board &newBoard) {
        snake = newSnake;
        board = newBoard;
        board.makeBoard();
        snake.printSnake();

        // generating foodAmount number of random food coordinates in foodList
        auto empty = emptyCells(newSnake, newBoard);
        foodAmount = FOOD_AMOUNT;
        for (int i = 0; i < foodAmount; i++) {
            Food newfood(empty);
            foodList.push_back(newfood);
        }

        score = 0;
    }
    
    // remove food when snake eats it
    void removeFood(int x, int y) {
        for (int i = 0; i < foodList.size(); i++) {
            Food &food = foodList[i];
            if (x == food.getx() && y == food.gety()) {
                foodList.erase(foodList.begin() + i);
                break;
            }
        }
    }
    
    // add new food element when snake eats existing food element
    void updateFood() {
        auto empty = emptyCells(snake, board);
        for (Food &food : foodList) {
            empty[food.gety()][food.getx()] = false;
        }
        Food newfood(empty);
        foodList.push_back(newfood);
    }

    // check if head of some snake coincides with some food element
    bool foodEaten() {
        int headx = snake.getHead().second, heady = snake.getHead().first;
        for (Food &food : foodList) {
            if (food.getx() == headx && food.gety() == heady) {
                score++;
                return true;
            }
        }
        return false;

    }

    // get the score
    int getScore() {
        return score;
    }

    // display the score
    void displayScore() {
        mvprintw(0, 0, "SCORE: %d", score);
        refresh();
    }

    // print new snake on the board along with the new food
    void updateGame() {
        clear();
        board.makeBoard();
        snake.printSnake();
        for (Food &food : foodList) {
            food.printFood();
        }
        refresh();
    }

    // find turning direction of snake from user input
    char handleInput(int ch) {
        if (ch == ERR) return 'F';

        else if (ch == KEY_UP || ch == 'W' || ch == 'w') {
            if (snake.headDirection() == 'U' || snake.headDirection() == 'D') return 'F';
            else if (snake.headDirection() == 'R') return 'L';
            else return 'R';
        }

        else if (ch == KEY_DOWN || ch == 'S' || ch == 's') {
            if (snake.headDirection() == 'U' || snake.headDirection() == 'D') return 'F';
            else if (snake.headDirection() == 'R') return 'R';
            else return 'L';
        }

        else if (ch == KEY_RIGHT || ch == 'D' || ch == 'd') {
            if (snake.headDirection() == 'R' || snake.headDirection() == 'L') return 'F';
            else if (snake.headDirection() == 'U') return 'R';
            else return 'L';
        }
        
        else if (ch == KEY_LEFT || ch == 'A' || ch == 'a') {
            if (snake.headDirection() == 'R' || snake.headDirection() == 'L') return 'F';
            else if (snake.headDirection() == 'U') return 'L';
            else return 'R';
        }
        else return 'F';
    }

    // update snake based on character input from user and also handle eating of food
    void update(char move) {
        if (move == 'F') snake.moveForward();
        else if (move == 'R') snake.moveRight();
        else if (move == 'L') snake.moveLeft();

        if (foodEaten()) {
            removeFood(snake.getHead().second, snake.getHead().first);
            updateFood();
            snake.extendSnake();
        }

        updateGame();
    }

    // condition for snake to die
    bool isSnakeDead() {
        pair<int, int> head = snake.getHead();
        if (head.first > board.getHeight() || head.second >= board.getWidth() || head.first <= 0 || head.second < 0) return true;
        else {
            deque<pair<int, int>> body = snake.getSnakeBody();
            for (int i = 1; i < snake.getLength(); i++) {
                if (body[i] == head) return true;
            }
        }
        return false;
    }
};

int main() {

    initscr();
    keypad(stdscr, TRUE);
    noecho();
    curs_set(0);
    
    // start menu
    mvprintw(0, 0, "========= THE SNAKE GAME =========");
    mvprintw(1, 0, "START GAME? (Press any key)");
    mvprintw(2, 0, "---------------------------");
    mvprintw(3, 0, "QUIT(q)");
    refresh();
    char start = getch();
    if (start == 'q' || start == 'Q') return 0;

    bool backTomenu = false;
    while(true) {
        // select difficulty menu
        clear();
        mvprintw(0, 0, "SELECT GAME DIFFICULTY:");
        mvprintw(1, 0, "---------------------------");
        mvprintw(2, 0, "EASY (e)");
        mvprintw(3, 0, "MEDIUM (m)");
        mvprintw(4, 0, "HARD (h)");
        mvprintw(5, 0, "---------------------------");
        mvprintw(6, 0, "QUIT(q)");
    
        int difficulty = 0;
        int c = 0;
        while(true) {
            char diff = getch();
            if (diff == 'e' || diff == 'E') {
                difficulty = 0;
                break;
            }
            else if (diff == 'm' || diff == 'M') {
                difficulty = 1;
                break;
            }
            else if (diff == 'h' || diff == 'H') {
                difficulty = 2;
                break;
            }
            else if (diff == 'q' || diff == 'Q') {
                difficulty = -1;
                break;
            }
            else {
                mvprintw(7+c, 0, "INVALID INPUT");
            }
        }
        if (difficulty == -1) break;
        TIME_DELAY = 150000 - 50000*(difficulty);
        clear();
    
        // main game loop
        while(true) {
            // initialize game
            Game game;
            
            // apply no delay for real-time input
            nodelay(stdscr, TRUE);
    
            int ch;
            // play until game over
            while(!game.isSnakeDead()) {
                ch = getch();
                game.update(game.handleInput(ch));
                game.displayScore();
                usleep(TIME_DELAY);
            }
            clear();
    
            // game over screen
            nodelay(stdscr, FALSE);
            mvprintw(0, 0, "========= GAME OVER =========");
            mvprintw(1, 0,"YOUR SCORE IS: %d", game.getScore());
            mvprintw(2, 0, "---------------------------");
            mvprintw(3, 0, "PLAY AGAIN? (Press ENTER)");
            mvprintw(4, 0, "CHANGE DIFFICULTY? (Press c)");
            mvprintw(5, 0, "QUIT(q)");
            refresh();
    
            bool restart = false;
            int i = 0;
            while(true) {
                char end = getch();
                if (end == '\r' || end == '\n' || end == KEY_ENTER) {
                    restart = true;
                    backTomenu = false;
                    break;
                }
                else if (end == 'c' || end == 'C') {
                    restart = false;
                    backTomenu = true;
                    break;
                }
                else if (end == 'q' || end == 'Q') {
                    restart = false;
                    backTomenu = false;
                    break;
                }
                else {
                    mvprintw(7+i, 0, "INVALID INPUT!");
                    i++;
                }
            }
            if (restart) continue;
            else break;
        }
        if (backTomenu) continue;
        else break;
    }
    endwin();
    return 0;
}