#include <stdio.h>
#include <stdlib.h>
#include <string.h>


struct terrainCell{
int cellHeigh;
double attendance;
int terrainTypeOfCell;
};

void terrainCreation();
void terrainCellsInsertion();
void writeBinary();
void readMapCell();
void getCellHeight();
void getCellTerrainType();
void buildPath();
void Bikers();
void memoryFree();
int isNextCell();
int isEastWest();

struct terrainCell *ptr;
struct terrainCell *path;
struct terrainCell **arr;
int row, collomn;
FILE *fp;

int main(){

int choice;

while(1){
    printf("\n1 - Create terrain.\n");
    printf("\n2 - Get high of certain cell from the terrain.\n");
    printf("\n3 - Get type of certain cell from the terrain.\n");
    printf("\n4 - Build path in the terrain.\n");
    printf("\n5 - Bikers.\n");
    printf("\n6 - Quit.\n");

    printf("\nEnter your choice :\t");
    fflush(stdin);
    scanf("%d", &choice);

    switch(choice){
        case 1: terrainCreation();
                terrainCellsInsertion();
                writeBinary();
                readMapCell();
                break;

        case 2: getCellHeight();
                break;

        case 3: getCellTerrainType();
                break;

        case 4: buildPath();
                break;

        case 5: Bikers();
                break;

        case 6: memoryFree();
                exit(3);

        default: printf("\nInvalid input.\n");
                 break;
    }
}
    return 0;
}

//function that alocates memory for the data that we gonna insert and work with later
void terrainCreation(){
int i, j;

//entering size of 2 dimensional arr
printf("\nEnter lentgh of the map :\t");
scanf("%d", &row);

printf("\nEnter height of the map:\t");
scanf("%d", &collomn);

//alocating memory for 2 dimensional array
arr = (struct terrainCell**) malloc(row*sizeof(struct terrainCell*));
if(arr == NULL){
printf("\nMemory does not allocated successfully!\n");
exit(1);
}

//allocating memory for each cell of 2 dimensional array
for(i = 0; i < row; i++){
    *(arr+i) = (struct terrainCell*) malloc(collomn*sizeof(struct terrainCell));
    if(*(arr+i) == NULL){
        printf("\nMemory does not allocated successfully!\n");
        exit(1);
    }
}
}
//function that insert data into allocated memory cells of 2D array of structs
void terrainCellsInsertion(){
char cType[12];

int k = 1, i, j;//k is used to check the input

//inserting elements into the 2 dimesional array(for each cell of the terrain)
for(i = 0; i < row; i++){
   for(j = 0; j < collomn; j++){

        do{
            if(k==0)
                printf("\nInvlaid input");

            printf("\n\nEnter cell height of %d %d element:\t", i, j);

            fflush(stdin);
            k = scanf("%d", &arr[i][j].cellHeigh);
            while(getchar() !='\n');

            }while(k == 0);

       do{
            if(k == 0)
                printf("\nInvalid input!\n");

            printf("\n\nEnter cell attendance of %d %d element:\t", i, j);

            fflush(stdin);
            k = scanf("%lf", &arr[i][j].attendance);
            while(getchar() != '\n');

        }while(k == 0);


    do{
        if(k == 0)
            printf("\nInvalid input!\n");

        printf("\nEnter terrain type of the cell of %d %d element:\t", i, j);
        fflush(stdin);
        k = scanf("%d", &arr[i][j].terrainTypeOfCell);
        //while(getchar() != '\n');

      }while(k == 0 || arr[i][j].terrainTypeOfCell < 0 || arr[i][j].terrainTypeOfCell > 4);
        }
    }
}


//inserting 2D arr sizes and data of each cell into a file
void writeBinary(){
int i, j;

fp = fopen("terrainData.bin","wb");

if(fp == NULL){
    printf("\nFile does not open successfully!\n");
    exit(2);
}else{
    printf("\nSuccessfully created file!\n");
}

//inputing length and high of the terrain into a file
if(fwrite(&row, sizeof(int), 1, fp) != 1){
    printf("\nInformation does not inputed into the file!\n");
    exit(2);
}

if(fwrite(&collomn, sizeof(int), 1, fp) != 1){
    printf("\nInformation does not inputed into the file!\n");
    exit(2);
}

//inputing data into file from 2D array of structs
for(i = 0; i < row; i++){
   for(j = 0; j < collomn; j++){
        if(fwrite(&arr[i][j], sizeof(struct terrainCell), 1, fp) != 1){
            printf("\nData does not inputed into the file!\n");
            break;
        }
       }
    }
         fclose(fp);
}

void readMapCell(){
int i, j;

fp = fopen("terrainData.bin", "rb");

if(fp == NULL){
    printf("/nERROR!/n");
    exit(2);
}

if(fread(&row, sizeof(int), 1, fp) != 1){
    printf("\nData was not read successfully!\n");
    return;
}

if(fread(&collomn, sizeof(int), 1, fp) != 1){
printf("\nData was not read successfully!\n");
    return;
}


for(i = 0; i < row; i++){
    for(j = 0; j < collomn; j++){

        if(fread(&arr[i][j], sizeof(struct terrainCell), 1, fp) != 1){
            printf("\nData was not read successfully!\n");
            break;
        }
    }
}
fclose(fp);
}

