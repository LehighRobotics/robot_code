#include <MemoryFree.h>
int counter = 0;
byte Lx = 0; // Current X coordinate
byte Ly = 0; // Current Y coordinate
const int Size = 7;
byte centMin = 255; // The smallest index in the center square that has been found so far (the LENGTH of the current shortest path)

//TODO: continue writing methods to manipulate squareData and replace old arrays
byte squareData[Size + 1][Size + 1]; //0 - Explored, 1 and 2 - Parent Direction, 3, 4, 5, 6 - Walls (N, S, E, W) 7 - virtual walls on/off
//vvv between is being replaced by above line
byte I[Size+1][Size+1]; // Array of indicies
boolean E[Size+1][Size+1]; // Array of explored locations

boolean Wn[Size+1][Size+1]; // Array of northern walls
boolean Ws[Size+1][Size+1]; // Array of southern walls
boolean Ww[Size+1][Size+1]; // Array of western walls
boolean We[Size+1][Size+1]; // Array of eastern walls

boolean vWn[Size+1][Size+1];
boolean vWs[Size+1][Size+1];
boolean vWw[Size+1][Size+1];
boolean vWe[Size+1][Size+1];

byte P[Size+1][Size+1];
//^^^
byte whereToGo[2];
byte finalCenterSquare[2]; //holds the destination final center square
byte breadCrumbs[((Size+1)^2)];
byte finishToStartDirections[((Size+1)^2)];

// I is the horizontal component of the matrix definition and J is the vertical component. The origin is defined as the bottom left corner.

byte testSize = Size;

//const boolean testWn[8][8] = {
//  { 
//    0,0,0,0,0,0,0,0      } 
//  ,
//  {
//    1,0,0,1,1,0,1,0      } 
//  ,
//  {
//    0,0,0,0,0,1,0,0      } 
//  ,
//  {
//    1,0,1,0,0,0,0,0      } 
//  ,
//  {
//    1,0,1,0,1,1,0,0      } 
//  ,
//  {
//    1,1,0,1,0,0,0,0      } 
//  ,
//  {
//    1,1,1,0,0,0,0,0      } 
//  ,
//  {
//    0,0,0,1,0,1,0,0      }
//};
///
//const boolean testWw[8][8] ={
//  {
//    0,0,0,0,0,0,0,0        }
//  ,
//  {
//    1,0,1,0,0,1,0,0        }
//  ,
//  {
//    0,1,0,0,1,0,0,1        }
//  ,
//  {
//    0,1,0,1,1,1,0,0        }
//  ,
//  {
//    0,1,0,0,0,0,0,0      }
//  ,
//  {
//    0,0,0,1,1,0,1,1      }
//  ,
//  {
//    0,0,0,0,1,1,0,1      }
//  ,
//  {
//    0,1,0,1,0,0,0,1      }
//};
//


//revised test wall north
//const boolean testWn[16][16] = {
//{0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0},
//{0,0,0,0,0,0,0,1,0,1,0,0,1,0,0,1},
//{1,1,1,0,0,0,0,0,1,0,1,0,0,1,0,1},
//{1,1,1,1,1,1,0,0,0,0,0,0,1,0,1,1},
//{1,0,0,0,1,1,0,0,0,0,0,1,1,0,1,1},
//{0,1,0,0,1,0,0,0,0,0,0,0,1,1,1,1},//5
//{1,0,1,0,1,1,0,0,0,1,1,1,1,1,1,1},
//{0,1,0,1,0,1,1,0,1,0,1,1,1,1,1,1},
//{1,0,1,0,1,0,1,0,0,1,1,1,1,0,0,1},
//{1,1,0,1,0,1,0,1,0,1,1,1,0,0,0,1},//9
//{1,1,1,0,0,0,1,0,1,0,1,1,1,1,1,1},
//{1,1,1,0,1,0,0,0,0,0,0,1,1,1,1,1},
//{1,1,1,1,0,1,0,1,0,0,0,1,1,1,1,1},//12
//{1,1,1,0,0,0,0,0,1,1,0,1,0,0,1,1},
//{1,0,0,0,0,0,0,0,1,0,0,0,0,0,0,1},
//{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1}};
//
//
//
//const boolean testWw[16][16] = {
//  {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},
//{1,1,0,0,1,1,1,1,1,0,1,1,1,1,1,0},
//{0,0,1,1,1,1,1,0,0,0,0,1,0,0,1,1},
//{0,0,0,0,0,1,1,1,1,1,1,0,1,0,0,0},
//{0,0,0,1,0,0,0,1,1,1,0,1,0,1,1,0},//3
//{0,1,1,1,1,0,1,1,1,1,1,1,0,0,0,0},
//{0,1,0,1,0,0,1,1,1,1,0,0,0,0,0,0},//5
//{0,0,1,0,1,0,0,1,1,0,0,0,0,0,0,0},
//{0,1,0,1,0,1,0,0,0,1,0,0,0,0,0,0},
//{0,0,1,0,1,0,1,1,1,0,0,0,0,1,1,0},//8
//{0,0,0,0,0,1,0,0,0,1,0,0,0,1,0,0},
//{0,0,0,0,1,0,1,1,1,0,1,0,0,0,0,0},
//{0,0,0,0,0,1,0,0,1,1,1,0,0,0,0,0},//11
//{0,0,0,0,1,0,1,1,0,0,1,1,0,0,0,0},
//{0,0,0,1,1,1,1,1,1,0,1,0,1,1,1,0},//13
//{0,1,1,1,1,1,1,1,0,1,1,1,1,1,1,0},
//};



