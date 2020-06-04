#include <graphics.h>
#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <Windows.h>
#include <windows.h>
#include <MMSystem.h>

typedef struct {
    char src[30];
    int resistencia;
    int pontuacao;
    int w; // WIDTH
    int h; // HEIGHT
    float f; // Fator de velocidade
} model;

typedef struct {
    int x;
    int y;
    model m;
} asteroide;

typedef struct {
    int x;
    int y;
} laser;

typedef struct {
    char src[30];
    int x;
    int y;
} nave;

struct noAsteroide {
    struct noAsteroide *ant;
    asteroide a;
    struct noAsteroide *prox;
};

typedef struct
{
    struct noAsteroide *inicio;
    struct noAsteroide *fim;
} listalineardeAst;

struct noLaser {
    struct noLaser *ant;
    laser l;
    struct noLaser *prox;
};

typedef struct
{
    struct noLaser *inicio;
    struct noLaser *fim;
} listalineardeLaser;

void initModel(model *a, char *src, int resistencia, int pontuacao, int w, int h, float f);
void criaListaAsteroide(listalineardeAst *q);
int  insereListaAsteroide(listalineardeAst *q, asteroide a);
void mostraAsteroides(listalineardeAst *q);
void clearAsteroides(listalineardeAst *q);
int removeeAsteroide(listalineardeAst *q, struct noAsteroide *ast);

void criaListaLaser(listalineardeLaser *q);
int  insereListaLaser(listalineardeLaser *q, laser l);
void mostraLasers(listalineardeLaser *q);
void clearLasers(listalineardeLaser *q);
int removeeLaser(listalineardeLaser *q, struct noLaser *laser);

void verificaColisao(listalineardeAst *lAst, listalineardeLaser *lLaser);
void clear_keyboard_buffer(void);

int speed = 8, vidas = 3, pontuacao = 0;

main()
{
    int i, ultimo_ast, ultimo_laser, r;
    int gdriver = EGA, gmode = EGAHI; // MODO GR�FICO PARA SUPORTE DE M�LTIPLAS P�GINAS
    char in, exitflag, pontuacao_aux[30], *trash;
    model models[6];
    nave n = {"assets/ship.gif",630,580};
    asteroide ast;
    asteroide novoAst;
    laser l;
    listalineardeAst listaAsteroide;
    listalineardeLaser listaLaser;

    initModel(&models[0],"assets/asteroide1.gif",3, 1, 60, 60, 1);
    initModel(&models[1],"assets/asteroide2.gif",6, 3, 85, 75, 0.8);
    initModel(&models[2],"assets/asteroide3.gif",9, 6, 100, 85, 0.7);
    initModel(&models[3],"assets/asteroide4.gif",12, 10, 110, 104, 0.5);
    initModel(&models[4],"assets/asteroide5.gif",15, 15, 110, 107, 0.3);
    initModel(&models[5],"assets/asteroide6.gif",25, 25, 180, 173, 0.1);

    //CRIANDO AS LISTAS
    criaListaAsteroide(&listaAsteroide);
    criaListaLaser(&listaLaser);

    initgraph(&gdriver, &gmode, "");
    initwindow(1360,730);

    while (exitflag != 'e') {
        PlaySound(TEXT("assets/background_menu.wav"), NULL, SND_ASYNC);

        clearAsteroides(&listaAsteroide);
        clearLasers(&listaLaser);

        readimagefile("assets/background.jpg",0,0,1360,730);
        readimagefile("assets/title.gif",280,200,1080,422);
        readimagefile("assets/presstocontinue.gif",480,440,880,520);
        readimagefile("assets/dir.gif",1120,570,1320,680);
        swapbuffers();

        in = '.';
        do {
            if (kbhit())
                in = getch();
        } while (in != ' ');

        PlaySound(TEXT("assets/start.wav"), NULL, SND_ASYNC);

        srand(clock());
        ast.m = models[rand()%6];
        ast.x = rand()%1160 + 100;
        ast.y = 0;
        insereListaAsteroide(&listaAsteroide, ast);
        ultimo_ast = time(NULL);
        ultimo_laser = time(NULL);

        vidas = 3; pontuacao = 0;
        while(vidas > 0) {
            swapbuffers();

            readimagefile("assets/background.jpg",0,0,1360,730);

            if (vidas == 3) readimagefile("assets/earth_3.gif",0,0,120,120);
            if (vidas == 2) readimagefile("assets/earth_2.gif",0,0,120,120);
            if (vidas == 1) readimagefile("assets/earth_1.gif",0,0,120,120);

            sprintf(pontuacao_aux,"%03d",pontuacao);

            settextstyle(3, HORIZ_DIR, 2);
            outtextxy(1250,20,"SCORE");
            settextstyle(3, HORIZ_DIR, 6);
            outtextxy(1245,45,pontuacao_aux);

            ast.y += 2; // #1

            // VERIFICA SE A DIFEREN�A DE TEMPO ENTRE O �LTIMO ASTEROIDE E AGORA � MAIOR QUE 6 SEG
            if (time(NULL) - ultimo_ast > 6) {
                ultimo_ast = time(NULL); // ATUALIZA O TEMPO DO �LTIMO ASTER�IDE PARA AGORA

                r = rand()%6;
                novoAst.m = models[r];
                novoAst.x = rand()%1160 + 100;
                novoAst.y = 0;
                insereListaAsteroide(&listaAsteroide, novoAst);
            }

            mostraAsteroides(&listaAsteroide);

            readimagefile(n.src,n.x,n.y,n.x+100,n.y+140); // DESENHA A NAVE

            verificaColisao(&listaAsteroide,&listaLaser);

            while (kbhit()) {
                in = getch();
            }

            switch(in) {
                case 'a':
                case 'A':
                    if (n.x>0)
                        n.x-=50;
                    break;
                case 'd':
                case 'D':
                    if (n.x<1260)
                        n.x+=50;
                    break;
                case 'l':
                case 'L':
                    if (time(NULL) - ultimo_laser > 0.25) {
                        ultimo_laser = time(NULL);
                        l.x=n.x;
                        l.y=n.y;
                        insereListaLaser(&listaLaser,l);
                        PlaySound(TEXT("assets/blaster.wav"), NULL, SND_ASYNC);
                        break;
                    }
            }

            in = ' ';
        }

        PlaySound(TEXT("assets/ending.wav"), NULL, SND_ASYNC);
        readimagefile("assets/background.jpg",0,0,1360,730);
        readimagefile("assets/earth_0.gif",380,110,980,710);
        readimagefile("assets/presstoexit.gif",380,10,980,185);
        settextstyle(3, HORIZ_DIR, 6);
        outtextxy(577,340,"SCORE");
        settextstyle(3, HORIZ_DIR, 6);
        outtextxy(613,388,pontuacao_aux);
        swapbuffers();

        do {
            if (kbhit())
                exitflag = getch();
        } while (exitflag!='c' && exitflag!='e');

        PlaySound(TEXT("assets/start.wav"), NULL, SND_ASYNC);
    }
}

