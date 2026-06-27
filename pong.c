#include <SDL2/SDL.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define WIDTH  640
#define HEIGHT 480
#define PADDLE_W 80
#define PADDLE_H 10
#define BALL_SIZE 10
#define PLAYER_SPEED 5
#define BALL_SPEED 3

typedef struct {
    int x, y, w, h, dx, dy;
} Entity;

Entity player, ai, ball;
int player_score = 0, ai_score = 0;

void init_game() {
    player.x = WIDTH/2 - PADDLE_W/2;
    player.y = HEIGHT - 30;
    player.w = PADDLE_W;
    player.h = PADDLE_H;

    ai.x = WIDTH/2 - PADDLE_W/2;
    ai.y = 20;
    ai.w = PADDLE_W;
    ai.h = PADDLE_H;

    ball.x = WIDTH/2 - BALL_SIZE/2;
    ball.y = HEIGHT/2 - BALL_SIZE/2;
    ball.w = BALL_SIZE;
    ball.h = BALL_SIZE;

    srand(time(NULL));
    ball.dx = (rand() % 2 == 0) ? BALL_SPEED : -BALL_SPEED;
    ball.dy = (rand() % 2 == 0) ? BALL_SPEED : -BALL_SPEED;
}

void draw(SDL_Renderer *renderer) {
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);

    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);

    SDL_Rect r1 = {player.x, player.y, player.w, player.h};
    SDL_RenderFillRect(renderer, &r1);

    SDL_Rect r2 = {ai.x, ai.y, ai.w, ai.h};
    SDL_RenderFillRect(renderer, &r2);

    SDL_Rect r3 = {ball.x, ball.y, ball.w, ball.h};
    SDL_RenderFillRect(renderer, &r3);

    SDL_RenderPresent(renderer);
}

void update() {
    ball.x += ball.dx;
    ball.y += ball.dy;

    if (ball.x <= 0 || ball.x + ball.w >= WIDTH) {
        ball.dx = -ball.dx;
    }

    if (ball.y + ball.h >= player.y &&
        ball.y < player.y + player.h &&
        ball.x + ball.w > player.x &&
        ball.x < player.x + player.w) {
        ball.dy = -ball.dy;
        ball.y = player.y - ball.h - 1;
    }

    if (ball.y <= ai.y + ai.h &&
        ball.y + ball.h > ai.y &&
        ball.x + ball.w > ai.x &&
        ball.x < ai.x + ai.w) {
        ball.dy = -ball.dy;
        ball.y = ai.y + ai.h + 1;
    }

    if (ball.y < 0) {
        player_score++;
        init_game();
    } else if (ball.y + ball.h > HEIGHT) {
        ai_score++;
        init_game();
    }

    const Uint8 *keys = SDL_GetKeyboardState(NULL);

    if (keys[SDL_SCANCODE_LEFT] && player.x > 0)
        player.x -= PLAYER_SPEED;
    if (keys[SDL_SCANCODE_RIGHT] && player.x + player.w < WIDTH)
        player.x += PLAYER_SPEED;

    if (keys[SDL_SCANCODE_H] && player.x > 0)
        player.x -= PLAYER_SPEED;
    if (keys[SDL_SCANCODE_L] && player.x + player.w < WIDTH)
        player.x += PLAYER_SPEED;

    int target_x = ball.x - ai.w/2 + ball.w/2;
    if (ai.x < target_x && ai.x + ai.w < WIDTH) {
        ai.x += PLAYER_SPEED - 1;
    } else if (ai.x > target_x && ai.x > 0) {
        ai.x -= PLAYER_SPEED - 1;
    }
}

int main(int argc, char *argv[]) {
    SDL_Init(SDL_INIT_VIDEO);
    SDL_Window *window = SDL_CreateWindow("Pong",
        SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WIDTH, HEIGHT, SDL_WINDOW_SHOWN);
    SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

    init_game();

    int running = 1;
    SDL_Event e;
    while (running) {
        while (SDL_PollEvent(&e)) {
            if (e.type == SDL_QUIT) running = 0;
        }

        update();
        draw(renderer);
        SDL_Delay(16);
    }

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}
