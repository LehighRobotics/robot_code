/*
Final version of the code employed by Team Lehigh's robot at the 2015 Brown Robotics Olympiad
http://students.brown.edu/institute-electrical-electronics-engineers/competition/

This program was orginally written by Lehigh's 2014 robotics competition team for that year's competition. Unfortunately 
the algorithmic code was rather inefficient and couldn't be run on a Arduino Micro or Uno and instead needed to be run on an Arduino Mega. 
For the 2015 competition the algorithmic code was optimized to be more memory efficient thus could be run on an Arduino Micro.

That being said, all of the 'low' level code in this version was authored by the 2015 team.

WORKING_FINAL_FINAL.ino
Original Authors: Jamie Cahill, Wesley Vogel, Bradley Natarian, Oleh Mukleh
Modified By(for the 2015 competition): Jamie Cahill, Drew Sidel, James Fish
4/11/15  3:30am

Compiler version: Arduino 1.0.5
Intended Target: Arduino Micro (unknown revision)
*/


//#include <MemoryFree.h>
int counter = 0;
byte Lx = 0; // Current X coordinate
byte Ly = 0; // Current Y coordinate
const int Size = 15;
byte centMin = 255; // The smallest index in the center square that has been found so far (the LENGTH of the current shortest path)


//TODO: continue writing methods to manipulate squareData and replace old arrays
byte squareData[Size + 1][Size + 1]; //0 - Explored, 1 and 2 - Parent Direction, 3, 4, 5, 6 - Walls (N, S, E, W) 7 - virtual walls on/off
byte I[Size+1][Size+1]; // Array of indicies //index array

//PARENT DIRECTION: 00 - north, 11 - south, 10 - east, 01 - west
//I is the horizontal component of the matrix definition and J is the vertical component. The origin (0,0) is defined as the bottom left corner.

//vvv between is being replaced by above line
/*
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
*/
//^^^

boolean testing = false;    //sets testing mode

byte whereToGo[2];
byte finalCenterSquare[2]; //holds the destination final center square
byte breadCrumbs[((Size+1)^2)];
byte finishToStartDirections[((Size+1)^2)];



byte testSize = Size;
/*
const boolean testWn[8][8] = {
  { 
    0,0,0,0,0,0,0,0      } 
  ,
  {
    1,0,0,1,1,0,1,0      } 
  ,
  {
    0,0,0,0,0,1,0,0      } 
  ,
  {
    1,0,1,0,0,0,0,0      } 
  ,
  {
    1,0,1,0,1,1,0,0      } 
  ,
  {
    1,1,0,1,0,0,0,0      } 
  ,
  {
    1,1,1,0,0,0,0,0      } 
  ,
  {
    0,0,0,1,0,1,0,0      }
};

const boolean testWw[8][8] ={
  {
    0,0,0,0,0,0,0,0        }
  ,
  {
    1,0,1,0,0,1,0,0        }
  ,
  {
    0,1,0,0,1,0,0,1        }
  ,
  {
    0,1,0,1,1,1,0,0        }
  ,
  {
    0,1,0,0,0,0,0,0      }
  ,
  {
    0,0,0,1,1,0,1,1      }
  ,
  {
    0,0,0,0,1,1,0,1      }
  ,
  {
    0,1,0,1,0,0,0,1      }
};


*/
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


/*
//decker maze
const boolean testWn[16][16] = {
{0,0,0,0,0,1,0,1,0,0,0,1,0,0,1,1},
{0,0,0,0,1,0,1,1,0,0,0,0,0,0,0,1},
{0,0,0,1,0,1,0,0,0,0,1,0,0,1,0,1},
{1,0,0,1,1,1,0,0,0,1,1,1,0,1,0,1},
{0,0,1,0,1,1,0,1,1,1,1,1,1,1,1,1},
{1,1,1,0,1,0,0,1,1,1,1,1,1,1,1,1},
{1,1,1,0,1,1,0,1,0,0,0,1,1,0,1,1},//6
{1,1,1,1,1,1,0,0,1,0,0,0,0,0,1,1},
{1,1,1,1,1,1,1,0,1,0,0,0,0,1,1,1},
{1,0,1,0,1,1,0,0,0,0,0,0,0,0,0,1},
{1,1,1,1,1,0,0,0,1,1,1,0,1,0,0,1},
{1,1,0,0,0,0,0,0,0,1,1,0,0,0,0,1},
{0,0,0,1,1,1,1,1,0,0,0,0,1,0,0,1},
{1,1,0,1,1,1,0,0,0,0,0,0,1,0,0,1},//13
{1,0,0,1,0,0,0,0,0,0,0,0,1,0,0,1},
{0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1}
};

//decker maze
const boolean testWw[16][16] = {
 
  {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},
  {1,1,1,0,1,0,0,1,0,1,1,1,1,1,0,0},
  {0,1,1,1,1,0,0,0,1,1,0,0,1,1,1,0},
  {0,1,1,0,1,0,1,1,1,1,1,0,1,0,1,0},
  {0,1,1,0,0,1,1,0,0,0,0,0,0,0,0,0},
  {0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0},
  {0,0,0,0,1,0,1,0,0,0,0,0,0,0,0,0},
  {0,0,0,0,0,0,0,1,1,0,1,1,0,1,1,0},
  {0,1,0,1,0,0,0,0,0,1,1,1,1,0,0,0},
  {0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,0},
  {0,0,0,0,0,0,1,1,0,1,0,0,0,1,1,0},
  {0,1,0,0,1,1,1,1,1,0,0,0,0,1,0,1},
  {0,0,0,1,0,0,0,0,0,1,1,1,0,1,1,0},
  {0,1,1,0,0,0,1,1,0,1,0,1,1,0,1,1},
  {0,0,1,0,0,1,1,1,1,1,1,1,0,1,1,0},
  {0,1,1,0,0,1,1,1,1,1,0,1,0,0,0,0}
};
    */
//boolean testWs[Size+1][Size+1];
//boolean testWe[Size+1][Size+1];

//************LOW LEVEL CONSTS**************
//************CONSTANTS *********
int const MOTOR_DELAY = 3000; //delaymicro
const int STEPS_PER_SQ = 220;
const int RAMP_DELAY = 3000;
const int RAMP_LENGTH = 60;
const int IS_WALL_THRESH = 130; //if the sensor reads > than 100 we know a wall is in the adjacent square
const int ERROR_SCALE_FACTOR = 30; //smaller is more 
const int CORRECTION_RATE = 1;
const int MOTOR_PRECISION = 70;
int const CENTERDIST = 350;  //If there is only one wall that the robot can see, how far away ideally from it, it should be
const int SQRTCENTERDIST = 21;
const int SMASH_STEPS = 80;
const int SMASH_DELAY = 5000;
const int SMASH_STEPS_OUT = 25;
const int NEXT_TO_WALL_THRESH = 550;
const int CORRECTION_CAP = 10;
/*************END Constants************/

void setup() {
  //Start the Serial Monitor and print out debugging info
  Serial.begin(115200);
  Serial.println("Starting");
//  Serial.print("freeMemory()=");
//  Serial.println(freeMemory());
//  Serial.println("Initialization Started");
  //initTestWalls();
  
  //Initialize the byte array we use to map out the maze
  //Note that for some of the arrays we are initializing we are initializing it to one 
  initializeSquareData();
  
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
  //**************LOW LEVEL CODE INIT***********
    pinMode(A2, OUTPUT);
  pinMode(5, OUTPUT);      // sets the digital pin as output
  pinMode(6, OUTPUT);      // sets the digital pin as output
  pinMode(7, OUTPUT);      // sets the digital pin as output
  pinMode(8, OUTPUT);      // sets the digital pin as output
  pinMode(9, OUTPUT);      // sets the digital pin as output
  pinMode(10, OUTPUT);      // sets the digital pin as output
  pinMode(11, OUTPUT);      // sets the digital pin as output
  pinMode(12, OUTPUT);      // sets the digital pin as output
  digitalWrite(12,HIGH); 
  digitalWrite(11,LOW); 
  digitalWrite(10,HIGH);
  digitalWrite(9,HIGH); 
  digitalWrite(8,LOW);
  Serial.begin(38400);
  analogReference(EXTERNAL);

  //*******Init IMU**********
  Serial.println("Initializing I2C devices...");

  //********END IMU INIT************
  //***************END LOW LEVEL CODE INIT*******************

  
  Serial.println("Initialization Finished");
}


