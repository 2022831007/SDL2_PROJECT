#include <SDL2/SDL.h>
#include <iostream>
#include<cmath>

const int SCREEN_WIDTH = 700;
const int SCREEN_HEIGHT = 400;
const int CIRCLE_RADIUS = 20;
const int CIRCLE_SPEED = 50;

SDL_Window* window = nullptr;
SDL_Renderer* renderer = nullptr;

// Circle positions
int movingCircleX = 0;
int movingCircleY = SCREEN_HEIGHT / 2;
int controlledCircleX = SCREEN_WIDTH / 2;
int controlledCircleY = CIRCLE_RADIUS;

bool initSDL() {
    // Initialize SDL
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        std::cerr << "SDL could not initialize! SDL_Error: " << SDL_GetError() << std::endl;
        return false;
    }

    // Create window
    window = SDL_CreateWindow("Created by Fatema", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
    if (window == nullptr) {
        std::cerr << "Window could not be created! SDL_Error: " << SDL_GetError() << std::endl;
        return false;
    }

    // Create renderer
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (renderer == nullptr) {
        std::cerr << "Renderer could not be created! SDL_Error: " << SDL_GetError() << std::endl;
        return false;
    }

    return true;
}

void closeSDL() {
    // Destroy renderer
    SDL_DestroyRenderer(renderer);
    renderer = nullptr;

    // Destroy window
    SDL_DestroyWindow(window);
    window = nullptr;

    // Quit SDL subsystems
    SDL_Quit();
}

void drawCircle(int x, int y, int radius, Uint8 r, Uint8 g, Uint8 b) {
    SDL_SetRenderDrawColor(renderer, r, g, b, SDL_ALPHA_OPAQUE);
    for (int w = -radius; w <= radius; ++w) {
        for (int h = -radius; h <= radius; ++h) {
            if (w * w + h * h <= radius * radius) {
                SDL_RenderDrawPoint(renderer, x + w, y + h);
            }
        }
    }
}

bool checkCollision(int x1, int y1, int x2, int y2, int radius) {
    int dx = x2 - x1;
    int dy = y2 - y1;
    int distanceSquared = dx * dx + dy * dy;
    int combinedRadius = 2 * radius; // Diameter of the circle

    return distanceSquared <= combinedRadius * combinedRadius;
}

int main(int argc, char* args[]) {
    if (!initSDL()) {
        std::cerr << "Failed to initialize SDL!" << std::endl;
        return 1;
    }

    bool quit = false;
    SDL_Event e;

    while (!quit) {
        while (SDL_PollEvent(&e) != 0) {
            if (e.type == SDL_QUIT) {
                quit = true;
            }
        }

        // Move moving circle continuously from left to right
        movingCircleX += CIRCLE_SPEED;
        if (movingCircleX > SCREEN_WIDTH + CIRCLE_RADIUS) {
            movingCircleX = -CIRCLE_RADIUS;
        }

        // Handle keyboard input for controlled circle
        const Uint8* currentKeyStates = SDL_GetKeyboardState(NULL);
        if (currentKeyStates[SDL_SCANCODE_UP]) {
            controlledCircleY -= CIRCLE_SPEED;
        }
        if (currentKeyStates[SDL_SCANCODE_DOWN]) {
            controlledCircleY += CIRCLE_SPEED;
        }
        if (currentKeyStates[SDL_SCANCODE_LEFT]) {
            controlledCircleX -= CIRCLE_SPEED;
        }
        if (currentKeyStates[SDL_SCANCODE_RIGHT]) {
            controlledCircleX += CIRCLE_SPEED;
        }

        // Clear the screen
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, SDL_ALPHA_OPAQUE);
        SDL_RenderClear(renderer);

        // Draw moving circle
        drawCircle(movingCircleX, movingCircleY, CIRCLE_RADIUS, 255, 0, 0);

        // Draw controlled circle
        drawCircle(controlledCircleX, controlledCircleY, CIRCLE_RADIUS, 0, 0, 255);

        // Check collision
        bool collision = checkCollision(movingCircleX, movingCircleY, controlledCircleX, controlledCircleY, CIRCLE_RADIUS);

        // Apply visual effect for collision
        if (collision) {
            // For example, change the color of the controlled circle
            drawCircle(controlledCircleX, controlledCircleY, CIRCLE_RADIUS, 255, 255, 0);
        }

        // Render
        SDL_RenderPresent(renderer);
    }

    closeSDL();

    return 0;
}