void initModel(model *a, char *src, int resistencia, int pontuacao, int w, int h, float f) {
    strcpy(a->src,src);
    a->resistencia = resistencia;
    a->pontuacao = pontuacao;
    a->w = w;
    a->h = h;
    a->f = f;
}

void criaListaAsteroide(listalineardeAst *q)
{
    q->inicio = NULL;
    q->fim = NULL;
}

int  insereListaAsteroide(listalineardeAst *q, asteroide a)
{
    struct noAsteroide *aux;
    aux = (struct noAsteroide*) malloc(sizeof(struct noAsteroide));
    if (aux == NULL)
        return 0;

    if (q->inicio == NULL)
    {
        aux->a = a;
        aux->prox = NULL;
        aux->ant = NULL;
        q->inicio = aux;
        q->fim = aux;
        return 1;
    }

    aux->a = a;
    aux->prox = NULL;
    q->fim->prox = aux;
    aux->ant = q->fim;
    q->fim = aux;
    return 1;
}

void mostraAsteroides(listalineardeAst *q){
    struct noAsteroide *aux, *aux2;
    aux = q->inicio;
    while(aux!=NULL){
        if (aux->a.y >= getmaxy()) {
            vidas--;
            aux2 = aux->prox;
            removeeAsteroide(q, aux);
            aux = aux2;
            PlaySound(TEXT("assets/life_loss.wav"), NULL, SND_ASYNC);
        } else {
            aux->a.y += aux->a.m.f * speed + 1;
            readimagefile(aux->a.m.src,aux->a.x,aux->a.y,aux->a.x+aux->a.m.w,aux->a.y+aux->a.m.h);
            aux = aux->prox;
        }
    }
}

void clearAsteroides(listalineardeAst *q){
    struct noAsteroide *aux, *aux2;
    aux = q->inicio;
    while(aux!=NULL){
        aux2 = aux->prox;
        removeeAsteroide(q, aux);
        aux = aux2;
    }
}

int removeeAsteroide(listalineardeAst *q, struct noAsteroide *ast) {
    struct noAsteroide *aux, *atual, *frente;
    if(q->inicio==NULL){
        return 0;
    }
    if(q->inicio == ast){
        if(q->inicio == q->fim){
            aux = q->inicio;
            q->inicio = NULL; q->fim = NULL;
            free(aux);
            return 1;
        }
        else{
            aux = q->inicio;
            q->inicio = q->inicio->prox;
            q->inicio->ant = NULL;
            free(aux);
            return 1;
        }
    }
    if(q->fim == ast){
        aux = q->fim;
        q->fim = q->fim->ant;
        q->fim->prox = NULL;
        free(aux);
        return 1;
    }
    atual = q->inicio; frente = q->inicio->prox;
    while(atual != NULL){
        if(atual != ast){
            atual = frente;
            frente = frente->prox;
        }
        else break;
    }
    if(atual == q->fim){
        return 0;
    }
    aux = atual;
    atual->ant->prox = frente;
    frente->ant = atual->ant;
    free(aux);
    return 1;
}

