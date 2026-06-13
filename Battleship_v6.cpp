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
    //const int MAXCOLS = 2; //maximum amount of columns 
//Function Prototypes
void getCrds(char &cCrds, char &rCrds); //Gets coordinates from player
bool duplHit(char, char, char [][2],int sze); //Returns true if player coordinate matches that of an already sunk ship. Will also utilize to check user coordinate with the coordinates of the 10 ships
bool chckBnd(char, char); //Retruns true if player coordinate is out bounds from the grid
void filCrds(char[], char [], int); //Fill in the array with coordinates for the 10 ships
void shwGams(); //increments a static integer variable after each game
bool alrdSht(vector<string> &, char, char); //linear search to find any repeated shots in the shot log vector
void filLog(vector<string> &, char, char); //store user coordinate to shot log vector
bool filSunk(char, char, char [], char [], char [][2], int); //fills the sunk coordinate array if user has sunk a ship
void gmrslts(vector<string> &, int); //displays results after each game 

//Execution beings here

int main(int argv, char **argc){
    //set the random number seed
    //'srand' function use satisfies the library requirement for <cstdlib> header for project 1
    srand(time(0)); // time function satisfies the use of <ctime> library //Get time in miliseconds
    //declare variables
    const int MXCRDS=2; //Holds the maximum amount of columns for each coordinate
    const int MXSHPS = 10; //Holds the maximum amount of ships
    //This satisfies the <fstream> library use requirement for project 1 by using data type 'ofstream' and 'ifstream'
    ofstream acrcyOF; //Writes accuracy data from each game to file named "Accuracy.txt"
    ifstream acrcyIF; //Reads data from "Accuracy.txt" to display at end of session to display accuracy percentage of user
    //This satisfies the float data type requirement for project 1
    float accrcy; //Holds the accuracy percentage of ships successfully destroyed out of number of attempts round((3.0f / atemps) * 100.0f) %

    int numSunk; //holds the number of ships sunk
    //string for full coordinate of user and program
    //This satisfies the string requirement for project 1
    //string sunk; //Holds the coordinates of the sunk ships by the user. Used to verify the user does not use the same sunk coordinate multiple times to evade finding the other 2
    string rcShips;
    string rcShots; //Holds the history of coordinate strikes by the user

    //Note: You can convert a ASCII character that represents a number to a integer data type AND then subtracting that by 48 to get the ACTUAL number. So, you do not have to use - '0' like they said in lab
    //Holds the coordinate of the sunk ships as a 2d-array. Row represents each a sunk ship, column represents each coordinate
    char sunkShp[MXSHPS][2]; 
    //Parallel array type-shi
    char cShips[10]; //Holds the column values of the 10 randomly generated ships
    char rShips[10]; //Holds the row values of the 10 randomly generated ships
    string crdntes; //Holds the coordinates of the user


    //useful variables
        //User coordinates
    char uColumn; //The user coordinate for the column is a char represented by its ASCII decimal value
    char uRow; //Used to int but now is char data type
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
    dstryd = 0; //Set number of ships destroyed to 0
    atemps = 0; //How many attempts the user has done to find enemy ships
    accrcy = 0.0f; //Accuracy is calculated by dividing the number of the successful ships sunk by the total number of attemps used
    acrcyOF.open("Accuracy.txt"); //Create and open file to write accuracy data to text file
    inBnds = false; //assume user types in coordinate out of bounds of the grid, so set to false
    isSunk = true; //assume user typed in a coordinate of a sunk ship
    isRptd = true; //assume user typed in a coordinate they had previously typed in

    //begin the random coordinates for all 3 ships
    //'%' and '+' satsifies the arithmetic operators requirement for project 1
    ship1Cl = (rand() % 5) + 97; //get a random column character a-e for ship 1
    ship1Rw = (rand() % 5)+49;   //Get a random row integer 1-3 for ship 1
    isSunk1 = false; //Set the status of ship 1's sunk status to false

    ship2Cl = (rand() %5) + 97; //Get a random column character for ship 2
    ship2Rw = (rand() % 5) + 49; //Get a random row coordinate for ship 2
    isSunk2 = false; //Set the status of ship 2's sunk status to false


    //Display the game name and the number of attempts the user has
    //This satisfies the cout requirement for project1
    cout << "BATTLESHIP" << endl << "Find all 10 Enemy Ships" << endl; 

    //Display Outputs
    //This satisfies the do/while requirement for project 1
    do{
        //This satisfies the cout requirement for project 1
        cout << endl << endl << endl << "Input Column Coordinate From the Range of Letters [a-e]" << endl << "Input Row Coordinate From the Range [1-5]" << endl; 
        //This satisfies the multiple assignment requirement for project 1
        getCrds(uColumn, uRow);

        //verify user coordinate is within the bounds of the grid
        inBnds = chckBnd(uColumn, uRow); //holds whether the user coordinate is within the bounds of the grid
        isSunk = duplHit(uColumn, uRow, sunkShp[][2], numSunk); //returns true if user coordinate matches the coordinate of a sunk ship
        isRptd = alrdSht(shotLog, uColumn, uRow); //returns true if user typed in a coordinate previously inputted

        //begin user coordinate validation
        while (!inBnds || isSunk || isRptd){ //while user coordinate is not within the bounds of the grid OR user coordinate matches that of an already sunk ship OR user typed in a previously typed in coordinate
            if (!inBnds){ //if user is out of bounds of the grid
                cout << "Coordinate " << '[' << uColumn << ']' << '[' << uRow << ']' << " is Out Of Bounds" << endl << "Enter Coordinate Again" << endl; //display out of bounds error message
                getCrds(uColumn, uRow); //get coordinates
                inBnds = chckBnd(uColumn, uRow); //returns true if user is within bounds and false if not
            }
            if (isRptd){ //if user typed in a coordinate they had previously typed in
                if (isSunk){ //check if that previously typed coordinate is that of a sunk ship
                    cout << "Coordinate " << '[' << uColumn << ']' << '[' << uRow << ']' << " Is a Coordinate Of a Sunk Ship" << endl << "Enter Coordinate Again" << endl; //display already sunk ship error message
                    getCrds(uColumn, uRow); //get coordinates 
                    isSunk = duplHit(uColumn, uRow, sunkShp[][2], numSunk); //returns true if user coordinate matches the coordinate of a sunk ship
                }
                else{ //otherwise user typed in a normal coordinate that had been previously typed in, so
                    cout << "Coordinate " << '[' << uColumn << ']' << '[' << uRow << ']' << " has already been typed in" << "Enter Coordinate Again" << endl; //display repeated short error message
                    getCrds(uColumn, uRow); //get coordinates
                    isRptd = alrdSht(shotLog, uColumn, uRow); //returns true if user typed in a coordinate already inputted
                }
            }
        }
        //add valid user coordinate to the shot log vector
        filLog(shotLog, uColumn, uRow); //fill shot log

        //begin checking if the user has hit a ship
        if (filSunk(uColumn, uRow, rShips, cShips, sunkShp[][2], 10) != 0){ //if user has hit a ship
            cout << "Engagement at Coordinate " << '[' << uColumn << ']' << '[' << uRow << ']' << " Aftermath : Hit " << endl; //display successful engagement
        }
        else{
            cout << "Engagement at Coordinate " << '[' << uColumn << ']' << '[' << uRow << ']' << " Aftermath : Miss" << endl; //display unsuccessful engagement
        }

        //Begin display of progress after each round
        //Display previous engaged coordinates, Display shots the user has shot, Display the number of ships the user has destroyed
        dstryd == 3? cout << "All Ships Destroyed" << endl << "You Win" << endl : cout << "Coordinate Engagements History: [" << rcShots << "]" << endl << "Shots: " << atemps << endl;
        switch(dstryd){
            case 1:
            cout << "You have sunk 1 ship";
            break;
            case 2:
            cout << "You have sunk 2 ships";
            break;
            case 3:
            cout << "You have sunk all 3 ships!" << endl;
            break;
            default:
            cout << "No ships sunk";
            break;
        }

    }while(dstryd != 3); //Continue prompting user for coordinates until all ships have been destroyed

    //Begin writing accuracy data to "Accuracy.txt"
    accrcy = round((3.0f / atemps) * 100.0f);
    acrcyOF << fixed << setprecision(2) << accrcy << " " << atemps << " " << rcShips << endl;

    cout << "Play again (y/n)" << endl; //Prompt user whether to start a new game after previous game has ended
    //This satisfies the flag requirement for project 1
    cin >> cntnue; //Input y flag for yes or n flag for no

    if (cntnue == 'n' || cntnue == 'N'){
        acrcyOF.close();
        acrcyIF.open("Accuracy.txt");
        for (int i=1; acrcyIF>>accrcy>>atemps>>rcShips; i++){
            cout << endl << endl << endl << "Results" << endl << "------------------------------" << endl; //Display the header for the game
            cout << fixed << setprecision(2) << "Game " << i << " Accuracy: " << accrcy << "%" << endl; //Display the accuracy of the player
            cout << "Game "<< i << " Total attempts: " << atemps << endl; //Display total attempts by the user to destroy ships
            cout << "Enemy Ship locations: [" << rcShips << ']'; //Display the location of the randomly generated ships
        }
        acrcyIF.close(); //Close "Accuracy.txt" file
    }

    //If 'cntnue' flag is y or Y for yes then start a new game
    while (cntnue == 'y' || cntnue == 'Y'){
        cout << endl << endl << endl << endl << endl; //Make space for new game

        //Begin setting all necessary variables back to normal
        atemps = 0; //set number of attempts variable back to 0
        rcShots = ""; //Clear the engagements history
        rcShips = ""; //Clear the locations of previous 3 ships coordinates
        dstryd = 0; //Reset the number of ships destroyed
       
        //Begin Randomly generating all 3 ships coordinates again
        ship1Cl = (rand() % 5) + 97; //get a random column character a-e for ship 1
        ship1Rw = (rand() % 5)+49;   //Get a random row integer 1-3 for ship 1
        isSunk1 = false; //Reset ship 1's sunk status back to false

        ship2Cl = (rand() %5) + 97; //Get a random column character for ship 2
        ship2Rw = (rand() % 5) + 49; //Get a random row coordinate for ship 2
        isSunk2 = false;    //Set ship 2's sunk status back to false

        while (ship2Cl == ship1Cl && ship2Rw == ship1Rw){  //Continue looping if ship2 column and row coordinates are the same as ship1
            ship2Cl = (rand() % 5) + 97; //Re-roll both coordinates for column and row
            ship2Rw = (rand() % 5) + 49;
        };

        ship3Cl = (rand() % 5) + 97; //Get a random column character for ship 3
        ship3Rw = (rand() % 5) + 49; //Get a random row character for ship 3
        isSunk3 = false; //reset ship 3's sunk status to false

        //Continue generating random coordinates for ship3 if it matches both the first or second ships coordinates
        while ((ship3Cl == ship2Cl && ship3Rw == ship2Rw) || (ship3Cl == ship1Cl && ship3Rw == ship1Rw)){
            ship3Cl = (rand() % 5) + 97; //Re-roll column coordinate from the range a-e in ascii decimal value
            ship3Rw = (rand() % 5) + 49; //re-roll row coordinate from the range 1-5 in ascii decimal value
        };

        //beginning of adding all ships coordinates to the string of rcShips to display at the end of the game
        rcShips += ship1Cl; //Add ship1's column coordinate to the string
        //This satifies the type-casting requirement for project 1
        rcShips += static_cast<char>(ship1Rw); //Convert integer to char //Add the now converted char coordinate of ship1's row to the string
        rcShips += ","; //Add comma and space to prepare for the next ships coordinate

        rcShips += ship2Cl; //Add ship2's column coordinate to the string
        //Satisfies the type-casting requirement for project 1
        rcShips += static_cast<char>(ship2Rw); //Convert the integer of ship2's row coordinate to a char value //Add the char row coordinate for ship2 to the string
        rcShips += ","; //Prepare for the final ships coordinate

        rcShips += ship3Cl; //Add the column coordinate for ship3
        //This satisfies the tyep-casting requirement for project 1
        rcShips += static_cast<char>(ship3Rw); //Convert the integer row coordinate from ship3 to char //Add the char row coordinate for ship3 to the string

        //This satisfies the do/while requirement for project 1
        do{
            //This satisfies the cout requirement for project 1
            cout << endl << endl << endl << "Input Column Coordinate From the Range of Letters [a-e]" << endl << "Input Row Coordinate From the Range [1-5]" << endl; 
            //This satisfies the multiple assignment requirement for project 1
                    //cin >> uColumn >> uRow; //Get column coordinate as char and row coordinate as an integer
            getline(cin, crdntes);
            uColumn = crdntes[0];
            uRow = crdntes[1];

            //Begin checking if the coordinate macthes that of an already sunk ship
            //This satisfies the logical operators requirement for project 1
            while ((isSunk1 == true && (uColumn == ship1Cl && uRow == ship1Rw)) || (isSunk2 == true && (uColumn == ship2Cl && uRow == ship2Rw)) || (isSunk3 == true && (uColumn == ship3Cl && uRow == ship3Rw))){
                cout << uColumn << uRow << " Matches the Coordinate of an Already Sunk Ship; Pick another coordinate" << endl; //Prompt user to re-enter coordinate
                        //cin >> uColumn >> uRow; //Input of new coordinate
                getline(cin, crdntes);
                uColumn = crdntes[0];
                uRow = crdntes[1];
            }
        
            //Check if user input is within the bounds of the grid
            //This satisfies the User Validation requirement for project 1
            while (uColumn > 101 || uColumn < 97 || uRow > 53 || uRow < 49){ //Continue prompting user for coordinates if any of their coordinates is out of bounds
                cout << "Invalid Coordinate: " << uColumn << uRow << endl << "Pick a Column Coordinate From the Letters [a-e] And a Row Coordinate From Numbers [1-5]" << endl;
                //This satisfies the multiple assignment requirement for project 1
                        //cin >> uColumn >> uRow; //Enter column coordinate and row coordinate again
                getline(cin, crdntes);
                uColumn = crdntes[0];
                uRow = crdntes[1];
                cout << endl << endl; //Prep for results after valid coordinates have been targeted and engaged
            }

            //combine the column character and row number of the inputted coordinate to form the full coordinates and store in string as history of user engaged coordinates
            rcShots += uColumn; //Add current column coordinate to the history of engaged coordinates by the user
            rcShots += uRow;    //Add current row coordinate to the history of engaged coordinates by the user
            rcShots += ", ";    //Prep for the next engaged coordinate

            //This satisfies the if/else if/else requirement for project 1
            //Begin enaging the user targeted coordinates to the coordinates of all 3 ships
            if (uRow == ship1Rw && uColumn == ship1Cl){ //If the row and column coordinates match that of the coordinates of ship 1 then set isRow and isColmn bool to true
                isRow = true; //Confirm the existance of ship1's row coordinate
                isColmn = true; //Confirm the existance of ship1's column coordinate
            }
            else if(uRow == ship2Rw && uColumn == ship2Cl){ //Else check if the targeted coordinates matches the coordinates of ship 2's coordinates
                isRow = true; //Confirm the existance of ship2's row coordinate
                isColmn = true; //Confirm the existance of ship2's column coordinate
            }
            else if(uRow == ship3Rw && uColumn == ship3Cl){ //Else check if the targeted coordinates matches the coordinates of ship 3's coordinates
                isRow = true; //Confirm the existance of ship 3's row coordinate
                isColmn = true; //Confirm the existance of ship 3's column coordinate
            }
            else{ //Otherwise the engaged coordinate did not contain a ship
                isRow = false; //Confirm the non-existance of all 3 ships in this specific engaged coordinate inputted by the user
                isColmn = false; //Confirm the non-existance of all 3 ships in this specific engaged coordinate inputted by the user
            }

            //This satisfies the nesting requirement for project 1
            if (isRow && isColmn){ //Output successful engagement by the user
                cout << "Engagement at Coordinate " << uColumn << uRow << " : Hit" << endl; //Display succesful engagement
                //Begin checking which ship has been destroyed
                if ((uColumn == ship1Cl) && (uRow == ship1Rw)){
                    isSunk1 = true;
                }
                if ((uColumn == ship2Cl) && (uRow == ship2Rw)){
                    isSunk2 = true;
                }
                if ((uColumn == ship3Cl) && (uRow == ship3Rw)){
                    isSunk3 = true;
                }

                //This satisfies the increment/decrement requirement for project 1
                dstryd += 1; //Increment number of destroyed ships by 1 after successful engagement
                atemps += 1; //Increment number of attempts by 1

            }
            else{ //Otherwise display no hit
                cout << "Engagement at Coordinate " << uColumn << uRow << " : No Hit" << endl; //Display failed engagement at targeted coordinate
                //This satisfies the decrement/increment requirement for project 1
                atemps += 1; //Increment the number of attempts by 1 after failed engagement
            }

            //Begin display of progress after each round
            //Begin display of progress after each round
            //Display previous engaged coordinates, Display shots the user has shot, Display the number of ships the user has destroyed
            dstryd == 10? cout << "All Ships Destroyed" << endl << "You Win" << endl : cout << "Coordinate Engagements History: [" << rcShots << "]" << endl << "Shots: " << atemps << endl;
            switch(dstryd){
                case 0:
                cout << "No Ships Sunk" << endl;
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
                cout << dstryd << " Ships Sunk" << endl;
                break;
            }

        }while(dstryd != 10); //Continue prompting user for coordinates until all ships have been destroyed

        
        //Begin writing accuracy data to "Accuracy.txt"
        accrcy = round((3.0f / atemps) * 100.0f);
        acrcyOF << fixed << setprecision(2) << accrcy << " " << atemps << " " << rcShips << endl;
        
        cout << "Play Again (y/n)" << endl; //Prompt to restart game or not
        cin >> cntnue; //Store flag in continue variable

        if (cntnue == 'n' || cntnue == 'N'){
            acrcyOF.close();
            acrcyIF.open("Accuracy.txt");
            for (int i=1; acrcyIF>>accrcy>>atemps>>rcShips; i++){
                cout << endl << endl << endl << "Results" << endl << "------------------------------" << endl; //Display the header for the game
                cout << fixed << setprecision(2) << "Game " << i << " Accuracy: " << accrcy << "%" << endl; //Display the accuracy of the player
                cout << "Game "<< i << " Total attempts: " << atemps << endl; //Display total attempts by the user to destroy ships
                cout << "Enemy Ship locations: [" << rcShips << ']'; //Display the location of the randomly generated ships


            }
            acrcyIF.close(); //Close "Accuracy.txt" file
        }

    };
    //Exit Stage Right
    return 0;
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

    if (crds[1] == 1 && crds[2] == 0){ //If both chars combined are 10 then assume the user typed in 10
        cCrds = '0'; //10 will be represented as 0
    }
    cCrds = crds[0];
    rCrds = crds[1];
};

