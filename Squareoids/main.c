//gcc main.c game.c -o main -lSDLmain -lSDL -lGL -lSDL_image -lglut -lGLU

#include<stdio.h>
#include"game.h"


int main(int argc, char * argv[]){

    //variáveis
    Uint32 FPS;
    Uint32 frameDelay;                  //variáveis para controle do frametime
    Uint32 frameStart;                  //
    int frameTime;                      //
    int error_of_read_config;            //}

    SDL_Surface *screen = NULL;

    bool running = TRUE;
    SDL_Event events;

    int menu_event;


    float dT;                                       //{variaveis para controle  da fisica de movimento
    Uint32 PMoveLastTime=SDL_GetTicks();            //
    Uint32 PMoveCurrentTime;                        //}


    Uint32 ShotLastTime=PMoveLastTime;             //{  variaveis para controle do tiro
    Uint32 ShotCurrentTime;                        //}

    Uint32 EnemyLastSpawnTime=PMoveLastTime;
    Uint32 EnemyCurrentSpawnTime;

    player p1;
    bool left = FALSE, right = FALSE, up = FALSE, down = FALSE;
    bool shooting = FALSE;
    Uint64 LastScore=0;
    GLuint playerTexture=0;
    GLuint lifeTexture=0;

    enemy e1[MAX_ENEMIES];
    Uint16 max_enemies = 5;
    GLuint enemyTexture=0;

    int i, shootingCounter=0, shootingCounter2=0;//contadores
    int enemiesCounter;


    //lê o arquivo de configuração do jogo e trata qualquer possivel erro na leitura.
    error_of_read_config = config_read_file(&FPS);
    if(error_of_read_config == 0){
        FPS = STANDARD_FPS;
    }

    //seta o cap do fps baseado no FPS que o usuario escolheu ou caso contrário, de acordo com o STANDARD_FPS;
    frameDelay = 1000/FPS;


    //===================================
    SDL_Init(SDL_INIT_EVERYTHING);

    //memoria
    SDL_GL_SetAttribute(SDL_GL_RED_SIZE, 8);
    SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, 8);
    SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, 8);
    SDL_GL_SetAttribute(SDL_GL_ALPHA_SIZE, 8);
    SDL_GL_SetAttribute(SDL_GL_BUFFER_SIZE, 32);
    SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 16);
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);

    //nome da janela aberta
    SDL_WM_SetCaption("Squareoids", NULL);

    //tamanho da janela
    screen = SDL_SetVideoMode(600, 400, 32, SDL_OPENGL);

    playerTexture = loadTexture("assets/spaceship.png");
    lifeTexture = loadTexture("assets/heart.png");
    enemyTexture = loadTexture("assets/asteroid.png");

    //cor da janela
    glClearColor(0,0,0,0);

    //area exibida
    glViewport(0,0,600,400);

    //sombra
    glShadeModel(GL_SMOOTH);

    //2D
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    //desabilita 3D
    glDisable(GL_DEPTH_TEST);

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    //inicializa player e inimigos
    start_game(&p1, e1);


    //Inicia os sons


    //LOOP do jogo

    while(running){
        frameStart = SDL_GetTicks();

        //event listener
        while (SDL_PollEvent(&events)){

            //fecha a janela clicando no X
            if(events.type ==  SDL_QUIT){
                printf("Janela fechou clicando no X dela\n");
                running = FALSE; ;
            }

            //fecha a janela clicando no X
            if(events.type ==  SDL_KEYUP && events.key.keysym.sym == SDLK_ESCAPE){
                running = FALSE;
            }

            //reseta a posiçao do player

            if(events.type ==  SDL_KEYUP && events.key.keysym.sym == SDLK_r){
                start_game(&p1, e1);
                max_enemies = 5;
                system("cls");
                printf("Score:  %lu\nAsteroides na tela: %hi/%d\n", p1.score, max_enemies, MAX_ENEMIES);
            }

            //movimenta ou nao o player
            if(events.type == SDL_KEYDOWN){

                if(events.key.keysym.sym == SDLK_LEFT) left = TRUE;         //esquerda
                else if(events.key.keysym.sym == SDLK_RIGHT) right = TRUE;  //direita

                if(events.key.keysym.sym == SDLK_UP) up = TRUE;
                else if(events.key.keysym.sym == SDLK_DOWN) down = TRUE;

            }else if(SDL_KEYUP){

                if(events.key.keysym.sym == SDLK_LEFT) left = FALSE;
                else if(events.key.keysym.sym == SDLK_RIGHT) right = FALSE;

                if(events.key.keysym.sym == SDLK_UP) up = FALSE;
                else if(events.key.keysym.sym == SDLK_DOWN) down = FALSE;

            }

            //ato do player atirar
            if(events.type ==  SDL_KEYDOWN && events.key.keysym.sym == SDLK_SPACE){
                shooting = TRUE;
            }else if(events.type ==  SDL_KEYUP && events.key.keysym.sym == SDLK_SPACE){
                shooting = FALSE;
            }

        }



        //LÓGICA DO JOGO


        //COLISÕES

        //Colisão tiro com inimigo
        for(shootingCounter=0; shootingCounter < SHOTS; shootingCounter++){
            if(p1.shots[shootingCounter].SHOOTED == TRUE){
                for(enemiesCounter=0; enemiesCounter < max_enemies; enemiesCounter++){
                    if(e1[enemiesCounter].ALIVE == TRUE){

                        if(collision(p1.shots[shootingCounter].x, p1.shots[shootingCounter].y, p1.shots[shootingCounter].side, p1.shots[shootingCounter].side,
                        e1[enemiesCounter].x, e1[enemiesCounter].y, e1[enemiesCounter].side, e1[enemiesCounter].side) == TRUE){

                            p1.score += 10*(max_enemies/3);

                            if(max_enemies < MAX_ENEMIES){
                                if(p1.score - LastScore >= 300){
                                    max_enemies++;
                                    LastScore = p1.score;
                                }
                            }

                            p1.shots[shootingCounter].SHOOTED = FALSE;
                            e1[enemiesCounter].ALIVE = FALSE;
                            system("cls");
                            printf("Score:  %lu\nAsteroides na tela: %hi/%d\nApertar 'r' reinicia o jogo.\n", p1.score, max_enemies, MAX_ENEMIES);
                        }
                    }
                }
            }
        }

        //Colisão inimigo com player
        for(enemiesCounter=0; enemiesCounter<max_enemies; enemiesCounter++){
            if(e1[enemiesCounter].ALIVE == TRUE){
                if(collision(p1.x, p1.y, p1.length, p1.height, e1[enemiesCounter].x, e1[enemiesCounter].y, e1[enemiesCounter].side, e1[enemiesCounter].side) == TRUE){



                    p1.lives--;
                    e1[enemiesCounter].ALIVE = FALSE;

                    if(p1.lives == 0){
                        p1.ALIVE = FALSE;

                        break;
                    }
                }
            }
        }

        //logica do mecanismo de tiro
        ShotCurrentTime = SDL_GetTicks();
        if(shooting == TRUE){
            if((ShotCurrentTime - ShotLastTime) > 200){ //faz com que execute a cada 200ms
                if(shootingCounter2 > SHOTS-1) shootingCounter2 = 0;

                if(p1.shots[shootingCounter2].SHOOTED == FALSE){

                    p1.shots[shootingCounter2].SHOOTED = TRUE;
                    p1.shots[shootingCounter2].x = p1.x + p1.length/2;
                    p1.shots[shootingCounter2].y = p1.y-p1.shots[i].side;

                }

                shootingCounter2++;

                ShotLastTime = ShotCurrentTime;

            }
        }

        //logica do inimigo
        for(enemiesCounter=0; enemiesCounter<max_enemies; enemiesCounter++){
            if(e1[enemiesCounter].ALIVE == TRUE){
                if(e1[enemiesCounter].y > 400){
                    e1[enemiesCounter].ALIVE = FALSE;
                }
            }
        }

        EnemyCurrentSpawnTime = SDL_GetTicks();
        if(EnemyCurrentSpawnTime - EnemyLastSpawnTime > 300){
            for(enemiesCounter=0; enemiesCounter<max_enemies; enemiesCounter++){
                if(e1[enemiesCounter].ALIVE == FALSE){
                    //start_enemy( &e1[enemiesCounter]);
                    e1[enemiesCounter].x = rand()%(600 - ((int)e1[enemiesCounter].side + 5));
                    e1[enemiesCounter].y = -(e1[enemiesCounter].side);
                    e1[enemiesCounter].ALIVE = TRUE;
                }
            }

            EnemyLastSpawnTime = EnemyCurrentSpawnTime;
        }


        //logica da movimentação do personagem no eixo x
        PMoveCurrentTime = SDL_GetTicks();
        dT = (PMoveCurrentTime - PMoveLastTime)/(float)(1000);
        if(left == TRUE){
            p1.x += -SPEED*dT;

        }else if(right == TRUE){
            p1.x += SPEED*dT;

        }//incrementa ou decrementa a posição no eixo x

        if(p1.x  > 600 + p1.length) p1.x = -p1.length;    //
        else if(p1.x < -p1.length) p1.x = 600 + p1.length;//verifica e faz com que o player apareça no lado oposto caso saia da tela no eixo x

        //logica da movimentação do personagem no eixo y
        if(up == TRUE){
            if (p1.y - SPEED*dT > 0){
                p1.y += -SPEED*dT;
            }

        }
        else if(down == TRUE){
            if (p1.y+ p1.height + SPEED*dT < 400){
                p1.y += SPEED*dT;
            }
        }//incrementa ou decrementa a posição no eixo y


        //movimentação


        //deslocamento do tiro
        for(shootingCounter=0; shootingCounter < SHOTS; shootingCounter++){
            if(p1.shots[shootingCounter].SHOOTED == TRUE){
                p1.shots[shootingCounter].y -= VEL_SHOT*dT;
            }

            if(p1.shots[shootingCounter].y - VEL_SHOT*dT < 0){
                p1.shots[shootingCounter].SHOOTED = FALSE;
            }

        }

        //movimentação do enimigo
        for(enemiesCounter=0; enemiesCounter<max_enemies; enemiesCounter++){
            if(e1[enemiesCounter].ALIVE == TRUE){
                switch (e1[enemiesCounter].behavior){
                    case 0:
                        e1[enemiesCounter].y += e1[enemiesCounter].speedY*dT;
                    break;

                    case 1:
                        e1[enemiesCounter].y += e1[enemiesCounter].speedY*dT;

                        if(e1[enemiesCounter].x + e1[enemiesCounter].speedX*dT + e1[enemiesCounter].side > 599){
                            e1[enemiesCounter].speedX = -e1[enemiesCounter].speedX;
                        }
                        if(e1[enemiesCounter].x + e1[enemiesCounter].speedX*dT < 0){
                            e1[enemiesCounter].speedX = -e1[enemiesCounter].speedX;
                        }

                        e1[enemiesCounter].x += e1[enemiesCounter].speedX*dT;
                    break;
                }
            }
        }

        PMoveLastTime = PMoveCurrentTime;


        //renderização
        glClear(GL_COLOR_BUFFER_BIT); //limpa o buffer

        //inicia a matriz de desenho
        glPushMatrix();

            //dimensões da matriz
            glOrtho(0, 600, 400, 0, -1, 1);


            //desenho do player
            //draw_rectangle_object(p1.x, p1.y, p1.length, p1.height, 255, 255, 255, 255, 0); //retirar o comentario permite com que a hitbox seja desenhada
            draw_rectangle_object(p1.x - RESIZE/2, p1.y - RESIZE/2, p1.length + RESIZE, p1.height + RESIZE, 255, 255, 255, 255, playerTexture);

            //desenho do tiro do player
            for(shootingCounter=0; shootingCounter < SHOTS; shootingCounter++){
                if(p1.shots[shootingCounter].SHOOTED == TRUE){
                    draw_rectangle_object(p1.shots[shootingCounter].x, p1.shots[shootingCounter].y, p1.shots[shootingCounter].side, p1.shots[shootingCounter].side, 0, 255, 0, 255, 0);
                }
            }

            //desenho do inimigo
            for(enemiesCounter=0; enemiesCounter < max_enemies; enemiesCounter++){
                if(e1[enemiesCounter].ALIVE == TRUE){
                    //draw_rectangle_object(e1[enemiesCounter].x, e1[enemiesCounter].y, e1[enemiesCounter].side, e1[enemiesCounter].side, 255, 255, 255, 255, 0); //retirar o comentario permite com que a hitbox seja desenhada
                    draw_rectangle_object(e1[enemiesCounter].x - RESIZE/2, e1[enemiesCounter].y - RESIZE/2, e1[enemiesCounter].side + RESIZE, e1[enemiesCounter].side + RESIZE, 255, 255, 255, 255, enemyTexture);
                }
            }

            for(i=0; i<p1.lives; i++){
                draw_rectangle_object(i*20+(15), 20, 25, 25, 200, 255, 255, 255, lifeTexture);
            }


        //fecha matriz
        glPopMatrix();

        if(p1.ALIVE == FALSE){

            menu_event = player_morto(&p1);

            if(menu_event == 1){
                running = FALSE;
            }else if(menu_event == 2){
                start_game(&p1, e1);
                max_enemies = 5;
                LastScore = 0;
                left = right = up = down = FALSE;
            }else if(menu_event == 3){
                display_ranking();
                start_game(&p1, e1);
                max_enemies = 5;
                LastScore = 0;
                left = right = up = down = FALSE;
            }
        }

        //animação
        SDL_GL_SwapBuffers();

        /* pega o TICK final e checa se está mais rapido que a velocidade
        ideal da aplicação. Se estiver, chama a função SDL_Delay e aplica
        a diferença necessária para ficar igual ao frameTime ideal. */
        frameTime = SDL_GetTicks() - frameStart;
        if(frameDelay > frameTime){
            SDL_Delay(frameDelay - frameTime);
        }

    }//fim do loop do jogo



    SDL_FreeSurface(screen);

    SDL_Quit();

    system("cls");

    return 0;
}