void criaListaLaser(listalineardeLaser *q)
{
    q->inicio = NULL;
    q->fim = NULL;
}

int  insereListaLaser(listalineardeLaser *q, laser l)
{
    struct noLaser *aux;
    aux = (struct noLaser*) malloc(sizeof(struct noLaser));
    if (aux == NULL)
        return 0;

    if (q->inicio == NULL)
    {
        aux->l = l;
        aux->prox = NULL;
        aux->ant = NULL;
        q->inicio = aux;
        q->fim = aux;
        return 1;
    }

    aux->l = l;
    aux->prox = NULL;
    q->fim->prox = aux;
    aux->ant = q->fim;
    q->fim = aux;
    return 1;
}

void clearLasers(listalineardeLaser *q){
    struct noLaser *aux, *aux2;
    aux = q->inicio;
    while(aux!=NULL){
        aux2 = aux->prox;
        removeeLaser(q,aux);
        aux = aux2;
    }
}

int removeeLaser(listalineardeLaser *q, struct noLaser *laser) {
    struct noLaser *aux, *atual, *frente;
    if(q->inicio==NULL){
        return 0;
    }
    if(q->inicio == laser){
        if(q->inicio == q->fim){
            aux = q->inicio;
            q->inicio = NULL; q->fim = NULL;
            free(aux);
            return 1;
        }
        else{
            aux = q->inicio;
            q->inicio = q->inicio->prox;
            q->inicio->ant = NULL;
            free(aux);
            return 1;
        }
    }
    if(q->fim == laser){
        aux = q->fim;
        q->fim = q->fim->ant;
        q->fim->prox = NULL;
        free(aux);
        return 1;
    }
    atual = q->inicio; frente = q->inicio->prox;
    while(atual != NULL){
        if(atual != laser){
            atual = frente;
            frente = frente->prox;
        }
        else break;
    }
    if(atual == q->fim){
        return 0;
    }
    aux = atual;
    atual->ant->prox = frente;
    frente->ant = atual->ant;
    free(aux);
    return 1;
}

void verificaColisao(listalineardeAst *lAst, listalineardeLaser *lLaser){
    struct noLaser *auxL, *auxL2;
    struct noAsteroide *auxA, *auxA2;
    int totalWidthLaser;
    int totalWidthAsteroid;
    int totalHeightAsteroid;
    int colisao = 0;

    auxL = lLaser->inicio;
    while(auxL!=NULL){
        if (auxL->l.y < -30) {
			auxL2 = auxL->prox;
            removeeLaser(lLaser, auxL);
            auxL = auxL2;
        } else {
            auxL->l.y -= 20;
            readimagefile("assets/laser.gif",auxL->l.x,auxL->l.y,auxL->l.x+100,auxL->l.y+140);

			auxA = lAst->inicio;
            colisao = 0;
			while (auxA != NULL && colisao != 1) {
				totalWidthLaser = auxL->l.x + 100;
				totalWidthAsteroid = auxA->a.x + auxA->a.m.w;
				totalHeightAsteroid = auxA->a.y + auxA->a.m.h;

				if ((auxL->l.y <= totalHeightAsteroid) && (auxL->l.y >= auxA->a.y) &&
				   ((totalWidthLaser >= auxA->a.x)&&(auxL->l.x <= totalWidthAsteroid))) {
                    auxL2 = auxL->prox;
                    removeeLaser(lLaser, auxL);

                    auxA->a.m.resistencia -= 3;
                    PlaySound(TEXT("assets/explosion.wav"), NULL, SND_ASYNC);
                    if (auxA->a.m.resistencia <= 0) {
                        pontuacao += auxA->a.m.pontuacao;
                        auxA2 = auxA->prox;
                        removeeAsteroide(lAst, auxA);
                        auxA = auxA2;
                        PlaySound(TEXT("assets/explosion_loud.wav"), NULL, SND_ASYNC);
                    } else {
                        auxA = auxA->prox;
                    }

                    colisao = 1;
				} else {
                    auxA = auxA->prox;
                }
			}
            if (colisao == 0)
			    auxL = auxL->prox;
            else
                auxL = auxL2;
		}
    }
}

void clear_keyboard_buffer(void)
{
    int c = 0;
    while (c = getch()) {}
    return;
}
