#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <math.h>

#define pi 3.1415926

//The structure of a Battleship
typedef struct {
  char shipname[20]; //1
  char notation[2];  //2
  char gunname[30]; //3
  int maxv;  //4
  int x;  //5
  int y;  //6
  int minangle;  //7
} BattleShip;

//The structure of an Escort Ship
typedef struct {
  char notation[4]; //1
  char shipname[30]; //2
  char gunname[30];  //3
  float impactpower; //4
  int anglerange; //5
  int minangle; //6
  int minv; //7
  float maxv; //8
  int x; //9
  int y; //10
  int id; //11
  int status; //12
  int maxangle; //13
} EscortShip;

//The structue of a coordinate (will be used to store the coordinates of the Bship's path)
typedef struct{
  int x;
  int y;
} PathXY;

//This function generates a random number
int randomnumber(int min, int max){    
  return min + rand() % (max - min + 1);
}

//This function converts angles from degrees to radians
double rad(int degrees){
  return degrees * (pi / 180.0);
}

//This function finds the shooting range of a ship (Distance of a projectile)
double range(int velocity, int angle){
  return (pow(velocity, 2) * sin(2 * rad(angle))) / 10;
}

//This function returns whether the target ship is within range
int isWithinRange(EscortShip Eships, BattleShip Bship, PathXY path){

  double distance = sqrt(pow(path.x - Eships.x, 2) + pow(path.y - Eships.y, 2));
  return (distance <= range(Bship.maxv, 45));
}

//This function returns the distance between 2 points
float distance_2(EscortShip Eships, PathXY path){
  return sqrt(pow(path.x - Eships.x, 2) + pow(path.y - Eships.y, 2));
}

//This function simulates attacking from the battleship to escortships
void BtoE(EscortShip Eships[], int eships, BattleShip Bship, int dx, int dy, PathXY path){

  for(int i = 0; i < eships; i++){
    if(isWithinRange(Eships[i], Bship, path)){
      Eships[i].status = 1;
    }
  }
}

//This function simulates attacking from the battleship to escortships after the Bship's gun get jammed
void BtoE_jammed(EscortShip Eships[], int eships, BattleShip Bship, int dx, int dy, PathXY path){

  float upperlimit=0, lowerlimit=0, BtoE_distance=0;
  
  for(int i = 0; i < eships; i++){

    if(Eships[i].status != 1){

      upperlimit = range(Bship.maxv, 45);
      lowerlimit = range(1, Bship.minangle);

      BtoE_distance = distance_2(Eships[i], path);

      if(BtoE_distance <= upperlimit && BtoE_distance >= lowerlimit){
        Eships[i].status = 1;
      }
      
    }
    
  }
}

//This function simulates attacking from escortships to the battleship
void EtoB(EscortShip Eships[], int eships, BattleShip Bship, int *id, int *status, PathXY path){

  float upperlimit=0, lowerlimit=0, EtoB_distance=0;

  for(int i=0; i<eships; i++){

    if(Eships[i].status != 1){

      if(Eships[i].maxangle >= 45){
        upperlimit = range(Eships[i].maxv, 45);
      }
      else{
        upperlimit = range(Eships[i].maxv, Eships[i].maxangle);
      }

      lowerlimit = range(Eships[i].minv, Eships[i].minangle);

      EtoB_distance = distance_2(Eships[i], path);

      if(EtoB_distance <= upperlimit && EtoB_distance >= lowerlimit){
        *status = 1;
        *id = Eships[i].id;
      }
    } 
  }
}

//This function calculates the Max V of each Escort Ship
void calculateMaxV(EscortShip escortships[], BattleShip Bship){
    for(int i = 0; i < 5; i++){
        if(i == 0) {
            escortships[i].maxv = 1.2 * Bship.maxv;  // max v of type E_A
        } else {
            escortships[i].maxv = randomnumber(1,Bship.maxv-1);  // max v lower than battleship max v
        }
    }
}

