/**
 * @file enemy.h
 * @author [Your Name]
 * @date May 11, 2025
 * @brief Header file defining structures and function prototypes for enemies and game objects.
 * This file includes definitions for enemy states, structures for enemies, background images,
 * and coins, along with their associated function declarations.
 */
#ifndef ENEMY_H_INCLUDED
#define ENEMY_H_INCLUDED

#include <SDL/SDL.h>
#include <SDL/SDL_image.h>
#include <SDL/SDL_mixer.h>
#include <SDL/SDL_ttf.h>

/**
 * @brief Enumeration of possible enemy states.
 */
enum STATE
{
  WAITING,   /**< Enemy is idle, moving in a default pattern */
  FOLLOWING, /**< Enemy is chasing the player */
  ATTACKING  /**< Enemy is close enough to attack the player */
};
typedef enum STATE STATE;

/**
 * @brief Structure representing an enemy entity.
 */
typedef struct
{
  SDL_Rect pos_depart;      /**< Starting position of the enemy on the screen */
  SDL_Rect pos_actuelle;    /**< Current position of the enemy */
  int direction;            /**< Direction of movement (0 = up/left, 1 = down/right) */
  float vitesse;            /**< Speed of the enemy */
  SDL_Surface *spritesheet; /**< Image containing all animation frames */
  SDL_Rect pos_sprites;     /**< Portion of the spritesheet to display */
  int frame;                /**< Current frame of animation */
  int frameCount;           /**< Total number of frames */
  int frameWidth;           /**< Width of each frame */
  int frameHeight;          /**< Height of each frame */
  int alive;                /**< Flag to check if the enemy is alive */
  STATE state;              /**< Current state of the enemy */
  int health;               /**< Health points of the enemy */
} Ennemi;

/**
 * @brief Structure representing a background image.
 */
typedef struct
{
  char *url;               /**< File path of the background image */
  SDL_Rect pos_img_affiche;/**< Portion of the image to display */
  SDL_Rect pos_img_ecran;  /**< Position on the screen */
  SDL_Surface *img;        /**< The actual image data */
} image;

/**
 * @brief Structure representing a collectible coin.
 */
typedef struct
{
  SDL_Rect pos;            /**< Position of the coin */
  SDL_Surface *img;        /**< Image of the coin */
  int visible;             /**< Flag to check if the coin is visible */
} Coin;

// Function declarations for background handling
/**
 * @brief Initializes the background image with a file and display properties.
 * @param image Pointer to the image structure to initialize.
 */
void initialiser_imageBACK(image *image);
/**
 * @brief Displays the background image on the screen.
 * @param screen The SDL surface to draw the background on.
 * @param image The image structure containing the background data.
 */
void afficher_imageBMP(SDL_Surface *screen, image image);

// Function declarations for enemy handling
/**
 * @brief Initializes the enemy (bat) for Level 1.
 * @param e Pointer to the Ennemi structure to initialize.
 */
void initEnnemi(Ennemi *e);
/**
 * @brief Initializes the enemy for Level 2.
 * @param e Pointer to the Ennemi structure to initialize.
 */
void initEnnemiLevel2(Ennemi *e);
/**
 * @brief Displays the enemy on the screen.
 * @param e Pointer to the Ennemi structure to display.
 * @param screen The SDL surface to draw the enemy on.
 */
void afficherEnnemi(Ennemi *e, SDL_Surface *screen);
/**
 * @brief Animates the enemy's sprite frames.
 * @param e Pointer to the Ennemi structure to animate.
 */
void animerEnemi(Ennemi *e);
/**
 * @brief Moves the Level 1 enemy vertically.
 * @param e Pointer to the Ennemi structure to move.
 */
void move(Ennemi *e);
/**
 * @brief Makes the Level 1 enemy follow the player.
 * @param e Pointer to the Ennemi structure to move.
 * @param posperso SDL_Rect representing the player's position.
 */
void moveEnnemi(Ennemi *e, SDL_Rect posperso);
/**
 * @brief Updates the enemy's state based on distance from the player.
 * @param e Pointer to the Ennemi structure to update.
 * @param distx Horizontal distance from the player.
 * @param disty Vertical distance from the player.
 */
void updateEnnemiState(Ennemi *e, int distx, int disty);
/**
 * @brief Controls the Level 1 enemy's AI behavior.
 * @param e Pointer to the Ennemi structure to control.
 * @param posperso SDL_Rect representing the player's position.
 */
void moveIA(Ennemi *e, SDL_Rect posperso);
/**
 * @brief Moves the Level 2 enemy (stays at ground level).
 * @param e Pointer to the Ennemi structure to move.
 * @param posperso SDL_Rect representing the player's position.
 */
void moveIALevel2(Ennemi *e, SDL_Rect posperso);
/**
 * @brief Checks for collision between an enemy and the player using circular approximation.
 * @param e Pointer to the Ennemi structure to check collision with.
 * @param posPerso SDL_Rect representing the player's position.
 * @return 1 if collision detected, 0 otherwise.
 */
int collisionTri(Ennemi *e, SDL_Rect posPerso);

#endif
