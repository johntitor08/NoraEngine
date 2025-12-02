#include <iostream>
#include <SDL2/SDL.h>
#include <cstdlib>
#include <ctime>

using namespace std;

const int SCREEN_WIDTH = 800;
const int SCREEN_HEIGHT = 600;
const int FPS = 60;
const int frameDelay = 1000 / FPS;

// Player settings
const int PLAYER_SIZE = 40;
const int PLAYER_SPEED = 5;

// Target settings
const int TARGET_SIZE = 30;

int main(int argc, char* argv[])
{
    srand(time(nullptr));

    // --- Initialize SDL ---
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        cerr << "SDL could not initialize! SDL_Error: " << SDL_GetError() << endl;
        return 1;
    }

    SDL_Window* window = SDL_CreateWindow(
        "Mini SDL2 Game",
        SDL_WINDOWPOS_CENTERED,
        SDL_WINDOWPOS_CENTERED,
        SCREEN_WIDTH,
        SCREEN_HEIGHT,
        SDL_WINDOW_SHOWN
    );

    if (!window) {
        cerr << "Window could not be created! SDL_Error: " << SDL_GetError() << endl;
        SDL_Quit();
        return 1;
    }

    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (!renderer) {
        cerr << "Renderer could not be created! SDL_Error: " << SDL_GetError() << endl;
        SDL_DestroyWindow(window);
        SDL_Quit();
        return 1;
    }

    bool isRunning = true;
    SDL_Event event;

    // Player rectangle
    SDL_Rect player = { SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, PLAYER_SIZE, PLAYER_SIZE };

    // Target rectangle
    SDL_Rect target = {
        rand() % (SCREEN_WIDTH - TARGET_SIZE),
        rand() % (SCREEN_HEIGHT - TARGET_SIZE),
        TARGET_SIZE,
        TARGET_SIZE
    };

    int score = 0;

    Uint32 frameStart;
    int frameTime;

    // --- GAME LOOP ---
    while (isRunning) {
        frameStart = SDL_GetTicks();

        // --- INPUT ---
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT)
                isRunning = false;
        }

        const Uint8* keystates = SDL_GetKeyboardState(NULL);

        // Movement
        if (keystates[SDL_SCANCODE_W]) player.y -= PLAYER_SPEED;
        if (keystates[SDL_SCANCODE_S]) player.y += PLAYER_SPEED;
        if (keystates[SDL_SCANCODE_A]) player.x -= PLAYER_SPEED;
        if (keystates[SDL_SCANCODE_D]) player.x += PLAYER_SPEED;

        // Clamp to screen (so the player cannot leave)
        if (player.x < 0) player.x = 0;
        if (player.y < 0) player.y = 0;
        if (player.x + PLAYER_SIZE > SCREEN_WIDTH) player.x = SCREEN_WIDTH - PLAYER_SIZE;
        if (player.y + PLAYER_SIZE > SCREEN_HEIGHT) player.y = SCREEN_HEIGHT - PLAYER_SIZE;

        // --- COLLISION CHECK ---
        if (SDL_HasIntersection(&player, &target)) {
            score++;
            cout << "Score: " << score << endl;

            // Move target to new random position
            target.x = rand() % (SCREEN_WIDTH - TARGET_SIZE);
            target.y = rand() % (SCREEN_HEIGHT - TARGET_SIZE);
        }

        // --- RENDER ---
        SDL_SetRenderDrawColor(renderer, 20, 20, 20, 255);
        SDL_RenderClear(renderer);

        // Draw player (blue)
        SDL_SetRenderDrawColor(renderer, 50, 150, 255, 255);
        SDL_RenderFillRect(renderer, &player);

        // Draw target (red)
        SDL_SetRenderDrawColor(renderer, 255, 60, 60, 255);
        SDL_RenderFillRect(renderer, &target);

        SDL_RenderPresent(renderer);

        // --- FPS LIMITER ---
        frameTime = SDL_GetTicks() - frameStart;
        if (frameDelay > frameTime) {
            SDL_Delay(frameDelay - frameTime);
        }
    }

    // --- CLEANUP ---
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}