//This function prints the battlefield canvas before the simulation
void battlefield1(EscortShip Eships[], BattleShip Bship, int dx, int dy, int eships, int **canvas, int Bstatus, PathXY path[], int points){
  for(int i = 0; i < dy; i++){
    for(int j = 0; j < dx; j++){
        canvas[i][j] = 0; //'.'
    }
  }

  canvas[dy - Bship.y][Bship.x - 1] = -1; //B

  for(int i = 0; i < eships; i++){
    canvas[dy - Eships[i].y][Eships[i].x - 1] = Eships[i].id;
  }

  for(int i = 1; i < points; i++){
    canvas[dy - path[i].y][path[i].x - 1] = -2; //" P "
  }

  //print the battlefield
  printf("\n");
  for(int i=0; i<dy; i++){
    for(int j=0; j<dx; j++){
      if(canvas[i][j] == 0){
        printf(".  ");
      }
      else if(canvas[i][j] == -1){
        printf("B  ");
      }
      else if(canvas[i][j] == -2){
        printf("P  ");
      }
      else if(canvas[i][j]>=10){
        printf("%d ", canvas[i][j]);
      }
      else{
        printf("%d  ",canvas[i][j]);
      }
    }
    printf("\n\n");
  }
}

//This function prints the battlefield canvas after the simulation
void battlefield2(EscortShip Eships[], BattleShip Bship, int dx, int dy, int eships, int **canvas, int Bstatus, PathXY path[], int points, int count, int Bx, int By){
  for(int i = 0; i < dy; i++){
    for(int j = 0; j < dx; j++){
        canvas[i][j] = 0; //'.'
    }
  }

  if(Bstatus == 1){
    canvas[dy - By][Bx - 1] = -1; //'*'
  }
  else if(Bstatus == 0){
    canvas[dy - By][Bx - 1] = -2; //B
  }

  for(int i = 0; i < eships; i++){
    if(Eships[i].status == 1){
      canvas[dy - Eships[i].y][Eships[i].x - 1] = -3; //'X'
    }
    else{
      canvas[dy - Eships[i].y][Eships[i].x - 1] = Eships[i].id;
    }
  }

  for(int i = 0 ; i < count-1; i++){
    canvas[dy - path[i].y][path[i].x - 1] = -4; //" Q "
  }

  //print the battlefield
  printf("\n");
  for(int i=0; i<dy; i++){
    for(int j=0; j<dx; j++){
      if(canvas[i][j] == 0){
        printf(".  ");
      }
      else if(canvas[i][j] == -1){
        printf("*  ");
      }
      else if(canvas[i][j] == -2){
        printf("B  ");
      }
      else if(canvas[i][j] == -3){
        printf("X  ");
      }
      else if(canvas[i][j] == -4){
        printf("Q  ");
      }
      else if(canvas[i][j]>=10){
        printf("%d ", canvas[i][j]);
      }
      else{
        printf("%d  ",canvas[i][j]);
      }
    }
    printf("\n\n");
  }
}

//This function generates the path of the Battleship (coordinates)
void BshipPath(PathXY path[], EscortShip Eships[], int eships, BattleShip Bship, int dx, int dy, int points){

  path[0].x = Bship.x;  // Initializing the first point of the path as the Battleship's initial position
  path[0].y = Bship.y;

  for (int i = 1; i < points; i++) {
    int newX, newY;

    do {
      // Generating the next point of the path
      newX = path[i - 1].x + randomnumber(-10, 10);
      newY = path[i - 1].y + randomnumber(-10, 10);
    } while (newX <= 0 || newY <= 0 || newX >= dx || newY >= dy);

     // Checking if the point collides with an Escort Ship
     for (int j = 0; j < eships; j++) {
       // If collision occurs, generate a new point
       if (newX == Eships[j].x && newY == Eships[j].y) {
         newX = path[i - 1].x + randomnumber(-10, 10);
         newY = path[i - 1].y + randomnumber(-10, 10);
         j = -1; // Restart collision check loop
       }
     }
    // Assigning the new point to the path
    path[i].x = newX;
    path[i].y = newY;
  }
}