void getCellHeight(){
int i, j;

for(i = 0; i < row; i++){
    for(j = 0; j < collomn; j++){
        printf("[%d]\t", arr[i][j].cellHeigh);
    }
    printf("\n----------------\n");
}
}

void getCellTerrainType(){
int i, j;

    for(i=0; i<row; i++){
        for(j=0; j<collomn; j++){

            switch(arr[i][j].terrainTypeOfCell){
                case 0: printf("[0 - Vegetation");
                break;

                case 1 : printf("[1 - Wasteland");
                break;

                case 2 : printf("[2 - Water");
                break;

                case 3 : printf("[3 - Road");
                break;

                case 4 : printf("[4 - Building");
                break;

                default : printf("Not existing element!");
                break;
            }
            printf(" ] \t");
        }
        printf("\n---------------------------------\n");
    }
}

void buildPath(){
int x, y, prevX, prevY, countCoordinates = 0, pathC = 1, i = 0;


fp = fopen("terrainData.bin", "ab");
if(fp == NULL){
    printf("\nFile has not oppened successfully!\n");
    exit(3);
}

path = (struct terrainCell*) malloc(sizeof(struct terrainCell) * 1);
if(path == NULL){
    printf("\nERROR!\n");
    exit(1);
}

do{
    printf("Enter %d-x coordinate:\t", countCoordinates);
    fflush(stdin);
    scanf("%d", &x);

    printf("Enter %d-y coordinate:\t", countCoordinates);
    fflush(stdin);
    scanf("%d", &y);

    }while((arr[x][y].terrainTypeOfCell == 2 || arr[x][y].terrainTypeOfCell == 4) || (x > row || y > collomn));

    countCoordinates++;

    prevX = x;
    prevY = y;

    path[i].cellHeigh = arr[x][y].cellHeigh;
    path[i].attendance = arr[x][y].attendance;
    path[i].terrainTypeOfCell = arr[x][y].terrainTypeOfCell;

    path = (struct terrainCell*) realloc(path, (++pathC)*sizeof(struct terrainCell));
    if(path == NULL){
        printf("\nERROR!\n");
        exit(1);
    }

        while(1){

            do{

                printf("\nEnter %d-x coordinate:\t", countCoordinates);
                fflush(stdin);
                scanf("%d", &x);

                printf("\nEnter %d-y coordinate:\t", countCoordinates);
                fflush(stdin);
                scanf("%d", &y);

                if(x == -1 || y == -1){
                    printf("\nYou have stopped the creation of path!\n");

                    for(i = 0; i < countCoordinates; i++){
                        printf("[%d]\t", path[i].cellHeigh);
                        printf("[%lf]\t", path[i].attendance);
                        printf("[%d]\n", path[i].terrainTypeOfCell);
                            if(fwrite(&path[i], sizeof(struct terrainCell), 1, fp) != 1){
                                printf("\nData was not inserted successfully!\n");
                                return;
                            }
                        }
                            fclose(fp);
                            return;
                    }

            }while((x > row || y > collomn) || (arr[x][y].terrainTypeOfCell == 2 || arr[x][y].terrainTypeOfCell == 4) ||
            isNextCell(x, y, prevX, prevY));

            ++i;
            countCoordinates++;
            prevX = x;
            prevY = y;

            path[i].cellHeigh = arr[x][y].cellHeigh;
            path[i].attendance = arr[x][y].attendance;
            path[i].terrainTypeOfCell = arr[x][y].terrainTypeOfCell;

//reallocating memory for path
            path = (struct terrainCell*) realloc(path, (++pathC)*sizeof(struct terrainCell));
            if(path == NULL){
                printf("\nERROR!\n");
                exit(1);
            }
        }
}


