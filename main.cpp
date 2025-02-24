#define SDL_MAIN_HANDLED
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <algorithm>
#include <string>
#include <iostream>

// Game constants
const int GRID_SIZE     = 40;
const int SCREEN_WIDTH  = 800;
const int SCREEN_HEIGHT = 600;
const int GRID_WIDTH    = SCREEN_WIDTH / GRID_SIZE;
const int GRID_HEIGHT   = SCREEN_HEIGHT / GRID_SIZE;
const int TARGET_FPS    = 60;
const int FRAME_DELAY   = 1000 / TARGET_FPS;
const int SNAKE_UPDATE_DELAY = 15; // Update snake every 15 frames
const int MAX_PARTICLES = 200;
const int SPLASH_SCREEN_DURATION = 3000; // Duration in milliseconds

// Directions
enum Direction { UP, DOWN, LEFT, RIGHT };

// Simple point structure for grid positions
struct Point {
    int x, y;
    bool operator==(const Point& other) const {
        return x == other.x && y == other.y;
    }
};

// Particle structure with active flag
struct Particle {
    bool active = false;
    float x, y;     // position
    float vx, vy;   // velocity (pixels per second)
    float lifetime; // total time it should live
    float age;      // how long it has existed
};

class SnakeGame {
private:
    SDL_Window* window;
    SDL_Renderer* renderer;
    TTF_Font* font;

    // Game state
    bool running;
    bool gameOver;
    Direction direction;
    std::vector<Point> snake;
    Point food;
    int frameCounter;
    int score;

    // Particle system
    std::vector<Particle> particles;
    SDL_Texture* scoreTexture = nullptr;
    SDL_Texture* gameOverTexture = nullptr;
    SDL_Texture* splashTexture = nullptr;
    int lastScore = -1;
    bool gameOverStateChanged = true;

    SDL_Texture* CreateTextTexture(const std::string& text, SDL_Color color) {
        SDL_Surface* surface = TTF_RenderText_Solid(font, text.c_str(), color);
        if (!surface) return nullptr;
        SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
        SDL_FreeSurface(surface);
        return texture;
    }

    void resetGame() {
        snake.clear();
        snake.push_back({GRID_WIDTH / 2, GRID_HEIGHT / 2});
        spawnFood();
        score = 0;
        gameOver = false;
        direction = RIGHT;
        particles.clear();
        frameCounter = 0;
        gameOverStateChanged = true;
        lastScore = -1;
        if (scoreTexture) SDL_DestroyTexture(scoreTexture);
        if (gameOverTexture) SDL_DestroyTexture(gameOverTexture);
        scoreTexture = nullptr;
        gameOverTexture = nullptr;
    }

    void spawnFood() {
        std::srand(std::time(nullptr));
        do {
            food = { std::rand() % GRID_WIDTH, std::rand() % GRID_HEIGHT };
        } while (std::find(snake.begin(), snake.end(), food) != snake.end());
    }

    void addParticles(int count, float x, float y) {
        for (int i = 0; i < count; ++i) {
            if (particles.size() < MAX_PARTICLES) {
                Particle p;
                p.x = x;
                p.y = y;
                p.vx = (std::rand() % 100 - 50) * 6.0f;
                p.vy = (std::rand() % 100 - 50) * 6.0f;
                p.lifetime = 1.0f;
                p.age = 0.0f;
                p.active = true;
                particles.push_back(p);
            }
        }
    }

public:
    SnakeGame()
        : window(nullptr), renderer(nullptr), font(nullptr),
          running(true), gameOver(false), direction(RIGHT),
          frameCounter(0), score(0)
    {
        if (SDL_Init(SDL_INIT_VIDEO) != 0 || TTF_Init() != 0) return;

        window = SDL_CreateWindow("Snake Game", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
                                  SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
        if (!window) return;

        renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
        if (!renderer) return;
        SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);

        font = TTF_OpenFont("assets/arial.ttf", 24);
        if (!font) return;

        particles.reserve(MAX_PARTICLES);
        resetGame();

        // Create splash screen texture
        SDL_Color color = {255, 255, 255, 255};
        splashTexture = CreateTextTexture("Welcome to Snake Game!\nWritten by Abdullah Al Siam", color);
    }

    ~SnakeGame() {
        if (scoreTexture) SDL_DestroyTexture(scoreTexture);
        if (gameOverTexture) SDL_DestroyTexture(gameOverTexture);
        if (splashTexture) SDL_DestroyTexture(splashTexture);
        TTF_CloseFont(font);
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        TTF_Quit();
        SDL_Quit();
    }

    void handleInput() {
        SDL_Event e;
        while (SDL_PollEvent(&e)) {
            if (e.type == SDL_QUIT) running = false;
            else if (e.type == SDL_KEYDOWN) {
                if (gameOver && e.key.keysym.sym == SDLK_r) resetGame();
                else if (!gameOver) {
                    switch (e.key.keysym.sym) {
                        case SDLK_UP: if (direction != DOWN) direction = UP; break;
                        case SDLK_DOWN: if (direction != UP) direction = DOWN; break;
                        case SDLK_LEFT: if (direction != RIGHT) direction = LEFT; break;
                        case SDLK_RIGHT: if (direction != LEFT) direction = RIGHT; break;
                    }
                }
            }
        }
    }