//This function simulates a full battle
void simulation(BattleShip battleships[], EscortShip escortships[]){

  int dx, dy;
  //User inputs the dimensions of the battlefield
  printf("\nEnter dimenions of the battlefield (x y): ");
  scanf("%d %d", &dx, &dy);

  printf("\nBattleship Information:\n");
  printf("\n%-23s %-10s %-25s %-16s\n", "Battleship Name", "Notation", "Gun Name", "Maximum Velocity");
  for (int i = 0; i < 4; i++) {
    printf("%-23s %-10s %-25s %-16d\n", battleships[i].shipname, battleships[i].notation, battleships[i].gunname, battleships[i].maxv);
  }

  char type;
  //User chooses a Battleship type
  printf("\nChoose your Battleship type: ");
  scanf(" %c", &type);

  //Declaring a varible of a structure to store information about the Battleship
  BattleShip Bship;

  //Assigning information and calculating max V of each E ship type according to the chosen Battleship
  if (type == 'u' || type == 'U') {
    printf("\nYou selected %s Battleship\n", battleships[0].shipname);
    Bship = battleships[0];  //copying attributes
    calculateMaxV(escortships, Bship);  //Calling of the function to calculate the Max V of each Escort Ship
  }
  if (type == 'm' || type == 'M') {
    printf("\nYou selected %s Battleship\n", battleships[1].shipname);
    Bship = battleships[1];
    calculateMaxV(escortships, Bship);

  }
  if (type == 'r' || type == 'R') {
    printf("\nYou selected %s Battleship\n", battleships[2].shipname);
    Bship = battleships[2];
    calculateMaxV(escortships, Bship);
  }
  if (type == 's' || type == 'S') {
    printf("\nYou selected %s Battleship\n", battleships[3].shipname);
    Bship = battleships[3];
    calculateMaxV(escortships, Bship);
  }

  //User inputs the coordinates of the Battleship
  printf("\nInput coordinates for your Battleship: ");
  scanf("%d %d", &Bship.x, &Bship.y);

  int eships;
  //User inputs the number of Escort Ships
  printf("Enter number of Escort ships: ");
  scanf("%d",&eships);

  int points;
  //User inputs the number of points the Battleship will move
  printf("Enter number of points you want the Battleship to move: ");
  scanf("%d", &points);

  //Declaring another array of structures to store information of each Escort Ship
  EscortShip Eships[eships];

  //Declaring an array of structures to store all the coordinates of the path
  PathXY path[points];

  //Randomly selecting the type of each Escort Ship
  for(int j=0; j<eships; j++){
    //Generating a number between 0 & 4 to randomly select the type of Escort Ship
    int indexno = randomnumber(0,4);
    Eships[j] = escortships[indexno];  //copying attributes
    //Randomly generating the coordinates within the range
    Eships[j].x = randomnumber(1,dx-1);
    Eships[j].y = randomnumber(1,dy-1);
    Eships[j].id = j+1;  //Assigning the ID of each Escort Ship
  }

  //declaring and allocating memory for an int array to print the battlefield
  int **canvas = (int **)malloc(dy * sizeof(int *));
  for(int i=0; i<dy; i++){
    canvas[i] = (int *)malloc(dx * sizeof(int));
  }

  //calculating the maximum angle of each Escort Ship
  for(int i=0; i<eships; i++){
    Eships[i].maxangle = Eships[i].minangle + Eships[i].anglerange; //calculating the maximum angle
  }

  //declaring a vairable to count the number of destroyed Escort Ships
  int destroyedEships = 0;

  //declaring a variable to store the ID of the Escortship that attcked the Battleship
  int This_E_hit_B;

  //a separate variable to store the status of the Battleship(using the Bship structure for this didn't work)
  int Bstatus = 0;

  //calling of the function that generates the path of the Battleship
  BshipPath(path, Eships, eships, Bship, dx, dy, points);
  
  printf("\nThe Battlefield before simulation:\n");
  //calling battlefield1 function to print the battlefield before the battle
  battlefield1(Eships, Bship, dx, dy, eships, canvas, Bstatus, path, points); //calling of the battlefield function which prints the canvas 

  //declaring 2 variables to store the current position of the battleship along the path
  int Bx,By;

  //count(to keep track of the path)  jammedin(to keep track of where the B ship gun got jammed)
  int count=0,jammedin=0;

  //randomly selecting a point where the gun will be getting jammed
  jammedin = randomnumber(1,points-1);

  //simulating the battle until the gun gets jammed
  for(int i=0; i<jammedin; i++){

    BtoE(Eships, eships, Bship, dx, dy, path[i]);
    EtoB(Eships, eships, Bship, &This_E_hit_B ,&Bstatus, path[i]);  

    //keeping track of the current coordinates of the Battleship along the path
    Bx = path[i].x;
    By = path[i].y;

    //keeping track of the point number of the path
    count = i+1;

    //stop the simulation if the Battleship is destroyed
    if(Bstatus == 1){
      break;
    }
  }

  //simualting the battle after the gun gets jammed
  for(int i=jammedin; i<points; i++){

    //randomly generating the minimum angle of the Battleship since the gun is jammed
    Bship.minangle = randomnumber(1,29);
    
    BtoE_jammed(Eships, eships, Bship, dx, dy, path[i]);
    EtoB(Eships, eships, Bship, &This_E_hit_B ,&Bstatus, path[i]);

    //keeping track of the current coordinates of the Battleship along the path
    Bx = path[i].x;
    By = path[i].y;

    //keeping track of the point number of the path
    count = i+1;

    //stop the simulation if the Battleship is destroyed
    if(Bstatus == 1){
      break;
    }
  }

  //counting all the destroyed Escort Ships
  for(int i=0; i<eships; i++){
    if(Eships[i].status == 1){
      destroyedEships++;
    }
  }

  printf("\nThe Battlefield after simulation:\n");
  //calling battlefield2 function to print the battlefield after the battle
  battlefield2(Eships, Bship, dx, dy, eships, canvas, Bstatus, path, points, count, Bx, By);

  //Free the allocated memory for the 2D array (important!)
  for (int i = 0; i < dy; i++) {
      free(canvas[i]);
  }
  free(canvas);

  //Assigning the coordinates of the Battleship into a single array (easy to read)
  char Bcoordinates[10];  
  sprintf(Bcoordinates, "(%d, %d)", Bship.x, Bship.y);

  //Assigning the coordinates of each Escort Ship into a 2D array (easy to read)
  char Ecoordinates[eships][10];
  for(int n=0; n<eships; n++){
    sprintf(Ecoordinates[n], "(%d, %d)", Eships[n].x, Eships[n].y);
  }

  //Declaring a file pointer to handle files
  FILE *fileptr;

  //Opening the Initial_Conditions text file in write mode
  fileptr = fopen("Initial_Conditions.txt", "w");
  if (fileptr == NULL) {
     printf("\nFile cannot be created\n");
     //return -1;    //Properly handling a potential error in case the file didn't open
  }
  //Storing the information of the Battlefield into the text file
  fprintf(fileptr, "Battlefield Dimensions: %d x %d\n", dx, dy);
  fprintf(fileptr, "\nBattleship:\n");
  fprintf(fileptr, "%-6s %-18s %-17s\n", "Type","Initial Position","Maximum Velocity");
  fprintf(fileptr, "%-6c %-18s %-17d\n", type, Bcoordinates, Bship.maxv);
  fprintf(fileptr, "\nThe number of points the Battleship is supposed move: %d\n", points);
  fprintf(fileptr, "\nEscort Ships:\n");
  fprintf(fileptr, "%-6s %-4s %-10s %-7s %-7s %-7s %-7s\n", "Type","ID","Position","Min V","Max V","Min θ","Max θ");
  for(int k=0; k<eships; k++){
    fprintf(fileptr,"%-6s %-4d %-10s %-7d %-7.2f %-7d %-7d\n", Eships[k].notation, Eships[k].id, Ecoordinates[k], Eships[k].minv, Eships[k].maxv, Eships[k].minangle, Eships[k].maxangle);
  }
  fclose(fileptr);  //closing the file

  //declaring a variable to keep track of the simulation run count
  int runcount=0;

  //opening a separate text file to read the simulation runcount
  fileptr = fopen("Run_Count.txt","r");
  if (fileptr == NULL){
    runcount=0;
  }
  else{
    fscanf(fileptr, "%d", &runcount);
    fclose(fileptr);  //closing the file
  }

  //incrementing the runcount and re-writing it to the file
  runcount++;
  fileptr = fopen("Run_Count.txt","w");
  if (fileptr == NULL){
    printf("\nFile cannot be created\n");
    //return -1;    //Properly handling a potential error in case the file didn't open
  }
  fprintf(fileptr, "%d",runcount);
  fclose(fileptr);  //closing the file

  //Opening the Simulation_Statistics text file in append mode to store the details of each simulation
  fileptr = fopen("Simulation_Statistics.txt", "a");
  if (fileptr == NULL) {
    printf("\nFile cannot be created\n");
    //return -1;    //Properly handling a potential error in case the file didn't open
  }

  fprintf(fileptr, "Run #%d\n", runcount);

  //details if the Battleship is destroyed
  if(Bstatus == 1){
    fprintf(fileptr, "The Battleship gets hit in this simulation\n");
    fprintf(fileptr, "Initial position: %s\n",Bcoordinates);
    fprintf(fileptr, "B ship's gun got jammed in point:%d\n", jammedin);
    fprintf(fileptr, "B had %d points to move but got hit in point #%d:(%d, %d)\n",points, count, path[count].x, path[count].y);
    fprintf(fileptr, "ID:%d Escort Ship hit the Battleship\n", This_E_hit_B);
    fprintf(fileptr, "%d Escort Ships got hit\n", destroyedEships);
    fprintf(fileptr, "Details of all the Escort Ships that got hit:\n");
    fprintf(fileptr, "%-6s %-4s %-10s %-7s %-7s %-7s %-7s\n", "Type","ID","Position","Min V","Max V","Min θ","Max θ");
    for(int k=0; k<eships; k++){
      if(Eships[k].status == 1){
       fprintf(fileptr,"%-6s %-4d %-10s %-7d %-7.2f %-7d %-7d\n", Eships[k].notation, Eships[k].id, Ecoordinates[k], Eships[k].minv, Eships[k].maxv, Eships[k].minangle, Eships[k].maxangle);
      }
    }
  }
  //details if the Battleship is not destroyed
  else{
    fprintf(fileptr, "Battleship does not get hit in this simulation\n");
    fprintf(fileptr, "Initial position: %s\n",Bcoordinates);
    fprintf(fileptr, "B ship's gun got jammed in point:%d\n", jammedin);
    fprintf(fileptr, "B travelled to all %d points\n", points);
    fprintf(fileptr, "%d Escort Ships got hit\n", destroyedEships);
    if(destroyedEships != 0){
      fprintf(fileptr, "Details of all the Escort Ships that got hit:\n");
      fprintf(fileptr, "%-6s %-4s %-10s %-7s %-7s %-7s %-7s\n", "Type","ID","Position","Min V","Max V","Min θ","Max θ");
      for(int k=0; k<eships; k++){
        if(Eships[k].status == 1){
          fprintf(fileptr,"%-6s %-4d %-10s %-7d %-7.2f %-7d %-7d\n", Eships[k].notation, Eships[k].id, Ecoordinates[k], Eships[k].minv, Eships[k].maxv, Eships[k].minangle, Eships[k].maxangle);
        }
      }
    }
  }
  fprintf(fileptr,"\n");
  fclose(fileptr);  //closing the file
  
  //Opening the Final_Conditions text file in write mode to store some final conditions of the simulation
  fileptr = fopen("Final_Conditions.txt", "w");   //Opening a text file in write mode
  if (fileptr == NULL) {
    printf("\nFile cannot be created\n");
    //return -1;    //Properly handling a potential error in case the file didn't open
  }

  if(Bstatus == 1){
    fprintf(fileptr, "Battleship was attacked in this simulation\n");
  }
  else{
    fprintf(fileptr, "Battleship was not attacked in this simulation\n");
  }
  fprintf(fileptr,"%d Escort Ships got hit\n", destroyedEships);
  fprintf(fileptr,"%d Escort Ships survived\n", eships-destroyedEships);
  fclose(fileptr);  //closing the file 
}

