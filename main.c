// I am currently attempting to build a basic tetris clone using my current knowledge of the C programming language.
// My motivation for doing this project is to learn and push myself to get a functioning personal project done. 
// The main problem that this project will tackle is making me a stronger and more proficient thinker.
// I have had a lot of fun and some difficulties throughout building this project and hope to get it done soon. 
// A further goal would be to optimize and implement this project on hardware.


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
int shapes[7][4][4] = 
{
   //I
   {{0,0,1,0},
    {0,0,1,0},
    {0,0,1,0},
    {0,0,1,0}},
   //Z
   {{0,0,1,0},
    {0,1,1,0},
    {0,1,0,0},
    {0,0,0,0}},
   //S
   {{0,1,0,0},
    {0,1,1,0},
    {0,0,1,0},
    {0,0,0,0}},
   //T
   {{0,1,0,0},
    {0,1,1,0},
    {0,1,0,0},
    {0,0,0,0}},
   //L
   {{0,1,0,0},
    {0,1,0,0},
    {0,1,1,0},
    {0,0,0,0}},
   //J
   {{0,0,1,0},
    {0,0,1,0},
    {0,1,1,0},
    {0,0,0,0}},
   //O
   {{0,0,0,0},
    {0,1,1,0},
    {0,1,1,0},
    {0,0,0,0}},

};

struct point
{
   int x, y;
}nCurrentPoint[4][4], nAxis;

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
   // for (int i = 0; i < 4; i++)
   // {
   //    nCurrentPoint[i].x = (shapes[sel][i] % 2) + 4;
   //    nCurrentPoint[i].y = shapes[sel][i] / 2;
   //    nTempField[nCurrentPoint[i].y][nCurrentPoint[i].x] = 1;
   // }
   for (int i = 0; i < 4; i++)
    {
        for (int j = 0; j < 4; j++)
        {  
            nCurrentPoint[i][j].x = 4+j;
            nCurrentPoint[i][j].y = 4+i;
            nTempField[nCurrentPoint[i][j].y][nCurrentPoint[i][j].x]=shapes[sel][i][j];
        }
    }
}

void lockShape()
{
   
}

//checks field array and prints '. ' if 0 and '# ' if otherwise
void outputShape(WINDOW *w, int sel)
{
   for (int i = 0; i < 4; i++)
   {
      for (int j = 0; j < 4; j++)
      {  
         (shapes[sel][i][j] == 1) ? (mvwaddch(w, nCurrentPoint[i][j].y, nCurrentPoint[i][j].x*2, '#')) : 0;
      }
   }
   
   wrefresh(w);
}

bool borderCheck(int shape)
{
   for (int i = 0; i < 4; i++)
   {
      for (int j = 0; j < 4; j++)
      {  
         if((nCurrentPoint[i][j].y == nHeight-1 || nCurrentPoint[i][j].x > nWidth-1 || nCurrentPoint[i][j].x < 0) && shape[i][j] == 1)
         {
            return 1;
         }
      }
   }
   return 0;
}

bool collisionCheck()
{
   for (int i = 0; i < 4; i++)
   {
      for (int j = 0; j < 4; j++)
      { 
         if(nField[nCurrentPoint[i][j].y][nCurrentPoint[i][j].x])
         {
            return 1;
         }
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
            for (int j = 0; j < 4; j++)
            { 
               nCurrentPoint[i][j].x--;
            }
         }
         break;
      case 'd':
         for (int i = 0; i < 4; i++)
         {
            for (int j = 0; j < 4; j++)
            { 
               nCurrentPoint[i][j].x++;
            }
         }
         break;
      case 's':
         for (int i = 0; i < 4; i++)
         {
            for (int j = 0; j < 4; j++)
            { 
               nCurrentPoint[i][j].y++;
            }
         }
         break;
      case 'l':
         
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
         outputShape(win, nCurrentID);
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

//resources:

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