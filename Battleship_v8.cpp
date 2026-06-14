/*
    Author:  Ivan Ruiz
    Date:    6-13-2026
    Purpose: Battleship
*/

//System Libraries

#include <iostream> //Input/Output Library //X
#include <ctime> //time function()
#include <iomanip> //setprecision() & fixed
#include <fstream> //File Input/Output //ofstream and ifstream data type and open() and close() function
#include <cmath> //round() function
#include <cstdlib> //Random Number generator rand() and seed to prevent same number from running //X
#include <string> //string data type and length() function
#include <vector> //vector data type
using namespace std;

//User Libraries

//Global Constants - Scientific/Physics/Chemistry
//Function Prototypes
void getCrds(char &, char &); //Gets coordinates from player
bool duplHit(char, char, char [][2],int sze); //Returns true if player coordinate matches that of an already sunk ship. Will also utilize to check user coordinate with the coordinates of the 10 ships
bool chckBnd(char, char); //Retruns true if player coordinate is out bounds from the grid
void filCrds(char[], char [], int); //Fill in the array with coordinates for the 10 ships
void shwGams(int &); //increments a static integer variable after each game
bool alrdSht(vector<string> &, char, char); //linear search to find any repeated shots in the shot log vector
void filLog(vector<string> &, char, char); //store user coordinate to shot log vector
bool filSunk(char, char, char [], char [], char [][2], int); //fills the sunk coordinate array if user has sunk a ship
void gmrslts(vector<string> &, char [][2], int, int); //displays results after each game 
void ldrBord(float [], int [], int); //sorts the accuracy data from highest to lowest
void srtVctr(vector <string> &); //sorts the vector by alphabet order a-j and then sorts the numbers within each letter a1-a0

void repeat(); //repeat the program if user wishes to continue
//Execution beings here

