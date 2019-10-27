//#define CATCH_CONFIG_MAIN  // This tells Catch to provide a main() - only do this in one cpp file
//#include "catch.hpp"
#include <iostream>
#include <stdio.h>
#include <cstdio>
#include <cstdlib>
#include <stdlib.h>
#include <windows.h>
#include <conio.h>
#include <list>

#define UP 72
#define LEFT 75
#define RIGHT 77
#define DOWN 80

using namespace std;

int puntos=0;

//MODELO
class GameData
{
    int x, y;

    unsigned int opcion;
    HANDLE hCon;
    COORD dwPos;
    CONSOLE_CURSOR_INFO cci;

public:
    void gotoxy (int x, int y)
    {
        this->hCon = GetStdHandle(STD_OUTPUT_HANDLE);
        this->dwPos.X=x;
        this->dwPos.Y=y;

        SetConsoleCursorPosition(hCon, dwPos);
    }

    void HideCursor ()
    {
        this->hCon = GetStdHandle(STD_OUTPUT_HANDLE);

        this->cci.dwSize=2;
        this->cci.bVisible=FALSE;

        SetConsoleCursorInfo(hCon, &cci);
    }
};

class NAVE : public GameData
{
    int x, y;
    int heart;
    int life;

public:
    NAVE (int nax, int nay)
    {
        this->x=nax;
        this->y=nay;
        this->heart=3;
        this->life=3;
    }
    void pintar()
    {
        gotoxy(x,y);
        printf("  %c", 94);
        gotoxy(x, y+1);
        printf(" %c%c%c", 47, 254, 92);
        gotoxy(x, y+2);
        printf(" %c %c", 223, 223);
    }
    void borrar()
    {
        gotoxy(x, y);
        printf("      ");
        gotoxy(x, y+1);
        printf("      ");
        gotoxy(x, y+2);
        printf("      ");
    }
    void mover()
    {
        if (kbhit())
        {
            char tecla=getch();
            borrar();

            if(tecla==LEFT && x>3)
            {
                x--;
            }
            else if (tecla==RIGHT && x+6<117)
            {
                x++;
            }
            else if (tecla==UP && y>4)
            {
                y--;
            }
            else if (tecla==DOWN && y+3<31)
            {
                y++;
            }
            pintar();
            SeeHearts();
        }
        Sleep(30);
    }
    void SeeHearts()
    {
        gotoxy(100,2);
        printf("SALUD");
        gotoxy(106,2);
        printf("      ");

        for(int i=0; i<heart; i++)
        {
            gotoxy(106+i,2);
            printf("%c",207);
        }
        gotoxy(80,2);
        printf("VIDAS %d",life);
    }
    void Die()
    {
        if (heart==0)
        {
            borrar();
            gotoxy(x,y);
            printf("******");
            gotoxy(x,y+1);
            printf(" BOOM ");
            gotoxy(x,y+2);
            printf("******");
            Sleep(200);
            borrar();

            life--;
            heart=3;
            SeeHearts();;
            pintar();
        }
    }
    void hearts()
    {
        this->heart--;
    }
    int getLife()
    {
        return life;
    }
    int getX()
    {
        return x;
    }
    int getY()
    {
        return y;
    }
};
class AST : public GameData
{
    int x, y;

public:
    AST (int asx, int asy)
    {
        this->x=asx;
        this->y=asy;
    }
    void paint()
    {
        gotoxy(x,y);
        printf("%c",184);
    }
    void moveit()
    {
        gotoxy(x,y);
        printf("  ");
        y++;
        if(y>30)
        {
            x=rand()%113+3;
            y=4;
        }
        paint();
    }
    void crash(NAVE &N)
    {
        if(x>=N.getX() && x<=N.getX()+4 && y>=N.getY() && y<=N.getY()+2)
        {
            N.hearts();
            N.borrar();
            N.pintar();
            N.SeeHearts();

            x=rand()%113+3;
            y=4;
        }
    }
    int getX()
    {
        return x;
    }
    int getY()
    {
        return y;
    }
};

class BULLET : public GameData
{
    int x, y;
public:
    BULLET(int bux, int buy)
    {
        this->x=bux;
        this->y=buy;
    }
    void mover ()
    {
        gotoxy(x, y);
        printf("  ");
        y--;
        gotoxy(x,y);
        printf("*");
    }
    bool fuera()
    {
        if(y==4)
        {
            return true;
        }
        else
        {
            return false;
        }
    }
    int getX()
    {
        return x;
    }
    int getY()
    {
        return y;
    }
};

/*TEST_CASE( "evaluar clase NAVE y BULLET", "[NAVE]" ) {
    NAVE ship= NAVE (4,5);

    REQUIRE(ship.getLife() == 3 );
    REQUIRE(ship.getX() == 4 );
    REQUIRE(ship.getY() == 5 );

    BULLET bala = BULLET (3,2);

    REQUIRE( bala.fuera() == false );
    REQUIRE( bala.getX() == 3 );
    REQUIRE( bala.getY() == 2 );
}*/


