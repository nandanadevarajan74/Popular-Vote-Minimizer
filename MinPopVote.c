/*-------------------------------------------
Popular Vote Minimizer
System: Advanced zyLab
Author: Nandana Devarajan
------------------------------------------- */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#include "MinPopVote.h"

//function: reads command line argument, sets the election year, fast and quiet mode
//parameters: number of command line arguments, string array of arguments, pointer to year, pointer to booleans for fast and quiet mode
//returns: returns true if the argument is valid or false if its invalid
bool setSettings(int argc, char** argv, int* year, bool* fastMode, bool* quietMode) {
    *year = 0;
    *fastMode = false;
    *quietMode = false;
    for(int i = 1; i < argc; ++i){
        if(strcmp(argv[i], "-y") == 0){
            if(++i < argc){
                int yearInput = atoi(argv[i]);
                if(yearInput >= 1828 && yearInput <= 2020 && yearInput % 4 == 0){
                    *year = yearInput;
                }
                else{
                    *year = 0;
                }
        }
            else{
                printf("Missing value for -y option\n");
                return false;
                }
        }
        else if(strcmp(argv[i], "-f") == 0){
            *fastMode = true;
        }
        else if(strcmp(argv[i], "-q") == 0){
            *quietMode = true;
        }
        else{
            printf("Unknown option entered: %s\n", argv[i]);
            return false;
        }
    }
return true;
}


void inFilename(char* filename, int year) {
    sprintf(filename, "data/%d.csv", year);
}


void outFilename(char* filename, int year) {
    sprintf(filename, "toWin/%d_win.csv", year);
}


//function: reads from input file
//parameters: line from file and pointer to State structure to store data in
//returns: returns true if a line from the file is valid, false if invalid
bool parseLine(char* line, State* myState) {
    if(line == NULL || myState == NULL){
        return false;
    }
    char copyOfLine[256];
    strncpy(copyOfLine, line, sizeof(copyOfLine));
    copyOfLine[sizeof(copyOfLine) -1] = '\0';

    char *commaptr;
    char* comma;
    comma = strtok_r(copyOfLine, ",", &commaptr);
    if(comma == NULL){
        return false;
    } 
    strncpy(myState->name, comma, sizeof(myState-> name));
    myState->name[sizeof(myState->name) -1] = '\0';
    comma = strtok_r(NULL, ",", &commaptr);
    if(comma == NULL){
        return false;
    }
    strncpy(myState->postalCode, comma, sizeof(myState-> postalCode));
    myState->postalCode[sizeof(myState->postalCode) -1] = '\0';
    comma = strtok_r(NULL, ",", &commaptr);
    if(comma == NULL){
        return false;
    }
    myState->electoralVotes = atoi(comma);
    if(myState->electoralVotes <= 0){
        return false;
    }
    comma = strtok_r(NULL, ",", &commaptr);
    if(comma == NULL){
        return false;
    }
    char* newline = strchr(comma, '\n');
    if(newline) *newline = '\0';
    myState -> popularVotes = atoi(comma);
    if(myState->popularVotes < 0){
        return false;
    }
        return true;
}
        

//function: reads data from file into the allStates array
//parameters: file name, array to store, pointer to where the numbers of states is stored
//return: returns true if the file is read, otherwise returns false
bool readElectionData(char* filename, State* allStates, int* nStates) {
    *nStates = 0; 
    FILE *file = fopen(filename, "r");
    if(file == NULL){
        return false;
    }
    char line[256];
    while(fgets(line, sizeof(line), file)){
        if(parseLine(line, &(allStates[*nStates]))){
            (*nStates)++;
            if(*nStates >= 52){
                fclose(file);
                return false;
            }
        }
        else{
            fclose(file);
            return false;
        }
    }
    if(ferror(file)){
        fclose(file);
        return false;
    }
fclose(file);
return true;
}


//function: calculates total number of electoral votes
//parameters: array of states, number of states in array
//returns: total number of electoral votes
int totalEVs(State* states, int szStates) {
    int totalNum = 0;
    for(int i = 0; i < szStates; i++){
        totalNum += states[i].electoralVotes;
    }
    return totalNum;
}


//function: calculates total number of popular votes
//parameters: array of states, number of states in array
//returns: total number of popular votes
int totalPVs(State* states, int szStates) {
    int totalNum = 0;
    for(int i = 0; i < szStates; ++i){
        totalNum += states[i].popularVotes;
    }
    return totalNum; 
}