int main(int argv, char **argc){
    //set the random number seed
    //'srand' function use satisfies the library requirement for <cstdlib> header for project 1
    srand(time(0)); // time function satisfies the use of <ctime> library //Get time in miliseconds

    //declare variables
    const int MXCRDS=2; //Holds the maximum amount of columns for each coordinate
    const int MXSHPS = 11; //Holds the maximum amount of ships, 10 and 1 more for safety

    //Holds the coordinate of the sunk ships as a 2d-array. Row represents each a sunk ship, column represents each coordinate
    char sunkShp[MXSHPS][MXCRDS]; //declare 2d-array with size of number of ships as rows and their coordinates which is only 2 characters so column size is 2 
    char cShips[10]; //Holds the column values of the 10 randomly generated ships plus 1 for sentinel flag
    char rShips[10]; //Holds the row values of the 10 randomly generated ships plus 1 for sentinel flag
    float accry[10]; //Holds the accuracy data for each game where the maximum amount of games allowed in one session is 10
    int numGams[10]; //holds the index for the game

    //This satisfies the <fstream> library use requirement for project 1 by using data type 'ofstream' and 'ifstream'
    ofstream acrcyOF; //Writes accuracy data from each game to file named "Accuracy.txt"
    ifstream acrcyIF; //Reads data from "Accuracy.txt" to display at end of session to display accuracy percentage of user
    string crdntes; //Holds the coordinates of the user
    vector<string> shotLog; //holds the log of coordinate shots

    //This satisfies the float data type requirement for project 1
    float accrcy; //Holds the accuracy percentage of ships successfully destroyed out of number of attempts round((3.0f / atemps) * 100.0f) %
    int numSunk; //holds the number of ships sunk
    int games; //How many games the user has played. Used to access the index of the accry float array by passing as reference to a function with a static variable
    int atemps; //Holsa number of attempts the user made
    int game; //holds the game number when displaying leaderboard
    //User coordinates
    char uColumn; //holds the letter of the coordinate inputted by the user
    char uRow; //holds the coordinate number of the user coordinate inputted by the user
    //bool flags
    bool inBnds; //holds true if user is within bounds and false if not
    bool isSunk; //holds true if user typed in a coordinate of a sunk ship and false if not
    bool isRptd; //holds true if user typed in a coordinate that was previously typed in
    //if user typed 'y' then repeat do-while loop
    char cntnue; //bool flag to continue after finished game

    //Initialize Variables
    //begin setting the first column for every ship to null terminator to indicate empty row
    for (int i=0; i<MXSHPS; i++){
        sunkShp[i][0] = '\0'; //set the first column of ship 'i' to a null terminator to indicate empty row
    }
    acrcyOF.open("Accuracy.txt"); //Create and open file to write accuracy data to text file
    accrcy = 0.0f; //Accuracy is calculated by dividing the number of the successful ships sunk by the total number of attemps used
    numSunk = 0; //set number of ships sunk to 0
    games = 0; //set number of games played to 0
    atemps = 0; //How many attempts the user has done to find enemy ships
    inBnds = false; //assume user types in coordinate out of bounds of the grid, so set to false
    isSunk = true; //assume user typed in a coordinate of a sunk ship
    isRptd = true; //assume user typed in a coordinate they had previously typed in

    filCrds(cShips, rShips, 10); //randomly generate 10 ships

    //Display the game name and the number of attempts the user has
    //This satisfies the cout requirement for project1
    cout << setw(20) << "----BATTLESHIP----" << endl << setw(20) << "Find all 10 Enemy Ships" << endl; 

    //Display Outputs
    //This satisfies the do/while requirement for project 1
    do{
        //This satisfies the cout requirement for project 1
         //This satisfies the multiple assignment requirement for project 1
        getCrds(uColumn, uRow);

        //verify user coordinate is within the bounds of the grid
        inBnds = chckBnd(uColumn, uRow); //holds whether the user coordinate is within the bounds of the grid
        isSunk = duplHit(uColumn, uRow, sunkShp, numSunk); //returns true if user coordinate matches the coordinate of a sunk ship
        isRptd = alrdSht(shotLog, uColumn, uRow); //returns true if user typed in a coordinate previously inputted

        //begin user coordinate validation

        while (!inBnds || isSunk || isRptd){ //while user coordinate is not within the bounds of the grid OR user coordinate matches that of an already sunk ship OR user typed in a previously typed in coordinate
            //begin displaying respective error messages
            if (!inBnds){ //if user is out of bounds of the grid
                uRow == ':'? cout << "Coordinate " << '[' << uColumn << ']' << '[' << '0' << ']' << " is Out Of Bounds" << endl << endl: cout << "Coordinate " << '[' << uColumn << ']' << '[' << uRow << ']' << " is Out Of Bounds" << endl << "Enter Coordinate Again" << endl << endl; //display out of bounds error message
            }
            else if (isRptd && isSunk){ //is user typed in a valid repeated coordinate and the coordinate is that of a sunk ship
                cout << "Coordinate " << '[' << uColumn << ']' << '[' << uRow << ']' << " Is a Coordinate Of a Sunk Ship" << endl << endl; //display already sunk ship error message
            }
            else if (isRptd){ //else if the user just typed in a valid repeated coordinate
                cout << "Coordinate " << '[' << uColumn << ']' << '[' << uRow << ']' << " has already been typed in" << endl << endl; //display repeated shot error message
            }
            //get new coordinate
            getCrds(uColumn, uRow);
            inBnds = chckBnd(uColumn, uRow); //check if new coordinate if is within the bounds of the grid
            isSunk = duplHit(uColumn, uRow, sunkShp, numSunk); //check if new coordinate is the coordinate of an already sunk ship
            isRptd = alrdSht(shotLog, uColumn, uRow); //check if new coordinate is repeated
        }
        //add valid user coordinate to the shot log vector
        filLog(shotLog, uColumn, uRow); //fill shot log

        //begin checking if the user has hit a ship
        if (filSunk(uColumn, uRow, rShips, cShips, sunkShp, 10) != 0){ //if user has hit a ship
            cout << setw(20) << "Engagement at Coordinate " << '[' << uColumn << setw(1) << ']' << '[' << uRow << ']' << setw(4) << " Aftermath : Hit " << endl; //display successful engagement
            numSunk += 1; //increment num of ships destroyed
            atemps += 1; //increment number of attempts
        }
        else{
            cout << setw(20) << "Engagement at Coordinate " << '[' << uColumn << setw(1) << ']' << '[' << uRow << ']' << setw(4) << " Aftermath : Miss " << endl; //display unsuccessful engagement
            atemps += 1; //increment number of attemps
        }

        //Begin display of progress after each round
        //Display previous engaged coordinates, Display shots the user has shot, Display the number of ships the user has destroyed

        for (int i=0; i<10; i++){
                cout << '[' << cShips[i] << ']' << '[' << rShips[i] << ']' << endl;
        }

        switch(numSunk){
            case 0:
            cout << setw(20) << "No Ships Sunk" << endl;
            gmrslts(shotLog, sunkShp, 4, numSunk);
            break;
            case 1:
            cout << setw(20) << "1 ship Sunk" << endl;
            gmrslts(shotLog, sunkShp, 4, numSunk);
            break;
            case 5:
            cout << setw(20) << "Half Of The Ships Have Been Sunk. 5 More to Sink" << endl;
            gmrslts(shotLog, sunkShp, 4, numSunk);
            break;
            case 8:
            cout << setw(20) << "Only 2 Ships Left to Sink" << endl;
            gmrslts(shotLog, sunkShp, 4, numSunk);
            break;
            case 9:
            cout << setw(20) << "Only 1 Ship Left to Sink" << endl;
            gmrslts(shotLog, sunkShp, 4, numSunk);

            break;
            case 10:
            cout << setw(20) << "All 10 Ships Destroyed" << endl << "You Win";
            gmrslts(shotLog, sunkShp, 4, numSunk);
            break;
            default:
            cout << setw(20) << numSunk << " Ships Sunk" << endl;
            gmrslts(shotLog, sunkShp, 4, numSunk);
            break;
        }

    }while(numSunk != 10); //Continue prompting user for coordinates until all ships have been destroyed
    
    accry[games] = round((3.0f / atemps) * 100.0f);
    acrcyOF << fixed << setprecision(2) << games << " " << accry[games] << " " << atemps << endl;
    gmrslts(shotLog, sunkShp, 4, numSunk); //display the map of the game

    cout << "Play Again (y/n)" << endl; //Prompt to restart game or not
    cin >> cntnue; //Store flag in continue variable

    if (cntnue == 'n' || cntnue == 'N'){ //if user types 'n' to end session or the number of games for one session has been reached
        acrcyOF.close(); //close the file to write to
        acrcyIF.open("Accuracy.txt"); //open the file to read from
        for (int i=0; acrcyIF>>game>>accrcy>>atemps; i++){
            cout << endl << endl << endl << "Leaderboard" << endl << "--------------------------------------" << endl;//display the header for the leaderboard
            cout << fixed << setprecision(2) << "Game " << game << " Accuracy: " << accrcy << '%' << endl; //display the accuracy of the player
            cout << "Game " << game << " Total Attempts: " << atemps << endl; //Display the total attempts by the user to destroy all ships 
        }
        shwGams(games); 
        acrcyIF.close(); //Close "Accuracy.txt" file
    }
    cout << endl; //prepapre for next line
    shwGams(games); 



    while (cntnue == 'y' || cntnue == 'Y'){
        shotLog.clear(); //clear the shot history of the user
        for (int i=0; i<MXSHPS; i++){
            sunkShp[i][0] = '\0'; //set the first column of ship 'i' to a null terminator to indicate empty row
        }
        //begin reseting variables
        accrcy = 0.0f; //Accuracy is calculated by dividing the number of the successful ships sunk by the total number of attemps used
        numSunk = 0; //set number of ships sunk to 0
        atemps = 0; //How many attempts the user has done to find enemy ships
        inBnds = false; //assume user types in coordinate out of bounds of the grid, so set to false
        isSunk = true; //assume user typed in a coordinate of a sunk ship
        isRptd = true; //assume user typed in a coordinate they had previously typed in

        filCrds(cShips, rShips, 10); //randomly generate 10 ships
        cin.ignore(); //ignore newline character from input of 'y'

        do{
            //This satisfies the cout requirement for project 1
            //This satisfies the multiple assignment requirement for project 1
            getCrds(uColumn, uRow);

            //verify user coordinate is within the bounds of the grid
            inBnds = chckBnd(uColumn, uRow); //holds whether the user coordinate is within the bounds of the grid
            isSunk = duplHit(uColumn, uRow, sunkShp, numSunk); //returns true if user coordinate matches the coordinate of a sunk ship
            isRptd = alrdSht(shotLog, uColumn, uRow); //returns true if user typed in a coordinate previously inputted

            //begin user coordinate validation

            while (!inBnds || isSunk || isRptd){ //while user coordinate is not within the bounds of the grid OR user coordinate matches that of an already sunk ship OR user typed in a previously typed in coordinate
                //begin displaying respective error messages
                if (!inBnds){ //if user is out of bounds of the grid
                    uRow == ':'? cout << "Coordinate " << '[' << uColumn << ']' << '[' << '0' << ']' << " is Out Of Bounds" << endl << endl: cout << "Coordinate " << '[' << uColumn << ']' << '[' << uRow << ']' << " is Out Of Bounds" << endl << "Enter Coordinate Again" << endl << endl; //display out of bounds error message
                }
                else if (isRptd && isSunk){ //is user typed in a valid repeated coordinate and the coordinate is that of a sunk ship
                    cout << "Coordinate " << '[' << uColumn << ']' << '[' << uRow << ']' << " Is a Coordinate Of a Sunk Ship" << endl << endl; //display already sunk ship error message
                }
                else if (isRptd){ //else if the user just typed in a valid repeated coordinate
                    cout << "Coordinate " << '[' << uColumn << ']' << '[' << uRow << ']' << " has already been typed in" << endl << endl; //display repeated shot error message
                }
                //get new coordinate
                getCrds(uColumn, uRow);
                inBnds = chckBnd(uColumn, uRow); //check if new coordinate if is within the bounds of the grid
                isSunk = duplHit(uColumn, uRow, sunkShp, numSunk); //check if new coordinate is the coordinate of an already sunk ship
                isRptd = alrdSht(shotLog, uColumn, uRow); //check if new coordinate is repeated
            }
            //add valid user coordinate to the shot log vector
            filLog(shotLog, uColumn, uRow); //fill shot log

            //begin checking if the user has hit a ship
            if (filSunk(uColumn, uRow, rShips, cShips, sunkShp, 10) != 0){ //if user has hit a ship
                cout << setw(20) << "Engagement at Coordinate " << '[' << uColumn << setw(1) << ']' << '[' << uRow << ']' << setw(4) << " Aftermath : Hit " << endl; //display successful engagement
                numSunk += 1; //increment num of ships destroyed
                atemps += 1; //increment number of attempts
            }
            else{
                cout << setw(20) << "Engagement at Coordinate " << '[' << uColumn << setw(1) << ']' << '[' << uRow << ']' << setw(4) << " Aftermath : Miss " << endl; //display unsuccessful engagement
                atemps += 1; //increment number of attemps
            }

            for (int i=0; i<10; i++){
                    cout << '[' << cShips[i] << ']' << '[' << rShips[i] << ']' << endl;
            }

            switch(numSunk){
                case 0:
                cout << setw(20) << "No Ships Sunk" << endl;
                gmrslts(shotLog, sunkShp, 4, numSunk);
                break;
                case 1:
                cout << setw(20) << "1 ship Sunk" << endl;
                gmrslts(shotLog, sunkShp, 4, numSunk);
                break;
                case 5:
                cout << setw(20) << "Half Of The Ships Have Been Sunk. 5 More to Sink" << endl;
                gmrslts(shotLog, sunkShp, 4, numSunk);
                break;
                case 8:
                cout << setw(20) << "Only 2 Ships Left to Sink" << endl;
                gmrslts(shotLog, sunkShp, 4, numSunk);
                break;
                case 9:
                cout << setw(20) << "Only 1 Ship Left to Sink" << endl;
                gmrslts(shotLog, sunkShp, 4, numSunk);

                break;
                case 10:
                cout << setw(20) << "All 10 Ships Destroyed" << endl << "You Win";
                gmrslts(shotLog, sunkShp, 4, numSunk);
                break;
                default:
                cout << setw(20) << numSunk << " Ships Sunk" << endl;
                gmrslts(shotLog, sunkShp, 4, numSunk);
                break;
            }

    }while(numSunk != 10); //Continue prompting user for coordinates until all ships have been destroyed

    accry[games] = round((3.0f / atemps) * 100.0f);
    ldrBord(accry, numGams, games); //sort accuracy data from highest to lowest
    acrcyOF << fixed << setprecision(2) << games << " " << accry[games] << " " << atemps << endl;

    shwGams(games);
    cout << "Play again (y/n)" << endl; //Prompt user whether to start a new game after previous game has ended
    //This satisfies the flag requirement for project 1
    cin >> cntnue; //Input y flag for yes or n flag for no
    }

    acrcyOF.close(); //close the file to write to
    acrcyIF.open("Accuracy.txt"); //open the file to read from
    for (int i=0; acrcyIF>>game>>accrcy>>atemps; i++){
        cout << endl << endl << endl << "Leaderboard" << endl << "--------------------------------------" << endl;//display the header for the leaderboard
        cout << fixed << setprecision(2) << "Game " << game+1 << " Accuracy: " << accrcy << '%' << endl; //display the accuracy of the player
        cout << "Game " << game+1 << " Total Attempts: " << atemps << endl; //Display the total attempts by the user to destroy all ships 
    }
    acrcyIF.close(); //Close "Accuracy.txt" file
    shwGams(games);
    //Exit Stage Right
    return 0; //otherwise exit session
}