void Bikers(){
int x, y, prevX, prevY, countCoordinates = 0/*displaying serial number of cell*/, i = 0, pathC = 1;

fp = fopen("terrainData.bin", "ab");
if(fp == NULL){
    printf("\nFile has not open successfully!\n");
    exit(3);
}

struct terrainCell *jumpTerrain;

jumpTerrain = (struct terrainCell*) malloc(sizeof(struct terrainCell) * 1);
if(jumpTerrain == NULL){
    printf("\nMemory does not allocated successfully!\n");
    exit(1);
}

do{
    printf("\nEnter %d - x coordinate :\t", countCoordinates);
    fflush(stdin);
    scanf("%d", &x);

    printf("\nEnter %d - y coordinate :\t", countCoordinates);
    fflush(stdin);
    scanf("%d", &y);

}while((x > row || y > collomn) || (arr[x][y].terrainTypeOfCell != 1) || (arr[x][y].cellHeigh >= 45));

prevX = x;
prevY = y;

jumpTerrain[i].cellHeigh = arr[x][y].cellHeigh;
jumpTerrain[i].attendance = arr[x][y].attendance;
jumpTerrain[i].terrainTypeOfCell = arr[x][y].terrainTypeOfCell;

++i;
countCoordinates++;

jumpTerrain = (struct terrainCell*) realloc(jumpTerrain, (++pathC)*sizeof(struct terrainCell));
if(jumpTerrain == NULL){
    printf("\nMemory does not reallocate successfully!\n");
    exit(1);
}

        do{

            printf("\nEnter %d - x coordinate :\t", countCoordinates);
            fflush(stdin);
            scanf("%d", &x);

            printf("\nEnter %d - y coordinate :\t", countCoordinates);
            fflush(stdin);
            scanf("%d", &y);

            if(x == -1 || y == -1){
                printf("\nYou have stopped the creation of path!\n");
                return;
            }

        }while((x > row || y > collomn) || (arr[x][y].terrainTypeOfCell != 1) ||
            isEastWest(x, y, prevX, prevY) || (arr[x][y].cellHeigh >= 45));


    prevX = x;
    prevY = y;

    jumpTerrain[i].cellHeigh = arr[x][y].cellHeigh;
    jumpTerrain[i].attendance = arr[x][y].attendance;
    jumpTerrain[i].terrainTypeOfCell = arr[x][y].terrainTypeOfCell;

    ++i;
    countCoordinates++;

    jumpTerrain = (struct terrainCell*) realloc(jumpTerrain, (++pathC)*sizeof(struct terrainCell));
    if(jumpTerrain == NULL){
        printf("\nMemory does not reallocate successfully!\n");
        exit(1);
        }

    if(countCoordinates == 2){

            while(1){

                do{
                    printf("\nEnter %d - x coordinate :\t", countCoordinates);
                    fflush(stdin);
                    scanf("%d", &x);

                    printf("\nEnter %d - y coordinate :\t", countCoordinates);
                    fflush(stdin);
                    scanf("%d", &y);

                    if(x == -1 || y == -1){
                        printf("\nYou have stopped the creation of the jump!\n");
                        free(jumpTerrain);
                        return;
                    }

                }while((x > row || y > collomn) || (arr[x][y].terrainTypeOfCell != 1) ||
                        isEastWest(x, y, prevX, prevY) || (arr[x][y].cellHeigh > 80));

                    prevX = x;
                    prevY = y;

                    jumpTerrain[i].cellHeigh = arr[x][y].cellHeigh;
                    jumpTerrain[i].attendance = arr[x][y].attendance;
                    jumpTerrain[i].terrainTypeOfCell = arr[x][y].terrainTypeOfCell;

                    ++i;
                    ++countCoordinates;

                    if(countCoordinates >= 5){
                        printf("\nYou have created the perfect jump!\n");

                        for(i = 0; i < countCoordinates; i++){
                            printf("\n[%d]\t", jumpTerrain[i].cellHeigh);
                            printf("%[lf]\t", jumpTerrain[i].attendance);
                            printf("[%d]\n", jumpTerrain[i].terrainTypeOfCell);
                            if(fwrite(&jumpTerrain[i], sizeof(struct terrainCell), 1, fp) != 1){
                                printf("\nData was not inserted successfully!\n");
                                return;
                            };
                        }
                        fclose(fp);
                        free(jumpTerrain);
                        return;
                    }

                    jumpTerrain = (struct terrainCell*) realloc(jumpTerrain, (++pathC)*sizeof(struct terrainCell));
                    if(jumpTerrain == NULL){
                        printf("\nMemory does not reallocate successfully!\n");
                        exit(1);
            }
        }
    }
}


int isEastWest(int currentX, int currentY, int prevX, int prevY){

    if((currentY == prevY+1 && currentX == prevX-1) ||
    (currentY == prevY+1 && currentX == prevX) ||
    (currentY == prevY+1 && currentX == prevX+1)){
        return 0;
    }else{
        return 1;
    }

}
                //x              y
int isNextCell(int currentX, int currentY, int prevX, int prevY){
//left right up down fields check
        if((currentY == prevY-1 && currentX == prevX) || //left
            (currentY == prevY+1 && currentX == prevX) || //right
            (currentX == prevX-1 && currentY == prevY) ||//up
            (currentX == prevX+1 && currentY == prevY) ||//down
//diagonal fields check
            (currentX == prevX-1 && currentY == prevY-1)||//up left diagonal
            (currentX == prevX+1 && currentY == prevY+1)||//down right diagonal
            (currentX == prevX-1 && currentY == prevY+1)||//up right diagonal
            (currentX == prevX+1 && currentY == prevY-1))//down left diagonal
        {
            return 0;
        }
            else{
                    return 1;
                }

}

//freing memory of every cell in the array
void memoryFree(){
int i;

    for(i = 0; i < row; i++){
       free(arr[i]);// pointer to the structure must free first
       free(path);
    }
}
