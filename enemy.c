/**
 * @file enemy.c
 * @author [Your Name]
 * @date May 11, 2025
 * @brief Implementation of functions for enemy and background management.
 * This file contains the definitions of functions declared in enemy.h,
 * handling background display, enemy initialization, movement, and collision detection.
 */
#include <stdlib.h>
#include <stdio.h>
#include <SDL/SDL.h>
#include <SDL/SDL_ttf.h>
#include <SDL/SDL_image.h>
#include <SDL/SDL_mixer.h>
#include "enemy.h"

// Initialize the background image with a file and set its display properties
/**
 * @brief Initializes the background image with a file and display properties.
 * @param image Pointer to the image structure to initialize.
 */
void initialiser_imageBACK(image *image)
{
    image->url = "background.png";
    image->img = IMG_Load(image->url);
    if (image->img == NULL)
    {
        printf("unable to load background image %s \n", SDL_GetError());
    }
    image->pos_img_ecran.x = 0;
    image->pos_img_ecran.y = 0;
    image->pos_img_affiche.x = 0;
    image->pos_img_affiche.y = 0;
    image->pos_img_affiche.h = 1060;
    image->pos_img_affiche.w = 594;
}

// Display the background image on the screen
/**
 * @brief Displays the background image on the screen.
 * @param screen The SDL surface to draw the background on.
 * @param image The image structure containing the background data.
 */
void afficher_imageBMP(SDL_Surface *screen, image image)
{
    SDL_BlitSurface(image.img, NULL, screen, &image.pos_img_ecran);
}

// Initialize the enemy (bat) with starting values for Level 1
/**
 * @brief Initializes the enemy (bat) for Level 1.
 * @param e Pointer to the Ennemi structure to initialize.
 */
void initEnnemi(Ennemi *e)
{
    e->pos_depart.x = 260;
    e->pos_depart.y = 100;
    e->pos_actuelle = e->pos_depart;
    e->direction = 0;
    e->vitesse = 0;
    e->alive = 1;
    e->health = 50;

    e->spritesheet = IMG_Load("batt.png");
    if (e->spritesheet == NULL)
    {
        printf("Erreur lors du chargement de la spritesheet de l'ennemi : %s\n", SDL_GetError());
    }

    e->frame = 0;
    e->frameCount = 3;
    e->frameWidth = 64;
    e->frameHeight = 64;

    e->pos_sprites.x = 0;
    e->pos_sprites.y = 0;
    e->pos_sprites.w = e->frameWidth;
    e->pos_sprites.h = e->frameHeight;
    e->state = WAITING;
}

// Initialize the Level 2 enemy with "ennemi.png"
/**
 * @brief Initializes the enemy for Level 2.
 * @param e Pointer to the Ennemi structure to initialize.
 */
void initEnnemiLevel2(Ennemi *e) {
    e->pos_depart.x = 500;
    e->pos_depart.y = 500;
    e->pos_actuelle = e->pos_depart;
    e->direction = 0;
    e->vitesse = 0;
    e->alive = 1;
    e->health = 70;

    e->spritesheet = IMG_Load("ennemi.png");
    if (e->spritesheet == NULL) {
        printf("Erreur lors du chargement de la spritesheet de l'ennemi : %s\n", SDL_GetError());
        return;
    }

    SDL_SetColorKey(e->spritesheet, SDL_SRCCOLORKEY, SDL_MapRGB(e->spritesheet->format, 0, 0, 0));

    e->frame = 0;
    e->frameCount = 4;
    e->frameWidth = 64;
    e->frameHeight = 64;
    e->pos_sprites.x = 0;
    e->pos_sprites.y = 0;
    e->pos_sprites.w = e->frameWidth;
    e->pos_sprites.h = e->frameHeight;
    e->state = WAITING;
}

// Display the enemy on the screen
/**
 * @brief Displays the enemy on the screen.
 * @param e Pointer to the Ennemi structure to display.
 * @param screen The SDL surface to draw the enemy on.
 */
void afficherEnnemi(Ennemi *e, SDL_Surface *screen)
{
    if (e->spritesheet == NULL) {
        printf("Error: Enemy spritesheet is NULL\n");
        return;
    }

    SDL_SetAlpha(e->spritesheet, 0, 255);
    SDL_SetColorKey(e->spritesheet, SDL_SRCCOLORKEY, SDL_MapRGB(e->spritesheet->format, 0, 0, 0));

    SDL_BlitSurface(e->spritesheet, &e->pos_sprites, screen, &e->pos_depart);
}

// Animate the enemy based on its state
/**
 * @brief Animates the enemy's sprite frames.
 * @param e Pointer to the Ennemi structure to animate.
 */
void animerEnemi(Ennemi *e)
{
    static int frame_counter = 0;
    frame_counter++;
    if (frame_counter < 4) return;
    frame_counter = 0;

    if (e->frameCount == 3) {
        e->frame = (e->frame + 1) % e->frameCount;
        e->pos_sprites.x = e->frame * e->frameWidth;
    } else {
        e->frame = (e->frame + 1) % e->frameCount;
        e->pos_sprites.x = e->frame * e->frameWidth;
    }
}

