#include <graphics.h>
#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <Windows.h>
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
void removeeAsteroide(listalineardeAst *q, struct noAsteroide *ast);

void criaListaLaser(listalineardeLaser *q);
int  insereListaLaser(listalineardeLaser *q, laser l);
void mostraLasers(listalineardeLaser *q);
void removeeLaser(listalineardeLaser *q, struct noLaser *laser);

void verificaColisao(listalineardeAst *lAst, listalineardeLaser *lLaser);

int speed = 8, vidas = 3, pontuacao = 0;

main()
{
    int i, ultimo_ast, ultimo_laser, r;
    int gdriver = EGA, gmode = EGAHI; // MODO GR�FICO PARA SUPORTE DE M�LTIPLAS P�GINAS
    char in;
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

    readimagefile("assets/background.jpg",0,0,1360,730);
    readimagefile("assets/title.gif",280,200,1080,422);
    readimagefile("assets/presstocontinue.gif",480,440,880,520);
    readimagefile("assets/dir.gif",1120,570,1320,680);

    do {
        if (kbhit())
            in = getch();
    } while (in != ' ');

    srand(clock());
    ast.m = models[rand()%6];
    ast.x = rand()%1160 + 100;
    ast.y = 0;
    insereListaAsteroide(&listaAsteroide, ast);
    ultimo_ast = time(NULL);
    ultimo_laser = time(NULL);

    while(vidas > 0) {
        swapbuffers();

        readimagefile("assets/background.jpg",0,0,1360,730);

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

        // PERCORRER A LISTA DE ASTER�IDES:
            // AUMENTAR Y EM 10 UNIDADES (#1)
            // LER A IMAGEM DO MODELO (#0)
            // SE Y > 730 (altura da tela)
                // remover asteroide da lista
        mostraAsteroides(&listaAsteroide);

        readimagefile(n.src,n.x,n.y,n.x+100,n.y+140); // DESENHA A NAVE

        // PERCORRER A LISTA DE LASERS
            // decrementar 1 unidade do y do laser
            // PERCORRER A LISTA DE ASTEROIDES
                // p(a) = POSI��O DO ASTEROIDE a (x,y)
                // p(l) = POSI��O DO ASTEROIDE l (x,y)
                // SE [0 < p(a).x - p(l).x < a.w] && [0 < p(a).y - p(l).y < a.h]
                    // remover 'a' da lista
                    // somar a.m.pontuacao � pontua��o total
        verificaColisao(&listaAsteroide,&listaLaser);

        if (kbhit())
            in = getch();

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
                // CRIAR LASER E ADICIONAR � LISTA NA POSI��O (n.x,537)
                // 537 = n.y - 43, altura dos canh�es
                if (time(NULL) - ultimo_laser > 0.5) {
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

    readimagefile("assets/background.jpg",0,0,1360,730);
    readimagefile("assets/title.gif",280,200,1080,422);
    readimagefile("assets/presstocontinue.gif",480,440,880,520);
    readimagefile("assets/dir.gif",1120,570,1320,680);

    getch();
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
    struct noAsteroide *aux;
    aux = q->inicio;
    while(aux!=NULL){
        if (aux->a.y > getmaxy()) {
            vidas--;
            removeeAsteroide(q, aux);
        } else {
            aux->a.y += aux->a.m.f * speed + 1;
            readimagefile(aux->a.m.src,aux->a.x,aux->a.y,aux->a.x+aux->a.m.w,aux->a.y+aux->a.m.h);
        }

        aux = aux->prox;
    }
}

void removeeAsteroide(listalineardeAst *q, struct noAsteroide *ast) {
    if (q->inicio == ast) {
        q->inicio = ast->prox;
        free(ast);
    } else if (q->fim == ast) {
        q->fim = ast->ant;
        free(ast);
    } else {
        ast->ant->prox = ast->prox;
        ast->prox->ant = ast->ant;
        free(ast);
    }
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
    aux->ant = q->fim;
    q->fim->prox = aux;
    q->fim = aux;
    return 1;
}

void removeeLaser(listalineardeLaser *q, struct noLaser *laser) {
    if (q->inicio == laser) {
        q->inicio = laser->prox;
        free(laser);
    } else if (q->fim == laser) {
        q->fim = laser->ant;
        free(laser);
    } else {
        laser->ant->prox = laser->prox;
        laser->prox->ant = laser->ant;
        free(laser);
    }
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
