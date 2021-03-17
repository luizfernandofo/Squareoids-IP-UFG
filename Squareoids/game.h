#ifndef __GAME_H__
#define __GAME_H__

#include<SDL.h>
#include"SDL_opengl.h"
#include"GL_glut/glut.h"
//#include<SDL/SDL_image.h>


//#defines
#define STANDARD_FPS 60
#define FALSE 0
#define TRUE 1
#define SPEED 300
#define SPEEDENEMY 150
#define VEL_ESCALAR 350
#define VEL_DECREMENTADORA 250
#define VEL_SHOT 300
#define SHOTS 15
#define SIDE_SHOT 5
#define MAX_ENEMIES 15
#define ENEMY_SIDE 20
#define LIVES 5
#define MAX_NAME_LENGTH 50
#define PLAYER_HEIGHT 30
#define PLAYER_LENGTH 30
#define RESIZE 20

//#defines sounds
#define FREQUENCY 22050
#define CHANNEL 2 //1 = mono; 2 = stereo
#define SOUNDS_BUFFER_SIZE 4096

//typedefs
typedef unsigned char bool;

typedef struct{

    float x,y;
    float side;
    bool SHOOTED;

}shot;

typedef struct{
    unsigned char R,G,B,A;
} rgba;

typedef struct{
    float x;
    float y;
    float height;
    float length;
    float speedX;
    float speedY;
    shot shots[SHOTS];
    Uint16 lives;
    Uint64 score;
    bool ALIVE;

}player;


typedef struct{
    float x;
    float y;
    float side; //inimigo é um quadrado, então side vale para altura e largura
    float speedX;
    float speedY;
    bool ALIVE;
    unsigned char behavior; //0-2 //0 inimigo nao se movimenta //1 percorre o eixo y crescentemente em linha reta //2 faz "zig-zag" na tela descendo o eixo y.
    rgba body_color;

}enemy;

typedef struct{

    char name[MAX_NAME_LENGTH];
    Uint64 score;

}score;

//functions

/**
 * @brief Indica se houve colisão entre dois elementos
 *
 * @param Ax Coordenada x do elemento A.
 * @param Ay Coordenada y do elemento A.
 * @param ALength Comprimento do elemento A.
 * @param AHeight Altura do elemento A.
 * @param Bx Coordenada x do elemento B.
 * @param By Coordenada y do elemento B.
 * @param BLength Comprimento do elemento B.
 * @param BHeight Altura do elemento B.
 * @return true se houve colisão.
 * @return false se não houve colisão.
 */
bool collision(float Ax, float Ay, float ALength, float AHeight,
               float Bx, float By, float BLength, float BHeight);



/**
 * @brief Desenha um retângulo na cor indicada com valores RGBa.
 *
 * @param x Posição x do objeto.
 * @param y Posição y do objeto.
 * @param length Comprimento do objeto.
 * @param height Altura do objeto.
 * @param R RED (RGBa).
 * @param G GREEN (RGBa).
 * @param B BLUE (RGBa).
 * @param Alpha ALPHA (RGBa).
 */
void draw_rectangle_object(float x, float y, float length, float height, char R, char G, char B, char Alpha, GLuint texture);

/**
 * @brief Inicia o inimigo.
 *
 * @param e Entidade a ser iniciada.
 */
void start_enemy(enemy * e);

/**
 * @brief Lê um arquivo chamado config que armazena o FPS desejado pelo usuário.
 *
 * @param FPS Endereço da variável FPS contida na main();
 * @return Retorna o valor (int) 0 caso dê algum erro ou 1 caso ocorra a leitura correta do arquivo.
 */
int config_read_file(Uint32 * FPS);


/**
 * @brief Lida com o evento do player morrer.
 *
 * @param p endereço da struct do player.
 * @return (int) 1 para sair do jogo; 2 para jogar novamente; 3 para ver o ranking de recordes.
 */
int player_morto(player * p);

/**
 * @brief Inicializa os elementos das structs.
 *
 * @param p Endereço da struct player.
 * @param e Endereço da struct de inimigos.
 */
void start_game(player *p, enemy *e);


/**
 * @brief Imprime o ranking do arquivo "scores" em ordem decrescente.
 *
 */
void display_ranking();

GLuint loadTexture(const char name[]);

#endif
