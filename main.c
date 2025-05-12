/**
 * @file main.c
 * @author [Your Name]
 * @date May 11, 2025
 * @brief Main game loop implementation for a 2D platformer with enemies and coins.
 * This file contains the main function and game logic, including player movement,
 * enemy interactions, coin collection, and health management.
 */
#include <stdlib.h>
#include <stdio.h>
#include <SDL/SDL.h>
#include <SDL/SDL_ttf.h>
#include <SDL/SDL_image.h>
#include <SDL/SDL_mixer.h>
#include "enemy.h"

/**
 * @brief Draws a health bar on the screen based on the entity's health.
 * @param screen The SDL surface to draw the health bar on.
 * @param health Current health value of the entity.
 * @param max_health Maximum health value of the entity.
 * @param x X-coordinate of the health bar's position.
 * @param y Y-coordinate of the health bar's position.
 * @param w Width of the health bar.
 * @param h Height of the health bar.
 */
void draw_health_bar(SDL_Surface *screen, int health, int max_health, int x, int y, int w, int h) {
    SDL_Rect bg_rect = {x, y, w, h};
    SDL_FillRect(screen, &bg_rect, SDL_MapRGB(screen->format, 0, 0, 0));
    
    float percentage = (health <= 0) ? 0 : (float)health / max_health;
    int current_width = (int)(percentage * (w - 2));
    
    Uint32 color;
    if (percentage > 0.5) color = SDL_MapRGB(screen->format, 0, 255, 0);
    else if (percentage > 0.25) color = SDL_MapRGB(screen->format, 255, 255, 0);
    else color = SDL_MapRGB(screen->format, 255, 0, 0);
    
    SDL_Rect health_rect = {x + 1, y + 1, current_width, h - 2};
    SDL_FillRect(screen, &health_rect, color);
}

/**
 * @brief Initializes a coin with a simple gold square appearance.
 * @param coin Pointer to the Coin structure to initialize.
 */
void initCoin(Coin *coin) {
    coin->img = SDL_CreateRGBSurface(SDL_SWSURFACE, 20, 20, 32, 0, 0, 0, 0);
    if (coin->img == NULL) {
        printf("Failed to create fallback coin surface: %s\n", SDL_GetError());
        return;
    }
    SDL_FillRect(coin->img, NULL, SDL_MapRGB(coin->img->format, 255, 215, 0));
    SDL_SetColorKey(coin->img, SDL_SRCCOLORKEY, SDL_MapRGB(coin->img->format, 0, 0, 0));
    SDL_Rect outline = {0, 0, 20, 20};
    SDL_FillRect(coin->img, &outline, SDL_MapRGB(coin->img->format, 0, 0, 0));
    SDL_Rect inner = {2, 2, 16, 16};
    SDL_FillRect(coin->img, &inner, SDL_MapRGB(coin->img->format, 255, 215, 0));
    coin->pos.w = 20;
    coin->pos.h = 20;
    coin->visible = 0;
}

/**
 * @brief Displays a coin on the screen if it is visible.
 * @param coin Pointer to the Coin structure to display.
 * @param screen The SDL surface to draw the coin on.
 */
void displayCoin(Coin *coin, SDL_Surface *screen) {
    if (coin->visible) {
        if (coin->img != NULL) {
            SDL_BlitSurface(coin->img, NULL, screen, &coin->pos);
        } else {
            printf("Coin image is NULL, cannot render coin at position (%d, %d)\n", coin->pos.x, coin->pos.y);
        }
    }
}

/**
 * @brief Checks for collision between a coin and the player using circular approximation.
 * @param coin Pointer to the Coin structure to check collision with.
 * @param posPerso SDL_Rect representing the player's position and size.
 * @return 1 if collision detected, 0 otherwise.
 */
int collisionTriCoin(Coin *coin, SDL_Rect posPerso) {
    int estcoli;
    float R1, R2, X1, X2, D1, D2, Y1, Y2;
    X1 = posPerso.x + posPerso.w / 2;
    Y1 = posPerso.y + posPerso.h / 2;
    R1 = sqrt(pow(posPerso.w / 2, 2) + pow(posPerso.h / 2, 2));
    if (posPerso.w < posPerso.h) {
        R1 = posPerso.w / 2;
    } else {
        R1 = posPerso.h / 2;
    }
    X2 = coin->pos.x + coin->pos.w / 2;
    Y2 = coin->pos.y + coin->pos.h / 2;
    R2 = sqrt(pow(coin->pos.w / 2, 2) + pow(coin->pos.h / 2, 2));
    if (coin->pos.w < coin->pos.h) {
        R2 = coin->pos.w / 2;
    } else {
        R2 = coin->pos.h / 2;
    }
    D1 = sqrt(pow(X2 - X1, 2) + pow(Y2 - Y1, 2));
    D2 = R1 + R2;
    if (D1 <= D2) {
        estcoli = 1;
    } else {
        estcoli = 0;
    }
    return estcoli;
}

/**
 * @brief Main function to run the game.
 * @param argc Number of command-line arguments.
 * @param argv Array of command-line argument strings.
 * @return 0 on successful execution, -1 on failure.
 */