// Move the Level 1 enemy vertically
/**
 * @brief Moves the Level 1 enemy vertically.
 * @param e Pointer to the Ennemi structure to move.
 */
void move(Ennemi *e)
{
    if (e->pos_depart.y < 12)
        e->direction = 1;
    else if (e->pos_depart.y > 400)
        e->direction = 0;

    if (e->direction == 1)
        e->pos_depart.y += 7;
    if (e->direction == 0)
        e->pos_depart.y -= 7;
}

// Make the Level 1 enemy follow the player
/**
 * @brief Makes the Level 1 enemy follow the player.
 * @param e Pointer to the Ennemi structure to move.
 * @param posperso SDL_Rect representing the player's position.
 */
void moveEnnemi(Ennemi *e, SDL_Rect posperso)
{
    if (e->pos_depart.x > posperso.x)
    {
        e->pos_depart.x -= 3;
        if (e->pos_depart.y > posperso.y)
            e->pos_depart.y -= 3;
        if (e->pos_depart.y < posperso.y)
            e->pos_depart.y += 3;
    }
    if (e->pos_depart.x < posperso.x)
    {
        e->pos_depart.x += 3;
        if (e->pos_depart.y > posperso.y)
            e->pos_depart.y -= 3;
        if (e->pos_depart.y < posperso.y)
            e->pos_depart.y += 3;
    }
}

// Update the enemy's state based on its distance from the player
/**
 * @brief Updates the enemy's state based on distance from the player.
 * @param e Pointer to the Ennemi structure to update.
 * @param distx Horizontal distance from the player.
 * @param disty Vertical distance from the player.
 */
void updateEnnemiState(Ennemi *e, int distx, int disty)
{
    if (distx > 150 && disty > 150) {
        e->state = WAITING;
    }
    else if (distx <= 50 && disty <= 50) {
        e->state = ATTACKING;
    }
    else {
        e->state = FOLLOWING;
    }
}

// Control the Level 1 enemy's AI
/**
 * @brief Controls the Level 1 enemy's AI behavior.
 * @param e Pointer to the Ennemi structure to control.
 * @param posperso SDL_Rect representing the player's position.
 */
void moveIA(Ennemi *e, SDL_Rect posperso)
{
    int dx = abs(e->pos_depart.x - posperso.x);
    int dy = abs(e->pos_depart.y - posperso.y);
    switch (e->state)
    {
    case WAITING:
        move(e);
        break;
    case FOLLOWING:
        moveEnnemi(e, posperso);
        break;
    case ATTACKING:
        e->vitesse = 0;
        break;
    }
    updateEnnemiState(e, dx, dy);
}

// Move the Level 2 enemy (stays at ground level, runs when detecting player)
/**
 * @brief Moves the Level 2 enemy (stays at ground level).
 * @param e Pointer to the Ennemi structure to move.
 * @param posperso SDL_Rect representing the player's position.
 */
void moveEnnemiLevel2(Ennemi *e, SDL_Rect posperso) {
    int dx = abs(e->pos_depart.x - posperso.x);
    int dy = abs(e->pos_depart.y - posperso.y);
    e->pos_depart.y = 500;
    updateEnnemiState(e, dx, dy);
    switch (e->state) {
    case WAITING:
        break;
    case FOLLOWING:
        if (e->pos_depart.x > posperso.x)
            e->pos_depart.x -= 10;
        else if (e->pos_depart.x < posperso.x)
            e->pos_depart.x += 10;
        break;
    case ATTACKING:
        e->vitesse = 0;
        break;
    }
}

// Control the Level 2 enemy's AI
/**
 * @brief Controls the Level 2 enemy's AI behavior.
 * @param e Pointer to the Ennemi structure to control.
 * @param posperso SDL_Rect representing the player's position.
 */
void moveIALevel2(Ennemi *e, SDL_Rect posperso) {
    moveEnnemiLevel2(e, posperso);
}

// Check for collision using circular approximation
/**
 * @brief Checks for collision between an enemy and the player using circular approximation.
 * @param e Pointer to the Ennemi structure to check collision with.
 * @param posPerso SDL_Rect representing the player's position.
 * @return 1 if collision detected, 0 otherwise.
 */
int collisionTri(Ennemi *e, SDL_Rect posPerso) {
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
    X2 = e->pos_depart.x + e->pos_sprites.w / 2;
    Y2 = e->pos_depart.y + e->pos_sprites.h / 2;
    R2 = sqrt(pow(e->pos_sprites.w / 2, 2) + pow(e->pos_sprites.h / 2, 2));
    if (e->pos_sprites.w < e->pos_sprites.h) {
        R2 = e->pos_sprites.w / 2;
    } else {
        R2 = e->pos_sprites.h / 2;
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