////decker maze
//const boolean testWn[16][16] = {
//{0,0,0,0,0,1,0,1,0,0,0,1,0,0,1,1},
//{0,0,0,0,1,0,1,1,0,0,0,0,0,0,0,1},
//{0,0,0,1,0,1,0,0,0,0,1,0,0,1,0,1},
//{1,0,0,1,1,1,0,0,0,1,1,1,0,1,0,1},
//{0,0,1,0,1,1,0,1,1,1,1,1,1,1,1,1},
//{1,1,1,0,1,0,0,1,1,1,1,1,1,1,1,1},
//{1,1,1,0,1,1,0,1,0,0,0,1,1,0,1,1},//6
//{1,1,1,1,1,1,0,0,1,0,0,0,0,0,1,1},
//{1,1,1,1,1,1,1,0,1,0,0,0,0,1,1,1},
//{1,0,1,0,1,1,0,0,0,0,0,0,0,0,0,1},
//{1,1,1,1,1,0,0,0,1,1,1,0,1,0,0,1},
//{1,1,0,0,0,0,0,0,0,1,1,0,0,0,0,1},
//{0,0,0,1,1,1,1,1,0,0,0,0,1,0,0,1},
//{1,1,0,1,1,1,0,0,0,0,0,0,1,0,0,1},//13
//{1,0,0,1,0,0,0,0,0,0,0,0,1,0,0,1},
//{0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1}
//};
//
////decker maze
//const boolean testWw[16][16] = {
// 
//  {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},
//  {1,1,1,0,1,0,0,1,0,1,1,1,1,1,0,0},
//  {0,1,1,1,1,0,0,0,1,1,0,0,1,1,1,0},
//  {0,1,1,0,1,0,1,1,1,1,1,0,1,0,1,0},
//  {0,1,1,0,0,1,1,0,0,0,0,0,0,0,0,0},
//  {0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0},
//  {0,0,0,0,1,0,1,0,0,0,0,0,0,0,0,0},
//  {0,0,0,0,0,0,0,1,1,0,1,1,0,1,1,0},
//  {0,1,0,1,0,0,0,0,0,1,1,1,1,0,0,0},
//  {0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,0},
//  {0,0,0,0,0,0,1,1,0,1,0,0,0,1,1,0},
//  {0,1,0,0,1,1,1,1,1,0,0,0,0,1,0,1},
//  {0,0,0,1,0,0,0,0,0,1,1,1,0,1,1,0},
//  {0,1,1,0,0,0,1,1,0,1,0,1,1,0,1,1},
//  {0,0,1,0,0,1,1,1,1,1,1,1,0,1,1,0},
//  {0,1,1,0,0,1,1,1,1,1,0,1,0,0,0,0}
//};
    
boolean testWs[Size+1][Size+1];
boolean testWe[Size+1][Size+1];



void setup() {
  //Start the Serial Monitor and print out debugging info
  Serial.begin(115200);
//  Serial.print("freeMemory()=");
//  Serial.println(freeMemory());
//  Serial.println("Initialization Started");
  //initTestWalls();
  
  //Initialize the various matricies we use to map out the maze
  //Note that for some of the arrays we are initializing we are initializing it to one 
  destroyVirtualWalls(); //initialize virtual walls to zero
  initializeWalls();
  initializeParents();
  for (int i = 0; i < ((Size+1)^2); i++) {
    breadCrumbs[i] = 4;
    finishToStartDirections[i] = 4;
  }
  whereToGo[0] = Size+1;
  whereToGo[1] = Size+1;
  //Note initializing these to Size+1 is important to the GoneToFar function  TODO: Fix this
  finalCenterSquare[0] =Size+1;
  finalCenterSquare[1] =Size+1;
  
   I[0][0] = 1;  //Set the index of the starting square to one
  Serial.println("Initialization Finished");
}

void loop() {
  while(!isCenter()) {
    updateWalls();
    updateIndicies();
    correctLinearity();
    updateCentMin;
    manhattanSweep();
    deadEndSweep();
     parentSweep();
//Output current robot data
//    Serial.print(Lx);
//    Serial.print(",");
//    Serial.print(Ly);
//    Serial.print(" Index:");
//    Serial.print(I[Lx][Ly]);
//    Serial.println();
//    Serial.print("Current Parent is = ");
//    Serial.println(P[Lx][Ly]);
//    Serial.println();
//    outputMaze();
    if(goneTooFar(Lx,Ly)) {
      setAllVirtualWalls(Lx,Ly);
      moveUnit(P[Lx][Ly]);
    } else {
    moveUnit(moveDir());
    }
  }
      //run all the methods one more time to map the center
    updateWalls();
    updateIndicies();
    correctLinearity();
    manhattanSweep();
    deadEndSweep();
     parentSweep();
    outputMaze();
 
  
  //Needs to run these methods again to update center square
  visitCenter();


  Serial.print("Hansel and Gretel");
  hanselAndGretel(goFromCenterToStart());
  
  if(foundShortest()) {
    Serial.print("SPEED RACER");
    while(true) {
    hanselAndGretel(goFromStartToSquare(finalCenterSquare[0],finalCenterSquare[1]));
    hanselAndGretel(goFromCenterToStart());
    Serial.print("Update Motor Speed");
    delay(50);
    }
  }
  
  whereDoWeGoNow();
  hanselAndGretel(goFromStartToSquare(whereToGo[0],whereToGo[1]));
}

//void initTestWalls() {
//  for(int i = 0; i <= testSize; i++) {
//    for(int j = 0; j <= testSize; j++) {
//      if(testWn[i][j]) {
//        setTestNorthWall(i,j);
//      }
//      if(testWw[i][j]) {
//        setTestWestWall(i,j); 
//      }
//      //      if(j==0) {
//      //        testWs[i][j] = 1;
//      //      }
//      //      if(i==testSize) {
//      //        testWe[i][j] = 1; 
//      //      }
//    }
//  }
//}



//void setTestNorthWall(int lx, int ly) {
//  if(ly < Size) {
//    testWs[lx][ly+1] = true;
//  }
//}
//
//void setTestWestWall(int lx, int ly) {
//  if(lx > 0) {
//    testWe[lx-1][ly] = true;
//  }
//}

/* initializeSquareData
*  sets all eight bits to 0 for each square in the maze
*/
void intializeSquareData()
{
  for (int i = 0; i < Size; i++)
  {
    for (int j = 0; j < Size; j++)
    {

      bitClear(squareData[i][j] ,0); //sets bit 0 to 0
      bitSet(squareData[i][j] ,1); //sets bit 1 to 0
      bitSet(squareData[i][j] ,2); //sets bit 2 to 0
      bitSet(squareData[i][j] ,3); //sets bit 3 to 0
      bitSet(squareData[i][j] ,4); //sets bit 4 to 0
      bitSet(squareData[i][j] ,5); //sets bit 5 to 0
      bitSet(squareData[i][j] ,6); //sets bit 6 to 0
      bitSet(squareData[i][j] ,7); //sets bit 7 to 0
      
      
    } 
    
  }  
}

//
//
//NEW CODE
//
//
/* setExplored
*  given an x and y location, as well as a 0/1 for true false,
*  sets the bit for explored to that value
*/
void setExplored(int lx, int ly, int q)
{
  bitSet(squareData[lx][ly], q);
}

/* setParent
*  given an x and y location, as well as two 0/1 values,
*  sets the parent of the block at (x,y) 
*  by changing the values of bits 1 and 2
*  00 - North
*  01 - South
*  10 - East
*  11 - West
*/
void setParent(int lx, int ly, int q1, int q2)
{
  //north
  if ((q1 == 0) && (q2 == 0))
  {
    bitClear(squareData[lx][ly], 1);
    bitClear(squareData[lx][ly], 2);
  }
  
  //south
  else if ((q1 == 0) && (q2 == 1))
  {
    bitClear(squareData[lx][ly], 1);
    bitSet(squareData[lx][ly], 2);
  }
  
  //east
  else if ((q1 == 1) && (q2 == 0))
  {
    bitSet(squareData[lx][ly], 1);
    bitClear(squareData[lx][ly], 2);
  }
  
  //west
  else if ((q1 == 1) && (q2 == 1))
  {
    bitSet(squareData[lx][ly], 1);
    bitSet(squareData[lx][ly], 2);
  }
  
}