int main(int argc, char *argv[]) {
    int loop = 1;
    SDL_Surface *screen;
    SDL_Event event;
    image IMAGE;
    Ennemi e;
    Coin coin1;
    Coin coin2;
    SDL_Surface *perso = IMG_Load("perso.png");
    SDL_Rect posPerso = {10, 450};
    int direction = -1;
    
    int health = 100;
    const int max_health = 100;
    Uint32 last_hit_time = 0;
    const Uint32 hit_cooldown = 500;

    int score = 0;
    int level = 1;

    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO | SDL_INIT_TIMER) == -1) {
        printf("SDL init failed: %s\n", SDL_GetError());
        return -1;
    }

    screen = SDL_SetVideoMode(1060, 594, 32, SDL_SWSURFACE | SDL_DOUBLEBUF | SDL_RESIZABLE);
    initialiser_imageBACK(&IMAGE);
    initEnnemi(&e);
    initCoin(&coin1);
    initCoin(&coin2);
    coin2.img = IMG_Load("coin.png");
    if (coin2.img == NULL) {
        printf("Failed to load coin.png: %s\n", SDL_GetError());
        return -1;
    }

    Uint32 start;
    const int FPS = 60;
    while (loop) {
        start = SDL_GetTicks();
        Uint32 current_time = SDL_GetTicks();

        while (SDL_PollEvent(&event)) {
            switch (event.type) {
                case SDL_QUIT:
                    loop = 0;
                    break;
                case SDL_KEYDOWN:
                    switch (event.key.keysym.sym) {
                        case SDLK_RIGHT: direction = 1; break;
                        case SDLK_LEFT:  direction = 0; break;
                        case SDLK_UP:    direction = 3; break;
                        case SDLK_DOWN:  direction = 2; break;
                    }
                    break;
                case SDL_KEYUP:
                    switch (event.key.keysym.sym) {
                        case SDLK_RIGHT:
                        case SDLK_LEFT:
                        case SDLK_UP:
                        case SDLK_DOWN:
                            direction = -1;
                            break;
                    }
                    break;
            }
        }

        if (direction == 1) posPerso.x += 5;
        if (direction == 0) posPerso.x -= 5;
        if (direction == 3) posPerso.y -= 5;
        if (direction == 2) posPerso.y += 5;

        if (posPerso.x < 0) posPerso.x = 0;
        if (posPerso.x > 1060 - perso->w) posPerso.x = 1060 - perso->w;
        if (posPerso.y < 0) posPerso.y = 0;
        if (posPerso.y > 594 - perso->h) posPerso.y = 594 - perso->h;

        afficher_imageBMP(screen, IMAGE);
        SDL_BlitSurface(perso, NULL, screen, &posPerso);
        
        if (e.alive) {
            afficherEnnemi(&e, screen);
            animerEnemi(&e);
            if (level == 1) {
                moveIA(&e, posPerso);
            } else {
                moveIALevel2(&e, posPerso);
            }
            draw_health_bar(screen, e.health, (level == 1 ? 50 : 70), e.pos_depart.x, e.pos_depart.y - 15, 40, 10);
        }

        if (e.alive && collisionTri(&e, posPerso)) {
            if (current_time - last_hit_time >= hit_cooldown) {
                int e_was_alive = e.alive;
                e.health -= 10;
                health -= 5;
                if (e.health <= 0 && e_was_alive) {
                    e.alive = 0;
                    score += 100;
                    printf("Enemy defeated! Score: %d\n", score);
                    if (level == 1) {
                        coin1.pos.x = e.pos_depart.x;
                        coin1.pos.y = e.pos_depart.y;
                        coin1.visible = 1;
                        printf("Coin dropped at (%d, %d)\n", coin1.pos.x, coin1.pos.y);
                    } else if (level == 2) {
                        coin2.pos.x = e.pos_depart.x;
                        coin2.pos.y = e.pos_depart.y;
                        coin2.visible = 1;
                        printf("Coin dropped at (%d, %d)\n", coin2.pos.x, coin2.pos.y);
                    }
                }
                if (health <= 0) {
                    printf("Player defeated! Game Over.\n");
                    loop = 0;
                }
                last_hit_time = current_time;
            }
        }

        if (coin1.visible && collisionTriCoin(&coin1, posPerso)) {
            coin1.visible = 0;
            score += 50;
            printf("Coin collected! Score: %d\n", score);
            if (level == 1 && !e.alive) {
                initEnnemiLevel2(&e);
                level = 2;
            }
        }

        if (coin2.visible && collisionTriCoin(&coin2, posPerso)) {
            coin2.visible = 0;
            score += 50;
            printf("Coin collected! Score: %d\n", score);
        }

        displayCoin(&coin1, screen);
        displayCoin(&coin2, screen);

        draw_health_bar(screen, health, max_health, 840, 20, 200, 20);

        SDL_Flip(screen);
        if (1000/FPS > SDL_GetTicks() - start)
            SDL_Delay(1000/FPS - (SDL_GetTicks() - start));
    }

    SDL_FreeSurface(IMAGE.img);
    SDL_FreeSurface(perso);
    SDL_FreeSurface(coin1.img);
    SDL_FreeSurface(coin2.img);
    SDL_FreeSurface(e.spritesheet);
    SDL_Quit();
    return 0;
}