//default parameter for filling coordinates
void filCrds(char shipC[], char shipR[], int sze=10){ //takes in the column and row array of the ships and the number of ships to generate
    //First fill both arrays with coordinate values
    for (int i=0; i<sze; i++){
        shipC[i] = static_cast<char>((rand() % 10) + 97); //Randomly generate an integer and convert it into a char for the column value
        shipR[i] = static_cast<char>((rand() % 10) + 48); //Randomly generate an integer and convert it into a char for the row value
    }
    //Check for duplicate coordinates
    for (int i=0; i<sze;i++){ //Continue looping to the size of the array
        char cVle = shipC[i]; //Column value for array index 'i'
        char rVle = shipR[i]; //Row value for array index 'i'
        
        //Begin checking beginning coordinate with all other coordinates
        for (int j=i; j<sze; j++){ //Iterate through every coordinate to check for duplicate coordinates
            while (cVle == shipC[j] && rVle == shipR[j]){ //Continue looping until the coordinate of no longer matches that of all coordinates
                    shipC[i] = static_cast<char>((rand() % 10) + 97); //generate new column value 
                    shipR[i] = static_cast<char>((rand() % 10) + 48); //generate new row value
            }
        }
    }
};


bool duplHit(char c1, char c2, char snkShps[][2], int rows){ //Returns true if the player coordinates match those of an already sunk ship. This prevents duplicate ship sunk counters
    for (int i=0; i<rows; i++){ //iterate through each row
        //check if user coordinate matches those of an already sunk ship
        for (int k=0; k<2-1; k++){
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

void shwGams(){ //increment after user types 'y' or 'Y' to play another game
    static int totGams = 0; //initialize total number of games played
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
    bool hit; //holds true if user has hit a ship
    hit = false; //assume user did not hit a ship
    for (int i=0; i<sze; i++){
        if (c1 == shipRow[i] && c2 == shipClmn[i]){ //if user coordinate matches the coordinate a ship
            hit = true; //set hit status to true
        }
    }

    if (hit){
        for (int i=0; i<sze; i++){
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

// void gmrslts(vector<string> &log, char sunk[][2], int wdth, int numSnk){
//     int index; //holds the row index for the sunk ship array
//     int number; //holds the column index of the grid
//     int letter; //holds the ASCII value for letter
//     string coord; //holds the coordinate the user typed in
//     index = 0; //initialize
//     number = 0; //start off with the first column of the grid of the game //to display the actual number just do number + 1
//     letter = 97; //start off with lowercase 'a'
//     cout << setw(wdth) << "Coordinate Engagements" << endl; //Display header
//     //display the numbers of the grid
//     for (int i=0; i<10; i++){
//         cout << setw(wdth) << i;
//     }
//     cout << endl; //newline for border
//     //display border
//     for (int i=0; i<10; i++){
//         cout << setw(wdth*2) << '_';
//     }
//     cout << endl; //prepare for letter and mark rows
//     for (int i=0; i<10; i++){ //continue looping until 10 rows have been displayed
//         cout << static_cast<char>(letter) << '|'; //convert integer to char to display it's letter and then display border
//         for (int k=0; k<10; k++){ //continue looping until 10 columns have been displayed
//             for (int l=0; l<log.size(); l++){ //continue looping until every user-shot coordinate has been compared to every sunk ship coordinate
//                 coord = log[l]; //get the first shot string from the shot log
//                 //continue looping until user coordinate is compared to every coordinate of a sunk ship
//                 for (int m=0; m<numSnk; m++){ //iterate through each ship
//                     for (int b=0; b<2; b++){ //iterate thorugh every column of the ship
//                         if (coord[b] == sunk[m][b] && coord[b+1] == sunk[m][b+1]){ //compare letter of coordinate and then compare the number of the coordinate
//                         }
//                     }
//                 }
//             }
//         }
//     }
// }
//tread carefully when reading this function

void gmrslts(vector<string> &log, char sunk[][2], int wdth, int numSnk){
    bool isMtch; //holds true if user coordinate matches the grid coordinate being checked
    string coord; //holds the coordinate
    int letter; //holds the ASCII value for lowercase letter
    int number; //holds the number of the coordinate
    isMtch = false; //assume user coordinate does not match grid coordinate being checked
    letter = 97; //start at lowercase 'a'
    number = 49; //start at 1 from ASCII value
    string grid[100]; //Holds the grid of the game
    //begin filling grid with coordinates
    for (int i=0;i<100; i++){
        coord = static_cast<char>(letter); //get the letter of the coordinate
        coord += static_cast<char>(number); //get the coordinate number
        grid[i] = coord; //fill array index with coordinate
    }

    //begin comparing shot log to grid coordinate
    for (int i=0; i<100; i++){//continue looping until every grid coordinate has been checked for a mark
        for (int k=0; k<log.size() || !isMtch; k++){ //continue looping until every user coordinate has been compared to the grid coordinate being checked OR stop when user coordinate matches the grid coordinate
            if (log[k] == grid[i]){ //if user coordinate matches the current grid coordinate being checked ('i')
                isMtch = true; //the user coordinate matches the grid coordinate, so set to true
                coord = log[k]; //get user coordinate from shot log
                if (duplHit(coord[0], coord[1], sunk[][2], 10) != 0){ //input letter of user coordinate, coordinate number of user, sunk ships array, and number of ships sunk and check if user coordinate is a hit or a miss
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
};