    void update(float deltaTime) {
        if (gameOver) return;

        frameCounter++;
        if (frameCounter < SNAKE_UPDATE_DELAY) return;
        frameCounter = 0;

        Point newHead = snake.front();
        switch (direction) {
            case UP: newHead.y--; break;
            case DOWN: newHead.y++; break;
            case LEFT: newHead.x--; break;
            case RIGHT: newHead.x++; break;
        }

        if (newHead.x < 0 || newHead.y < 0 || newHead.x >= GRID_WIDTH || newHead.y >= GRID_HEIGHT ||
            std::find(snake.begin(), snake.end(), newHead) != snake.end()) {
            gameOver = true;
            gameOverStateChanged = true;
            return;
        }

        snake.insert(snake.begin(), newHead);
        if (newHead == food) {
            score += 10;
            spawnFood();
            addParticles(20, food.x * GRID_SIZE + GRID_SIZE/2.0f,
                            food.y * GRID_SIZE + GRID_SIZE/2.0f);
        } else {
            snake.pop_back();
        }

        // Update particles
        for (auto& p : particles) {
            p.x += p.vx * deltaTime;
            p.y += p.vy * deltaTime;
            p.age += deltaTime;
            if (p.age >= p.lifetime) p.active = false;
        }

        // Remove inactive particles
        particles.erase(std::remove_if(particles.begin(), particles.end(),
            [](const Particle& p) { return !p.active; }), particles.end());
    }

    void render() {
        SDL_SetRenderDrawColor(renderer, 25, 25, 35, 255);
        SDL_RenderClear(renderer);

        // Draw grid lines
        SDL_SetRenderDrawColor(renderer, 40, 40, 50, 50);
        for (int x = 0; x <= SCREEN_WIDTH; x += GRID_SIZE)
            SDL_RenderDrawLine(renderer, x, 0, x, SCREEN_HEIGHT);
        for (int y = 0; y <= SCREEN_HEIGHT; y += GRID_SIZE)
            SDL_RenderDrawLine(renderer, 0, y, SCREEN_WIDTH, y);

        // Batch draw snake
        SDL_SetRenderDrawColor(renderer, 100, 150, 255, 255);
        std::vector<SDL_Rect> snakeRects;
        snakeRects.reserve(snake.size());
        for (const auto& seg : snake)
            snakeRects.push_back({seg.x * GRID_SIZE, seg.y * GRID_SIZE, GRID_SIZE, GRID_SIZE});
        SDL_RenderFillRects(renderer, snakeRects.data(), snakeRects.size());

        // Draw food
        SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
        SDL_Rect foodRect = {food.x * GRID_SIZE, food.y * GRID_SIZE, GRID_SIZE, GRID_SIZE};
        SDL_RenderFillRect(renderer, &foodRect);

        // Draw particles
        for (auto& p : particles) {
            float alpha = (1.0f - p.age / p.lifetime) * 255;
            SDL_SetRenderDrawColor(renderer, 255, 255, 0, (Uint8)alpha);
            SDL_Rect rect = {(int)p.x - 2, (int)p.y - 2, 4, 4};
            SDL_RenderFillRect(renderer, &rect);
        }

        // Update cached score texture if needed
        if (score != lastScore || !scoreTexture) {
            if (scoreTexture) SDL_DestroyTexture(scoreTexture);
            SDL_Color color = {255, 255, 255, 255};
            scoreTexture = CreateTextTexture("Score: " + std::to_string(score), color);
            lastScore = score;
        }

        if (scoreTexture) {
            int w, h;
            SDL_QueryTexture(scoreTexture, nullptr, nullptr, &w, &h);
            SDL_Rect rect = {10, 10, w, h};
            SDL_RenderCopy(renderer, scoreTexture, nullptr, &rect);
        }

        if (gameOver) {
            if (gameOverStateChanged) {
                if (gameOverTexture) SDL_DestroyTexture(gameOverTexture);
                SDL_Color color = {255, 50, 50, 255};
                gameOverTexture = CreateTextTexture("Game Over! Press R to restart", color);
                gameOverStateChanged = false;
            }
            if (gameOverTexture) {
                int w, h;
                SDL_QueryTexture(gameOverTexture, nullptr, nullptr, &w, &h);
                SDL_Rect rect = {(SCREEN_WIDTH - w)/2, (SCREEN_HEIGHT - h)/2, w, h};
                SDL_RenderCopy(renderer, gameOverTexture, nullptr, &rect);
            }
        }

        SDL_RenderPresent(renderer);
    }

    void showSplashScreen() {
        Uint32 startTime = SDL_GetTicks();
        while (SDL_GetTicks() - startTime < SPLASH_SCREEN_DURATION) {
            SDL_Event e;
            while (SDL_PollEvent(&e)) {
                if (e.type == SDL_QUIT) running = false;
            }

            SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
            SDL_RenderClear(renderer);

            if (splashTexture) {
                int w, h;
                SDL_QueryTexture(splashTexture, nullptr, nullptr, &w, &h);
                SDL_Rect rect = {(SCREEN_WIDTH - w)/2, (SCREEN_HEIGHT - h)/2, w, h};
                SDL_RenderCopy(renderer, splashTexture, nullptr, &rect);
            }

            SDL_RenderPresent(renderer);
            SDL_Delay(100);
        }
    }

    void run() {
        showSplashScreen();

        Uint32 frameStart, frameTime;
        Uint32 lastTime = SDL_GetTicks();
        while (running) {
            frameStart = SDL_GetTicks();
            float deltaTime = (frameStart - lastTime) / 1000.0f;
            lastTime = frameStart;

            handleInput();
            update(deltaTime);
            render();

            frameTime = SDL_GetTicks() - frameStart;
            if (frameTime < FRAME_DELAY) SDL_Delay(FRAME_DELAY - frameTime);
        }
    }
};

int main(int argc, char* argv[]) {
    SnakeGame game;
    game.run();
    return 0;
}