//Gets coordinates from player

void getCrds(char &cCrds, char &rCrds){
    string crds; //holds the coordinates of the user
    cout << "Input Coordinate from columns [a-e] and rows [1-5]" << endl;
    getline(cin, crds); //get coordinates

    //Determine if user enters 'q' or 'Q' to quit game
    if (crds == "q" || crds == "Q"){ //flag to quit game
        exit(0); //exit game
    }

    cCrds = crds[0]; //letter is always the first character
    if (crds[1] == '1' && crds[2] == '0' && crds[3] == '\0'){
        rCrds = '0'; //10 is represented by 0
    }
    else if (crds[1] == '0' && crds[2] == '\0'){//otherwise if the user typed in row 0, that does not exist, then
        rCrds = ':'; //then set the coordinate number equal to an out of bounds ASCII value
    }
    else{ //otherwise user did not input row 0
        rCrds = crds[1]; //otherwise set number value for the coordinate to the character in the second element of coordinate string
    }
};
//default parameter for filling coordinates
void filCrds(char shipC[], char shipR[], int sze=10){ //takes in the column and row array of the ships and the number of ships to generate plus 1 to place sentinel flag at end of both arrays
    bool isDupl; //holds true if any duplicate coordinates were found
    //first fill the arrays with the coordinate values
    for (int i=0; i<sze; i++){
        shipC[i] = static_cast<char>((rand() % 10) + 97); //gets a char value from a-j
        shipR[i] = static_cast<char>((rand() % 10) + 48); //gets a char value from 0-9
    }

    do{
        isDupl = false; //set to false to assume no duplicates until proven otherwise
        for (int i=0; i<sze; i++){
            for (int j=0; j<sze; j++){
                if (j!=i){
                        while (shipC[i] == shipC[j] && shipR[i] == shipR[j]){ //Continue looping until the coordinate of no longer matches that of all coordinates
                            shipC[i] = static_cast<char>((rand() % 10) + 97); //generate new coordinate letter value 
                            shipR[i] = static_cast<char>((rand() % 10) + 48); //generate new coordinate number value
                            isDupl = true; //duplicate has been found, so set status to true
                    } 
                }
            }
            
        }
    }while(isDupl);
};


