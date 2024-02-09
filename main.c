#include <stdio.h>
#include <ncurses.h>
#include <time.h>
#include <stdlib.h>

//globals
const int nWidth = 10;
const int nHeight = 20;
int nField[20][10] = {0}; //set field to all zeroes
int nTempField[20][10] = {0};
char cKeyPress;
bool bRotate = 0;

// Shape implementation [block ID][4 blocks]:
int shapes[7][4] = 
{
   1,3,5,7, //I
   2,4,5,7, //Z
   3,5,4,6, //S
   3,5,4,7, //T
   2,3,5,7, //L
   3,5,7,6, //J
   2,3,4,5  //O
};

struct point
{
   int x, y;
}nCurrentPoint[4], nRotatedPoint[4], nAxis;

void setField(WINDOW *wCurrent)
{
    for (int i = 0; i < nHeight; ++i)
    {
        for (int j = 0; j < nWidth; ++j)
        {  
            if(nField[i][j])
            {
               mvwaddch(wCurrent, i, j*2, '#'); 
            }
            else
            {
               mvwaddch(wCurrent, i, j*2, '.'); 
            }
            
        }
    }
    wrefresh(wCurrent);
}

//compute the location of the cells in a selected shape
void spawn(int sel)
{
   for (int i = 0; i < 4; i++)
   {
      nCurrentPoint[i].x = (shapes[sel][i] % 2) + 4;
      nCurrentPoint[i].y = shapes[sel][i] / 2;
      nTempField[nCurrentPoint[i].y][nCurrentPoint[i].x] = 1;
   }
}

void lockShape()
{
   for (int i = 0; i < 4; i++)
   {
      nField[nCurrentPoint[i].y][nCurrentPoint[i].x] = nTempField[nCurrentPoint[i].y][nCurrentPoint[i].x];
   }
   for (int i = 0; i < nHeight; ++i)
    {
        for (int j = 0; j < nWidth; ++j)
        {
            nTempField[i][j] = nField[i][j];
        }
    }
}

//checks field array and prints '. ' if 0 and '# ' if otherwise
void outputShape(WINDOW *w)
{
   for (int i = 0; i < 4; i++)
   {
      mvwaddch(w, nCurrentPoint[i].y, nCurrentPoint[i].x*2, '#');
   }
   wrefresh(w);
}

bool borderCheck()
{
   for (int i = 0; i < 4; i++)
   {
      if(nCurrentPoint[i].y == nHeight-1 || nCurrentPoint[i].x > nWidth-1 || nCurrentPoint[i].x < 0)
      {
         return 1;
      }
   }
   return 0;
}

bool collisionCheck()
{
   for (int i = 0; i < 4; i++)
   {
      if(nField[nCurrentPoint[i].y][nCurrentPoint[i].x])
      {
         return 1;
      }
   }
   return 0;
}

void input(char press, int sel)
{
   if(borderCheck())
   {
      return;
   }
   switch(press)
   {
      case 'a': 
         for (int i = 0; i < 4; i++)
         {
            nCurrentPoint[i].x--;
            nTempField[nCurrentPoint[i].y][nCurrentPoint[i].x] = 1;
         }
         break;
      case 'd':
         for (int i = 0; i < 4; i++)
         {
            nCurrentPoint[i].x++;
            nTempField[nCurrentPoint[i].y][nCurrentPoint[i].x] = 1;
         }
         break;
      case 's':
         for (int i = 0; i < 4; i++)
         {
            nCurrentPoint[i].y++;
            nTempField[nCurrentPoint[i].y][nCurrentPoint[i].x] = 1;
         }
         break;
      case 'l':
         nAxis = nCurrentPoint[1];
         if(bRotate)
         {
            for (int i = 0; i < 4; i++)
            { 
               nCurrentPoint[i].x = shapes[sel][i] / 2;
               nCurrentPoint[i].y = shapes[sel][i] % 2;
               
            }
         }
         else
         {
            for (int i = 0; i < 4; i++)
            { 
               nCurrentPoint[i].x = shapes[sel][i] % 2;
               nCurrentPoint[i].y = shapes[sel][i] / 2;
            }
         }
         bRotate = !bRotate;
         break;

      default:
         break;
   }
}

int main()
{
   bool bGameOver = 0;
   int nCurrentID = 0;
   int nTimer = 250;
   bool bRotate = 1;
   
   //clear screen
   initscr();
   WINDOW *win = newwin(20, 20, 5, 10); //primary window for housing the field
   WINDOW *curwin = newwin(20, 20, 5, 10); //secondary window for only updating the four tiles that will change
   refresh();
   noecho();
   timeout(50);

   //Game loop: for-loop that will run the game
   while (!bGameOver)
   {
      //clock: this will dictate how fast the game ticks
      //outputShape: screen, use ncurses
      
      
      
      //LOGIC=======================================
      //spawn peice
      srand(time(NULL));
      nCurrentID = rand() % 7;
      spawn(nCurrentID);
      //Fall sequence
      while(!borderCheck())
      {
         setField(win);
         outputShape(win);
         input(getch(), nCurrentID);
         // //Input: Left, Right, Down, Rotate
         // 
         // setField(win);
         // //fall: nHeight -1
         // for (int i = 0; i < 4; i++)
         // {
         //    if(!borderCheck())
         //    {
         //       nCurrentPoint[i].y++;
         //    }
         //    outputShape(curwin);
         //    wrefresh(win);
         // }
         
         // //check for collision
         
         // napms(nTimer); //(milliseconds)
         // //update
         // lockShape();
         // mvprintw(30, 30, "ID: %d", nCurrentID);
      
      //check for lines
      }
         

         
      //============================================      
      
      mvprintw(30, 30, "yeet ");
      
      // break;
   }
   
   endwin();

   
   return 0;
}

//gcc main.c -lncurses

//Rotation of matrix:
//https://www.enjoyalgorithms.com/blog/cr-a-matrix-by-90-degrees-in-an-anticlockwise-direction
//Random Int:
//https://stackoverflow.com/questions/822323/how-to-generate-a-random-int-in-c
//Return function:
//https://stackoverflow.com/questions/346613/how-do-you-exit-from-a-void-function-in-c
//Code it yourself tetris:
//https://www.youtube.com/watch?v=8OK8_tHeCIA
//Spawan location and orientation
//https://www.google.com/search?client=firefox-b-1-d&sca_esv=600142611&sxsrf=ACQVn0_bdoky6AFH1N70A928m0h_Y6k6TA:1705801129395&q=where+do+pieces+spawn+in+tetris&tbm=isch&source=lnms&sa=X&ved=2ahUKEwibsf-yrO2DAxXglGoFHeGdAcIQ0pQJegQICxAB&biw=1920&bih=927&dpr=1#imgrc=s0KJVclVSPe6tM
//github reference
//https://github.com/embeddedmz/16-Games/blob/master/01%20Tetris/main.cpp