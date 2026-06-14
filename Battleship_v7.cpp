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
void getCrds(char &cCrds, char &rCrds); //Gets coordinates from player
bool duplHit(char, char, char [][2],int sze); //Returns true if player coordinate matches that of an already sunk ship. Will also utilize to check user coordinate with the coordinates of the 10 ships
bool chckBnd(char, char); //Retruns true if player coordinate is out bounds from the grid
void filCrds(char[], char [], int); //Fill in the array with coordinates for the 10 ships
void shwGams(int &); //increments a static integer variable after each game
bool alrdSht(vector<string> &, char, char); //linear search to find any repeated shots in the shot log vector
void filLog(vector<string> &, char, char); //store user coordinate to shot log vector
bool filSunk(char, char, char [], char [], char [][2], int); //fills the sunk coordinate array if user has sunk a ship
void gmrslts(vector<string> &, char [][2], int, int); //displays results after each game 
void ldrBord(float [], int [], int); //sorts the accuracy data from highest to lowest

void repeat(); //repeat the program if user wishes to continue
//Execution beings here

int main(int argv, char **argc){
    //set the random number seed
    //'srand' function use satisfies the library requirement for <cstdlib> header for project 1
    srand(time(0)); // time function satisfies the use of <ctime> library //Get time in miliseconds
    //declare variables
    const int MXCRDS=2; //Holds the maximum amount of columns for each coordinate
    const int MXSHPS = 11; //Holds the maximum amount of ships, 10, and the sentinel value 'z'
    //This satisfies the <fstream> library use requirement for project 1 by using data type 'ofstream' and 'ifstream'
    ofstream acrcyOF; //Writes accuracy data from each game to file named "Accuracy.txt"
    ifstream acrcyIF; //Reads data from "Accuracy.txt" to display at end of session to display accuracy percentage of user
    //This satisfies the float data type requirement for project 1
    float accrcy; //Holds the accuracy percentage of ships successfully destroyed out of number of attempts round((3.0f / atemps) * 100.0f) %

    int numSunk; //holds the number of ships sunk

    //string sunk; //Holds the coordinates of the sunk ships by the user. Used to verify the user does not use the same sunk coordinate multiple times to evade finding the other 2
    
    //Note: You can convert a ASCII character that represents a number to a integer data type AND then subtracting that by 48 to get the ACTUAL number. So, you do not have to use - '0' like they said in lab
    //Holds the coordinate of the sunk ships as a 2d-array. Row represents each a sunk ship, column represents each coordinate
    char sunkShp[MXSHPS][MXCRDS]; //declare 2d-array with size of number of ships as rows and their coordinates which is only 2 characters so column size is 2 
    //Parallel array type-shi
    char cShips[10]; //Holds the column values of the 10 randomly generated ships plus 1 for sentinel flag
    char rShips[10]; //Holds the row values of the 10 randomly generated ships plus 1 for sentinel flag
    string crdntes; //Holds the coordinates of the user
    float accry[10]; //Holds the accuracy data for each game where the maximum amount of games allowed in one session is 10
    int numGams[10]; //holds the index for the game
    //useful variables
        //User coordinates
    char uColumn; //holds the letter of the coordinate inputted by the user
    char uRow; //holds the coordinate number of the user coordinate inputted by the user
    vector<string> shotLog; //holds the log of coordinate shots
    bool inBnds; //holds true if user is within bounds and false if not
    bool isSunk; //holds true if user typed in a coordinate of a sunk ship and false if not
    bool isRptd; //holds true if user typed in a coordinate that was previously typed in
    //Potential useful variables
        //Bool for if column and row of user corresponds to the randomly generated coordinates of the ship
        //This satisfies the bool requirement for project 1
    bool isColmn;  //Bool for if column and row of user corresponds to the randomly generated coordinates of the ship
    bool isRow;
    int dstryd; //How many ships user has destroyed
    int games; //How many games the user has played. Used to access the index of the accry float array by passing as reference to a function with a static variable


    //Useless variables 
    int atemps; //7 rounds of fighting for the entire duration of the game
    char cntnue; //bool flag to continue after finished game
    char ship1Cl; //Holds the column coordinate for ship 1
    int ship1Rw;   //Holds the row coordinate for ship 1
    bool isSunk1; //Holds the bool true if ship 1 has been sunk
    char ship2Cl; //Holds the column coordinate for ship 2
    int ship2Rw; //Holds the row coordinate for ship 2
    bool isSunk2; //Holds the bool true if ship 2 has been sunk
    char ship3Cl; //Holds the column coordinate for ship 3
    int ship3Rw; //Holds the row coordinate for ship 3
    bool isSunk3;//Holds the bool true if ship 3 has been sunk


    //Initialize Variables
    sunkShp[MXSHPS][MXCRDS] = {}; //set all values to null
    sunkShp[10][0] = 'z'; //set the last row to the sentinel value

    games = 0; //set number of games played to 0
    numSunk = 0; //set number of ships sunk to 0
    dstryd = 0; //Set number of ships destroyed to 0
    atemps = 0; //How many attempts the user has done to find enemy ships
    accrcy = 0.0f; //Accuracy is calculated by dividing the number of the successful ships sunk by the total number of attemps used
    acrcyOF.open("Accuracy.txt"); //Create and open file to write accuracy data to text file
    inBnds = false; //assume user types in coordinate out of bounds of the grid, so set to false
    isSunk = true; //assume user typed in a coordinate of a sunk ship
    isRptd = true; //assume user typed in a coordinate they had previously typed in

    filCrds(cShips, rShips, 10); //randomly generate 10 ships


//-------------------------------------------------Debug Code -------------------------------------------------------------
    for (int i=0; i<10; i++){ //displays the coordinates of the randomly generated ships
        cout << "Ship " << i+1 << " Coordinate: " << '[' << cShips[i] << ']' << '[' << rShips[i] << ']' << endl;
    }
//-------------------------------------------------Debug Code -------------------------------------------------------------


    //Display the game name and the number of attempts the user has
    //This satisfies the cout requirement for project1
    cout << "BATTLESHIP" << endl << "Find all 10 Enemy Ships" << endl; 

    //Display Outputs
    //This satisfies the do/while requirement for project 1
    do{
            //This satisfies the cout requirement for project 1
            //cout << endl << endl << endl << "Input Column Coordinate From the Range of Letters [a-e]" << endl << "Input Row Coordinate From the Range [1-5]" << endl; 
            //This satisfies the multiple assignment requirement for project 1
        getCrds(uColumn, uRow);

        //verify user coordinate is within the bounds of the grid
        inBnds = chckBnd(uColumn, uRow); //holds whether the user coordinate is within the bounds of the grid
        isSunk = duplHit(uColumn, uRow, sunkShp, numSunk); //returns true if user coordinate matches the coordinate of a sunk ship
        isRptd = alrdSht(shotLog, uColumn, uRow); //returns true if user typed in a coordinate previously inputted

        //begin user coordinate validation

        while (!inBnds || isSunk || isRptd){ //while user coordinate is not within the bounds of the grid OR user coordinate matches that of an already sunk ship OR user typed in a previously typed in coordinate
            if (!inBnds){ //if user is out of bounds of the grid
                cout << "Coordinate " << '[' << uColumn << ']' << '[' << uRow << ']' << " is Out Of Bounds" << endl << "Enter Coordinate Again" << endl; //display out of bounds error message
                getCrds(uColumn, uRow); //get coordinates
                inBnds = chckBnd(uColumn, uRow); //returns true if user is within bounds and false if not
                isSunk = duplHit(uColumn, uRow, sunkShp, numSunk); //return boolean value of new coordinate compared to the coordinates of the sunk ships
                isRptd = alrdSht(shotLog, uColumn, uRow); //return boolean value of the new coordinate that is compared to every coordinate the user has previously inputted
            }
            if (isRptd){ //if user typed in a coordinate they had previously typed in
                if (isSunk){ //check if that previously typed coordinate is that of a sunk ship
                    cout << "Coordinate " << '[' << uColumn << ']' << '[' << uRow << ']' << " Is a Coordinate Of a Sunk Ship" << endl << "Enter Coordinate Again" << endl; //display already sunk ship error message
                    getCrds(uColumn, uRow); //get coordinates 
                    isSunk = duplHit(uColumn, uRow, sunkShp, numSunk); //returns true if user coordinate matches the coordinate of a sunk ship
                    isRptd = alrdSht(shotLog, uColumn, uRow); //return boolean value of the new coordinate that is compared to every coordinate the user has previously inputted
                    inBnds = chckBnd(uColumn, uRow); //checks if the new coordinate is within the bounds of the grid
                }
                else{ //otherwise user typed in a normal coordinate that had been previously typed in, so
                    cout << "Coordinate " << '[' << uColumn << ']' << '[' << uRow << ']' << " has already been typed in" << endl << "Enter Coordinate Again" << endl; //display repeated shot error message
                    getCrds(uColumn, uRow); //get coordinates
                    isRptd = alrdSht(shotLog, uColumn, uRow); //returns true if user typed in a coordinate previously inputted
                    inBnds = chckBnd(uColumn, uRow); //checks if the new coordinate is within the bounds of the grid
                    isSunk = duplHit(uColumn, uRow, sunkShp, numSunk); //return boolean value of new coordinate compared to the coordinates of the sunk ships
                }
            }
        }
        //add valid user coordinate to the shot log vector
        filLog(shotLog, uColumn, uRow); //fill shot log
        atemps += 1; //increment number of attemps
        //begin checking if the user has hit a ship

//-------------------------------------------------Debug Code -------------------------------------------------------------
        bool didItHit = filSunk(uColumn, uRow, rShips, cShips, sunkShp, 10);
        cout << "If Value Is 0, The User Did Not Hit The Ship. But If The User Hit The Ship, It will Display a Number Other Than 0" << endl;
        cout << "Hit Status Boolean Value: " << didItHit << endl;
//-------------------------------------------------Debug Code -------------------------------------------------------------


        if (filSunk(uColumn, uRow, rShips, cShips, sunkShp, 10) != 0){ //if user has hit a ship
            cout << "Engagement at Coordinate " << '[' << uColumn << ']' << '[' << uRow << ']' << " Aftermath : Hit " << endl; //display successful engagement
            numSunk += 1; //increment num of ships destroyed
        }
        else{
            cout << "Engagement at Coordinate " << '[' << uColumn << ']' << '[' << uRow << ']' << " Aftermath : Miss" << endl; //display unsuccessful engagement
        }

        //Begin display of progress after each round
        //Display previous engaged coordinates, Display shots the user has shot, Display the number of ships the user has destroyed
        numSunk == 10? cout << "All Ships Destroyed" << endl << "You Win" << endl : cout << "FUFILL LATER DAWG" << endl;
        switch(numSunk){
            case 0:
            cout << "No Ships Sunk" << endl;
            break;
            case 1:
            cout << "1 ship Sunk" << endl;
            break;
            case 5:
            cout << "Half Of The Ships Have Been Sunk. 5 More to Sink" << endl;
            break;
            case 8:
            cout << "Only 2 Ships Left to Sink" << endl;
            break;
            case 9:
            cout << "Only 1 Ship Left to Sink" << endl;
            break;
            default:
            cout << numSunk << " Ships Sunk" << endl;
            break;
        }

    }while(numSunk != 10); //Continue prompting user for coordinates until all ships have been destroyed

        //Begin writing accuracy data to "Accuracy.txt"

    cout << "Play again (y/n)" << endl; //Prompt user whether to start a new game after previous game has ended
    //This satisfies the flag requirement for project 1
    cin >> cntnue; //Input y flag for yes or n flag for no
    
    accry[games] = round((3.0f / atemps) * 100.0f);
    ldrBord(accry, numGams, games); //sort accuracy data from highest to lowest
    acrcyOF << fixed << setprecision(2) << accry[games] << " " << atemps << endl;

    gmrslts(shotLog, sunkShp, 10, numSunk); //display the map of the game
    cout << "Play Again (y/n)" << endl; //Prompt to restart game or not
    cin >> cntnue; //Store flag in continue variable

    if (cntnue == 'n' || cntnue == 'N' || games == 10){ //if user types 'n' to end session or the number of games for one session has been reached
        acrcyOF.close(); //close the file to write to
        acrcyIF.open("Accuracy.txt"); //open the file to read from
        for (int i=0; acrcyIF>>accrcy>>atemps; i++){
            cout << endl << endl << endl << "Leaderboard" << endl << "--------------------------------------" << endl;//display the header for the leaderboard
            cout << fixed << setprecision(2) << "Game " << i+1 << " Accuracy: " << accrcy << '%' << endl; //display the accuracy of the player
            cout << "Game " << i+1 << " Total Attempts: " << atemps << endl; //Display the total attempts by the user to destroy all ships 
        }
        acrcyIF.close(); //Close "Accuracy.txt" file
    }
    else if(cntnue == 'y' || cntnue == 'Y'){}//place function in here to repeat game or just copy and paste everything back to here
    else{
        acrcyIF.close(); //close "Accuracy.txt" file
    }
    //Exit Stage Right
    return 0; //otherwise exit session
}

