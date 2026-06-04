/*
    Author:  Ivan Ruiz
    Date:    4-24-2026
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
using namespace std;

//User Libraries

//Global Constants - Scientific/Physics/Chemistry

//Function Prototypes
void getCrds(char &cCrds, char &rCrds); //Gets coordinates from player
bool duplHit(char, char, char[], char[]); //Returns true if player coordinate matches that of an already sunk ship
bool chckBnd(char, char); //Retruns true if player coordinate is out bounds from the grid

//Execution beings here

int main(int argv, char **argc){
    //set the random number seed
    //'srand' function use satisfies the library requirement for <cstdlib> header for project 1
    srand(time(0)); // time function satisfies the use of <ctime> library //Get time in miliseconds
    //declare variables
    //This satisfies the named constant requirement for project 1

    //This satisfies the <fstream> library use requirement for project 1 by using data type 'ofstream' and 'ifstream'
    ofstream acrcyOF; //Writes accuracy data from each game to file named "Accuracy.txt"
    ifstream acrcyIF; //Reads data from "Accuracy.txt" to display at end of session to display accuracy percentage of user
    //This satisfies the float data type requirement for project 1
    float accrcy; //Holds the accuracy percentage of ships successfully destroyed out of number of attempts round((3.0f / atemps) * 100.0f) %

    //string for full coordinate of user and program
    //This satisfies the string requirement for project 1
    //string sunk; //Holds the coordinates of the sunk ships by the user. Used to verify the user does not use the same sunk coordinate multiple times to evade finding the other 2
    string rcShips;
    string rcShots; //Holds the history of coordinate strikes by the user
    char cShips[500]; //Column of the randomly generated ships
    char rShips[3];//Row of the randomly generated ships
    string crdntes; //Holds the coordinates of the user

    //Bool for if column and row of user corresponds to the randomly generated coordinates of the ship
    //This satisfies the bool requirement for project 1
    bool isColmn;  //Bool for if column and row of user corresponds to the randomly generated coordinates of the ship
    bool isRow;

    //User coordinates
    char uColumn; //The user coordinate for the column is a char represented by its ASCII decimal value
    char uRow; //Used to int but now is char data type

    //This satisfies int requirement for project 1
    int atemps; //7 rounds of fighting for the entire duration of the game
    char cntnue; //bool flag to continue after finished game
    int dstryd; //How many ships user has destroyed

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

    //begin the random coordinates for all 3 ships
    //'%' and '+' satsifies the arithmetic operators requirement for project 1
    ship1Cl = (rand() % 5) + 97; //get a random column character a-e for ship 1
    ship1Rw = (rand() % 5)+49;   //Get a random row integer 1-3 for ship 1
    isSunk1 = false; //Set the status of ship 1's sunk status to false

    ship2Cl = (rand() %5) + 97; //Get a random column character for ship 2
    ship2Rw = (rand() % 5) + 49; //Get a random row coordinate for ship 2
    isSunk2 = false; //Set the status of ship 2's sunk status to false

    while (ship2Cl == ship1Cl && ship2Rw == ship1Rw){  //Continue looping if ship2 column and row coordinates are the same as ship1
        ship2Cl = (rand() % 5) + 97; //Re-roll both coordinates for column and row
        ship2Rw = (rand() % 5) + 49;
    };

    ship3Cl = (rand() % 5) + 97; //Get a random column character for ship 3
    ship3Rw = (rand() % 5) + 49; //Get a random row character for ship 3
    isSunk3 = false; //Set the status of ship 3's sunk status to false

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
    rcShips +=   static_cast<char>(ship3Rw); //Convert the integer row coordinate from ship3 to char //Add the char row coordinate for ship3 to the string

    //cout << ship1Cl << ship1Rw << endl << endl << endl; //Debugging code

    //Display the game name and the number of attempts the user has
    //This satisfies the cout requirement for project1
    cout << "BATTLESHIP" << endl << "Find all 3 Enemy Ships" << endl; 

    //Display Outputs
    //This satisfies the do/while requirement for project 1
    do{
        //This satisfies the cout requirement for project 1
        cout << endl << endl << endl << "Input Column Coordinate From the Range of Letters [a-e]" << endl << "Input Row Coordinate From the Range [1-5]" << endl; 
        //This satisfies the multiple assignment requirement for project 1
        getline(cin, crdntes);
        uColumn = crdntes[0];
        uRow = crdntes[1];
                //cin >> uColumn >> uRow; //Get column coordinate as char and row coordinate as an integer
        //Begin checking if the coordinate macthes that of an already sunk ship
        //This satisfies the logical operators requirement for project 1


        while ((isSunk1 == true && (uColumn == ship1Cl && uRow == ship1Rw)) || (isSunk2 == true && (uColumn == ship2Cl && uRow == ship2Rw)) || (isSunk3 == true && (uColumn == ship3Cl && uRow == ship3Rw))){
            cout << uColumn << uRow << " Matches the Coordinate of an Already Sunk Ship; Pick another coordinate" << endl; //Prompt user to re-enter coordinate
                    //cin >> uColumn >> uRow; //Input of new coordinate
            getline(cin, crdntes); //Input new coordinates
            uColumn = crdntes[0]; //Only take the first index of the string as column coordinate
            uRow = crdntes[1];  //Only take the second index of the string as row coordinate
        }

        //Check if user input is within the bounds of the grid
        //This satisfies the User Validation requirement for project 1
        while (uColumn > 101 || uColumn < 97 || uRow > 53 || uRow < 49){ //Continue prompting user for coordinates if any of their coordinates is out of bounds
            cout << "Invalid Coordinate: " << uColumn << uRow << endl << "Pick a Column Coordinate From the Letters [a-e] And a Row Coordinate From Numbers [1-5]" << endl;
            //This satisfies the multiple assignment requirement for project 1
                    //cin >> uColumn >> uRow; //Enter column coordinate and row coordinate again
            getline(cin, crdntes); //Input new coordinates
            uColumn = crdntes[0]; //Only take the first index of the string as column coordinate
            uRow = crdntes[1];  //Only take the second index of the string as row coordinate
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
            dstryd += 1; //Increment number of destroyed ships by 1
            atemps += 1; //Increment number of attempts by 1 after successful engagement

        }
        else{ //Display victory output if user won and display attempt number if not
            cout << "Engagement at Coordinate " << uColumn << uRow << " : No Hit" << endl; //Display failed engagement at targeted coordinate
            //This satisfies the decrement/increment requirement for project 1
            atemps += 1; //Increment the number of attempts by 1 after failed engagement
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

void getCrds(char &cCrds, char &rCrds){
    string crds;
    cout << "Input Coordinates from columns [a-e] and rows [1-5]" << endl;
    getline(cin, crds);
    cCrds = crds[0];
    rCrds = crds[1];
};

bool duplHit(char c, char r, char cCrds[], char rCrds[]){
    for (int i=0; i<=cCrds[]; i++)
    
}; //Returns true if player coordinate matches that of an already sunk ship
