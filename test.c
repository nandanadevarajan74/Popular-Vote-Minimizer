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


//tests totalEVs()
bool test_totalEVs() {
    State aStates[10];
    int res;

    aStates[0].electoralVotes = 5;
    aStates[1].electoralVotes = 8;
    aStates[2].electoralVotes = 12;
    aStates[3].electoralVotes = 6;
    aStates[4].electoralVotes = 7;
    aStates[5].electoralVotes = 10;

    printf(" Checking totalEVs() for 5 States:\n");
    res = totalEVs(aStates,5);
    if (res != 38) {
        printf("  individual state EVs are 5, 8, 13, 6, 7\n");
        printf("  expected total EVs = 38, actual total EVs = %d\n",res);
        return 0;
    }

    aStates[0].electoralVotes = 5;
    aStates[1].electoralVotes = 8;
    aStates[2].electoralVotes = 12;
    aStates[3].electoralVotes = 6;
    aStates[4].electoralVotes = 7;
    aStates[5].electoralVotes = 15;
    aStates[6].electoralVotes = 12;
    aStates[7].electoralVotes = 8;
    aStates[8].electoralVotes = 14;
    aStates[9].electoralVotes = 13;
    
    printf(" Checking totalEVs() for 10 States:\n");
    res = totalEVs(aStates,10);
    if (res != 100) {
        printf("  expected total EVs = 100, actual total EVs = %d\n",res);
        return false;
    }
    return true;
}


//tests total_PVs()
bool test_totalPVs() {
    State aStates[10];
    int res;
    aStates[0].popularVotes = 5000;
    aStates[1].popularVotes = 8000;
    aStates[2].popularVotes = 12000;
    aStates[3].popularVotes = 6000;
    aStates[4].popularVotes = 7000;
    //aStates[5].popularVotes = 15000;
    printf(" Checking totalPVs() for 10 States:\n");
    res = totalPVs(aStates,5);
    if (res != 38000) {
        printf("  expected total PVs = 38000, actual total PVs = %d\n",res);
        return false; //modify this
}

    aStates[5].popularVotes = 15000;
    aStates[6].popularVotes = 13000;
    aStates[7].popularVotes = 8000;
    aStates[8].popularVotes = 14000;
    aStates[9].popularVotes = 13000;

    printf(" Checking totalPVs() for 10 States:\n");
    res = totalPVs(aStates,5);
    if (res != 123000) {
        printf("  expected total PVs = 123000, actual total PVs = %d\n",res);
        return false; //modify this
}
return true;
}


bool test_setSettings() {
    int year;
    bool fastMode;
    bool quietMode;
    char* argv[5];
    int argc;
    bool res;

    argv[0] = "./test.exe";
    argv[1] = "-y";
    argv[2] = "2016";
    argv[3] = "-f";
    argc = 4;
    res = setSettings(argc, argv, &year, &fastMode, &quietMode);
    if (!res || year != 2016 || !fastMode || quietMode) {
       printf("  Failed setSettings Test 1: Expected year = 2016, fastMode= true, quietMode = false, Actual year = %d, fastMode = %d, quietMode = %d\n", year, fastMode, quietMode);
        return false; //modify this
    }
    else{
        printf("setSettings test 1 passed");
        return true;
    }
    argv[0] = "./test.exe";
    argv[1] = "-y";
    argv[2]= "2017";
    argc = 3;
    res = setSettings(argc, argv, &year, &fastMode, &quietMode);
    if (!res || year != 2016 || !fastMode || quietMode) {
       printf("  Failed setSettings Test 2: %d should be invalid\n", year);
        return false; 
    }
}


bool test_inFilename() {
    char filename[50];

    int testYear1 = 2016;
    inFilename(filename, testYear1);
    if(strcmp(filename, "data/2016.csv")!= 0){
        printf(" Failed inFilename Test 1");
    return false; 
}
    int testYear2 = 2017;
    inFilename(filename, testYear2);
    if(strcmp(filename, "data/2017.csv")!= 0){
        printf(" Failed inFilename Test 2");
    return false;
    }
    return true;
}


bool test_outFilename() {
    char filename[50];

    int testYear1 = 2016;
    outFilename(filename, testYear1);
    if(strcmp(filename, "toWin/2016_win.csv") !=0){
    printf(" Failed outFile Test 1\n");
    return false; 
}
    int testYear2 = 2017;
    outFilename(filename, testYear1);
    if(strcmp(filename, "toWin/2017_win.csv") !=0){
    printf(" Failed outFile Test 1\n");
    return false; 
}
return true;
}


bool test_parseLine() {
    return false;
}


bool test_readElectionData() {
    State allStates[52];
    int nStates;

    if(readElectionData("1999.csv", allStates, &nStates)){
        printf("Test 1 failed, invalid file should have returned false\n");
        return false;
    }
    if(readElectionData("2016.csv", allStates, &nStates)){
        printf("Test 2 failed, valid file should have returned true");
        return false;
    }
    return true; 
}


bool test_minPVsSlow() {
    //----------------------------------------------------------------- 
    // TODO: Task 7 - write your own test case for minPopVoteAtLeast();
    //                make sure to test all components
    //-----------------------------------------------------------------
    return false; //modify this
}


bool test_minPVsFast() {
    //--------------------------------------------------------------------- 
    // TODO: Task 8 - write your own test case for minPopVoteAtLeastFast();
    //                make sure to test all components
    //---------------------------------------------------------------------
    return false; //modify this
}


int main() {
    printf("Welcome to the Popular Vote Minimizer Testing Suite!\n\n");
    
    printf("Testing totalEVs()...\n"); 
    if (test_totalEVs()) {
        printf("  All tests PASSED!\n");
    } else {
        printf("  test FAILED.\n");
    }

    printf("Testing totalPVs()...\n"); 
    if (test_totalPVs()) {
        printf("  All tests PASSED!\n");
    } else {
        printf("  test FAILED.\n");
    }

    printf("Testing setSettings()...\n"); 
    if (test_setSettings()) {
        printf("  All tests PASSED!\n");
    } else {
        printf("  test FAILED.\n");
    }
    
    printf("Testing inFilename()...\n"); 
    if (test_inFilename()) {
        printf("  All tests PASSED!\n");
    } else {
        printf("  test FAILED.\n");
    }

    printf("Testing outFilename()...\n"); 
    if (test_outFilename()) {
        printf("  All tests PASSED!\n");
    } else {
        printf("  test FAILED.\n");
    }

    printf("Testing parseLine()...\n"); 
    if (test_parseLine()) {
        printf("  All tests PASSED!\n");
    } else {
        printf("  test FAILED.\n");
    }

    printf("Testing readElectionData()...\n"); 
    if (test_readElectionData()) {
        printf("  All tests PASSED!\n");
    } else {
        printf("  test FAILED.\n");
    }
    
    printf("Testing minPopVoteToWin()...\n"); 
    if (test_minPVsSlow()) {
        printf("  All tests PASSED!\n");
    } else {
        printf("  test FAILED.\n");
    }

    printf("Testing minPopVoteToWinFast()...\n"); 
    if (test_minPVsFast()) {
        printf("  All tests PASSED!\n");
    } else {
        printf("  test FAILED.\n");
    }

    return 0;
}