//Gets coordinates from player

void getCrds(char &cCrds, char &rCrds){
    string crds;
    cout << "Input Coordinates from columns [a-e] and rows [1-5]" << endl;
    getline(cin, crds);
    //Determine if user enters 'q' or 'Q' to quit game
    if (crds == "q" || crds == "Q"){ //flag to quit game
        exit(0); //exit game
    }


    if (crds[2] == '\0'){//if user did not type more than 2 characters for the coordinate
        cCrds = crds[0]; //set the letter of the coordinate to the first character of the user input
        rCrds = crds[1]; //set the coordinate number 
    }//otherwise check to see if user typed in the number 10 for the coordinate number
    else if (crds[1] == '1' && crds[2] == '0'){ //If both chars combined are 10 then assume the user typed in 10
        rCrds = '0'; //10 will be represented as 0
    }
    else{//otherwise
        cCrds = crds[0]; //set the letter of the coordinate equal to the first character the user typed in
        rCrds = crds[1]; //then set the coordinate number equal to the second character the user typed in
    }
};

//default parameter for filling coordinates
void filCrds(char shipC[], char shipR[], int sze=10){ //takes in the column and row array of the ships and the number of ships to generate plus 1 to place sentinel flag at end of both arrays

    //First fill both arrays with coordinate values
    for (int i=0; i<sze; i++){
        shipC[i] = static_cast<char>((rand() % 10) + 97); //Randomly generate an integer and convert it into a char for the column value
        shipR[i] = static_cast<char>((rand() % 10) + 48); //Randomly generate an integer and convert it into a char for the row value
    }
    //Check for duplicate coordinates
    for (int i=0; i<sze;i++){ //Continue looping to the size of the array
        for (int k=0; k<sze; k++){

        }
        char cVle = shipC[i]; //Column value for array index 'i'
        char rVle = shipR[i]; //Row value for array index 'i'
        
        //Begin checking beginning coordinate with all other coordinates
        for (int j=0; j<sze; j++){ //Iterate through every coordinate to check for duplicate coordinates
            if (j != i){ //if
                    while (cVle == shipC[j] && rVle == shipR[j]){ //Continue looping until the coordinate of no longer matches that of all coordinates
                        shipC[i] = static_cast<char>((rand() % 10) + 97); //generate new column value 
                        shipR[i] = static_cast<char>((rand() % 10) + 48); //generate new row value
                    }       
            }
        }
    }
};


