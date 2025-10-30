#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <math.h>

#define pi 3.1415926

typedef struct {            //The structure of a Battleship
  char shipname[20]; //1
  char notation[2];  //2
  char gunname[30]; //3
  int maxv;  //4
  int x;  //5
  int y;  //6
} BattleShip;

typedef struct {           //The structure of an Escort Ship
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

typedef struct{
  int x;
  int y;
} PathXY;

//The function for generating a random number
int randomnumber(int min, int max){    
  return min + rand() % (max - min + 1);
}

//The function for converting angles from degrees to radians
double rad(int degrees){
  return degrees * (pi / 180.0);
}

//The function for finding the shooting range of a ship (Distance of a projectile)
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

//This function simulates attacking from the escortships to the battleship
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

//The function for calculating the Max V of each Escort Ship
void calculateMaxV(EscortShip escortships[], BattleShip Bship){
    for(int i = 0; i < 5; i++){
        if(i == 0) {
            escortships[i].maxv = 1.2 * Bship.maxv;  // max v of type E_A
        } else {
            escortships[i].maxv = randomnumber(1,Bship.maxv-1);  // max v lower than battleship max v
        }
    }
}

//The function for printing the battlefield canvas
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
  printf("\nEnter dimenions of the battlefield (x y): ");  //User inputs the dimensions of the battlefield
  scanf("%d %d", &dx, &dy);

  printf("\nBattleship Information:\n");
  printf("\n%-23s %-10s %-25s %-16s\n", "Battleship Name", "Notation", "Gun Name", "Maximum Velocity");
  for (int i = 0; i < 4; i++) {
    printf("%-23s %-10s %-25s %-16d\n", battleships[i].shipname, battleships[i].notation, battleships[i].gunname, battleships[i].maxv);
  }

  char type;
  printf("\nChoose your Battleship type: ");  //User chooses a Battleship type
  scanf(" %c", &type);

  BattleShip Bship;

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

  printf("\nInput coordinates for your Battleship: ");  //User inputs the coordinates of the Battleship
  scanf("%d %d", &Bship.x, &Bship.y);

  int eships;
  printf("Enter number of Escort ships: ");  //User inputs the number of Escort Ships
  scanf("%d",&eships);

  int points;
  printf("Enter number of points you want the Battleship to move: ");  //User inputs the number of points the Battleship will move
  scanf("%d", &points);

  EscortShip Eships[eships];   //Declaring another array of structures to store information of each Escort Ship
  PathXY path[points];  //Declaring an array of structures to store all the coordinates of the path

  for(int j=0; j<eships; j++){
    //Generating a number between 0 & 4 to randomly select the type of Escort Ship
    int indexno = randomnumber(0,4);
    Eships[j] = escortships[indexno];  //copying attributes
    //Randomly generating the coordinates within the range
    Eships[j].x = randomnumber(1,dx-1);
    Eships[j].y = randomnumber(1,dy-1);
    Eships[j].id = j+1;  //Assigning the ID of each Escort Ship
  }

  int **canvas = (int **)malloc(dy * sizeof(int *));
  for(int i=0; i<dy; i++){
    canvas[i] = (int *)malloc(dx * sizeof(int));
  }

  for(int i=0; i<eships; i++){
    Eships[i].maxangle = Eships[i].minangle + Eships[i].anglerange; //calculating the maximum angle
  }

  int destroyedEships = 0;
  int This_E_hit_B;
  int Bstatus = 0; //a separate variable to store the status of the Battleship(using the Bship structure for this didn't work)

  BshipPath(path, Eships, eships, Bship, dx, dy, points);

  printf("\nThe Battlefield before simulation:\n");

  battlefield1(Eships, Bship, dx, dy, eships, canvas, Bstatus, path, points); //calling of the battlefield function which prints the canvas 

  int Bx,By,count=0;
  
  for(int i=0; i<points; i++){
    BtoE(Eships, eships, Bship, dx, dy, path[i]);
    EtoB(Eships, eships, Bship, &This_E_hit_B ,&Bstatus, path[i]);

    Bx = path[i].x;
    By = path[i].y;

    count = i+1;

    if(Bstatus == 1){
      break;
    }
  }

  for(int i=0; i<eships; i++){
    if(Eships[i].status == 1){
      destroyedEships++;
    }
  }

  printf("\nThe Battlefield after simulation:\n");
  
  battlefield2(Eships, Bship, dx, dy, eships, canvas, Bstatus, path, points, count, Bx, By);

  // Free the allocated memory (important!)
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

  //Declaring a file pointer to handle a text file
  FILE *fileptr;

  fileptr = fopen("Initial_Conditions.txt", "w");   //Opening a text file in write mode
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


  int runcount=0;
  fileptr = fopen("Run_Count.txt","r");
  if (fileptr == NULL){
    runcount=0;
  }
  else{
    fscanf(fileptr, "%d", &runcount);
    fclose(fileptr);  //closing the file
  }

  runcount++;
  
  fileptr = fopen("Run_Count.txt","w");
  if (fileptr == NULL){
    printf("\nFile cannot be created\n");
    //return -1;    //Properly handling a potential error in case the file didn't open
  }
  fprintf(fileptr, "%d",runcount);
  fclose(fileptr);  //closing the file


  fileptr = fopen("Simulation_Statistics.txt", "a");   //Opening a text file in append mode
  if (fileptr == NULL) {
    printf("\nFile cannot be created\n");
    //return -1;    //Properly handling a potential error in case the file didn't open
  }

  fprintf(fileptr, "Run #%d\n", runcount);
  if(Bstatus == 1){
    fprintf(fileptr, "The Battleship gets hit in this simulation\n");
    fprintf(fileptr, "Initial position: %s\n",Bcoordinates);
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
  else{
    fprintf(fileptr, "Battleship does not get hit in this simulation\n");
    fprintf(fileptr, "Initial position: %s\n",Bcoordinates);
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
  printf("\nThe type, position, and the maximum velocity of each Escort Ship will be randomly generated.\n");
  printf("\nBut, the maximum velocity of a type E_A Escort Ship will be 1.2 times the maximum velocity of the Battleship.\n");
  printf("\nThe maximum velocity of every other Escort ship type will be lower than the maximum velocity of the Battleship\n");

}

//Main Function
int main(){

    srand(time(NULL)); // seeding the random number generator

    BattleShip battleships[4] = {
    {"USS Iowa (BB-61)", "U", "50-caliber Mark VII gun", randomnumber(0, 10)},   //Initializing the "battleships" array of structures
    {"MS King George V", "M", "(356 mm) Mark VII gun", randomnumber(0, 10)},
    {"Richelieu", "R", "(15 inch) Mle 1935 gun", randomnumber(0, 10)},
    {"Sovetsky Soyuz-class", "S", "(16 inch) B-37 gun", randomnumber(0, 10)}};

    EscortShip escortships[5] = {    //Initializing the "escortships" array of structures
        {"E_A", "1936 A-Class Destroyer", "SK C/34 naval gun", 0.08, 20, randomnumber(5, 15), randomnumber(1, 3)},
        {"E_B", "Gabbiano-Class Corvette", "L/47 dual purpose gun", 0.06, 30, randomnumber(5, 15), randomnumber(1, 3)},
        {"E_C", "Matsu-Class Destroyer", "Type 89 dual purpose gun", 0.07, 25, randomnumber(5, 10), randomnumber(1, 3)},
        {"E_D", "F-Class Escort Ships", "SK C/32 naval gun", 0.05, 50, randomnumber(5, 10), randomnumber(1, 3)},
        {"E_E", "Japanese Kaibōkan", "(4.7 inch) naval guns", 0.04, 70, randomnumber(5, 15), randomnumber(1, 3)},
    };

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
        //system("gedit Simulation_Statistics.txt"); //for ubuntu
        system("start Simulation_Statistics.txt"); //for windows
        //system("xdg-open Simulation_Statistics.txt"); //for replit
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