bool duplHit(char c1, char c2, char snkShps[][2], int rows){ //Returns true if the player coordinates match those of an already sunk ship. This prevents duplicate ship sunk counters
    
    for (int i=0; i<rows; i++){ //iterate through each row and continue until number of rows has been exhausted OR the sentinel flag has been found
        //check if user coordinate matches those of an already sunk ship
        for (int k=0; k<2-1; k++){ //iterate through the current sunk ship's coordinate
            if (snkShps[i][k] == c1 && snkShps[i][k+1] == c2){ //if user coordinate matches the coordinate of a sunk ship
                return true; //the user has typed in a coordinate of an already destroyed ship, so return true
            }
        }
    }
    return false; //no sunk ship coordinate found, so return false
};

bool chckBnd(char cCrds, char rCrds){ //Retruns false if player coordinate is in the bounds of the grid
    if ((cCrds < 97 || cCrds > 106) || (rCrds < 48 || rCrds > 57)){
        return false; //Returns false if both column and row values are not in the range of the values accepted by their ascii value
    }
    else{
        return true; //otherwise the user coordinate is within the bounds of the grid, so return true
    }
};

void shwGams(int &gmes){ //increment after user types 'y' or 'Y' to play another game, takes in reference variable for number of games played
    static int totGams = 0; //initialize total number of games played
    gmes = totGams;
    totGams += 1; //increment
    cout << "Total Games Played this Session: " << totGams << endl; 
}