//This is the function for choice 2: Instruction Menu
void instructions(BattleShip battleships[], EscortShip escortships[]){

  //Printing the Battleship information
  printf("\nBattleship Information:\n");
  printf("\n%-23s %-10s %-25s %-16s\n", "Battleship Name", "Notation", "Gun Name", "Maximum Velocity");
  for (int i = 0; i < 4; i++) {
    printf("%-23s %-10s %-25s %-16d\n", battleships[i].shipname, battleships[i].notation, battleships[i].gunname, battleships[i].maxv);
  }

  //Printing the Escort Ship information
  printf("\nEscort Ship Information:\n");
  printf("\n%-15s %-25s %-27s %-15s %-13s %-15s %-17s\n", "Type Notation", "Type Name", "Gun Name", "Impact Power", "Angle Range", "Minimum Angle", "Minimum Velocity");
  for (int i = 0; i < 5; i++) {
    printf("%-15s %-25s %-27s %-15.2f %-13d %-15d %-17d\n", escortships[i].notation, escortships[i].shipname, escortships[i].gunname, escortships[i].impactpower, escortships[i].anglerange, escortships[i].minangle, escortships[i].minv);
  }

  printf("\nIn the begining, you will be asked to input the dimensions of the battlefield.\n");
  printf("\nThen, you have to choose a type of Battleship.\n");
  printf("\nThen, you will be asked to input the initial position(coordinates) of the Battleship.\n");
  printf("\nAfter that, you have to input the number of Escort Ships you want to have in your simulation.\n");
  printf("\nThen, you will be asked to input the number of points you want the Battleship to move.\n");
  printf("\nThe type, position, and the maximum velocity of each Escort Ship will be randomly generated.\n");
  printf("\nBut, the maximum velocity of a type E_A Escort Ship will be 1.2 times the maximum velocity of the Battleship.\n");
  printf("\nThe maximum velocity of every other Escort ship type will be lower than the maximum velocity of the Battleship\n");
  printf("\nThe Bship will move to the number of points you entered.\n");
  printf("\nAt each of those points, the Bship will attack all the EShips int its range.\n");
  printf("\nBut still, there will be a time between 2 consecutive gunfires.\n");
  printf("\nHowever, the gun on the Bship will get jammed in one of those points. That point will be a random one.\n");
  printf("\nOnce the gun is jammed, the Bships's minimum angle will be 0<θ<30\n");

}