bool duplHit(char c1, char c2, char snkShps[][2], int rows){ //Returns true if the player coordinates match those of an already sunk ship. This prevents duplicate ship sunk counters
    
    for (int i=0; i<rows && snkShps[i][0] != 'z'; i++){ //iterate through each row and continue until number of rows has been exhausted OR the sentinel flag has been found
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
    totGams++; //increment
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
    snkArry[sze][2] = {}; //set all elements to empty values
    bool hit; //holds true if user has hit a ship
    hit = false; //assume user did not hit a ship
    for (int i=0; i<sze && !hit; i++){ //continue looping until user coordinate have been compared to all of the generated ship's coordinates (10) or user coordinate matches the coordinate of a ship
        if (c1 == shipClmn[i] && c2 == shipRow[i]){ //if user coordinate matches the coordinate a ship
            hit = true; //set hit status to true
        }
    }

    //begin filling in the coordinate of the sunk ship to the sunk ship array
    if (hit){ //if user coordinate has sunk a ship
        for (int i=0; i<sze; i++){ //
            for (int k=0; k<1; k++){
                if (snkArry[i][k] == '\0'){ //if the row is currently empty
                    snkArry[i][k] = c1; //place letter of user coordinate to the sunk ship array
                    snkArry[i][k+1] = c2; //place number of user coordinate to the sunk ship array
                }
            }
        }
    }
    return hit; //return hits status
}

void gmrslts(vector<string> &log, char sunk[][2], int wdth, int numSnk){
    string tempry; //temporarily holds shot log vector element during swap
    bool isMtch; //holds true if user coordinate matches the grid coordinate being checked
    string coord; //holds the coordinate
    string coord2; //holds the second coordinate of the grid coordinate
    int letter; //holds the ASCII value for lowercase letter
    int number; //holds the number of the coordinate
    isMtch = false; //assume user coordinate does not match grid coordinate being checked
    letter = 97; //start at lowercase 'a'
    number = 49; //start at 1 from ASCII value
    string grid[100]; //Holds the grid of the game
    coord = ""; //set to empty string
    for (int i=0; i<10; i++){
        cout << setw(wdth) << i+1;
    }
    cout << endl << "__________________________________________________________" << endl;//set top border


    //begin filling grid with coordinates
    for (int i=0;i<10; i++){
        coord = static_cast<char>(i+65); //get the letter of the coordinate
        for (int k=0; k<10; k++){
            coord += static_cast<char>(k+48); //get the coordinate number
            grid[(i*10) + k] = coord; //fill array index with coordinate
        }
    }

    //sort shot log by ascending order a-j
    string tempry;
    
    for (int i=0; i<log.size()-1; i++){
        for (int k=i+1; k<log.size(); k++){
            if (log[k][0] < log[i][0]){ //if the letter coordinate of the user 
                tempry = log[i];
                log[i] = log[k];
                log[k] = tempry;
            }
        }
    }
    //sort by ascending order 1-10
    for (int i=0; i<log.size()-1; i++){
        for (int k=i; k<log.size()-1; k++){
            if (log[k+1][1] < log[i][1] && log[k+1][0] == log[i][0]){
                tempry = log[i];
                log[i] = log[k+1];
                log[k+1] = tempry;
            }
        }
    }


    //begin comparing shot log to grid coordinate
    for (int l=letter; l<107; l++){
        coord = static_cast<char>(l); //get the letter of the coordinate
        cout << coord << " |"; //display grid letter and the border
        for (int k=number; k<=58; k++){

        }
        for (int i=0; i<10; i++){
            isMtch = false;
            coord += static_cast<char>(l);
            for (int k=0; k<10; k++){

            }
        }
        for (int i=0; i<100; i++){//continue looping until every grid coordinate has been checked for a mark
            isMtch = false; //reset match status value back to false
            coord += static_cast<char>(l); //get the letter of the coordinate
            for (int k=0; k<log.size() && !isMtch; k++){ //continue looping until every user coordinate has been compared to the grid coordinate being checked OR stop when user coordinate matches the grid coordinate
                if (log[k] == grid[i]){ //if user coordinate matches the current grid coordinate being checked ('i')
                    isMtch = true; //the user coordinate matches the grid coordinate, so set to true
                    coord = log[k]; //get user coordinate from shot log
                    if (duplHit(coord[0], coord[1], sunk, 10) != 0){ //input letter of user coordinate, coordinate number of user, sunk ships array, and number of ships sunk and check if user coordinate is a hit or a miss
                        cout << setw(wdth) << 'x'; //user sunk the ship at grid coordinate being checked, so display a hit mark
                    }
                    else{
                        cout << setw(wdth) << 'O'; //otherwise user coordinate missed at the grid coordinate being checked, so display a missed mark
                    }
                }
            }

            if (!isMtch){ //check bool flag to determine if all user coordinates did not match the grid coordinate
                cout << setw(wdth) << 'o';  //if so, then display no mark with lowercase 'o'
            }
        }
    }
}
//    ldrBord(accry, numGams, games); //sort accuracy data from highest to lowest
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

void repeat(){
    int atemps; //reset number of attempts back to zero
    int numSunk; //reset number of ships sunk back to zero
    char cShips[10]; //reset the coordinate number for all 10 ships back to nothing
    char rShips[10]; //reset the coordinate letter for all 10 ships back to nothing

}