bool alrdSht(vector<string> &log, char c1, char c2){ //takes in the shot log vector and the user coordinate
    string coord; //holds the user coordinate as a string
    coord += c1; //add the letter of the coordinate
    coord += c2; //add the number of the coordinate
    for (int i=0; i<log.size(); i++){ //continue looping until the end of the vector size
        if (log[i] == coord){ //if user coordinate matches a coordinate from the shot log
            return true; //repeated coordinate found. return true
        }
    }
    return false; //no repeated coordinate found. return false
}

void filLog(vector<string> &log, char c1, char c2){ //takes in the shot log vector and user coordinate
    string coord; //holds the coordinate as a string
    coord += c1; //append the letter of the coordinate
    if (c2 == 0){ //if user coordinate is 0 which represents 10
        coord += "10"; //append 10 to the number of the coordinate
    }
    coord += c2; //append the number of the coordinate
    log.push_back(coord); //append to shot log vector
}

bool filSunk(char c1, char c2, char shipRow[], char shipClmn[], char snkArry[][2], int sze){
    bool hit; //holds true if user has hit a ship
    hit = false; //assume user did not hit a ship
    int index = 0; //set index to zero to check every row in sunk ship array for a null terminator

    for (int i=0; i<sze && !hit; i++){ //continue looping until user coordinate have been compared to all of the generated ship's coordinates (10) or user coordinate matches the coordinate of a ship
        if (c1 == shipClmn[i] && c2 == shipRow[i]){ //if user coordinate matches the coordinate a ship
            hit = true; //set hit status to true
        }
    }

    if (hit){ //if user coordinate matches the coordinate of a ship
        while (snkArry[index][0] != '\0'){ //then find a empty row in the sunk ship array
            index++; //increment index to find empty row
        }
        //after empty row is found
        snkArry[index][0] = c1; //replace empty column with coordinate letter
        snkArry[index][1] = c2; //replace next empty column with coordinate number
    }
    return hit; //return hits status
}