void loop() {
 int timed = 150;
 //setExploredBit(7, 1, false); //unexplore random-ass square
  while(!isCenter()) {
    correctLinearity();
    updateWalls();
    //Serial.println("update walls");
   // outputMaze();
   // delay(timed);
    
    Serial.println("Correct Linearity)");
    correctLinearity();
  //  outputMaze();
  //  delay(timed);
    Serial.println("update Indicies");
    //Serial.println(getWallBitNorth(0,0));
    //Serial.println(getWallBitEast(0,0));
    updateIndicies();
   // outputMaze();
   // delay(timed);
    Serial.println("Update CentMIN)");
    updateCentMin();
  //  outputMaze();
   // delay(timed);
    Serial.println("Manhattan Sweep)");
    manhattanSweep();
    outputMaze();
  //  delay(timed);
    Serial.println("deadEndSweep");
    deadEndSweep(); 
   // outputMaze();
   // delay(timed);
    Serial.println("ParentSweep");
    parentSweep();
   // outputMaze();
   // delay(timed);
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
      moveUnit(getParentBit(Lx, Ly));
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
*  and adds the outer walls (sets to 1)
*/
void initializeSquareData()
{
  for (int i = 0; i < Size; i++)
  {
    for (int j = 0; j < Size; j++)
    {
      bitClear(squareData[i][j] ,1); //sets bit 1 to 0
      bitClear(squareData[i][j] ,2); //sets bit 2 to 0
      bitClear(squareData[i][j] ,3); //sets bit 3 to 0
      bitClear(squareData[i][j] ,4); //sets bit 4 to 0
      bitClear(squareData[i][j] ,5); //sets bit 5 to 0
      bitClear(squareData[i][j] ,6); //sets bit 6 to 0
      bitClear(squareData[i][j] ,7); //sets bit 7 to 0
      
    } 
    
  }  
  
  destroyVirtualWalls();  
  
  //Set South walls
  for (int q = 0; q <= Size; q++)
  {
    bitSet(squareData[q][0] ,4);
  }
  
  //Set North walls
  for (int q = 0; q <= Size; q++)
  {
    bitSet(squareData[q][Size] ,3);
  }
  
  //Set West walls
  for (int q = 0; q <= Size; q++)
  {
    bitSet(squareData[0][q] ,6);
  }
  
  //Set East walls
  for (int q = 0; q <= Size; q++)
  {
    bitSet(squareData[Size][q] ,5);
  }
}


/* setExploredBit
*  given an x and y location
*  sets the bit for explored to 1 (true)
*/
void setExploredBit(int lx, int ly)
{
  Serial.print("Square ");
  Serial.print(lx);
  Serial.print(" ");
  Serial.print(ly);
  Serial.print(" is being explored\n");
   bitSet(squareData[lx][ly], 0);
}

/*
*  Overload setExploredBit - pass bool as well to 
*  set explored to 0 (false)
*/
void setExploredBit(int lx, int ly, boolean flag)
{
   bitClear(squareData[lx][ly], 0);
}

/* setParentBit
*  given an x and y location, as well as a single 0 - 3 int,
*  sets the parent of the block at (x,y) 
*  by changing the values of bits 1 and 2
*  0 - North
*  1 - South
*  2 - East
*  3 - West
*/
void setParentBit(int lx, int ly, int q)
{
  //north (00)
  if (q == 0)
  {
    bitClear(squareData[lx][ly], 1); 
    bitClear(squareData[lx][ly], 2);
  }
  
  //south (01)
  else if (q == 1)
  {
    bitClear(squareData[lx][ly], 1);
    bitSet(squareData[lx][ly], 2);
  }
  
  //east (10)
  else if (q == 2)
  {
    bitSet(squareData[lx][ly], 1);
    bitClear(squareData[lx][ly], 2);
  }
  
  //west (11)
  else if (q == 3)
  {
    bitSet(squareData[lx][ly], 1);
    bitSet(squareData[lx][ly], 2);
  }
  
  else 
  {
    Serial.println("Invalid parent parameter"); 
  }
  
}

/* setWallBitNorth
*  given coordinates of square, sets its north wall (and the south wall of the one above it)
*  to true (1) 
*  by changing bit 3
*/
void setWallBitNorth(int lx, int ly)
{
   //add wall
    bitSet(squareData[lx][ly], 3);
    if (ly < Size)
    {
      bitSet(squareData[lx][ly + 1], 4); //sets south of neighboring square
    }

}
  
/*
*  Overload- add boolean flag to set
*  to 0 (false) instead
*/
void setWallBitNorth(int lx, int ly, boolean flag)
{
   bitClear(squareData[lx][ly], 3);
   if (ly < Size)
   {
     bitClear(squareData[lx][ly + 1], 4); //clears south of neighboring square
   }
}

/* setWallBitEast
*  given coordinates of square, sets its east wall (and the west wall of the one to its right)
*  to true (1) 
*  by changing bit 5
*/
void setWallBitEast(int lx, int ly)
{
    bitSet(squareData[lx][ly], 5);
    if (lx < Size)
    {
      bitSet(squareData[lx + 1][ly], 6); //sets west of neighboring square
    }
  
}

/*
*  Overload - add boolean flag to 
*  set to 0 (false) instead
*/
void setWallBitEast(int lx, int ly, boolean flag)
{
    bitClear(squareData[lx][ly], 5);
    if (lx < Size)
    {
      bitClear(squareData[lx + 1][ly], 6); //clears west of neighboring square
    }

   
}

/* setWallBitsSouth
*  given coordinates of square, sets its south wall (and the north wall of the one under it)
*  to true (1)
*  by changing bit 4
*/
void setWallBitSouth(int lx, int ly)
{
   bitSet(squareData[lx][ly], 4); 
   if (ly != 0)
   {
     bitSet(squareData[lx][ly - 1], 3); //sets north of neighboring square
   }

}

/*
*  Overload - pass a boolean flag to 
*  set value to false (0) instead
*/
void setWallBitSouth(int lx, int ly, boolean flag)
{ 
   bitClear(squareData[lx][ly], 4);
   if (ly != 0)
   {
     bitClear(squareData[lx][ly - 1], 3); //clears north of neighboring square
   }
}

/* setWallBitWest
*  given coordinates of a square, sets its west wall (and the east wall of the one to its left)
*  to true (1) 
*  by changing bit 6
*/
void setWallBitWest(int lx, int ly)
{
    bitSet(squareData[lx][ly], 6);
    if (lx != 0)
    {
      bitSet(squareData[lx - 1][ly], 5);  //sets east of neighboring square
    }
}

/*
*  Overload - pass a boolean flag to
*  set value to flase (0) instead
*/
void setWallBitWest(int lx, int ly, boolean flag)
{
    bitClear(squareData[lx][ly], 6); 
    if (lx != 0)
    {
      bitClear(squareData[lx - 1][ly], 5); //clears east of neighboring square
    }
}

/* 
*  setVirtualWallsBit
*  sets a square to be boxed by virtual walls 
*  1 - boxed, 0 - unboxed
*  all four walls done at once
*/
void setVirtualWallsBit(int lx, int ly)
{
   bitSet(squareData[lx][ly], 7);   
}


/*
*  Overload - pass in a boolean flag to remove all four walls instead
*  (value 0)
*/
void setVirtualWallsBit(int lx, int ly, boolean flag)
{
   bitClear(squareData[lx][ly], 7);
}

/*
*  getExploredBit
*  returns true if the given square is explored
*  and false if it is not yet explored
*/
boolean getExploredBit(int lx, int ly)
{
  if (bitRead(squareData[lx][ly], 0) == 1)
  {
    return true; //is explored
  }
  
  else if (bitRead(squareData[lx][ly], 0) == 0)
  {
    return false; //is not explored
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

boolean getWallBitNorth(int lx, int ly)
{
  if (bitRead(squareData[lx][ly], 3) == 1)
  {
     return true;
  }

  else if (bitRead(squareData[lx][ly], 3) == 0)
  {
    return false;
  }
}

boolean getWallBitSouth(int lx, int ly)
{
   if (bitRead(squareData[lx][ly], 4) == 1)
   {
      return true;
   }
 
   else if (bitRead(squareData[lx][ly], 4) == 0)
   { 
      return false;
   }
}

boolean getWallBitEast(int lx, int ly)
{
   if (bitRead(squareData[lx][ly], 5) == 1)
   {
      return true;
   }
 
   else if (bitRead(squareData[lx][ly], 5) == 0)
   { 
      return false;
   }


}

boolean getWallBitWest(int lx, int ly)
{
   if (bitRead(squareData[lx][ly], 6) == 1)
   {
      return true;
   }
 
   else if (bitRead(squareData[lx][ly], 6) == 0)
   { 
      return false;
   }


}

boolean getVirtualWallsBit(int lx, int ly)
{
   if (bitRead(squareData[lx][ly], 7) == 1)
   {
      return true;
   }
 
   else if (bitRead(squareData[lx][ly], 7) == 0)
   { 
      return false;
   }


}

boolean getWallN(int lx,int ly) {
    
    if (getWallBitNorth(lx, ly)) {
       return true;
    }
    else if (ly < Size) {   
  if( getVirtualWallsBit(lx,ly+1)){
    return true;
  }
}
return false;
}

boolean getWallS(int lx,int ly) {
    
    if (getWallBitSouth(lx, ly)) {
       return true;
    }
    else if ((ly > 0)) {   
  if( getVirtualWallsBit(lx,ly-1)){
    return true;
  }
}
return false;
}

boolean getWallE(int lx,int ly) {
    
    if (getWallBitEast(lx, ly)) {
       return true;
    }
    else if ((lx < Size)) {   
  if( getVirtualWallsBit(lx+1,ly)){
    return true;
  }
}
return false;
}

boolean getWallW(int lx,int ly) {
    
    if (getWallBitWest(lx, ly)) {
       return true;
    }
    else if ((lx > 0)) {   
  if( getVirtualWallsBit(lx-1,ly)){
    return true;
  }
}
return false;
}

/* initializeParents
*  Initializes the parents array to all fours (Since four is an invalid number for a parent if the four is ever used an runtime error will occur)
*  This method also initializes whereToGo to Size+1, again an invalid number
*  TODO: Figure out a better way to initialize these values instead of using an invalid number
*/
/*
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
*/


// initialize all parent bits to 00 (north) as default. Default is irrelevant
void initializeParentBit()
{
  Serial.println("Initialize Parents");
  for (int i = 0; i <= Size; i++)
  {
    for (int j = 0; i <= Size; j++)
    {
      bitClear(squareData[i][j], 1);
      bitClear(squareData[i][j], 2);
    } 
  }
  
  whereToGo[0] = Size + 1;
  whereToGo[1] = Size + 1;
}


/* updateCentMin
*  Checks the indices of all the center squares and makes sure centMin is set to the lowest values
*  TODO: Make it so that this method is not dependant on finalCenterSquare being initialized to higher than Size
*  can be left as is for now
*/
void updateCentMin() {
  if(finalCenterSquare[0]<Size) {      //TODO: Make it so that this mtethod is not dependant on finalCenterSquare being initialized to higher than Size
   centMin = I[finalCenterSquare[0]][finalCenterSquare[1]]; 
  }
}





/* initializeWalls
*  Initializes all the wall physical wall matrices to 0, except for the outer walls which are set to 1.(Since we know them to be there)
*  Also Initializes all the indices to 0 and the Explored Square Matrix to 0
*  now replaced by code in initializeSquareData()
*/
/*
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
*/

/*  destroyVirtualWalls
 *  Sets all virtual walls to zero in the entire matrix
 *  Also, used to initialize the virtual walls
 */
void destroyVirtualWalls() {
  for (int i = 0; i <= Size; i++) {
    for (int j = 0; j <= Size; j++) {
       bitClear(squareData[i][j] ,7); //sets bit 0 to 0 to destroy virtual walls
    }
  }
}

/* isCenter
*  Returns true if the robot is physically in one of the four center squares
*  doesn't require updating
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
*
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
*/

/* updated visitCenter
*  This function is supposed to be run when the robot first reaches a center square
*  Once we reach the center we know which of the 4 center squares is our "destination square." By knowing the "destination square" we can improve our sweep functions by using that square
*  in particular instead of considering all four center squares.
*  This method updates center 4 squares and creates this "destination square"
*  WARNING: -This method sets the physical walls around all the center squares to true. It relies on the parent of the "destination square" to remove the improperly set physical wall
*           -This method also assumes you have run all the sweeps and linearity checks
*/
void visitCenter()
{
  centMin = I[Lx][Ly];		//Update centMin with the index of your current square
  
  //Figure out the matrix indices for all the center squares
  int low = (Size/2);
  int high = (Size/2)+1;

  //Even though we haven't technically explored all the center squares we know everything about them so we can say we've explored them all
  bitSet(squareData[low][low], 0);
  bitSet(squareData[low][high], 0);
  bitSet(squareData[high][low], 0);
  bitSet(squareData[high][high], 0);

  
  //Sets all the physical walls of all four of the center squares to true
  setWallBitNorth(low,low);
  setWallBitSouth(low,low);
  setWallBitEast(low,low);
  setWallBitWest(low,low);

  setWallBitNorth(low,high);
  setWallBitSouth(low,high);
  setWallBitEast(low,high);
  setWallBitWest(low,high);
  
  setWallBitNorth(high,low);
  setWallBitSouth(high,low);
  setWallBitEast(high,low);
  setWallBitWest(high,low);

  setWallBitNorth(high, high);
  setWallBitSouth(high, high);
  setWallBitEast(high, high);
  setWallBitWest(high, high);

  //Sets the "destination square" to our current location
  finalCenterSquare[0]=Lx;
  finalCenterSquare[1]=Ly;
  
  //Delete the physical wall that is blocking the entrance to the center that was improperly set above
  if(getParentBit(Lx, Ly) == 0) {
    setWallBitNorth(Lx, Ly, false); //boolean flag indicates clearing a wall
    setWallBitSouth(Lx, Ly + 1, false);
  } 
  else if(getParentBit(Lx, Ly) == 1) {
    setWallBitSouth(Lx, Ly, false);
    setWallBitNorth(Lx, Ly - 1, false);
  } 
  else if(getParentBit(Lx, Ly) == 2) {
    setWallBitEast(Lx, Ly, false);
    setWallBitWest(Lx + 1, Ly, false);
  } 
  else if (getParentBit(Lx, Ly) == 3) {
    setWallBitWest(Lx, Ly, false);
    setWallBitEast(Lx - 1, Ly, false);
  }
}



// Reads sensor data and determines if there is a wall to the north
boolean isNorthWall() {
    if (testing) {
    //  return testWn[Lx][Ly];
    }
    else {
      return isWallNorth();
    }
      //sensor code
}
// Reads sensor data and determines if there is a wall to the south
boolean isSouthWall() {
    if (testing){
      if (Ly == 0)   {      //bottom wall always set
        return 1;
      }
      else {
       // return testWn[Lx][Ly - 1];   //reads north wall of square under it
      }
    }
    else{
      //sensor code
      isWallSouth();
    }
}
// Reads sensor data and determines if there is a wall to the west
boolean isWestWall() {
    if (testing) {
     // return testWw[Lx][Ly];
    }
    else{
      //sensor code
      isWallWest();
    }
}
// Reads sensor data and determines if there is a wall to the east
boolean isEastWall(){
    if (testing){
      if (Lx == Size){
         return 1;
      }
      else{ 
       //  return testWw[Lx + 1][Ly];
      }
    }
    else{
      //sensor code
      isWallEast();
    }
}


/* moveUnit
*  Pass in the desired direction you want the robot to move(N = 0, S = 1, E = 2, W = 3)
*  TODO: Motor code needs to be added
*/
void moveUnit(int x){
  if ((x == 0)&&!isWallNorth()) {
    moveSquare(0);
    Ly = Ly+1;
  }
  else if ((x == 1)&&(!isWallSouth())) {
     moveSquare(1);
    Ly = Ly -1;
  }
  else if ((x == 2)&&(!isWallEast())) {
     moveSquare(2);
    Lx = Lx + 1;
  }
  else if ((x == 3)&&(!isWallWest())) {
     moveSquare(3);
    Lx = Lx -1;
  }
  else {
  moveUnit(readNorth()%4);
  }
}

/* numberOfWalls
*  Returns the number of walls detectetd by the sensors in the square the robot is in
*  Never used, instead the two parameter function is used in dead end sweep
*
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
*/

/* numberOfWalls
*  Never used, instead the two parameter function is used in dead end sweep
*
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
*/

/* set****Wall
 * The set wall methods should always be used when mapping walls as the set the walls of the square you are in as well 
 * as the complementary walls of the adjacent squares
 * For example setting the west wall of one square will set the east wall of the adjacent square to the west
 * obsolete with new set methods
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
*/


/* setVirtual*****Wall
*  Performs the same task as the normal set wall method but with virtual walls
*  obsolete due to new set method
*/
/*
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
*/

/* getWall****
*  Returns true if either the real or virtual wall is set in the specified position
*  obsolete because of new get methods
*
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
*/

/* updateWalls
*  Reads in sensor values and sets the walls of the square the robot is in accordingly
*
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
*/


// updateWalls
// new, updated version of updateWalls
void updateWalls()
{
 if(isNorthWall())
 {
   setWallBitNorth(Lx, Ly);
 } 
 
 if(isSouthWall())
 {
   setWallBitSouth(Lx, Ly);
 } 
 
 if(isEastWall())
 {
   setWallBitEast(Lx, Ly);
 } 
 
 if(isWestWall())
 {
   setWallBitWest(Lx, Ly);
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
*/
void updateIndicies()
{
  setExploredBit(Lx, Ly); //current square is explored
  
  if ((!getWallN(Lx, Ly)) && (Ly < Size))
  {
    if (I[Lx][Ly + 1] == 0)
    {
      I[Lx][Ly+1] = I[Lx][Ly] + 1;
      setParentBit(Lx, Ly, 1);
    }
  }
  
  if ((!getWallS(Lx, Ly)) && (Ly > 0))
  {
    if (I[Lx][Ly - 1] == 0)
    {
      I[Lx][Ly - 1] = I[Lx][Ly] + 1;
      setParentBit(Lx, Ly, 0);
    }
  }
  
  if((!getWallW(Lx, Ly)) && (Lx > 0)) 
  {
    if(I[Lx-1][Ly] == 0) 
    {
      I[Lx-1][Ly] = I[Lx][Ly]+1;
      setParentBit(Lx-1, Ly, 2); 
    }
  }
  
  if((!getWallE(Lx, Ly)) && (Lx < Size)) 
  {
    if(I[Lx+1][Ly] == 0) 
    {
      I[Lx+1][Ly] = I[Lx][Ly]+1;
      setParentBit(Lx+1, Ly, 3);
    }
  }
  
}

/* checkLinearity
*  Checks to see if there are discrepancies between the indices of adjacent squares without walls in between them 
*  (i.e A 13 adjacent to a 9 with no wall in between)
*  If a discrepancy is found correctLinearity is called to fix it
*  TODO: I beleive checkLinearity might never be called and correctLinearity is called instead
*/
/*
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
*/


/* correctLinearity
*  Checks to see if there are discrpeancies between the indices of adjacent squares without walls in between them 
*  (i.e A 13 adjacent to a 9 with no wall in between)
*  If a discrepancy is found it will be corrected and linearity will be restored
*/
void correctLinearity() {
  //Serial.println("Correcting Linearity");
  //We must get rid of all the virtual walls because after the linearity correction paths that were previously ruled out might be viable
  //destroyVirtualWalls();    
  boolean flag = true;      //Set a boolean flag which will cause correct linearity to keep looping until the entire maze is linear
  while(flag) {
    flag = false;
    for (int i = 0; i <= Size; i ++) {      //Loop through the maze
      for (int j = 0; j <= Size; j++) {
        if (getExploredBit(i, j)) {                      //We are only concerned with the linearity of explored squares as they are the only squares where we know where the walls are
          // North Wall
          if (j < Size)        //don't look to the north if we are at the top of the maze 
          {
            if (!getWallN(i, j)&&getExploredBit(i,j+1)) {
              if ((I[i][j] - I[i][j+1]) > 1) {
                I[i][j] = I[i][j+1]+1;        //If there is a wall to the North with lower 
                flag = true;
              }
              else if (((I[i][j] - I[i][j+1]) < -1)) {
                I[i][j+1] = I[i][j]+1;
                flag = true;
              }
            }
          }
          // South Wall
          if (j > 0)                   //don't look to the south if we are at the bottom of the maze 
          {
            if (!getWallS(i, j)&&getExploredBit(i,j-1)) {
              if ((I[i][j] - I[i][j-1]) > 1) {
                I[i][j] = I[i][j-1]+1;
                flag = true;
              }
              else if (((I[i][j] - I[i][j-1]) < -1)) {
                I[i][j-1] = I[i][j]+1;
                flag = true;
              }
            }
          }
          // West Wall
          if (i  > 0)                //don't look to the west if we are at the left edge of the maze 
          {
            if (!getWallW(i, j)&&getExploredBit(i-1,j)) {
              if ((I[i][j] - I[i-1][j]) > 1) {
                I[i][j] = I[i-1][j]+1;
                flag = true;
              }
              else if (((I[i][j] - I[i-1][j]) < -1)) {
                I[i-1][j] = I[i][j]+1;
                flag = true;
              }
            }
          }
          
          
          // East Wall
          if (i < Size)            //don't look to the north if we are at the top of the maze 
          {
            if (!getWallE(i, j)&&getExploredBit(i+1,j)) {
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
  Serial.print("Current Location: ");
  Serial.print(Lx);
  Serial.print(Ly);
  Serial.print("\n");
  boolean flag = true;
  while(flag) {
    flag = false; 
    for (int i = 0; i <= Size; i++) {
      for (int j = 0; j <= Size; j++) {
        if ((I[i][j] != 1) && !((Lx == i)&&(Ly == j)) && (!isCenter(i,j))) {
                    if(numberOfWalls(i,j) == 3) {
                      Serial.print(" Three Walls in: ");
                      Serial.print(i);
                      Serial.print(" ,");
                      Serial.println(j);
                      
                      Serial.println("Virtual walls: ");
                      Serial.println(getVirtualWallsBit(i, j));
                      
                      Serial.println("N, S, E, W: ");
                      Serial.println(getWallN(i ,j));
                      Serial.println(getWallS(i, j));
                      Serial.println(getWallE(i, j));
                      Serial.println(getWallW(i, j));
                      
                      setAllVirtualWalls(i,j);
                      flag = true;
                    }
          if(onlyLowerIndicies(i,j)) {
            Serial.print(" Only lower indexes in: ");
            Serial.print(i);
            Serial.print(" ,");
            Serial.println(j);
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
  if(!getWallN(lx, ly)) {
    if(I[lx][ly+1] == (I[lx][ly]-1)) {
      flag++;
    }
  }
  if(!getWallS(lx, ly)) {
    if(I[lx][ly-1] == (I[lx][ly]-1)) {
      flag++;
    }
  }
  if(!getWallE(lx, ly)) {
    if(I[lx+1][ly] == (I[lx][ly]-1)) {
      flag++;
    }
  }
  if(!getWallW(lx, ly)) {
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
         if(getExploredBit(i,j) != 0) { 
         if(goneTooFar(i,j)) { 
           if(!isCenter(i,j)) { 
             Serial.print("Gone Too Far in: "); 
             Serial.print(i); 
             Serial.print(", "); 
             Serial.println(j); 
             if ((Lx != i) && (Ly != j))
             {
               setAllVirtualWalls(i,j); 
             }
           } 
         } 
         } 
       } 
     } 
   } 
 } 


//checks whether any parent directions point towards walls. If so, corrects parent direction for that square
void parentSweep()
{
  Serial.println("Correcting Parents: ");
  for(int lx = 0; lx <= Size; lx++) {
    for(int ly = 0; ly <= Size; ly++) {
  if (lx>Size) {
  } else if (ly>Size) {
    //do nothing
  }else if(isCenter(lx,ly)){
   //do nothing
  } else {
  if(!getWallN(lx,ly)) {
    if(ly < Size){
      if((I[lx][ly]-1) == I[lx][ly+1]) {
        setParentBit(lx, ly, 0);
      }
    }
  }
  if(!getWallS(lx,ly)) {
    if(ly > 0){
      if((I[lx][ly]-1) == I[lx][ly-1]) {
        setParentBit(lx, ly, 1);
      }
    }
  }
  if(!getWallE(lx,ly)) {
    if(lx < Size){
      if((I[lx][ly]-1) == I[lx+1][ly]) {
        setParentBit(lx, ly, 2);
      }
    }
  }
  if(!getWallW(lx,ly)) {
    if(lx > 0){
      if((I[lx][ly]-1) == I[lx-1][ly]) {
        setParentBit(lx, ly, 3);
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
  if(getWallN(lx,ly)) {
    numberOfWalls++; 
  }
  if(getWallS(lx,ly)) {
    numberOfWalls++; 
  }
  if(getWallW(lx,ly)) {
    numberOfWalls++; 
  }
  if(getWallE(lx,ly)) {
    numberOfWalls++; 
  }
  if (getVirtualWallsBit(lx,ly)) {
   numberOfWalls = 4; 
  }
  return numberOfWalls;
}

//Sets all the virtual walls of the specified square to true
//Used in the dead end sweep
//now up-to-date with new method
void setAllVirtualWalls(int lx, int ly) {
  //  Serial.print("Setting Virtual walls");
  if(!isCenter(lx,ly)) {
    //setVirtualNorthWall(lx,ly); virtual walls no longer set individually
    //setVirtualSouthWall(lx,ly);
    //setVirtualWestWall(lx,ly);
    //setVirtualEastWall(lx,ly);
    setVirtualWallsBit(lx, ly); //now does all four of the above
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
   if (!getWallN(Lx,Ly)) { // If there's not a wall 
     if (!getExploredBit(Lx,Ly+1)) { // And it's not visited 
       double movesTo = movesToCenter(Lx,Ly+1); 
       if (movesTo < smallestMD) { // If the manhattan distance is smaller than the current smallest manhattan distance 
         smallestMD = movesTo; // It updates the smallest manhattan distance 
         dir = 0; // And sets the relevant direction 
       } 
     } 
   } 
   // CHECKS SOUTH 
   if (!getWallS(Lx,Ly)) { // If there's not a wall 
     if (!getExploredBit(Lx,Ly-1)) { // And it's not visited 
       double movesTo = movesToCenter(Lx,Ly-1); 
       if (movesTo < smallestMD) { // If the manhattan distance is smaller than the current smallest manhattan distance 
         smallestMD = movesTo; // It updates the smallest manhattan distance 
         dir = 1; // And sets the relevant direction 
       } 
     } 
   } 
   // CHECKS EAST 
   if (!getWallE(Lx,Ly)) { // If there's not a wall 
     if (!getExploredBit(Lx+1,Ly)) { // And it's not visited 
     double movesTo = movesToCenter(Lx+1,Ly); 
       if (movesTo < smallestMD) { // If the manhattan distance is smaller than the current smallest manhattan distance 
         smallestMD = movesTo; // It updates the smallest manhattan distance 
         dir = 2; // And sets the relevant direction 
       } 
     } 
   } 
   // CHECKS WEST 
   if (!getWallW(Lx,Ly)) { // If there's not a wall 
     if (!getExploredBit(Lx-1,Ly)) { // And it's not visited 
       double movesTo = movesToCenter(Lx-1,Ly); 
       if (movesTo < smallestMD) { // If the manhattan distance is smaller than the current smallest manhattan distance 
         smallestMD = movesTo; // It updates the smallest manhattan distance 
         dir = 3; // And sets the relevant direction 
       } 
     } 
   } 
   // CHECKS FOR ODDITIES (There were no unexplored options) 
   if (smallestMD == 257) { 
     
     //EAST 
     if (!getWallE(Lx,Ly)) { // If there's not a wall 
       if ((I[Lx][Ly]+1) == I[Lx+1][Ly] ) { 
         double movesTo = movesToCenter(Lx+1,Ly); 
         if (movesTo < smallestMD) { // If the manhattan distance is smaller than the current smallest manhattan distance 
           smallestMD = movesTo; // It updates the smallest manhattan distance 
           dir = 2; // And sets the relevant direction 
         } 
       } 
     } 
     
     
     //NORTH 
     if (!getWallN(Lx,Ly)) { // If there's not a wall 
       if ((I[Lx][Ly]+1) == I[Lx][Ly+1] ) { 
         double movesTo = movesToCenter(Lx,Ly+1); 
         if (movesTo < smallestMD) { // If the manhattan distance is smaller than the current smallest manhattan distance 
           smallestMD = movesTo; // It updates the smallest manhattan distance 
           dir = 0; // And sets the relevant direction 
         } 
       } 
     } 
     //SOUTH 
     if (!getWallS(Lx,Ly)) { // If there's not a wall 
       if ((I[Lx][Ly]+1) == I[Lx][Ly-1] ) { 
         double movesTo = movesToCenter(Lx,Ly-1); 
         if (movesTo < smallestMD) { // If the manhattan distance is smaller than the current smallest manhattan distance 
           smallestMD = movesTo; // It updates the smallest manhattan distance 
           dir = 1; // And sets the relevant direction 
         } 
       } 
     } 
     
     //WEST 
     if (!getWallW(Lx,Ly)) { // If there's not a wall 
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
     if (!getWallN(Lx,Ly)) { 
       dir = 0; 
     } 
     if (!getWallS(Lx,Ly)) { 
       dir = 1; 
     } 
     if (!getWallE(Lx,Ly)) { 
       dir = 2; 
     } 
     if (!getWallW(Lx,Ly)) { 
       dir = 3; 
     } 
   } 
    
   if (dir == 4) { 
     while(true) { 
       Serial.print("INVALID MOVE"); 
       moveUnit(readNorth()%4);
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
      if(!getExploredBit(i,j)) {
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
     breadCrumbs[c] = readBackwards(getParentBit(i,j)); 
     c++; 
     if(getParentBit(i,j) == 0) { 
       j=j+1; 
     } 
     else if(getParentBit(i,j) == 1) { 
       j=j-1; 
     } 
     else if(getParentBit(i,j) == 2) { 
       i= i + 1; 
     } 
     else if(getParentBit(i,j) == 3) { 
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
     breadCrumbs[c] = getParentBit(i,j); 
     c++; 
     if(getParentBit(i,j) == 0) { 
       j=j+1; 
     } 
     else if(getParentBit(i,j) == 1) { 
       j=j-1; 
     } 
     else if(getParentBit(i,j) == 2) { 
       i= i + 1; 
     } 
     else if(getParentBit(i,j) == 3) { 
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
      if(getExploredBit(i,j)) {
        if(numberOfWalls(i,j) != 4) {
          if(!getWallN(i,j)) {
              //WHY IS THERE A 7 HERE?????
            if(j<Size) {
              if(!getExploredBit(i,j+1)) {
                return false;
              }
            }
          }
          if(!getWallS(i,j)) {
            if(j>=1) {
              if(!getExploredBit(i,j-1)) {
                return false;
              }
            }//one fewer space because of double digit index
          }
          if(!getWallE(i,j)) {
            if(i<Size) {
              if(!getExploredBit(i+1,j)) {
                return false;
              }
            }
          }
          if(!getWallW(i,j)) {
            if(j>=1) {
              if(!getExploredBit(i-1,j)) {
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
      moveUnit(getParentBit(Lx, Ly));
    }
  }
}

/*
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
*/

//outputMaze() take 2
void outputMaze() 
{ 
  for(int j=testSize;j>=0;j--) {
    for (int i=0;i<=testSize;i++) {
      Serial.print("#");
      if((getWallBitNorth(i, j))) {   //print top row
          Serial.print("#####");
        //virtual walls
      } 
      else if(getVirtualWallsBit(i, j) || (getVirtualWallsBit(i, j + 1))) {
          Serial.print("@@@@@");
      }
      else {
        Serial.print("     ");
      }
    } 
    Serial.println("#");

    for(int k=0; k<=2;k++) {
      Serial.print("#");
      for (int i=0;i<=testSize;i++) {
        if (getWallBitEast(i,j)) {
          if(k==1) {                   //prints index of square
            if(I[i][j] > 9){     
              Serial.print(" ");       //one fewer space because of double digit index
              Serial.print(I[i][j]);
              Serial.print("  #");     //begins to print wall
            }
            else{
              Serial.print("  ");
              Serial.print(I[i][j]);
              Serial.print("  #");
            }
          }
          else if(((k==0)&&(i==Lx))&&(j==Ly)) {
            Serial.print(" $$$ #");          //prints three dollar signs if robot is in the square
          } 
          else if((k==2)&&(getExploredBit(i, j))) {
            Serial.print("    *#");          //prints explored asterisk in bottom right
          }
          else {
            Serial.print("     #");
          }
        } 
        else if (getVirtualWallsBit(i,j) || (getVirtualWallsBit(i + 1, j))) {
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
          else if((k==2)&&(getExploredBit(i, j))) {
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
          else if((k==2)&&(getExploredBit(i, j))) {
            Serial.print("    * "); 
          }
          else {
            Serial.print("      ");
          }
        }
      }
      Serial.println();      //next line at end of for loop
    }
  }
  for(int i=0;i<=testSize;i++){
    Serial.print("######");  //prints bottom wall
  }
  Serial.println("#");
  Serial.println();          //final indent
}

//***************************BEGIN LOW LEVEL CODE***************************************




void displaySensors() {
  int north = readNorth();
  int south = readSouth();
  int east = readEast();
  int west = readWest();
  Serial.print("N: ");
  Serial.print(north);
  Serial.print("   S: ");
  Serial.print(south);
  Serial.print("  W: ");
  Serial.print(west);
  Serial.print("  E: ");
  Serial.println(east);

}

int readNorth() {
  return analogRead(A5); 
}

int readSouth() {
  return analogRead(A1); 
}

int readEast() {
  return analogRead(A3); 
}

int readWest() {
  return analogRead(A4); 
}
/*
void readIMU() {
  accelgyro.getMotion9(&ax, &ay, &az, &gx, &gy, &gz, &mx, &my, &mz);

  // these methods (and a few others) are also available
  //accelgyro.getAcceleration(&ax, &ay, &az);
  //accelgyro.getRotation(&gx, &gy, &gz);

  // display tab-separated accel/gyro x/y/z values
  Serial.print("a/g/m:\t");
  Serial.print(ax); 
  Serial.print("\t");
  Serial.print(ay); 
  Serial.print("\t");
  Serial.print(az); 
  Serial.print("\t");
  Serial.print(gx); 
  Serial.print("\t");
  Serial.print(gy); 
  Serial.print("\t");
  Serial.print(gz); 
  Serial.print("\t");
  Serial.print(mx); 
  Serial.print("\t");
  Serial.print(my); 
  Serial.print("\t");
  Serial.println(mz);  
}
*/


void moveSquare(int dir) {
  //error checking stuff
  int error_update_rate  =60;
  int quarter_update_rate = error_update_rate/4;
  int north = 0;
  int south = 0;
  int east = 0;
  int west = 0;
  int which_sensor =0;
  const int CORRECTION_SAMPLES = 3;
  int corrections[CORRECTION_SAMPLES];
  int number_of_cor = 0;
  boolean error_flag = false;

  int half_precision = MOTOR_PRECISION/2;
  setMotors(dir);
  boolean flag1 = false;
  boolean flag2 = false;
  //Init our error and correction variables
  int error = 0;
  int correction = error/ERROR_SCALE_FACTOR;

  //Set which motors to step
  int motor1;
  int motor2;
  if (dir == 0) {
    motor1 = 5;
    motor2 = A2;
  } 
  else if (dir == 1) {
    motor1 = A2;
    motor2 = 5; 
  }
  else if(dir == 2) {
    motor1 = 6;
    motor2 = 7; 
  }
  else {
    motor2 = 6;
    motor1 = 7;
  }

  //ramp stuff
  boolean ramp_up = true;
  boolean ramp_down = false;
  int delaytime  = RAMP_DELAY/half_precision;
  int ramp_amount = ((RAMP_DELAY - MOTOR_DELAY)/RAMP_LENGTH)/half_precision;
  int number_of_steps = STEPS_PER_SQ*MOTOR_PRECISION;

  //Wall to no wall sensing stuff
  int junk;
  int wall_state = getPerpendicularWalls(dir);
  Serial.print("Initial Wall State: ");
  Serial.println(wall_state); 
  int wall_debounce_count = 0;
  boolean isWallL;  //left sensor relative to the motion
  boolean isWallR; //right sesor relative to the motion
  getLeftRightWalls(isWallL, isWallR,dir); //Uses a pass by reference to init the booleans
  boolean atend = false;

  //Start the stepping
  int NN = 0;
  for  (int i = 0; i <= number_of_steps; i++) {
    //How often you step one motor
    if (i%(half_precision+correction) == 0) {
      if(flag1) {
        digitalWrite(motor1,HIGH); //5
      } 
      else {
        digitalWrite(motor1,LOW);
      }
      flag1 = !flag1;
    }

    //How often you step the other motor
    if (i%(half_precision-correction) == 0) {
      if(flag2) {
        digitalWrite(motor2,HIGH);  //16
      } 
      else {
        digitalWrite(motor2,LOW);
      }
      flag2 = !flag2;
    }
    //update the errors
    if(i%quarter_update_rate==0&&which_sensor==0) {
      north = readNorth();  
      which_sensor++;
    } 
    else if(i%quarter_update_rate==0&&which_sensor==1) {
      south = readSouth(); 
      which_sensor++;
    } 
    else if(i%quarter_update_rate==0&&which_sensor==2) {
      east = readEast(); 
      which_sensor++;
    } 
    else if(i%quarter_update_rate==0) {
      west = readWest(); 
      which_sensor = 0;
    } 


    delayMicroseconds(delaytime);

    //Update Error 
    if (i%error_update_rate == 0) {
      
      if (rudder(dir)&&!atend&&(NN<4)) {
        number_of_steps = number_of_steps + 700;
    
      } else {
       NN++;
      }
      error = getError(dir,north,south,east,west,error_flag); 
      if(!error_flag) { 
        correction = error/ERROR_SCALE_FACTOR;
      }
      else {
        correction = error*(2/3);
      }
      

      //Average the corrections
      /*
      if (number_of_cor < CORRECTION_SAMPLES) {
        corrections[number_of_cor++] = correction;
      }
      else {
        number_of_cor = 0;
        corrections[0] = correction; 
      }
      int sum = 0;
      for (int j = 0; j <CORRECTION_SAMPLES; j++) {
        sum += corrections[j];
      }
      correction = sum/CORRECTION_SAMPLES;
*/
      //Clip the corrections if too bug
      if (correction > CORRECTION_CAP) {
        correction = CORRECTION_CAP;
      }
      else if (correction < -CORRECTION_CAP) {
        correction = -CORRECTION_CAP; 
      }
     // Serial.print("Error");
      //Serial.println(error);
      //Serial.print("Corrc");
      //Serial.println(correction);
    }

    //Check to see if the number of walls around you has changed
    if(i%100 == 0&&!atend) {
      if (getPerpendicularWalls(dir,north,south,east,west) != wall_state) {
       // Serial.print("Wall State: ");
       // Serial.println(getPerpendicularWalls(dir,north,south,east,west));  
        wall_debounce_count++;
        if (wall_debounce_count >8) {    //Set the robot up to go x amount of steps more
          atend = true;
          boolean oldWallL = isWallL; 
          boolean oldWallR = isWallR;
          getLeftRightWalls(isWallL, isWallR,dir); //Uses a pass by reference to init the booleans
          if (oldWallL != isWallL) {
            //Since the left wall has changed states we need to go a bit further
            i = 0;
            number_of_steps = 3800;
          //  Serial.println(number_of_steps);
          }
          else {
            //Since the right wall has changed we barely have anything left to go
            i = 0;
            number_of_steps = 8100;
           // Serial.println(number_of_steps);
          }
        }
      }
      else {
        wall_debounce_count = 0;
      }
    }


  }
  //smashCorner();
  //smashCorner();
  delay(200);
  disableAllMotors();
}

//7 - step North Motor
//6 - step South Motor
//5 - step East Motor
//A2 - step West Motor

boolean rudder(int dir) {
  int const BACK_RUDDER_THRESH = 500;
  int const RUDDER_THRESH = 625;
  int const RUDDER_LENGTH = 8;
  int const RUDDER_DELAY= 5000;
 int diff_spin = 7;
 int const BACK_RUDDER_LENGTH = 14;
 
  if (dir == 0) {
    if (readEast() > RUDDER_THRESH) {
     
      digitalWrite(12,LOW);
      //spin the north south west motors
      for(int i =0; i < RUDDER_LENGTH; i++) {
        digitalWrite(7,HIGH);
        digitalWrite(6,HIGH);
        delayMicroseconds(RUDDER_DELAY);
        digitalWrite(7,LOW);
        digitalWrite(6,LOW);
        delayMicroseconds(RUDDER_DELAY);
      }
      digitalWrite(12,HIGH);
      return true;
    }
    else if (readWest() > BACK_RUDDER_THRESH) { //go east

      digitalWrite(12,HIGH);
      for(int i =0; i < BACK_RUDDER_LENGTH; i++) {
        if( i > diff_spin) {
        digitalWrite(6,HIGH);
        }
        digitalWrite(7,HIGH);
        delayMicroseconds(RUDDER_DELAY);
        digitalWrite(7,LOW);
        digitalWrite(6,LOW);
        delayMicroseconds(RUDDER_DELAY);
      }
      digitalWrite(12,HIGH);
      return true;
    }
  }
  if (dir == 1) {
    if(readWest() > RUDDER_THRESH) {
      
      digitalWrite(12,HIGH);
      //spin the  motors
      for(int i =0; i < RUDDER_LENGTH; i++) {
        digitalWrite(6,HIGH);
        digitalWrite(7,HIGH);
        delayMicroseconds(RUDDER_DELAY);
        digitalWrite(6,LOW);
        digitalWrite(7,LOW);
        delayMicroseconds(RUDDER_DELAY);
      }
      digitalWrite(12,LOW);
      return true;
    }
    else if (readEast() > BACK_RUDDER_THRESH) { //go west
    
      digitalWrite(12,LOW);
      for(int i =0; i < BACK_RUDDER_LENGTH; i++) {
        if( i >diff_spin) { 
        digitalWrite(7,HIGH);
        }
        digitalWrite(6,HIGH);
        delayMicroseconds(RUDDER_DELAY);
        digitalWrite(7,LOW);
        digitalWrite(6,LOW);
        delayMicroseconds(RUDDER_DELAY);
      }
      digitalWrite(12,LOW);
      return true;
    }
  }
  if (dir ==2) {
    if(readSouth() > RUDDER_THRESH) {
      
      digitalWrite(12,HIGH);
      for(int i =0; i < RUDDER_LENGTH; i++) {
        digitalWrite(5,HIGH);
        digitalWrite(A2,HIGH);
        delayMicroseconds(RUDDER_DELAY);
        digitalWrite(5,LOW);
        digitalWrite(A2,LOW);
        delayMicroseconds(RUDDER_DELAY);
      }
      return true;
    }
    else if (readNorth() > BACK_RUDDER_THRESH) { 
      
      digitalWrite(12,LOW);
      for(int i =0; i < BACK_RUDDER_LENGTH; i++) {
        if (i > diff_spin) {
        digitalWrite(A2,HIGH);
        }
        digitalWrite(5,HIGH);
        delayMicroseconds(RUDDER_DELAY);
        digitalWrite(5,LOW);
        digitalWrite(A2,LOW);
        
        delayMicroseconds(RUDDER_DELAY);
      }
      
      digitalWrite(12,HIGH);
      return true;
    }
  }
    if (dir ==3) {
    if(readNorth() > RUDDER_THRESH) {
     
      digitalWrite(12,LOW);
      for(int i =0; i < RUDDER_LENGTH; i++) {
        digitalWrite(5,HIGH);
        digitalWrite(A2,HIGH);
        delayMicroseconds(RUDDER_DELAY);
        digitalWrite(5,LOW);
        digitalWrite(A2,LOW);
        delayMicroseconds(RUDDER_DELAY);
        return true;
      }
    }
    else if (readSouth() > BACK_RUDDER_THRESH) { 
      
      digitalWrite(12,HIGH);
      for(int i =0; i < BACK_RUDDER_LENGTH; i++) {
        if(i > diff_spin) {
        digitalWrite(5,HIGH);
        }
        digitalWrite(A2,HIGH);
        delayMicroseconds(RUDDER_DELAY);
        digitalWrite(5,LOW);
        digitalWrite(A2,LOW);
        delayMicroseconds(RUDDER_DELAY);
      }
      digitalWrite(12,LOW);
      return true;
    }
    
  }
  return false;
  
  /*else if(readWest() > 700) {
   //spin the south and east motors
   for(int i =0; i < 10; i++) {
   digitalWrite(7,HIGH);
   digitalWrite(A2,HIGH);
   delayMicroseconds(5000);
   digitalWrite(7,LOW);
   digitalWrite(A2,LOW);
   delayMicroseconds(5000);
   }
   }*/
}


//7 - step North Motor
//6 - step South Motor
//5 - step East Motor
//A2 - step West Motor
int smashCorner() {
  enableAllMotors();
  if(isWallNorth() && isWallEast()) {
    digitalWrite(12,HIGH);
    boolean flag11 = false;
    boolean stopsmash = false;
    for(int i = 0; i <= SMASH_STEPS; i++) {
      displaySensors();
      if ((readNorth() > 670)&&(readEast() > NEXT_TO_WALL_THRESH)){
        stopsmash = true; 
      }
      if (!stopsmash) {
        digitalWrite(7,LOW);
      }
      if ((readNorth() > 740 && readEast() > 740)) {
        // displaySensors();
        flag11 = true; 
      }

      digitalWrite(A2,LOW);
      digitalWrite(5,LOW);
      digitalWrite(6,LOW);
      delayMicroseconds(SMASH_DELAY);
      digitalWrite(6,HIGH);
      digitalWrite(A2,HIGH);
      digitalWrite(5,HIGH);
      digitalWrite(7,HIGH);
      delayMicroseconds(SMASH_DELAY);
    }
    digitalWrite(12,LOW);
    delay(500);
    for(int i = 0; i <= SMASH_STEPS_OUT; i++) {
   
      
      boolean stopS =false;
      boolean stopE = false;
      if (readNorth() < CENTERDIST-80) {
        stopS = true; 
      }
      if (readEast() < CENTERDIST-100) {
        stopE = true; 
      }
      if(!stopS) {
        digitalWrite(5,LOW);
        digitalWrite(A2,LOW);
      }
      if(!stopE) {
        digitalWrite(6,LOW);
        digitalWrite(7,LOW);
      }
      delayMicroseconds(SMASH_DELAY);
      digitalWrite(6,HIGH);
      if ( i < 25) {
      digitalWrite(5,HIGH);
      }
      digitalWrite(A2,HIGH);
      digitalWrite(7,HIGH);
      delayMicroseconds(SMASH_DELAY);
    }
    if (flag11) {
      delay(500);
      // Serial.println("Flag is Tru");
      delay(100);
      smashCorner(); 

    }
    //Serial.println("Flag is NOT Tru");
    delay(100);   

  }
  /*
  else if(isWallSouth() &&isWallEast()) {
   for(int i = 0; i <= SMASH_STEPS; i++) {
   digitalWrite(6,LOW);
   digitalWrite(A2,LOW);
   digitalWrite(5,LOW);
   digitalWrite(7,LOW);
   delayMicroseconds(SMASH_DELAY);
   digitalWrite(6,HIGH);
   digitalWrite(A2,HIGH);
   digitalWrite(5,HIGH);
   digitalWrite(7,HIGH);
   delayMicroseconds(SMASH_DELAY);
   } 
   }
   */
  else if(isWallSouth() && isWallWest()) {
    digitalWrite(12,LOW);
    boolean flag11 = false;
    boolean stopsmash = false;
    for(int i = 0; i <= SMASH_STEPS; i++) {
      if ((readSouth() > NEXT_TO_WALL_THRESH)&&(readWest() > NEXT_TO_WALL_THRESH)){
        stopsmash = true; 
      }
      if (!stopsmash) {
        digitalWrite(6,LOW);
      }

      if ((readSouth() > 750 && readWest() > 750)) {
        // displaySensors();
        flag11 = true; 

      }
      digitalWrite(A2,LOW);
      digitalWrite(5,LOW);
      digitalWrite(7,LOW);

      delayMicroseconds(SMASH_DELAY);
      digitalWrite(6,HIGH);
      digitalWrite(A2,HIGH);
      digitalWrite(5,HIGH);
      digitalWrite(7,HIGH);
      delayMicroseconds(SMASH_DELAY);
    }
    digitalWrite(12,HIGH);
    for(int i = 0; i <= SMASH_STEPS_OUT; i++) {
      boolean stopN =false;
      boolean stopW = false;
      if (readSouth() < CENTERDIST-100) {
        stopN = true; 
      }
      if (readWest() < CENTERDIST-100) {
        stopW = true; 
      }
      if(!stopN) {
        digitalWrite(5,LOW);
        digitalWrite(A2,LOW);
      }
      if(!stopW) {
        digitalWrite(6,LOW);
        digitalWrite(7,LOW);
      }
      delayMicroseconds(SMASH_DELAY);
      digitalWrite(6,HIGH);
      digitalWrite(A2,HIGH);
      digitalWrite(5,HIGH);
      digitalWrite(7,HIGH);
      delayMicroseconds(SMASH_DELAY);
    }   
    if (flag11) {
      delay(500);
      // Serial.println("Flag is Tru");
      delay(100);
      smashCorner(); 

    }
    // Serial.println("Flag is NOT Tru");
    delay(100);
  }
  /*
  else if(isWallNorth() && isWallWest()) {
   for(int i = 0; i <= SMASH_STEPS; i++) {
   digitalWrite(6,LOW);
   digitalWrite(A2,LOW);
   digitalWrite(5,LOW);
   digitalWrite(7,LOW);
   delayMicroseconds(SMASH_DELAY);
   digitalWrite(6,HIGH);
   digitalWrite(A2,HIGH);
   digitalWrite(5,HIGH);
   digitalWrite(7,HIGH);
   delayMicroseconds(SMASH_DELAY);
   } 
   }
   */

  delay(50);
  disableAllMotors();
}

//NOTE THE PASS by reference
//Returns whether or not there are walls to the left or right of the robot
void getLeftRightWalls(boolean &isWallL,boolean &isWallR,int dir) {
  if (dir == 0) {
    isWallL = isWallWest();
    isWallR = isWallEast();
  }
  else if (dir == 1) {
    isWallR = isWallWest();
    isWallL = isWallEast();
  }
  else if(dir == 2) {
    isWallL = isWallNorth();
    isWallR = isWallSouth();
  }
  else{
    isWallR = isWallNorth();
    isWallL = isWallSouth();
  }
}

//Relative to the way the robot is moving
//Returns 0 if there are no walls to the left or right
//Returns 1 if there is only one wall to either the left or right
//Returns 2 if the robot is in a tube

int getPerpendicularWalls(int dir,int north,int south,int east,int west) {
  if(inTube(dir,north,south,east,west)) {
    return 2; 
  } 
  else if((dir == 0) || (dir == 1)) {
    if ((east > IS_WALL_THRESH)||west > IS_WALL_THRESH) {
      return 1; 
    }
  }
  else if((dir == 2) || (dir == 3)) {
    if ((north > IS_WALL_THRESH)||south > IS_WALL_THRESH) {
      return 1; 
    }
  }
  else {
    return 0; 
  }
  return 0;
}

int getPerpendicularWalls(int dir) {
  if(inTube(dir)) {
    return 2; 
  } 
  else if((dir == 0) || (dir == 1)) {
    if ((readEast() > IS_WALL_THRESH)||readWest() > IS_WALL_THRESH) {
      return 1; 
    }
  }
  else if((dir == 2) || (dir == 3)) {
    if ((readNorth() > IS_WALL_THRESH)||readSouth() > IS_WALL_THRESH) {
      return 1; 
    }
  }
  else {
    return 0; 
  }
  return 0;
}

int getError(int dir,int north,int south,int east,int west,boolean & flag) {
  /*  int north = readNorth();
   int south = readSouth();
   int east = readEast();
   int west = readWest();
   */
  const int CLOSE_TO_WALL = 500;
  if  (inTube(dir,north,south,east,west)) {
    flag = false;
    return findTubeError(dir,north,south,east,west); 
  }
  else { //basically if there is only a wall on one side of the maze //NUMBERS are OPTIMIZED
    flag = true;
    if((north > IS_WALL_THRESH)&&(dir==2)) {
      int northsqrt = sqrt(north);
      if (north <= 350){//North is optimized
        return (northsqrt - 22);
      }
      else if (north > 425) {
        return (northsqrt - 21);
      }
      else if (north > 375 && north < 390) {
        return (northsqrt - 21); 
      }
      else {
        return 0; 
      }
    }
    else if((north > IS_WALL_THRESH)&&(dir==3)) {
      int northsqrt = sqrt(north);
      if (north <= 350){//North is optimized
        return (22 - northsqrt);
      }
      else if (north > 375 && north < 390) {
        return (21 - northsqrt);
      }
      else if (north > 425) {
        return (21 - northsqrt);
      }
      else {
        return 0;
      }
    }
    else if((south > IS_WALL_THRESH)&&(dir==2)) {//souths are good for center dist is 21
      int southsqrt = sqrt(south);
      if (south >= 375 && south <= 425){
        return (21 - southsqrt);
      }
      else {
        return 0;
      }
    }
    else if((south > IS_WALL_THRESH)&&(dir==3)) {
      int southsqrt = sqrt(south);
      if (south >= 375 && south <= 425) {
        return 0; 
      }
      else {
        return (southsqrt - 21);
      }
    }
    else if((east > IS_WALL_THRESH)&&(dir==0)) {//easts are good for center dist is 21
      int eastsqrt = sqrt(east);
      if (east >= 375 && east <= 425){
        return 0;
      }
     else { 
      return (21 - eastsqrt);
     }
    }
    else if((east > IS_WALL_THRESH)&&(dir==1)) {
      int eastsqrt = sqrt(east);
      if (east >= 375 && east <= 425){
        return 0;
      }
      else{
        return (eastsqrt - 21);
      }
    }
    else if((west > IS_WALL_THRESH)&&(dir==0)) {//sqrt center dist for west sensor is 24
      int westsqrt = sqrt(west);
      if (west <= 375) {//west is optimized
        return westsqrt-22;
      }
      else if (west > 425) {
        return westsqrt-19; 
      }
      else {
        return 0; 
      }
    }
    else if((west > IS_WALL_THRESH)&&(dir==1)) {//optimizing for each sensor individually
      int westsqrt = sqrt(west);
      if (west <= 375) {//west is optimized
        return 22-westsqrt;
      }
      else if (west > 425) {
        return 19-westsqrt; 
      }
      else {
        return 0;
      }
    }
    else {
      Serial.println("No Walls Present");
      return 0; 

      //case where no walls are present
    }
  }
  return 0;
}



/*

 void centerInTube(int dir, int mdelay) {
 int error = findTubeError(dir);
 setCorrectionMotor(dir, error);
 //475 is the worst error we can get
 // Serial.print("Error: ");
 // Serial.print(error);
 double error0 = (double) error;
 error0 = error0/ERROR_SCALE_FACTOR;
 // Serial.print("Error0: ");
 //Serial.print(error0);
 error0 = abs(error0);
 //Serial.print("Steps: ");
 int steps = (int) error0;
 // Serial.println(steps);
 for (int i =0; i < steps; i++) {
 stepMotors(mdelay);
 }
 }
 */

//disables all motors then sets one motor to spin in order to correct the robot
void setCorrectionMotor(int dir, int error) {
  if (((dir == 0) && (error >= 0)) || ((dir == 1) && (error < 0))) { //spin the west motor
    setWestMotor();
  } 
  else if(((dir == 0) && (error < 0)) || ((dir == 1) && (error >= 0))) {
    setEastMotor(); 
  }
  else if(((dir == 2) && (error >= 0)) || ((dir == 3) && (error < 0))) {
    setNorthMotor(); 
  }
  else {
    setSouthMotor();
  }
}

//By convetion the error will always be the left sensor minus the right sensor, define forward as the way the robot is moving
int findTubeError(int dir,int north,int south,int east,int west) {
  int error = 0;
  int BIAS = 50;
  if (dir ==0) {
    error  = west - east + BIAS;
  } 
  else if(dir == 1) {
    error  = east - west + BIAS+35;
  } 
  else if(dir == 2) {
    error  = north - south + BIAS;
  } 
  else if(dir == 3) {
    error  = (south-north + BIAS+40);
  } 
  else {
    Serial.println("Error: in findTubeError - invalid dir"); 
  }
  return error;
}

//Returns whether or not there is a wall on one side of the robot and what side that is
//If there are no walls return -1
//If there is more than one wall return 4
int wallOnSide() {
  int side = -1; //no walls
  int count = 0;
  if (readNorth() > IS_WALL_THRESH) {
    count++;
    side = 0;  
  } 
  if (readSouth() > IS_WALL_THRESH) { 
    count++;
    side = 1;  
  } 
  if (readEast() > IS_WALL_THRESH) {
    count++;
    side = 2;  
  } 
  if (readWest() > IS_WALL_THRESH) {
    count++;
    side = 3;  
  } 

  return side;
}

//Returns whether or not the robot is in a tube
boolean inTube(int dir,int north,int south,int east,int west) { 
  if (dir == 0 || dir == 1) { //moving North or South
    return ((east >IS_WALL_THRESH)&& (west > IS_WALL_THRESH));
  }
  else {
    return ((north >IS_WALL_THRESH)&& (south > IS_WALL_THRESH));
  }
}

boolean inTube(int dir) { 
  if (dir == 0 || dir == 1) { //moving North or South
    return ((readEast() >IS_WALL_THRESH)&& (readWest() > IS_WALL_THRESH));
  }
  else{
    return ((readNorth() >IS_WALL_THRESH)&& (readSouth() > IS_WALL_THRESH));
  }
}

//UNUSED OVERLOAD
/*
void ramp(int low_value, int high_value, boolean ramp_dir) {
 for  (int i = 0; i <= 50; i++) {
 digitalWrite(7,LOW);
 delayMicroseconds(d);
 digitalWrite(7,HIGH);
 delayMicroseconds(d);
 d = d + ((RAMP_DELAY - MOTOR_DELAY)/50);
 }
 }
 */

//Depending on the passed in direction set enable the right motors and directions
void setMotors(int dir) {
  if (dir == 0) {
    digitalWrite(12,HIGH); //dir 
    digitalWrite(11,LOW); //enables 
    digitalWrite(10,LOW);
    digitalWrite(9,LOW); 
    digitalWrite(8,LOW);
  } 
  else if (dir == 1) {
    digitalWrite(12,LOW); //dir 
    digitalWrite(11,LOW); //enables 
    digitalWrite(10,LOW);
    digitalWrite(9,LOW); 
    digitalWrite(8,LOW);  
  } 
  else if (dir == 2) {
    digitalWrite(12,HIGH); //dir 
    digitalWrite(11,LOW); //enables 
    digitalWrite(10,LOW);
    digitalWrite(9,LOW); 
    digitalWrite(8,LOW); 
  } 
  else {
    digitalWrite(12,LOW); //dir 
    digitalWrite(11,LOW); //enables 
    digitalWrite(10,LOW);
    digitalWrite(9,LOW); 
    digitalWrite(8,LOW); 
  }
}


void disableAllMotors(){
  digitalWrite(11,HIGH); //enables 
  digitalWrite(10,HIGH);
  digitalWrite(9,HIGH); 
  digitalWrite(8,HIGH); 
}


void setNorthMotor() {
  digitalWrite(11,LOW); //enables 
  digitalWrite(10,HIGH);
  digitalWrite(9,HIGH); 
  digitalWrite(8,HIGH); 
}


void setSouthMotor() {
  digitalWrite(11,HIGH); //enables 
  digitalWrite(10,HIGH);
  digitalWrite(9,LOW); 
  digitalWrite(8,HIGH); 
}


void setEastMotor() {
  digitalWrite(11,HIGH); //enables 
  digitalWrite(10,HIGH);
  digitalWrite(9,HIGH); 
  digitalWrite(8,LOW); 
}

void setWestMotor() {
  digitalWrite(11,HIGH); //enables 
  digitalWrite(10,LOW);
  digitalWrite(9,HIGH); 
  digitalWrite(8,HIGH); 
}

void stepMotors(int d) {
  digitalWrite(7,HIGH);
  delayMicroseconds(d); 
  digitalWrite(7,LOW);
  delayMicroseconds(d);
}

void enableAllMotors() {
  digitalWrite(11,LOW); //enables 
  digitalWrite(10,LOW);
  digitalWrite(9,LOW); 
  digitalWrite(8,LOW); 
}

boolean isWallNorth() {
  return readNorth() > IS_WALL_THRESH;
}

boolean isWallSouth() {
  return readSouth() > IS_WALL_THRESH;
}

boolean isWallWest() {
  return readWest() > IS_WALL_THRESH;
}

boolean isWallEast() {
  return readEast() > IS_WALL_THRESH;
}




