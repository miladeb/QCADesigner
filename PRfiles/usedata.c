#include "usedata.h"
#include "createQcaFile.h"
const char* gateArray[2] = {"inverter", "majorityGate","and","or"};//vetor com todos as portas implementadas
const char* orientationArray[4] = {"westEast", "southNorth", "eastWest", "northSouth"}; //orientação

char* convertFile(char* input){
  FILE* inputFile; // .txt
  FILE* useFile; // .txt
  qcaUseGate* Gate;
  int totalofGates;
  char useFileName[200], qcaFileName[200];
  inputFile = fopen(input,"r");

  if (input == NULL){
    printf("file not found");
    exit(1);
  }

  //get the name of the files we are going to create;
  strcpy(useFileName,"files/");
  char fileName[100];
  fgets(fileName, sizeof fileName, inputFile);
  fileName[strlen(fileName) -1] = '\0';
  strcat(useFileName, (const char*)fileName);
  printf("%s",useFileName);

  //get the qca file name
  strcpy(qcaFileName,"files/");
  fgets(fileName, sizeof fileName, inputFile);
  fileName[strlen(fileName) -1] = '\0';
  strcat(qcaFileName, (const char*)fileName);
  printf("%s",qcaFileName);

  useFile = fopen (useFileName,"w");
  totalofGates = getTotalofGates(inputFile);
  Gate = (qcaUseGate*) malloc(totalofGates*sizeof(qcaUseGate));
  printHeader(useFile, useFileName);
  readInput(inputFile, totalofGates, Gate);
  fclose(inputFile);
  createUseFile(useFile,totalofGates, Gate);
  fclose(useFile);
  free(Gate);

  createQFile(useFileName, qcaFileName);
  return qcaFileName;
}

int getTotalofGates(FILE* input){
    int totalofGates;
    fscanf(input, "%d \n", &totalofGates);
    return totalofGates;
}

void readInput(FILE* input,int totalofGates, qcaUseGate* Gate){
  int i;
  for(i = 0; i < totalofGates; i++){
    fscanf(input, "%d %d %d %d \n", &Gate[i].useX, &Gate[i].useY, &Gate[i].gateType, &Gate[i].gateOrientation);
  }
}

void createUseFile(FILE* output, int totalofGates, qcaUseGate* Gate){
  int orientation;
  int i;
  for (i = 0; i<totalofGates; i++){
    orientation = getFileOrientation(Gate[i].gateOrientation, Gate[i].useX, Gate[i].useY);
    fprintf(output, "Gate: %s \n", gateArray[Gate[i].gateType]);
    fprintf(output, "Gate Orientation: %s \n",  orientationArray[orientation]);
    fprintf(output, "useX: %d \n", Gate[i].useX);
    fprintf(output, "useY: %d \n", Gate[i].useY);
    fprintf(output, "FileName: %s_%s.qca \n",gateArray[Gate[i].gateType],orientationArray[orientation]);
    fprintf(output, "-----------------------------------------------------------------------\n");
  }
  fprintf(output,"end");
}

int getFileOrientation(int orientation, int useX, int useY){

  if(orientation == 0){
    return (useX % 2 == 0) ? 0 : 2;
  }
  else{
    return (useY % 2 == 0) ? 1 : 3;
  }
}



void printHeader(FILE* output,char* outputName){
  fprintf(output, "-------------------------------USEFile %s----------------------------\n",outputName);
  fprintf(output, "-----------------------------------------------------------------------\n");
  fprintf(output, "-----------------------------------------------------------------------\n");
}