void gmrslts(vector<string> &log, char sunk[][2], int wdth, int numSnk){
    string crdFnd = "";
    int index = 0;
    char status; //holds either the character 'o' for no shot at coordinate, '*' for shot at coordinate but was a miss, and 'x' for shot at coordinate and sunk a ship
    string coord; //holds a grid coordinate
    coord = ""; //set to empty string
    int numCord;  //holds the number value of the coordinate. to be inputted to duplHit function

    srtVctr(log); //sort vector

    cout << endl << setw(wdth*10) << "BattleShip Board" << endl << setw(wdth) << "|";
    for (int i=1; i<=10; i++){ //iterate through each number until 10 and display the current column number
        cout << setw(wdth) << i; //display the column numbers
    }
    cout << endl; //new line for the header border seperating the column numbers and the grid

    cout << "---|"; //display border

    for (int i=1; i<=10; i++){ //continue looping until each column number has a matching border width
        for (int k=1; k<=wdth; k++){ //continue displaying border to match the width of the numbers above
            cout << '-'; //display border piece
        }
    }
    cout << endl; //new line for next row

    for (int i=97; i<=106; i++){ //iterate through each letter, a-j,  by their ASCII value
        cout << " " << static_cast<char>(i) << " |"; //display the letter by converting the integer ASCII value to char data type to display its character and then border
        for (int k=1; k<=10; k++){ //iterate through each column for the variable value of 'i'
            index = 0; //reset index to start at 1 which is 0
            coord = static_cast<char>(i); //reset value of current grid coordinate back to empty string for each column and set the letter coordinate value back to the letter

            if (k == 10){ //if the column is 10 then 
                coord += '0';
            }
            else{//otherwise
                coord += static_cast<char>(k+48); 
            }

            while (index < log.size() && log[index] != coord){
                index++;
            }

            if (index < log.size()){
                if (duplHit(log[index][0], log[index][1], sunk, numSnk)){
                    cout << setw(wdth) << 'x';
                }
                else{
                    cout << setw(wdth) << '*';
                }
            }
            else{
                cout << setw(wdth) << 'o';
                }
    }
    cout << endl; //prepare for next letter's columns
    }
    cout << "   |   o = unknown   O = miss   x = hit/sunk" << endl << endl;
}

