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


// Shape implementation [block ID][4 blocks][4 blocks]:
int shapes[7][4][4] = 
{
   //I
   {{0,0,1,0},
    {0,0,1,0},
    {0,0,1,0},
    {0,0,1,0}},
   //Z
   {{0,1,0,0},
    {1,1,0,0},
    {1,0,0,0},
    {0,0,0,0}},
   //S
   {{1,0,0,0},
    {1,1,0,0},
    {0,1,0,0},
    {0,0,0,0}},
   //T
   {{0,1,0,0},
    {1,1,0,0},
    {0,1,0,0},
    {0,0,0,0}},
   //L
   {{0,1,0,0},
    {0,1,0,0},
    {0,1,1,0},
    {0,0,0,0}},
   //J
   {{0,1,0,0},
    {0,1,0,0},
    {1,1,0,0},
    {0,0,0,0}},
   //O
   {{1,1,0,0},
    {1,1,0,0},
    {0,0,0,0},
    {0,0,0,0}},

};

int dimension[7] = {4, 3, 3, 3, 3, 3, 2};

struct point
{
   int x, y;
}nCurrentPoint[4], nAxis;


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
    //wrefresh(wCurrent);
}

//compute the location of the cells in a selected shape
void spawn(int sel)
{
   int nCount = 0;
   for (int i = 0; i < dimension[sel]; i++)
   {
      for (int j = 0; j < dimension[sel]; j++)
      {  
         if(shapes[sel][i][j])
         {
            nCurrentPoint[nCount].x = j+3;
            nCurrentPoint[nCount].y = i;
            nTempField[nCurrentPoint[i].y][nCurrentPoint[i].x]=shapes[sel][i][j];
            nCount++;
         }
      }
    }
}

void lockShape()
{
   for (int i = 0; i < nHeight; i++)
   {
      for (int j = 0; j < nWidth; j++)
      {  
         nField[i][j] = nTempField[i][j];
         nTempField[i][j] = nField[i][j];
      }
   }
}

//checks field array and prints '. ' if 0 and '# ' if otherwise
void outputShape(WINDOW *w, int sel)
{
   for (int i = 0; i < 4; i++)
   {
      mvwaddch(w, nCurrentPoint[i].y, nCurrentPoint[i].x*2, '#');
   }
   
   wrefresh(w);
}

bool borderCheck(int sel)
{
   for (int i = 0; i < 4; i++)
   {
      if(nCurrentPoint[i].x >= 0 || nCurrentPoint[i].x < nWidth || nCurrentPoint[i].y >= nHeight)
      {
         return true;
      }
   }
   return false;
}


void input(char press, int sel)
{
   bool bCanGo = 1;
   switch(press)
   {
      case 'a':
         for (int i = 0; i < 4; i++)
         {
            if(nCurrentPoint[i].x == 0)
            {
               bCanGo = 0;
            }
         }
         if(bCanGo)
         {
            for (int i = 0; i < 4; i++)
            {
               nCurrentPoint[i].x--;
            }
         }
         break;
      case 'd':         
         for (int i = 0; i < 4; i++)
         {
            if(nCurrentPoint[i].x == nWidth-1)
            {
               bCanGo = 0;
            }
         }
         if(bCanGo)
         {
            for (int i = 0; i < 4; i++)
            {
               nCurrentPoint[i].x++;
            }
         }
         break;
      case 's':
         for (int i = 0; i < 4; i++)
         {
            // if(nCurrentPoint[i].y == nHeight-1)
            // {
            //    return;
            // }
            nCurrentPoint[i].y++;
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
      while(nCurrentPoint[3].y <= nHeight-1)
      {
         setField(win);
         outputShape(win, nCurrentID);
         input(getch(), nCurrentID);  //Input: Left, Right, Down, Rotate
         if(!borderCheck(nCurrentID))
         {
            break;
         }
         
         // //check for collision
         
         // napms(nTimer); //(milliseconds)
         // //update
         
         // mvprintw(30, 30, "ID: %d", nCurrentID);
      //check for lines
      }
         
      //lockShape();
      
      //============================================      
      
      mvprintw(30, 30, "yeet ");
      
      // break;
   }
   
   endwin();

   
   return 0;
}

//gcc main.c -lncurses
//cc -g main.c -lncurses (gcc)

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