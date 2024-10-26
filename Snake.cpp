
// Snake.cpp 源文件
#include "Snake.h"
#include <iostream>
#include <cstdlib>
#include <time.h>
#include <windows.h>

using namespace SnakeGame;

Snake::Snake(int width, int height) : width(width), height(height), currentDirection(RIGHT) {
    Position startPos = { width / 2, height / 2 };
    body.push(startPos);
    for (int i = 1; i <= 3; ++i) {
        Position newPos = { startPos.x - i, startPos.y };
        body.push(newPos);
    }
}

void Snake::Move() {
    Position head = body.front();
    Position new_head = head;
    switch (currentDirection) {
    case UP: new_head.y--; break;
    case DOWN: new_head.y++; break;
    case LEFT: new_head.x--; break;
    case RIGHT: new_head.x++; break;
    }

    body.push(new_head);
    body.pop();  // 删除尾部
}

void Snake::ChangeDirection(Direction new_dir) {
    if ((currentDirection == UP && new_dir != DOWN) ||
        (currentDirection == DOWN && new_dir != UP) ||
        (currentDirection == LEFT && new_dir != RIGHT) ||
        (currentDirection == RIGHT && new_dir != LEFT)) {
        currentDirection = new_dir;
    }
}

bool Snake::CheckCollision() const {
    Position head = body.front();
    if (head.x <= 0 || head.x >= width - 1 || head.y <= 0 || head.y >= height - 1) {
        return true;
    }
    std::queue<Position> tempQueue = body;
    tempQueue.pop();  // Skip the head
    while (!tempQueue.empty()) {
        Position pos = tempQueue.front();
        tempQueue.pop();
        if (pos.x == head.x && pos.y == head.y) {
            return true;
        }
    }
    return false;
}

bool Snake::EatFood(const Position& food_pos) {
    Position head = body.front();
    if (head.x == food_pos.x && head.y == food_pos.y) {
        return true;
    }
    return false;
}

void Snake::DrawSnake() const {
    setfillcolor(GREEN);
    std::queue<Position> tempQueue = body;
    while (!tempQueue.empty()) {
        Position pos = tempQueue.front();
        tempQueue.pop();
        fillrectangle(pos.x * 20, pos.y * 20, (pos.x + 1) * 20, (pos.y + 1) * 20);
    }
}

std::queue<Position> Snake::getBody() const {
    return body;
}



GameEngine::GameEngine(int width, int height) : Snake(width, height), score(0) {
    srand(static_cast<unsigned int>(time(NULL)));
    srand(time(NULL));
    food = GenerateFood();
}

Position GameEngine::GenerateFood() {
    Position newFood;
    bool validPosition = false;
    while (!validPosition) {
        newFood.x = rand() % (width - 2) + 1;
        newFood.y = rand() % (height - 2) + 1;
        validPosition = true;

        std::queue<Position> tempQueue = getBody();
        while (!tempQueue.empty()) {
            Position pos = tempQueue.front();
            tempQueue.pop();
            if (newFood.x == pos.x && newFood.y == pos.y) {
                validPosition = false;
                break;
            }
        }
    }
    return newFood;
}

void GameEngine::DrawFood() {
    setfillcolor(BLUE);
    fillrectangle(food.x * 20, food.y * 20, (food.x + 1) * 20, (food.y + 1) * 20);
}

void GameEngine::DrawScore() {
    wchar_t score_str[100];
    _itow_s(score, score_str, 10);
    settextstyle(20, 0, _T("宋体"));
    outtextxy((width / 2) - 40, height + 10, _T("SCORE:"));
    outtextxy((width / 2) + 30, height + 10, score_str);
}

void GameEngine::GetUserInput() {
    if (GetAsyncKeyState(VK_UP) & 0x8000) {
        ChangeDirection(UP);
    }
    if (GetAsyncKeyState(VK_DOWN) & 0x8000) {
        ChangeDirection(DOWN);
    }
    if (GetAsyncKeyState(VK_LEFT) & 0x8000) {
        ChangeDirection(LEFT);
    }
    if (GetAsyncKeyState(VK_RIGHT) & 0x8000) {
        ChangeDirection(RIGHT);
    }
}

void GameEngine::Run() {
    initgraph(width * 20, height * 20 + 40);
    while (true) {
        GetUserInput();
        bool foodEaten = EatFood(food);
        if (foodEaten) {
            score++;
            food = GenerateFood();
        }
        Move();
        if (CheckCollision()) {
            std::cout << "Game Over! Your score: " << score << std::endl;
            break;
        }
        cleardevice();
        DrawSnake();
        DrawFood();
        DrawScore();
        Sleep(300);
    }
    closegraph();
}

int main()
{
    int width = 20;
    int height = 20;
    GameEngine game(width, height);
    game.Run();
    return 0;
}