void ldrBord(float data[], int gameNum[], int numGms){ //takes in the accuracy data, array corresponding to the accuracy data's indexes, and the number of games played

    gameNum[numGms] = {}; //initialize all elements to 0

    float tempry; //temporarily holds the float accuracy data when swapping
    int iTempry; //temporarily holds the index corresponding to the accuracy data
    tempry = 0.0f; //initialize
    iTempry = 0; //initialize

    //begin sorting accuracy data from highest to lowest
    for (int i=0; i<numGms; i++){ //continue looping until all data has been compared
        for (int k=0; k<numGms; k++){ //continue looping until data being checked is compared to all other data elements in the data array
            if (data[i] < data[k]){ //if the accuracy decimal is smaller than the current accuracy decimal
                tempry = data[i]; //hold accuracy data
                data[i] = data[k]; //swap smaller element with bigger element
                data[k] = tempry; //swap bigger element with the smaller element
                //begin swapping index
                iTempry = gameNum[i]; //hold smaller element's corresponding index
                gameNum[i] = gameNum[k]; //swap smaller element's index with the bigger element's index
                gameNum[k] = iTempry; //swap the bigger element's corresponding index with the smaller element's corresponding index
            }
        }
    }
}
void srtVctr(vector<string> &lg){
    string tempry; //holds the coordinate when swapping
    int num1, num2; //holds the ASCII value of the first character from the coordinate string of beginning vector element and the one next to it. It also stores the second ASCII value (number) of the coordinate after sorting the vector alphabetically

    for (int i = 0; i < lg.size() - 1; i++){ //iterate through the vector element
        for (int k = i + 1; k < lg.size(); k++){ //iterate through the next vector element

            if (lg[k][0] < lg[i][0]){ //if the first character of the next vector element comes first alphabetically than the vector element
                tempry = lg[i]; //hold the vector element that comes later, alphabetically
                lg[i] = lg[k]; //swap the vector element that comes later, alphabetically with the vector element that comes first, alphabetically
                lg[k] = tempry; //swap the vector element that comes first, alphabetically with the vector element that comes lafter, alphabetically
            }

            //if the first character of the vector element is the same as the first character of the next vector element, then begin sorting the number by least to greatest
            else if (lg[k][0] == lg[i][0]){ //if the first character of the next vector element is the same as the first character of the vector element

                if (lg[i][1] == '0'){ //if the number of the vector element is 10 (represented by 0)
                    num1 = 10; //store the value of the vector element to it's coordinate value
                }
                else{ //if the number of the vector element is not 10
                    num1 = static_cast<int>(lg[i][1]) - 48; //static cast the char to an integer and subtract by 48 to get number from the ASCII value
                }

                if (lg[k][1] == '0'){ //if the number of the next vector element is 10 (represented by 0)
                    num2 = 10; //store the value of the next vector element to it's coordinate value
                }
                else{ //if the number of the next vector element is not 10
                    num2 = static_cast<int>(lg[k][1]) - 48; //static cast the char to an integer to get the number from the ASCII value
                }

                if (num2 < num1){ //if the number of the next vector element is smaller than the vector element then begin swapping
                    tempry = lg[i]; //hold the vector element that comes after, numerically
                    lg[i] = lg[k]; //swap the vector element that comes after, numerically with the next vector element that comes before, numerically
                    lg[k] = tempry; //swap the next vector element with the vector element that comes after, numerically
                }
            }
        }
    }
}