//Main Function
int main(){
  
  // seeding the random number generator
  srand(time(NULL));

  //Initializing the "battleships" array of structures
  BattleShip battleships[4] = {
  {"USS Iowa (BB-61)", "U", "50-caliber Mark VII gun", randomnumber(0, 10)},
  {"MS King George V", "M", "(356 mm) Mark VII gun", randomnumber(0, 10)},
  {"Richelieu", "R", "(15 inch) Mle 1935 gun", randomnumber(0, 10)},
  {"Sovetsky Soyuz-class", "S", "(16 inch) B-37 gun", randomnumber(0, 10)}};

  //Initializing the "escortships" array of structures
  EscortShip escortships[5] = {
  {"E_A", "1936 A-Class Destroyer", "SK C/34 naval gun", 0.08, 20, randomnumber(5, 15), randomnumber(1, 3)},
  {"E_B", "Gabbiano-Class Corvette", "L/47 dual purpose gun", 0.06, 30, randomnumber(5, 15), randomnumber(1, 3)},
  {"E_C", "Matsu-Class Destroyer", "Type 89 dual purpose gun", 0.07, 25, randomnumber(5, 10), randomnumber(1, 3)},
  {"E_D", "F-Class Escort Ships", "SK C/32 naval gun", 0.05, 50, randomnumber(5, 10), randomnumber(1, 3)},
  {"E_E", "Japanese KaibÅOkan", "(4.7 inch) naval guns", 0.04, 70, randomnumber(5, 15), randomnumber(1, 3)}};

  printf("Main Menu\n");
  printf("\t1.Start Simulation\n");
  printf("\t2.View Instructions\n");
  printf("\t3.Simulation Statistics\n");
  printf("\t4.Exit\n");

  int choice;
  printf("\nEnter your choice: ");
  scanf("%d", &choice);

  char exit;
  
  switch (choice){
    case 1:
      simulation(battleships, escortships);
      break;

    case 2:
      instructions(battleships, escortships);
      break;

    case 3:
      system("gedit Simulation_Statistics.txt"); //for ubuntu
      //system("start Simulation_Statistics.txt"); //for windows
      break;

    case 4:
      printf("\nAre you sure you want to exit? (y/n): ");
      scanf( " %c", &exit);

      if (exit == 'y' || exit == 'Y'){
        printf("\nExiting the program...\n");
        return 0;
      }
      else if(exit == 'n' || exit == 'N'){
        printf("\nReturning to the Main Menu... \n\n");
        main();
      }
      else{
        printf("\nInvalid input. Please enter 'y' or 'n'.\n\n");
        main();
      }
      break;
  }
  return 0;
}