#include <graphics.h>
#include <stdio.h>

typedef struct {
    char src[30];
    int resistencia;
    int pontuacao;
} asteroide;

typedef struct {
    int x;
    int y;
    asteroide a;
} asteroideObj;

typedef struct {
    char src[30];
    int x;
    int y;
} nave;

void initAsteroide(asteroide *a, char *src, int resistencia, int pontuacao);

main()
{
    int i, pontuacao, vidas = 3;
    char in;
    asteroide asteroides[6];
    nave n = {"assets/ship.gif",630,580};
    asteroideObj ast;

    initAsteroide(&asteroides[0],"assets/asteroide1.gif",3, 1);
    initAsteroide(&asteroides[1],"assets/asteroide2.png",6, 3);
    initAsteroide(&asteroides[2],"assets/asteroide3.png",9, 6);
    initAsteroide(&asteroides[3],"assets/asteroide4.png",12, 10);
    initAsteroide(&asteroides[4],"assets/asteroide5.png",15, 15);
    initAsteroide(&asteroides[5],"assets/asteroide6.png",25, 25);

    initwindow(1360,730);
    readimagefile("assets/background.jpg",0,0,1360,730);
    //readimagefile(n.src,n.x,n.y,n.x+100,n.y+140);

    readimagefile("assets/title.gif",280,200,1080,422);
    readimagefile("assets/presstocontinue.gif",480,440,880,520);

    do {
        in = getch();
    } while (in != ' ');

    ast.a = asteroides[0];
    ast.x = 200;
    ast.y = 0;
    while(1) {
        readimagefile("assets/background.jpg",0,0,1360,730);
        readimagefile(ast.a.src,ast.x,ast.y,ast.x+30,ast.y+30);
        readimagefile(n.src,n.x,n.y,n.x+100,n.y+140); // DESENHA A NAVE
        ast.y += 10;

        if (kbhit()) {
            in = getch();
        }

        switch(in) {
            case 'a':
            case 'A':
                n.x-=50;
                break;
            case 'd':
            case 'D':
                n.x+=50;
                break;
        }

        in = ' ';
        Sleep(100);
    }
}

void initAsteroide(asteroide *a, char *src, int resistencia, int pontuacao) {
    strcpy(a->src,src);
    a->resistencia = resistencia;
    a->pontuacao = pontuacao;
}
