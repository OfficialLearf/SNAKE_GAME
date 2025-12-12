#include <iostream>
#include <raylib.h>
#include <deque>
#include <algorithm>
#include <ranges>
#include <random>
#include <raymath.h>
using namespace std;

Color green = {173, 204, 96, 255};
Color darkgreen = {43, 51, 24, 255};

int cellSize = 30;
int cellCount = 25;
float offset = 50;

double lastUpdateTime = 0;

bool eventTriggered(const double interval) {
    if (const double currentTime = GetTime(); currentTime - lastUpdateTime >= interval) {
        lastUpdateTime = currentTime;
        return true;
    }
    return false;
}

class Snake {
private:
    deque<Vector2> body{{5, 10}, {4, 10}, {3, 10}};
    Vector2 direction{1, 0};
public:
    void Draw() const {
        for (const auto&[x, y] : body) {
            DrawRectangleRounded({x * static_cast<float>(cellSize)+offset, y * static_cast<float>(cellSize)+offset, static_cast<float>(cellSize), static_cast<float>(cellSize)}, 0.5, 6, darkgreen);
        }
    }
    const deque<Vector2>& getBody() {
        return body;
    }
    void Update(const bool eat) {
        body.push_front(Vector2Add(body.front(),direction));
        if (!eat) {
            body.pop_back();
        }
    }

    [[nodiscard]] Vector2 getDirection() const {
        return direction;
    }

    void setDirection(const Vector2 vector2) {
        direction = vector2;
    }

    void Reset() {
        body = {{5, 10}, {4, 10}, {3, 10}};
        direction = {1, 0};
    }
};

class Food {
private:
    Vector2 position{};
    Texture2D texture{};
public:
    Food(){
        const Image image = LoadImage("/SNAKE_GAME/food.png");
        texture = LoadTextureFromImage(image);
        UnloadImage(image);
        position = GenerateRandomPosition();
    }
    ~Food(){
        UnloadTexture(texture);
    }
    void Draw() const {
        DrawTexture(
        texture,
        static_cast<int>(position.x * static_cast<float>(cellSize) + offset),
        static_cast<int>(position.y * static_cast<float>(cellSize) + offset),
        WHITE
    );
    };
    [[nodiscard]] Vector2 getPosition() const {
        return position;
    }
    static Vector2 RandomCell() {
        return Vector2{static_cast<float>(GetRandomValue(0, (cellCount) - 1)), static_cast<float>(GetRandomValue(0, (cellCount) - 1))};
    }

    Vector2 GenerateRandomPosition(const deque<Vector2>& occupiedPositions = {}) {
        Vector2 newPos = RandomCell();
        while (ranges::find(occupiedPositions, newPos) != occupiedPositions.end()) {
            newPos = RandomCell();
        }
        position = newPos;
        return position;
    }
 };

class Game {
private:
    Snake snake = Snake();
    Food food = Food();
    bool running = false;
    int score {0};
public:
    void Draw() const {
        food.Draw();
        snake.Draw();
    }
    void Update() {
        if (!running) return;
        CheckBodyCollision();
        CheckWallCollision();
        if (CheckCollision()) {
            snake.Update(true);
        }
        snake.Update(false);
    }
    bool CheckCollision() {
        if (Vector2Equals(snake.getBody().front(),food.getPosition())) {
            food.GenerateRandomPosition(snake.getBody());
            score++;
            return true;
        }
        return false;
    }
    void CheckWallCollision() {
        if (snake.getBody().front().x == static_cast<float>(cellCount) || snake.getBody().front().x == -1 ||
            snake.getBody().front().y == -1 || snake.getBody().front().y == static_cast<float>(cellCount)) {
            GameOver();
        }
    }
    void CheckBodyCollision() {
        const Vector2 newPosition = Vector2Add(snake.getBody().front(),snake.getDirection());
        const auto& body = snake.getBody();
        if (body.size() < 5) return;
        if (auto innerBodyView = body | std::views::drop(1) | std::views::take(body.size() - 2); std::ranges::find(innerBodyView, newPosition) != innerBodyView.end()) {
            GameOver();
        }
    }
    void GameOver() {
        snake.Reset();
        food.GenerateRandomPosition();
        running = false;
        score = 0;
    }
    [[nodiscard]] int getScore() const {
        return score;
    }
    void HandleInput() {
        Vector2 tentative = snake.getDirection();
        int key;
        while ((key = GetKeyPressed()) != 0) {
            switch (key) {
                case KEY_RIGHT: case KEY_D: tentative = {1, 0}; running = true; break;
                case KEY_LEFT:  case KEY_A: tentative = {-1, 0}; running = true;break;
                case KEY_UP:    case KEY_W: tentative = {0, -1}; running = true;break;
                case KEY_DOWN:  case KEY_S: tentative = {0, 1}; running = true; break;
                default: break;
            }
        }
        if (!(tentative.x == -snake.getDirection().x && tentative.y == -snake.getDirection().y)) {
            snake.setDirection(tentative);
        }
    }
};
int main() {
    const int windowSize = static_cast<int>(static_cast<float>(cellSize)*static_cast<float>(cellCount)+(2*offset));
    InitWindow(windowSize, windowSize, "Retro Snake");
    SetTargetFPS(60);
    auto game = Game();
    while (!WindowShouldClose()) {
        BeginDrawing();
        game.HandleInput();
        if (eventTriggered(0.2)) {
            game.Update();
        }
        ClearBackground(green);
        DrawRectangleLinesEx(Rectangle{offset-5,offset-5,static_cast<float>(cellSize*cellCount)+10,static_cast<float>(cellSize*cellCount)+10},5,DARKGREEN);
        DrawText("Retro Snake",10,10,20,DARKGREEN);
        DrawText(TextFormat("%i",game.getScore()),windowSize - 30,10,20,DARKGREEN);
        game.Draw();
        EndDrawing();
    }

    CloseWindow();
    return 0;
}

