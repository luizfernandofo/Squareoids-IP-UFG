#include "game.h"
#include<math.h>
#include<stdio.h>
#include<stdlib.h>


int config_read_file(Uint32 * FPS){

    Uint32 fps_temp;
    const Uint32 StandardFPS = STANDARD_FPS;
    int log_fscanf;
    FILE * config_file=NULL;

    config_file = fopen("config.txt", "r");

    if(config_file == NULL){
        config_file = fopen("config.txt", "w");
        fprintf(config_file, "FPS = %d;", StandardFPS);
        *FPS = StandardFPS;
        fclose(config_file);

        return 1;

    }else{
        log_fscanf = fscanf(config_file, "FPS = %d;", &fps_temp);

        if(log_fscanf < 1){
            *FPS = StandardFPS;
        }

        if(fps_temp < 30 || fps_temp > 1000){
            *FPS = STANDARD_FPS;
            
        }else{
            *FPS = fps_temp;
            
        }
        fclose(config_file);

        config_file = fopen("config.txt", "w");
        fprintf(config_file, "FPS = %d;", *FPS);
        fclose(config_file);

        return 1;
    }

    fclose(config_file);

    return 0;

}


bool collision(float Ax, float Ay, float ALength, float AHeight,
               float Bx, float By, float BLength, float BHeight)
{
    if(Ay + AHeight < By) return FALSE;
    else if(Ay > By + BHeight) return FALSE;
    else if(Ax + ALength < Bx) return FALSE;
    else if(Ax > Bx + BLength) return FALSE;
    else return TRUE;
}


void draw_rectangle_object(float x, float y, float length, float height, char R, char G, char B, char Alpha, GLuint texture){

    glColor4ub(R,G,B,Alpha); //cor do retangulo 
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, texture);

    //inicia o desenho 
    glBegin(GL_QUADS); //GL_POINTS, GL_LINES, GL_LINES_LOOP, GL_QUADS, GL_TRIANGLE, GL_POLIGON

        glTexCoord2d(0, 0); glVertex2f(x, y);
        glTexCoord2d(1, 0); glVertex2f(x+length, y);
        glTexCoord2d(1, 1); glVertex2f(x+length, y+height);
        glTexCoord2d(0, 1); glVertex2f(x, y+height);

    //fecha o desenho 
    glEnd();
    glDisable(GL_TEXTURE_2D);
}


void start_enemy(enemy * e){
    unsigned char enemyColor;

    e->behavior = rand()%2; 

    switch(e->behavior){
        /*case 0:
            e->x = rand()%(600 - (int)e->side);
            e->y = rand()%(350 - (int)e->side);
            e->speedX = 0;
            e->speedY = 0;
        break;*/

        case 0:
            e->x = rand()%(600 - ((int)e->side + 5));
            e->y = -(e->side);
            e->speedX = 0;
            e->speedY = SPEEDENEMY;
        break;

        case 1:
            e->x = rand()%(600 - ((int)e->side + 5));
            e->y = -(e->side);
            e->speedX = SPEEDENEMY;
            e->speedY = SPEEDENEMY;
        break;
    }

    e->ALIVE =  TRUE;
    e->side = ENEMY_SIDE;

    enemyColor = (rand()%130)+60;

    e->body_color.R = enemyColor;
    e->body_color.G = enemyColor;
    e->body_color.B = enemyColor;
    //e->body_color.A = 255;
}

int player_morto(player * p){
    char y_n;
    char name[MAX_NAME_LENGTH];
    int op;
    FILE *score=NULL;

    printf("Game Over!\n");
    system("cls");
    SDL_Delay(2000);
    printf("Voce gostaria de registrar seu score? [y/n]\n");
    scanf("%c%*c", &y_n);

    if(y_n == 'y' || y_n == 'Y'){
        system("cls");
        printf("Digite seu nome com no maximo %d caracteres e sem espaco: ", MAX_NAME_LENGTH);
        scanf("%s%*c", name);
        printf("\n");

        score = fopen("scores", "a");
        if(score != NULL){
            fprintf(score, "%s %lu ", name, p->score);
            system("cls");
            printf("Score registrado com sucesso!\n");

            fclose(score);

            SDL_Delay(2000);
            system("cls");
        }else {
            system("cls");
            printf("Falha na abertura do arquivo de pontuacao.\nPontuacao nao registrada.\n");
            SDL_Delay(5000);
        }

    }

    system("cls");

    printf("Digite:\n1. para sair do jogo;\n2. para jogar novamente;\n3. para ver ranking de pontuacao.\n[1/2/3]\n");
    scanf("%d%*c", &op);

    system("cls");

    return op;
}


void start_game(player *p, enemy *e){
    int i;

    //seta os valores de player;
    p->x=300; p->y=350; p->height=PLAYER_HEIGHT; p->length=PLAYER_LENGTH; p->speedX=0; p->speedY=0; p->score=0; p->lives=LIVES; p->ALIVE=TRUE;

    for(i=0; i<SHOTS; i++){
        p->shots[i].SHOOTED = FALSE;
        p->shots[i].side = SIDE_SHOT;
    }

    //seta os valores dos inimigos
    for(i=0; i<MAX_ENEMIES; i++) start_enemy(&e[i]);
}


void display_ranking(){
    int qtd=0, i,j;
    score *scores=NULL, *scores_temp=NULL, temp;
    FILE *f=NULL;
    char vef;

    f = fopen("scores", "r");

    if(f == NULL){
        printf("Erro ao abrir arquivo de scores para visualizacao.\n"); 
        return;
    }else{
        
        while(fscanf(f,"%s %lu ", temp.name, &temp.score)!=EOF){
            qtd++;
            scores_temp = realloc(scores, sizeof(score)*qtd);
            if(scores_temp != NULL){
                scores = scores_temp;
                scores_temp = NULL;
                scores[qtd-1] = temp;
            }
        }

        for(i=0; i<qtd; i++){
            for(j=i+1; j<qtd; j++){
                if(scores[i].score < scores[j].score){
                    temp = scores[i];
                    scores[i] = scores[j];
                    scores[j] = temp;
                }
            }
        }

        system("cls");

        for(i=0; i<qtd; i++){
            printf("%d) %s:  %lu\n", i+1, scores[i].name, scores[i].score);
        }

    }

    fclose(f);
    if(scores!=NULL) free(scores);
    if(scores_temp!=NULL) free(scores_temp);

    printf("\nPressione ENTER para sair da visualizacao\n");
    scanf("%c", &vef);
    system("cls");
    return;
}

GLuint loadTexture(const char name[]){

    SDL_Surface *image = NULL;
    GLuint texture;

    image = IMG_Load(name);
    if(image == NULL){
        printf("Erro ao carregar imagem.\n");
        return 0;
    }

    SDL_DisplayFormatAlpha(image);

    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);

    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, image->w, image->h, 0, GL_RGBA, GL_UNSIGNED_BYTE, image->pixels);

    SDL_FreeSurface(image);

    return texture;
}