// Snake.h 头文件
#ifndef SNAKE_H
#define SNAKE_H

#include <queue>
#include <vector>
#include <easyx.h>

namespace SnakeGame
{
    enum Direction { UP, DOWN, LEFT, RIGHT };

    struct Position
    {
        int x;
        int y;
    };

    class Snake
    {
    public:
        Snake(int width, int height);
        void Move();
        void ChangeDirection(Direction new_dir);
        bool CheckCollision() const;
        bool EatFood(const Position& food_pos);
        void DrawSnake() const;
        std::queue<Position> getBody() const;

    protected:
        int width;
        int height;

    private:
        Direction currentDirection;
        std::queue<Position> body;
    };

    class GameEngine : public Snake
    {
    public:
        GameEngine(int width, int height);
        void Run();

    private:
        Position GenerateFood();
        void DrawFood();
        void DrawScore();
        void GetUserInput();
        Position food;
        int score;
    };
}

#endif