//CONTROLLER
class PlayGame
{
    GameData game;
public:
    void Jugar()
{
    for (int i=3; i<117; i++)
    {
        game.gotoxy(48,13);
        printf("                             ");
        game.gotoxy(45,18);
        printf("                                   ");
        game.gotoxy(54,20);
        printf("        ");
        game.gotoxy(54,22);
        printf("                  ");
        game.gotoxy(54,24);
        printf("        ");
        game.gotoxy(54,28);
        printf("                 ");
    }

    int px=59, py=28;

    NAVE ship = NAVE(px,py);
    game.gotoxy(px, py);
    ship.pintar();
    ship.SeeHearts();

    list <AST*> AstList;
    list <AST*> ::iterator itA;

    for (int i=0; i<10; i++) //creacion de asteroides
    {
        AstList.push_back(new AST(rand()%113+3, rand()%5+4));
    }

    bool game_over=false;

    list <BULLET*> Bl;
    list <BULLET*> ::iterator itb;

    while(!game_over)
    {
        if (kbhit())
        {
            char btecla=getch();
            if(btecla==32)
            {
                Bl.push_back(new BULLET(ship.getX()+2, ship.getY()-1)); //disparar balas
            }
            else if (btecla=='s' || btecla=='S')
            {
                game_over=true;
            }
        }
        for(itb=Bl.begin(); itb!=Bl.end(); itb++) //eliminar balas de la lista y de pantalla una vez que lleguen al limite
        {
            (*itb)->mover();
            if ((*itb)->fuera())
            {
                game.gotoxy((*itb)->getX(), (*itb)->getY());
                printf("  ");
                delete(*itb);
                itb=Bl.erase(itb);
            }
        }

        for (itA=AstList.begin(); itA!=AstList.end(); itA++)
        {
            (*itA)->moveit();
            (*itA)->crash(ship);
        }

        for (itA=AstList.begin(); itA!=AstList.end(); itA++) //choque bala y asteroide
        {
            for (itb=Bl.begin(); itb!=Bl.end(); itb++)
            {
                if ((*itA)->getX()==(*itb)->getX() && ((*itA)->getY()+1==(*itb)->getY() || (*itA)->getY()==(*itb)->getY()))
                {
                    game.gotoxy((*itb)->getX(), (*itb)->getY());
                    printf("  ");
                    delete(*itb);
                    itb = Bl.erase(itb);

                    AstList.push_back(new AST(rand()%113+3, 4));

                    game.gotoxy((*itA)->getX(), (*itA)->getY());
                    printf("  ");
                    delete(*itA);
                    itA = AstList.erase(itA);

                    puntos+=5;
                }
            }
        }

        game.gotoxy(4,2);
        printf("PUNTOS %d", puntos);

        ship.Die();
        ship.mover();
        Sleep(30);

        if (ship.getLife()==0)
        {
            game_over=true;
        }
    }

}

};


//VIEW

class GameView
{
    int x, y;
public:

    void DrawLimits ()
    {
        GameData game;
        for (int i=2; i<118; i++)
        {
            game.gotoxy(i,3);
            printf("%c", 205);
            game.gotoxy(i,31);
            printf("%c", 205);
        }
        for (int i=4; i<31; i++)
        {
            game.gotoxy(2,i);
            printf("%c", 186);
            game.gotoxy(117, i);
            printf("%c", 186);
        }
        game.gotoxy(2,3);
        printf("%c", 201);
        game.gotoxy(2,31);
        printf("%c", 200);
        game.gotoxy(117,3);
        printf("%c", 187);
        game.gotoxy(117,31);
        printf("%c", 188);

        game.gotoxy(2,32);
        cout<<"Utilice las flechas para mover la nave y la barra espaciadora para disparar";
        game.gotoxy(93,32);
        cout<<"Presione 's' para salir";
    }
    void MenuGame ()
    {
        GameData game;
        PlayGame jugar;

        DrawLimits();

        unsigned int opcion;
        do
        {
            for (int i=3; i<117; i++)
            {
                game.gotoxy(48,13);
                printf("A T A Q U E   E S P A C I A L");
                game.gotoxy(45,18);
                printf("MENU (elija una opcion por teclado)");
                game.gotoxy(50,16);
                printf("                  ");
                game.gotoxy(54,20);
                printf("1- JUGAR");
                game.gotoxy(54,22);
                printf("2- SALIR");

                game.gotoxy(54,28);
                printf("Ingrese opcion: ");
            }
            cin>>opcion;



        }while(opcion<0 || opcion>2);

        switch(opcion)
        {
            case 1: jugar.Jugar();
                    EndScreen();
                    break;
            case 2:
                    break;
        }
    }
    void EndScreen()
    {
        GameData game;

        system ("cls");

        for (int i=3; i<117; i++)
        {
            game.gotoxy(i,4);
            printf("*");
            game.gotoxy(i,30);
            printf("*");
            game.gotoxy(52,13);
            printf("G A M E  O V E R");

            game.gotoxy(50,16);
            printf("PUNTOS TOTALES: %d", puntos);

        }
        game.gotoxy(50,18);
        printf("Para continuar, presione cualquier tecla");
        getche();
        system ("cls");
        MenuGame();
        game.gotoxy(118,32);
    }
};


int main()
{
    PlaySound(TEXT("DWT.wav"), NULL, SND_LOOP|SND_ASYNC);

    char opcion;
    GameData Game;
    GameView Vista;

    Game.HideCursor();

    Vista.MenuGame();

    return 0;
}