//function: slow version of minimize popular vote algorithm
//parameters: array of states, number of states, index at the start, number of electoral votes 
//returns: subitems of MinInfo
MinInfo minPopVoteAtLeast(State* states, int szStates, int start, int EVs) {
   MinInfo includes;
   MinInfo excludes;
   includes.szSomeStates = 0;
   excludes.szSomeStates = 0;

   if(EVs <= 0){
       includes.szSomeStates = 0; 
       includes.sufficientEVs = true;
       includes.subsetPVs = 0;
       return includes;
   }
   if(start == szStates){
       includes.szSomeStates = 0; 
       includes.sufficientEVs = false;
       includes.subsetPVs = 0;
       return includes;
   }

    includes = minPopVoteAtLeast(states, szStates, start +1, EVs - states[start].electoralVotes);
    includes.subsetPVs += states[start].popularVotes/2 +1;
    includes.someStates[includes.szSomeStates] = states[start];
    includes.szSomeStates++;

    excludes = minPopVoteAtLeast(states, szStates, start +1, EVs);

    if(excludes.sufficientEVs && (includes.subsetPVs > excludes.subsetPVs)){
        return excludes;
    }
    else{
        return includes;
    }
}


MinInfo minPopVoteToWin(State* states, int szStates) {
    int totEVs = totalEVs(states,szStates);
    int reqEVs = totEVs/2 + 1; 
    return minPopVoteAtLeast(states, szStates, 0, reqEVs);
}


//function: fast version of minimize popular vote algorithm, uses memoization
//parameters: array of states, number of states in array, index at the start, number of electoral votes, memo
//returns: subitems of MisInfo
MinInfo minPopVoteAtLeastFast(State* states, int szStates, int start, int EVs, MinInfo** memo) {
   MinInfo includes;
   MinInfo excludes;
   includes.szSomeStates = 0;
   excludes.szSomeStates = 0;

   if(EVs <= 0){
       includes.szSomeStates = 0; 
       includes.sufficientEVs = true;
       includes.subsetPVs = 0;
       return includes;
   }
   if(start == szStates){
       includes.szSomeStates = 0; 
       includes.sufficientEVs = false;
       includes.subsetPVs = 0;
       return includes;
   }
   if(memo[start][EVs].subsetPVs >= 0){
       return memo[start][EVs];
   }

    includes = minPopVoteAtLeastFast(states, szStates, start +1, EVs - states[start].electoralVotes, memo);
    includes.subsetPVs += states[start].popularVotes/2 +1;
    includes.someStates[includes.szSomeStates] = states[start];
    includes.szSomeStates++;

    excludes = minPopVoteAtLeastFast(states, szStates, start +1, EVs, memo);

    if(excludes.sufficientEVs && (includes.subsetPVs > excludes.subsetPVs)){
        memo[start][EVs] = excludes;
        return excludes;
    }
    else{
         memo[start][EVs] = includes;
        return includes;
    }
    for(int i= 0; i < szStates+1; i++){
        free(memo[i]);
    }
    free(memo);
}


//function: fast version of minimize popular vote algorithm using memoization
//parameters: array of states, number of states in array, index at the start, number of electoral votes, memo
//returns: subitems of MisInfo
MinInfo minPopVoteToWinFast(State* states, int szStates) {
    int totEVs = totalEVs(states,szStates);
    int reqEVs = totEVs/2 + 1; // required EVs to win election

    MinInfo** memo = (MinInfo**)malloc((szStates+1)*sizeof(MinInfo*));
    for (int i = 0; i < szStates+1; ++i) {
        memo[i] = (MinInfo*)malloc((reqEVs+1)*sizeof(MinInfo));
        for (int j = 0; j < reqEVs+1; ++j) {
            memo[i][j].subsetPVs = -1;
        }
    }
    MinInfo res = minPopVoteAtLeastFast(states, szStates, 0, reqEVs, memo); 
    for(int i = 0; i < szStates+1; ++i){
        free(memo[i]);
    }
    free(memo);
    return res;
}


//function: writes the data to file
//parameters:output file, total electoral votes, total popular votes, number of electoral votes that have been won, number of popular votes that have been won, subset of states needed to win
//returns: returns true if data is written to file, false if not
bool writeSubsetData(char* filenameW, int totEVs, int totPVs, int wonEVs, MinInfo toWin) {
    FILE *file = fopen(filenameW, "w");
    if(file == NULL){
        return false;
    }
    fprintf(file, "%d,%d,%d,%d\n", totEVs, totPVs, wonEVs, toWin.subsetPVs);
    for(int i = toWin.szSomeStates -1; i >= 0; i--){
        fprintf(file, "%s,%s,%d,%d\n", toWin.someStates[i].name, toWin.someStates[i].postalCode, toWin.someStates[i].electoralVotes,toWin.someStates[i].popularVotes/2 +1);
    }
    fclose(file);
    return true; 
}