/* setNorthWallBit
*  given coordinates of square, sets its north wall (and the south wall of the one above it)
*  to either true (1) or false (0)
*  by changing bit 3
*/
void setNorthWallBit(int lx, int ly, int q)
{
  //remove a real wall
  if (q == 0)
  {
    bitClear(squareData[lx][ly], 3);
    if (ly < Size)
    {
      bitClear(squareData[lx][ly + 1], 4);
    }
  } 
  
  //add a real wall
  else if (q == 1)
  {
    bitSet(squareData[lx][ly], 3);
    if (ly < Size)
    {
      bitSet(squareData[lx][ly + 1], 4); 
    }
  }
  
}

/* setEastWallBit
*  given coordinates of square, sets its east wall (and the west wall of the one to its right)
*  to either true (1) or false (0)
*  by changing bit 5
*/
void setEastWallBit(int lx, int ly, int q)
{
  //remove a real wall
  if (q == 0)
  {
    bitClear(squareData[lx][ly], 5);
    if (lx < Size)
    {
      bitClear(squareData[lx + 1][ly], 6);
    }
  } 
  //add a real wall
  else if (q == 1)
  {
    bitSet(squareData[lx][ly], 5);
    if (lx < Size)
    {
      bitSet(squareData[lx + 1][ly], 6); 
    {
  }
   
  
}

/* setSouthWallBit
*  given coordinates of square, sets its south wall (and the north wall of the one under it)
*  to either true (1) or false (0)
*  by changing bit 4
*/
void setSouthWallBit(int lx, int ly, int q)
{
  //remove a real wall
  if (q == 0)
  {
    bitClear(squareData[lx][ly], 4);
    if (ly != 0)
    {
      bitClear(squareData[lx][ly - 1], 3);
    }
  } 
  //add a real wall
  else if (q == 1)
  {
    bitSet(squareData[lx][ly], 4); 
    if (ly != 0)
    {
      bitSet(squareData[lx][ly - 1], 3);
    }
  }
  
}

/* setWestWallBit
*  given coordinates of a square, sets its west wall (and the east wall of the one to its left)
*  to either true (1) or false (0)
*  by changing bit 6
*/
void setWestWallBit(int lx, int ly, int q)
{
  //remove a real wall
  if (q == 0)
  {
    bitClear(squareData[lx][ly], 6); 
    if (lx != 0)
    {
      bitClear(squareData[lx - 1][ly], 5); 
    }
  } 
  //add a real wall
  else if (q == 1)
  { 
    bitSet(squareData[lx][ly], 6);
    if (lx != 0)
    {
      bitSet(squareData[lx - 1][ly], 5); 
    }
  }
}

/* 
*  setVirtualWallsBit
*  sets whether a square is boxed by virtual walls 
*  1 - boxed, 0 - not boxed
*  all four walls done at once
*/
void setVirtualWallsBit(int lx, int ly, int q)
{ 
  if (q == 1)
  {
    bitSet(squareData[lx][ly], 7);
  }
  
  else if (q == 0)
  {
    bitClear(SquareData[lx][ly], 7);
  }
 
}

/*
*  getExploredBit
*  returns true if the given square is explored
*  and false if it is not yet explored
*/
bool getExploredBit(int lx, int ly)
{
  if (bitRead(squareData[lx][ly], 0) == 1)
  {
    return true;
  }
  
  else if (bitRead(squareData[lx][ly], 0) == 0)
  {
    return false;
  }
}

/*
*  getParentBit
*  returns an int to indicate the direction of the parent
*  of the given square
*  0 = north
*  1 = south
*  2 = east
*  3 = west
*/
int getParentBit(int lx, int ly)
{
  if ((bitRead(squareData[lx][ly], 1) == 0) && (bitRead(squareData[lx][ly], 2) == 0))
  {
    return 0; //north
  }
  
  if ((bitRead(squareData[lx][ly], 1) == 0) && (bitRead(squareData[lx][ly], 2) == 1))
  {
    return 1; //south
  }
  
  if ((bitRead(squareData[lx][ly], 1) == 1) && (bitRead(squareData[lx][ly], 2) == 0))
  {
    return 2; //east
  }
  
  if ((bitRead(squareData[lx][ly], 1) == 1) && (bitRead(squareData[lx][ly], 2) == 1))
  {
    return 3; //west
  }

}

bool getWallNorthBit(int lx, int ly)
{

}

bool getWallSouthBit(int lx, int ly)
{

}

bool getWallEastBit(int lx, int ly)
{

}

bool getWallWestBit(int lx, int ly)
{

}

bool getWallVirtualBit(int lx, int ly)
{

}


//
//
//END NEW CODE
//
//

/* initializeParents
*  Initializes the parents array to all fours (Since four is an invalid number for a parent if the four is ever used an runtime error will occur)
*  This method also initializes whereToGo to Size+1, again an invalid number
*  TODO: Figure out a better way to initialize these values instead of using an invalid number
*/
void initializeParents() {
  Serial.println("Initialize Parents");
  for(int i =0; i <= Size; i++) {
    for(int j = 0; j <= Size; j++) {
      P[i][j] = 4;
    }
  }
  whereToGo[0] = Size+1;
  whereToGo[1] = Size+1;
}

/* updateCentMin
*  Checks the indices of all the center squares and makes sure centMin is set to the lowest values
*  TODO: Make it so that this method is not dependant on finalCenterSquare being initialized to higher than Size
*/
void updateCentMin() {
  if(finalCenterSquare[0]<Size) {      //TODO: Make it so that this mtethod is not dependant on finalCenterSquare being initialized to higher than Size
   centMin = I[finalCenterSquare[0]][finalCenterSquare[1]]; 
  }
}

/* initializeWalls
*  Initializes all the wall physical wall matrices to 0, except for the outer walls which are set to 1.(Since we know them to be there)
*  Also Initializes all the indices to 0 and the Explored Square Matrix to 0
*/
void initializeWalls() {
  Serial.println("Initialize Walls");
  for (int i = 0; i <= Size; i++) {
    for (int j = 0; j <= Size; j++) {
      Ww[i][j] = 0;
      We[i][j] = 0;
      Ws[i][j] = 0;
      Wn[i][j] = 0;
      I[i][j] = 0;
      E[i][j] = 0;
      if (i == 0) {
        Ww[i][j] = 1;
      }
      if (i == Size) {
        We[i][j] = 1;
      }
      if (j == 0) {
        Ws[i][j] = 1;
      }
      if (j == Size) {
        Wn[i][j] = 1;
      }
    }
  }
}

/*  destroyVirtualWalls
 *  Sets all virtual walls to zero in the entire matrix
 *  Also, used to initialize the virtual walls
 */
void destroyVirtualWalls() {
  for (int i = 0; i <= Size; i++) {
    for (int j = 0; j <= Size; j++) {
      vWw[i][j] = 0;
      vWe[i][j] = 0;
      vWs[i][j] = 0;
      vWn[i][j] = 0;
    }
  }
}

/* isCenter
*  Returns true if the robot is physically in one of the four center squares
*/
boolean isCenter() {
  if((Lx == (Size/2) && Ly == (Size/2)) || (Lx == (Size/2) && Ly == ((Size/2)+1)) || ((Lx == (Size/2)+1) && Ly == (Size/2)) || ((Lx == (Size/2)+1) && Ly == ((Size/2)+1))) {
    return true;
  }
  else {
    return false;
  }
}

/* isCenter
*  Returns true if the passed in square is one of the four center squares
*/ 
boolean isCenter(int lx, int ly) {
  if((lx == (Size/2) && ly == (Size/2)) || (lx == (Size/2) && ly == ((Size/2)+1)) || ((lx == (Size/2)+1) && ly == (Size/2)) || ((lx == (Size/2)+1) && ly == ((Size/2)+1))) {
    return true;
  }
  else {
    return false;
  }
}

/* visitCenter
*  This function is supposed to be run when the robot first reaches a center square
*  Once we reach the center we know which of the 4 center squares is our "destination square." By knowing the "destination square" we can improve our sweep functions by using that square
*  in particular instead of considering all four center squares.
*  This method updates center 4 squares and creates this "destination square"
*  WARNING: -This method sets the physical walls around all the center squares to true. It relies on the parent of the "destination square" to remove the improperly set physical wall
*           -This method also assumes you have run all the sweeps and linearity checks
*/
void visitCenter(){
  centMin = I[Lx][Ly];		//Update centMin with the index of your current square
  
  //Figure out the matrix indices for all the center squares
  int low = (Size/2);
  int high = (Size/2)+1;

  //Even though we haven't technically explored all the center squares we know everything about them so we can say we've explored them all
  E[low][low] = 1;
  E[low][high] = 1;
  E[high][low] = 1;
  E[high][high] = 1;

  //Sets all the physical walls of all four of the center squares to true
  setNorthWall(low,low);
  setSouthWall(low,low);
  setEastWall(low,low);
  setWestWall(low,low);

  setNorthWall(low,high);
  setSouthWall(low,high);
  setEastWall(low,high);
  setWestWall(low,high); 

  setNorthWall(high,high);
  setSouthWall(high,high);
  setEastWall(high,high);
  setWestWall(high,high); 


  setNorthWall(high,low);
  setSouthWall(high,low);
  setEastWall(high,low);
  setWestWall(high,low); 

  //Sets the "destination square" to our current location
  finalCenterSquare[0]=Lx;
  finalCenterSquare[1]=Ly;
  
  //Delete the physical wall that is blocking the entrance to the center that was improperly set above
  if(P[Lx][Ly] == 0) {
    Wn[Lx][Ly] = 0;
    Ws[Lx][Ly+1] = 0;
  } 
  else if(P[Lx][Ly] == 1) {
    Ws[Lx][Ly] = 0;
    Wn[Lx][Ly-1] = 0;
  } 
  else if(P[Lx][Ly] == 2) {
    We[Lx][Ly] = 0;
    Ww[Lx+1][Ly] = 0;
  } 
  else if (P[Lx][Ly] == 3) {
    Ww[Lx][Ly] = 0;
    We[Lx-1][Ly] = 0;
  }
}

// Reads sensor data and determines if there is a wall to the north
boolean isNorthWall() {
//TODO: Insert sensor code here
}
// Reads sensor data and determines if there is a wall to the south
boolean isSouthWall() {
//TODO: Insert sensor code here
}
// Reads sensor data and determines if there is a wall to the west
boolean isWestWall() {
//TODO: Insert sensor code here
}
// Reads sensor data and determines if there is a wall to the east
boolean isEastWall() {
//TODO: Insert sensor code here
}


/* moveUnit
*  Pass in the desired direction you want the robot to move(N = 0, S = 1, E = 2, W = 3)
*  TODO: Motor code needs to be added
*/
void moveUnit(int x){
  if (x == 0) {
    Ly = Ly+1;
  }
  else if (x == 1) {
    Ly = Ly -1;
  }
  else if (x == 2) {
    Lx = Lx + 1;
  }
  else if (x == 3) {
    Lx = Lx -1;
  }
}

/* numberOfWalls
*  Returns the number of walls detectetd by the sensors in the square the robot is in
*  TODO: This method might never be used. Check this!!!
*/
int numberOfWalls(){
  int numberOfWalls = 0; //Keeps count of number of walls
  if(isNorthWall()) {
    numberOfWalls++; 
  }
  if(isSouthWall()) {
    numberOfWalls++; 
  }
  if(isEastWall()) {
    numberOfWalls++; 
  }
  if(isWestWall()) {
    numberOfWalls++; 
  }
//  Serial.print("\n#Walls: ");
//  Serial.println(numberOfWalls);
  return numberOfWalls;
}

/* numberOfWalls
*  TODO: I have no clue what this method does and it is probably unused.
*/
int numberOfWalls(int x){
  int numberOfWalls = 0; //keeps count of number of walls
  if ((x == 2) || (x == 3)){
    if(isNorthWall()) {
      numberOfWalls++; 
    }
    if(isSouthWall()) {
      numberOfWalls++; 
    }
  }
  if ((x == 0) || (x == 1)){
    if(isEastWall()) {
      numberOfWalls++; 
    }
    if(isWestWall()) {
      numberOfWalls++; 
    }
  }
  Serial.print("\n#Walls: ");
  Serial.println(numberOfWalls);
  return numberOfWalls;
}

/* set****Wall
 * The set wall methods should always be used when mapping walls as the set the walls of the square you are in as well 
 * as the complementary walls of the adjacent squares
 * For example setting the west wall of one square will set the east wall of the adjacent square to the west
 */
void setNorthWall(int lx, int ly) {
  Serial.print("Setting North Wall; ");
  Wn[lx][ly] = true;
  if(ly < Size) {
    Ws[lx][ly+1] = true;
  }
}

void setSouthWall(int lx, int ly) {
  Serial.print("Setting South Wall; ");
  Ws[lx][ly] = true;
  if(ly > 0) {
    Wn[lx][ly-1] = true;
  }
}

void setWestWall(int lx, int ly) {
  Serial.print("Setting West Wall; ");
  Ww[lx][ly] = true;
  if(lx > 0) {
    We[lx-1][ly] = true;
  }
}

void setEastWall(int lx, int ly) {
  Serial.print("Setting East Wall; ");
  We[lx][ly] = true;
  if(lx < Size) {
    Ww[lx+1][ly] = true;
  }
}



/* setVirtual*****Wall
*  Performs the same task as the normal set wall method but with virtual walls
*/
void setVirtualNorthWall(int lx, int ly) {
  vWn[lx][ly] = true;
  if(ly < Size) {
    vWs[lx][ly+1] = true;
  }
}

void setVirtualSouthWall(int lx, int ly) {
  vWs[lx][ly] = true;
  if(ly > 0) {
    vWn[lx][ly-1] = true;
  }
}

void setVirtualWestWall(int lx, int ly) {
  vWw[lx][ly] = true;
  if(lx > 0) {
    vWe[lx-1][ly] = true;
  }
}

void setVirtualEastWall(int lx, int ly) {
  vWe[lx][ly] = true;
  if(lx < Size) {
    vWw[lx+1][ly] = true;
  }
}

/* getWall****
*  Returns true if either the real or virtual wall is set in the specified position
*/
boolean getWallNorth(int lx, int ly) {
  if(vWn[lx][ly] || Wn[lx][ly]) {
    return true; 
  }
  return false;
}

boolean getWallSouth(int lx, int ly) {
  if(vWs[lx][ly] || Ws[lx][ly]) {
    return true; 
  }
  return false;
}

boolean getWallWest(int lx, int ly) {
  if(vWw[lx][ly] || Ww[lx][ly]) {
    return true; 
  }
  return false;
}

boolean getWallEast(int lx, int ly) {
  if(vWe[lx][ly] || We[lx][ly]) {
    return true; 
  }
  return false;
}

/* updateWalls
*  Reads in sensor values and sets the walls of the square the robot is in accordingly
*/
void updateWalls() {
  if(isNorthWall() == true) {
    setNorthWall(Lx, Ly);
  }
  if(isSouthWall() == true) {
    setSouthWall(Lx, Ly);
  }
  if(isWestWall() == true) {
    setWestWall(Lx, Ly);
  }
  if(isEastWall() == true) {
    setEastWall(Lx, Ly);
  }
}

/*VIRTUAL TEST METHOD
* Reads in values from the test wall matrices and then sets the actual wall matrices
*/
//void updateWalls() {
//  if(testWn[Lx][Ly]) {
//    setNorthWall(Lx,Ly);
//  } 
//  if(testWs[Lx][Ly]) {
//    setSouthWall(Lx,Ly);
//  } 
//  if(testWw[Lx][Ly]) {
//    setWestWall(Lx,Ly);
//  } 
//  if(testWe[Lx][Ly]) {
//    setEastWall(Lx,Ly);
//  } 
//}


/* updateIndices
*  Updates adjacent, unexplored, squares with no walls inbetween with appropriate index numbers and parent numbers
*  Aslo updates the Explored matrix for the square you are presently in
*  TODO: Rename this function to something more indicative of what it does
*/
void updateIndicies() {
    E[Lx][Ly] = true;
  if((!Wn[Lx][Ly]) && (Ly < Size)) {
    if(I[Lx][Ly+1] == 0) {
      I[Lx][Ly+1] = I[Lx][Ly]+1;
      P[Lx][Ly+1] = 1;
    }
  }
  if((!Ws[Lx][Ly]) && (Ly > 0)) {
    if(I[Lx][Ly-1] == 0) {
      I[Lx][Ly-1] = I[Lx][Ly]+1;
      P[Lx][Ly-1] = 0;
    }
  }
  if((!Ww[Lx][Ly]) && (Lx > 0)) {
    if(I[Lx-1][Ly] == 0) {
      I[Lx-1][Ly] = I[Lx][Ly]+1;
      P[Lx-1][Ly] = 2;
    }
  }
  if((!We[Lx][Ly]) && (Lx < Size)) {
    if(I[Lx+1][Ly] == 0) {
      I[Lx+1][Ly] = I[Lx][Ly]+1;
      P[Lx+1][Ly] = 3;
    }
  }
}

/* checkLinearity
*  Checks to see if there are discrpeancies between the indices of adjacent squares without walls in between them 
*  (i.e A 13 adjacent to a 9 with no wall in between)
*  If a discrepancy is found correctLinearity is called to fix it
*  TODO: I beleive checkLinearity might never be called and correctLinearity is called instead
*/
void checkLinearity() {
  if (!Wn[Lx][Ly]) {
    if(abs(I[Lx][Ly]-I[Lx][Ly+1]) > 1) {
      correctLinearity();
      return;
    }
  }
  if (!Ws[Lx][Ly]) {
    if(abs(I[Lx][Ly]-I[Lx][Ly-1]) > 1) {
      correctLinearity();
      return;
    }
  }
  if (!Ww[Lx][Ly]) {
    if(abs(I[Lx][Ly]-I[Lx-1][Ly]) > 1) {
      correctLinearity();
      return;
    }
  }
  if (!We[Lx][Ly]) {
    if(abs(I[Lx][Ly]-I[Lx+1][Ly]) > 1) {
      correctLinearity();
      return;
    }
  }
}

/* correctLinearity
*  Checks to see if there are discrpeancies between the indices of adjacent squares without walls in between them 
*  (i.e A 13 adjacent to a 9 with no wall in between)
*  If a discrepancy is found it will be corrected and linearity will be restored
*/
void correctLinearity() {
  //Serial.println("Correcting Linearity");
  //We must get rid of all the virtual walls because after the linearity correction paths that were previously ruled out might be viable
  destroyVirtualWalls();    
  boolean flag = true;      //Set a boolean flag which will cause correct linearity to keep looping until the entire maze is linear
  while(flag) {
    flag = false;
    for (int i = 0; i <= Size; i ++) {      //Loop through the maze
      for (int j = 0; j <= Size; j++) {
        if (E[i][j]) {                      //We are only concerned with the linearity of explored squares as they are the only squares where we know where the walls are
          // North Wall
          if (!Wn[i][j]) {
            if ((I[i][j] - I[i][j+1]) > 1) {
              I[i][j] = I[i][j+1]+1;        //If there is a wall to the North with lower 
              flag = true;
            }
            else if (((I[i][j] - I[i][j+1]) < -1)) {
              I[i][j+1] = I[i][j]+1;
              flag = true;
            }
          }
          // South Wall
          if (!Ws[i][j]) {
            if ((I[i][j] - I[i][j-1]) > 1) {
              I[i][j] = I[i][j-1]+1;
              flag = true;
            }
            else if (((I[i][j] - I[i][j-1]) < -1)) {
              I[i][j-1] = I[i][j]+1;
              flag = true;
            }
          }
          // West Wall
          if (!Ww[i][j]) {
            if ((I[i][j] - I[i-1][j]) > 1) {
              I[i][j] = I[i-1][j]+1;
              flag = true;
            }
            else if (((I[i][j] - I[i-1][j]) < -1)) {
              I[i-1][j] = I[i][j]+1;
              flag = true;
            }
          }
          // East Wall
          if (!We[i][j]) {
            if ((I[i][j] - I[i+1][j]) > 1) {
              I[i][j] = I[i+1][j]+1;
              flag = true;
            }
            else if (((I[i][j] - I[i+1][j]) < -1)) {
              I[i+1][j] = I[i][j]+1;
              flag = true;
            }
          }
        }
      }
    }
  }
}


/* Dead end sweep will look for all squares that have 3 walls, virtual or real, and then set all 4 virtual walls of that square true
 *  In addition it will look for all squares that only have lower index squares adjacent to it and set all virtual walls true in that square(think a corner that has lower index squares next to it
 *  Dead end sweep will continue to loop through until it has found no more dead ends or squares with only lower adjacent index values
 *  Finally Dead end sweep will never set the virtual walls of the staring square(index 1), the square the robot is currently in(in order to prevent boxing the robot in, and finally any of the center squares
 *  Not walling the first and center squares ensures that you will never fill in your shortest path and will only wall off invalid squares(i.e squares you don't want to visit
 *  NOTE: You should usually run dead end sweep after the Manhattan Sweep as the Manhattan sweep should create some dead ends that need filling
 */
void deadEndSweep() {
  //First let's run throgh and find any squares that have three walls set
  boolean flag = true;
  while(flag) {
    flag = false; 
    for (int i = 0; i <= Size; i ++) {
      for (int j = 0; j <= Size; j++) {
        if ((I[i][j] != 1) && !((Lx == i)&&(Ly == j)) && (!isCenter(i,j))) {
          //          if(numberOfWalls(i,j) == 3) {
          //            Serial.print(" Three Walls in: ");
          //            Serial.print(i);
          //            Serial.print(" ,");
          //            Serial.println(j);
          //            setAllVirtualWalls(i,j);
          //            flag = true;
          //          }
          if(onlyLowerIndicies(i,j)) {
//            Serial.print(" Only lower indexes in: ");
//            Serial.print(i);
//            Serial.print(" ,");
//            Serial.println(j);
            setAllVirtualWalls(i,j);
            flag = true;
          }
        }
      }
    }
  }
}

// Small functions that returns true if a square is surrounded by only lower indexed squares.
// Used as a short cut in the dead end sweep.
boolean onlyLowerIndicies(int lx, int ly) {
  int flag = 0;
  if(!getWallNorth(lx, ly)) {
    if(I[lx][ly+1] == (I[lx][ly]-1)) {
      flag++;
    }
  }
  if(!getWallSouth(lx, ly)) {
    if(I[lx][ly-1] == (I[lx][ly]-1)) {
      flag++;
    }
  }
  if(!getWallEast(lx, ly)) {
    if(I[lx+1][ly] == (I[lx][ly]-1)) {
      flag++;
    }
  }
  if(!getWallWest(lx, ly)) {
    if(I[lx-1][ly] == (I[lx][ly]-1)) {
      flag++;
    }
  }
  if (((4-flag) == numberOfWalls(lx,ly)) && (flag >= 1)) {
    return true;
  }
  else {
    return false;
  }
}

// Sweeps through the known maze and sets all virtual walls to true on any square whose Manhattan Distance plus the index is greater than the current index of the center squares.
//
void manhattanSweep() {
  for(int i = 0; i <= Size; i++) {
    for(int j = 0; j <= Size; j++) {
      if(I[i][j] != 0) {
        if(E[i][j] != 0) {
        if(goneTooFar(i,j)) {
          if(!isCenter(i,j)) {
//            Serial.print("Gone Too Far in: ");
//            Serial.print(i);
//            Serial.print(", ");
//            Serial.println(j);
            setAllVirtualWalls(i,j);
          }
        }
        }
      }
    }
  }
}

  void parentSweep() {
  Serial.print("Correcting Parents: ");
  for(int lx = 0; lx <= Size; lx++) {
    for(int ly = 0; ly <= Size; ly++) {
  if (lx>Size) {
  } else if (ly>Size) {
    
  }else if(isCenter(lx,ly)){
   //do nothing
  } else {
  if(!getWallNorth(lx,ly)) {
    if(ly < Size){
      if((I[lx][ly]-1) == I[lx][ly+1]) {
        P[lx][ly] = 0;
      }
    }
  }
  if(!getWallSouth(lx,ly)) {
    if(ly > 0){
      if((I[lx][ly]-1) == I[lx][ly-1]) {
        P[lx][ly] = 1;
      }
    }
  }
  if(!getWallEast(lx,ly)) {
    if(lx < Size){
      if((I[lx][ly]-1) == I[lx+1][ly]) {
        P[lx][ly] = 2;
      }
    }
  }
  if(!getWallWest(lx,ly)) {
    if(lx > 0){
      if((I[lx][ly]-1) == I[lx-1][ly]) {
        P[lx][ly] = 3;
      }
    }
  }
    }
    }
  }
}

// Small function that returns the number of walls(virtual or real) the specified square has
// Used in the deadEndSweep function
int numberOfWalls(int lx, int ly) {
  int numberOfWalls = 0; //keeps count of number of walls
  if(getWallNorth(lx,ly)) {
    numberOfWalls++; 
  }
  if(getWallSouth(lx,ly)) {
    numberOfWalls++; 
  }
  if(getWallWest(lx,ly)) {
    numberOfWalls++; 
  }
  if(getWallEast(lx,ly)) {
    numberOfWalls++; 
  }
  return numberOfWalls;
}

//Sets all the virtual walls of the specified square to true
//Used in the dead end sweep
void setAllVirtualWalls(int lx, int ly) {
  //  Serial.print("Setting Virtual walls");
  if(!isCenter(lx,ly)) {
    setVirtualNorthWall(lx,ly);
    setVirtualSouthWall(lx,ly);
    setVirtualWestWall(lx,ly);
    setVirtualEastWall(lx,ly);
  }
}


// Decides whether a certain square is a viable next step in relation to moving to the center
boolean goneTooFar(int lx, int ly) {
  int movesTo = 255;
  int high = (Size/2)+1;
  int low = (Size/2);
  
  if(finalCenterSquare[0]!=(Size+1) && finalCenterSquare[1]!=(Size+1)) {
    movesTo = (abs(lx-finalCenterSquare[0])+abs(ly-finalCenterSquare[1]));
  } else {
  if (abs(lx-low) + abs(ly-low) < movesTo) {
    movesTo = (abs(lx-low)+abs(ly-low));
  }
  if (abs(lx-high) + abs(ly-low) < movesTo) {
    movesTo = (abs(lx-high)+abs(ly-low));
  }
  if (abs(lx-low) + abs(ly-high) < movesTo) {
    movesTo = (abs(lx-low)+abs(ly-high));
  }
  if (abs(lx-high) + abs(ly-high) < movesTo) {
    movesTo = (abs(lx-high)+abs(ly-high));
  }
  }
  if ((movesTo + I[lx][ly]) > centMin) {
    return true;
  }
  else {
    return false;
  }
}


// Returns THE direction to move
// 0 - NORTH
// 1 - SOUTH
// 2 - EAST
// 3 - WEST
int moveDir() {
  
  double smallestMD = 257; // Smallest Manhattan distance variable. Initialized high to check for oddities later
  int dir = 4; // Direction to move! Arbitrarily initialized at to an INVALID value
  // CHECKS NORTH
  if (!getWallNorth(Lx,Ly)) { // If there's not a wall
    if (!E[Lx][Ly+1]) { // And it's not visited
      double movesTo = movesToCenter(Lx,Ly+1);
      if (movesTo < smallestMD) { // If the manhattan distance is smaller than the current smallest manhattan distance
        smallestMD = movesTo; // It updates the smallest manhattan distance
        dir = 0; // And sets the relevant direction
      }
    }
  }
  // CHECKS SOUTH
  if (!getWallSouth(Lx,Ly)) { // If there's not a wall
    if (!E[Lx][Ly-1]) { // And it's not visited
      double movesTo = movesToCenter(Lx,Ly-1);
      if (movesTo < smallestMD) { // If the manhattan distance is smaller than the current smallest manhattan distance
        smallestMD = movesTo; // It updates the smallest manhattan distance
        dir = 1; // And sets the relevant direction
      }
    }
  }
  // CHECKS EAST
  if (!getWallEast(Lx,Ly)) { // If there's not a wall
    if (!E[Lx+1][Ly]) { // And it's not visited
    double movesTo = movesToCenter(Lx+1,Ly);
      if (movesTo < smallestMD) { // If the manhattan distance is smaller than the current smallest manhattan distance
        smallestMD = movesTo; // It updates the smallest manhattan distance
        dir = 2; // And sets the relevant direction
      }
    }
  }
  // CHECKS WEST
  if (!getWallWest(Lx,Ly)) { // If there's not a wall
    if (!E[Lx-1][Ly]) { // And it's not visited
      double movesTo = movesToCenter(Lx-1,Ly);
      if (movesTo < smallestMD) { // If the manhattan distance is smaller than the current smallest manhattan distance
        smallestMD = movesTo; // It updates the smallest manhattan distance
        dir = 3; // And sets the relevant direction
      }
    }
  }
  // CHECKS FOR ODDITIES (There were no unexplored options)
  if (smallestMD == 257) {
    //NORTH
    if (!getWallNorth(Lx,Ly)) { // If there's not a wall
      if ((I[Lx][Ly]+1) == I[Lx][Ly+1] ) {
        double movesTo = movesToCenter(Lx,Ly+1);
        if (movesTo < smallestMD) { // If the manhattan distance is smaller than the current smallest manhattan distance
          smallestMD = movesTo; // It updates the smallest manhattan distance
          dir = 0; // And sets the relevant direction
        }
      }
    }
    //SOUTH
    if (!getWallSouth(Lx,Ly)) { // If there's not a wall
      if ((I[Lx][Ly]+1) == I[Lx][Ly-1] ) {
        double movesTo = movesToCenter(Lx,Ly-1);
        if (movesTo < smallestMD) { // If the manhattan distance is smaller than the current smallest manhattan distance
          smallestMD = movesTo; // It updates the smallest manhattan distance
          dir = 1; // And sets the relevant direction
        }
      }
    }
    //EAST
    if (!getWallEast(Lx,Ly)) { // If there's not a wall
      if ((I[Lx][Ly]+1) == I[Lx+1][Ly] ) {
        double movesTo = movesToCenter(Lx+1,Ly);
        if (movesTo < smallestMD) { // If the manhattan distance is smaller than the current smallest manhattan distance
          smallestMD = movesTo; // It updates the smallest manhattan distance
          dir = 2; // And sets the relevant direction
        }
      }
    }
    //WEST
    if (!getWallWest(Lx,Ly)) { // If there's not a wall
      if ((I[Lx][Ly]+1) == I[Lx-1][Ly] ) {
        double movesTo = movesToCenter(Lx-1,Ly);
        if (movesTo < smallestMD) { // If the manhattan distance is smaller than the current smallest manhattan distance
          smallestMD = movesTo; // It updates the smallest manhattan distance
          dir = 3; // And sets the relevant direction
        }
      }
    }
  }
  if (smallestMD == 257) {
    if (!getWallNorth(Lx,Ly)) {
      dir = 0;
    }
    if (!getWallSouth(Lx,Ly)) {
      dir = 1;
    }
    if (!getWallEast(Lx,Ly)) {
      dir = 2;
    }
    if (!getWallWest(Lx,Ly)) {
      dir = 3;
    }
  }
  
  if (dir == 4) {
    while(true) {
      Serial.print("INVALID MOVE");
      delay(1000);
    }
  }  else{

  //Serial.print(" Direction returned");
  //Serial.println(dir);
  return dir; // returns the chosen one
    }
}

double movesToCenter(int lx,int ly) {
  double movesTo;
  double sizeInDouble = Size;
  double sizeDivTwo = sizeInDouble/2;
  
  movesTo = abs(sizeDivTwo-lx)+abs(sizeDivTwo-ly);

return movesTo;
}


int readBackwards(int i) {
  if(i == 0) {
    return 1;
  }
  if(i == 1) {
    return 0;
  }
  if(i == 2) {
    return 3;
  }
  if(i == 3) {
    return 2;
  }
}


void whereDoWeGoNow() {
  int lowest = 257;
  int lowestLx = Size+1;
  int lowestLy = Size+1;
  for(int i = 0; i <= Size; i++) {
    for(int j = 0; j <=Size; j++) {
      if(!E[i][j]) {
        if(!isCenter(i,j)) {
        if(!goneTooFar(i,j)){
        if(numberOfWalls(i,j)!=4) {
          if(I[i][j] != 0) {
            if(I[i][j] < lowest) {
              lowest = I[i][j];
              lowestLx = i;
              lowestLy = j;
            }
          }
        }
          }
        }
      }
    }
  }
  Serial.println();
  Serial.println("############");
  Serial.println("Where do we go now: ");
  Serial.print(lowestLx);
  Serial.print(", ");
  Serial.println(lowestLy);
  Serial.println("############");
  if(lowestLx==Size+1) {
    whereToGo[0] = finalCenterSquare[0];
    whereToGo[1] = finalCenterSquare[1];
 Serial.println("No Valid whereToGo Square");
  } else{
    
     whereToGo[0] = lowestLx;
  whereToGo[1] = lowestLy;
  }
}

int goFromStartToSquare(int lx, int ly) {
  int i = lx;
  int j = ly;
  int c = 0;

  while(I[i][j] != 1) {
    breadCrumbs[c] = readBackwards(P[i][j]);
    c++;
    if(P[i][j] == 0) {
      j=j+1;
    }
    else if(P[i][j] == 1) {
      j=j-1;
    }
    else if(P[i][j] == 2) {
      i= i + 1;
    }
    else if(P[i][j] == 3) {
      i= i - 1;
    }
  }
  //flip breadcrumbs
  int d = c;
  int temp = 0;
  for (int i = 0; i < d; i++) {
    temp = breadCrumbs[i];
    breadCrumbs[i] = breadCrumbs[d-1];
    breadCrumbs[d-1] = temp;
    d--;
  }
//  Serial.println("Breadcrumbs: ");
//  for(int i = 0; i < c; i++){
//    Serial.print(breadCrumbs[i]);
//    Serial.print(", ");
//  }
  return c;
}

int goFromCenterToStart() {
  //  Serial.println("From Center to Star");
  //  Serial.println("##############");
  int i = Lx;
  int j = Ly;
  int c =0;
  while(I[i][j] != 1) {
    //    Serial.print("Location: ");
    //    Serial.print(i);
    //    Serial.print(", ");
    //    Serial.println(j);
    //    delay(1000);
    breadCrumbs[c] = P[i][j];
    c++;
    if(P[i][j] == 0) {
      j=j+1;
    }
    else if(P[i][j] == 1) {
      j=j-1;
    }
    else if(P[i][j] == 2) {
      i= i + 1;
    }
    else if(P[i][j] == 3) {
      i= i - 1;
    }
  }
  Serial.println("From Center to Start");
  Serial.println("##############");
  Serial.println(c);
  return c;
}

void hanselAndGretel(int c) {
  for(int i = 0; i < c; i++) {
    Serial.println("In hansel and gretel");
    Serial.println();
    outputMaze();
    moveUnit(breadCrumbs[i]);
  }
}

boolean foundShortest() {
  for(int i = 0; i <= Size; i++) {
    for(int j = 0; j <= Size; j++) {
      if(E[i][j]) {
        if(numberOfWalls(i,j) != 4) {
          if(!getWallNorth(i,j)) {
            if(j<7) {
              if(!E[i][j+1]) {
                return false;
              }
            }
          }
          if(!getWallSouth(i,j)) {
            if(j>1) {
              if(!E[i][j-1]) {
                return false;
              }
            }
          }
          if(!getWallEast(i,j)) {
            if(i<7) {
              if(!E[i+1][j]) {
                return false;
              }
            }
          }
          if(!getWallWest(i,j)) {
            if(j>1) {
              if(!E[i-1][j]) {
                return false;
              }
            }
          }
        }
      }
    }
  }
  return true;
}

void speedRacer() {
  int g = ((Size+1)^2)+1;
  int cx = ((Size+1)^2)+1;
  int cy = ((Size+1)^2)+1;
  /*
  
   
   Broken needs rewritting
   
   
   */
  if(I[(int)ceil(((double)Size)/2)][(int)ceil(((double)Size)/2)] < g) {
    g = I[(int)ceil(((double)Size)/2)][(int)ceil(((double)Size)/2)];
    cx = (int)ceil(((double)Size)/2);
    cy = (int)ceil(((double)Size)/2);
  }
  if(I[(int)floor(((double)Size)/2)][(int)ceil(((double)Size)/2)] < g) {
    g = I[(int)floor(((double)Size)/2)][(int)ceil(((double)Size)/2)];
    cx = (int)floor(((double)Size)/2);
    cy = (int)ceil(((double)Size)/2);
  }
  if(I[(int)floor(((double)Size)/2)][(int)floor(((double)Size)/2)] < g) {
    g = I[(int)floor(((double)Size)/2)][(int)floor(((double)Size)/2)];
    cx = (int)floor(((double)Size)/2);
    cy = (int)floor(((double)Size)/2);
  }
  if(I[(int)ceil(((double)Size)/2)][(int)floor(((double)Size)/2)] < g) {
    g = I[(int)ceil(((double)Size)/2)][(int)floor(((double)Size)/2)];
    cx = (int)ceil(((double)Size)/2);
    cy = (int)floor(((double)Size)/2);
  }
  int c = goFromStartToSquare(cx,cy);
  while(true) {
    hanselAndGretel(c);
    while(I[Lx][Ly] != 1) {
      moveUnit(P[Lx][Ly]);
    }
  }
}


void outputMaze() { 
  for(int j=testSize;j>=0;j--) {
    for (int i=0;i<=testSize;i++) {
      Serial.print("#");
      if((Wn[i][j])) {
        if(getWallNorth(i,j)) {
          Serial.print("#####");
        } 
        else {
          Serial.print("     "); 
        }

        //virtual walls
      } 
      else if(vWn[i][j]) {
        if(getWallNorth(i,j)) {
          Serial.print("@@@@@");
        } 
        else {
          Serial.print("     "); 
        }

      }
      else {
        Serial.print("     ");
      }
    } 
    Serial.println("#");

    for(int k=0; k<=2;k++) {
      Serial.print("#");
      for (int i=0;i<=testSize;i++) {
        if (getWallEast(i,j)&&(We[i][j])) {
          if(k==1) {
            if(I[i][j] > 9){
              Serial.print(" ");
              Serial.print(I[i][j]);
              Serial.print("  #");
            }
            else{
              Serial.print("  ");
              Serial.print(I[i][j]);
              Serial.print("  #");
            }
          }
          else if(((k==0)&&(i==Lx))&&(j==Ly)) {
            Serial.print(" $$$ #");
          } 
          else if((k==2)&&(E[i][j])) {
            Serial.print("    *#"); 
          }
          else {
            Serial.print("     #");
          }
        } 
        else if (getWallEast(i,j)&&vWe[i][j]) {
          if(k==1) {
            if(I[i][j] > 9){
              Serial.print(" ");
              Serial.print(I[i][j]);
              Serial.print("  @");
            }
            else{
              Serial.print("  ");
              Serial.print(I[i][j]);
              Serial.print("  @");
            }
          }
          else if(((k==0)&&(i==Lx))&&(j==Ly)) {
            Serial.print(" $$$ @");
          } 
          else if((k==2)&&(E[i][j])) {
            Serial.print("    *@"); 
          }
          else {
            Serial.print("     @");
          }
        }

        else {
          if(k==1) {
            if(I[i][j] > 9){
              Serial.print(" ");
              Serial.print(I[i][j]);
              Serial.print("   ");
            }
            else{
              Serial.print("  ");
              Serial.print(I[i][j]);
              Serial.print("   ");
            }
          }
          else if(((k==0)&&(i==Lx))&&(j==Ly)) {
            Serial.print(" $$$  ");
          } 
          else if((k==2)&&E[i][j]) {
            Serial.print("    * "); 
          }
          else {
            Serial.print("      ");
          }
        }
      }
      Serial.println();
    }
  }
  for(int i=0;i<=testSize;i++){
    Serial.print("######"); 
  }
  Serial.println("#");
  Serial.println();
}



