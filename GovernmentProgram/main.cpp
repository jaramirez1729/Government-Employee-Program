/*
 * By: Jose Adan Ramirez
 * Created on: 9/29/15
 * Originally created in Xcode
 * Tested on Code::Blocks for Mac
 *
 * Purpose: The program will illustrate structures control and
 *          binary file control. Required to get familiar working
 *          with the fstream class.
 *
 * Implementation: The entire data of the citizens are held in binary files.
 *                 When the program starts, the function populateIndex will read
 *                 the contents of the file and add the indexes and SSN into it.
 *                 This way, the index is kept in memory, and data kept in binary.
 *                 This is because without reading the file, the index array is empty
 *                 when the program first starts.
 */

#include <iostream>
#include <fstream>      //Required for file input and output
#include <string>       //Required for string type use
#include <limits>       //Required for numeric limits in cin function
using namespace std;


//Contant Variables for structure array controls for max size
const int NAME_SIZE = 30, ADDR_SIZE = 30, ARR_SIZE = 10;


//Global Structure to Allow Type Access for Functions
struct Address {
    //Address Structure Directly Related to the Record Struct Only
    //Created for struct practice
    char city[ADDR_SIZE];
    char state[ADDR_SIZE];
    int zipcode;
};
struct Record {
    //This struct will hold the data for each citizen
    char name[NAME_SIZE];
    int socialSecurityNum;
    int dateOfBirth;
    Address address;

    //Resets the values saved in the struct
    void reset() {
        memset(name, 0, sizeof(name));
        socialSecurityNum = 0;
        dateOfBirth = 0;
        memset(address.city, 0, sizeof(address.city));
        memset(address.state, 0, sizeof(address.state));
        address.zipcode = 0;
    }
};
struct IndexNode {
    //Used to hold an index array that corresponds to the citizen record
    //Using this method, the SSN will always correspond to the correct
    //index node or position
    int socialSecurityNum;
    int locationInNode;
};



//////////////////////////////////////FUNCTION PROTOTYPES BEGIN HERE/////////////////////////////////////
int didFileFail(fstream &, string);                             //Checks if the file failed... duh
long getBytePosition(int);                                      //Get binary position for random access
void insertNewNode(int position, IndexNode[], Record);          //Adds new index and SSN to node
int populateIndex(fstream &, string, IndexNode[], Record);      //Adds index/node data from the file

    //Important functions used for the menu for the user (self explainatory)
int addRecord(fstream &, string, Record, IndexNode[]);                      //Menu Choice 1
int editRecord(fstream &, string , Record, IndexNode[]);                    //menu Choice 2
int displayRecord(fstream &, string , Record, IndexNode[]);                 //Menu Choice 3
int displayAllRecords(fstream &, string , Record, IndexNode[]);             //menu Choice 4
int deleteRecord(fstream &, string , Record, IndexNode[], Record[]);        //Menu Choice 5
//////////////////////////////////////FUNCTION PROTOTYPES END HERE//////////////////////////////////////



//Program begins here for the user
int main() {


    //Start of the Variables Begins Here
    int menuChoice = 0;
    Record citizen;
    Record tempArray[ARR_SIZE];
    IndexNode indexLocation[ARR_SIZE];
    for (int i = 0; i < ARR_SIZE; i++) {
        indexLocation[i].locationInNode = -1;
    }
        //Binary file control variables
    string fileName = "CitizenDatabase.dat";
    fstream dataFile;


    //For testing purposes only when trying to clear the file
    //dataFile.open(fileName, ios::out | ios::binary); dataFile.close();

    
    //Reads the binary file to populate memory index and SSNs
    populateIndex(dataFile, fileName, indexLocation, citizen);


    //******************************* PROGRAM BEGINS HERE *******************************//
    //User Introduction
    cout << "\t\t\tUS CITIZEN'S INFORMATION DATABASE\n\n";
    cout << "Welcome to the US Government's Citizen Database! This database\n";
    cout << "can only hold a maximum of " << ARR_SIZE << " citizen records. Please ensure\n";
    cout << "proper use and control of this database.\n\n";


        //Menu will continously display until the user exits
    do {
        cout << "\t1) Add a new citizen to the database\n";
        cout << "\t2) Update an existing citizen's record\n";
        cout << "\t3) Display a citizen's record\n";
        cout << "\t4) Display all citizen's information\n";
        cout << "\t5) Delete a citizen's record (CANNOT BE UNDONE)\n";
        cout << "\t6) Exit the database\n\n";
        cout << "Chose an item menu by entering any number from 1 to 6: ";
        //Makes sure the user enters a correct choice for the menu item
        do {
            cin >> menuChoice;
            if (cin.fail() || menuChoice < 1 || menuChoice > 6) {
                //Required in case the user enters a letter (crash)
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                cout << "Invalid input; please enter a number from 1 to 6: ";
            }
        } while (cin.fail() || menuChoice < 1 || menuChoice > 6);


        cout << endl;
        //Switch statement to perform the indicated action depending on the user's choice
        switch (menuChoice) {
            case 1: addRecord(dataFile, fileName, citizen, indexLocation);
                break;
            case 2: editRecord(dataFile, fileName, citizen, indexLocation);
                break;
            case 3: displayRecord(dataFile, fileName, citizen, indexLocation);
                break;
            case 4: displayAllRecords(dataFile, fileName, citizen, indexLocation);
                break;
            case 5: deleteRecord(dataFile, fileName, citizen, indexLocation, tempArray);
                break;
            case 6:
                cout << "\nThe database will now close and the program will exit.\n";
                break;
        }
    } while (menuChoice != 6);  //Will exit if a 6 is entered, otherwise keep repeating the menu

    //******************************** PROGRAM ENDS HERE *******************************//
    cout << "\n\n";
    system("pause");    //REQUIRED FOR WINDOWS/CODE::BLOCK; ERROR IN XCODE NONE FOUND
    return 0;
}





//Error to display if the file fails to open DONE
int didFileFail(fstream &dataFile, string fileName) {

    string indent = "\t\t\t\t";
    cout << "\n\n";
    cout << "\t\t******************************************\n";
    cout << "\t\t*  Failed to open \"" << fileName << "\". *\n";
    cout << "\t\t*\t\t\t  Error Status:" << indent << "*\n";
    cout << "\t\t*\t\t\tFail Bit Flag: " << dataFile.fail() << indent << "*\n";
    cout << "\t\t*\t\t\tBad Bit Flag: " << dataFile.bad() << indent << "*\n";
    cout << "\t\t******************************************\n\n\n";

    return 0;
}

//Returns the value of the byte position to get the random access location in the binary file DONE
long getBytePosition(int bytePosition) {
    return sizeof(Record) * bytePosition;
}

//Used to populate the index with a new position and SSN DONE
void insertNewNode(int position, IndexNode indexLocation[], Record citizen) {
    indexLocation[position].socialSecurityNum = citizen.socialSecurityNum;
    indexLocation[position].locationInNode = position;
}

//!! Populates the array for use by reading data that exists from the file and uses the indexes/nodes later
int populateIndex(fstream &dataFile, string fileName, IndexNode indexLocation[], Record citizen) {

    //!! DO NOT CHANGE THIS FUNCTION: ANY CHANGES WILL BREAK THE PROGRAM. THE FUNCTION FILLS THE
    //!! INDEXES IN MEMORY BY READING FROM THE FILE WHERE DATA IS SAVED AND ALREADY EXISTS. IF THE
    //!! FUNCTION FAILS TO POPULATE THE INDEX CORRECTLY, THEN INDEXES WILL BE OFF IN THE PROGRAM.

    //variable Control
    int position = 0;
    dataFile.open(fileName, ios::in | ios::binary | ios::beg);

    if (!dataFile.fail()) {

        while (1) {

            //Continues as long as it's not the end of the file
            if (!dataFile.eof() && dataFile.peek() != EOF) {

                //Gets the location by random access of the file
                dataFile.seekg(getBytePosition(position), ios::beg);
                dataFile.read(reinterpret_cast<char *>(&citizen), sizeof(citizen));

                //This will add preexisting data from a file to the index nodes
                //as long as the position is not the end of the file. This prevents
                //the program from adding an empty set after the last byte

                insertNewNode(position, indexLocation, citizen);
                //cout << "At position " << position << ": " << indexLocation[position].locationInNode << " was added.\n";
                //cout << "At position " << position << ": " << citizen.socialSecurityNum << " was added.\n";

                position++;
                
            } else
                break;
        }

        dataFile.clear();
        dataFile.close();

    } else {
        didFileFail(dataFile, fileName);
        return 1;
    }

    return 0;
}





//Menu Choice 1: Adding a record function that increments a record into the file DONE
int addRecord(fstream &dataFile, string fileName, Record citizen, IndexNode indexLocation[]) {

    cout << "\t\t\tAdding a New Citizen Record\n\n";

    //Variable Control
    bool testLoop = true;
    int indexPosition = 0;
    int totalRecords = 0;
        //Opens the file in binary mode and adds new data to the end of the file
    dataFile.open(fileName, ios::in | ios::out | ios::binary | ios::beg);

    if (!dataFile.fail()) {


        //Gets the size of the database by checking if the index is empty for
        //that position. It will then assign the position that is empty to
        //the indexPosition to store the data in that position
        for (int position = 0; position < ARR_SIZE; position++) {
            if (indexLocation[position].locationInNode != -1) {
                totalRecords++;
            } else {
                indexPosition = position;
                break;
            }
        }
        //cout << "Data will be added at position(byte) " << indexPosition << endl;

        cout << "There are a total of " << totalRecords << " records. Max is "<< ARR_SIZE << " records.\n\n";
        if (totalRecords != ARR_SIZE) {
            cout << "Please only enter correct answers.\n\n";
            cin.ignore();


            cout << "What is the citizen's name? ";
            cin.getline(citizen.name, NAME_SIZE);
            cin.clear();


            //Makes sure the user enters a correct social security number
            while (testLoop) {
                cout << "What is the social security number (9 digits)? ";
                cin >> citizen.socialSecurityNum;

                //Returns an error if the user fails to enter an acceptable answer
                if (cin.fail() || citizen.socialSecurityNum > 999999999 || citizen.socialSecurityNum < 100000000) {

                    cin.clear();
                    //Ignores if the user enters any letters
                    cin.ignore(numeric_limits<streamsize>::max(), '\n');
                    cout << "Invalid input; must be between 100000000 - 999999999.\n";

                } else {

                    //Checks if the social security number already exists or not
                    for (int i = 0; i <= totalRecords; i++) {

                        //Compares the SSN the user put to every SSN that is in the database
                        if (indexLocation[i].socialSecurityNum == citizen.socialSecurityNum) {
                            cout << "That social security number already exists.\n";
                            testLoop = true;
                            break;

                        } else if (i == totalRecords) {
                            //Will only reach this if the SSN did not already exist
                            testLoop = false;
                        }
                    }
                }
            }


            //Makes sure the user enters a correct date of birth
            while (1) {
                cout << "What is the birth date (MMDDYY)? ";
                cin >> citizen.dateOfBirth;
                if (cin.fail() || citizen.dateOfBirth < 10100 || citizen.dateOfBirth > 123115) {
                    cin.clear();
                    //Ignores if the user enters any letters
                    cin.ignore(numeric_limits<streamsize>::max(), '\n');
                    cout << "Invalid date of birth or future/past date.\n";
                } else
                    break;
            }
            cin.ignore();


            cout << "What city are they currently residing? ";
            cin.getline(citizen.address.city, ADDR_SIZE);


            cout << "What state are they currently residing? ";
            cin.getline(citizen.address.state, ADDR_SIZE);


            while (1) {
                cout << "What is their current zipcode (5 numbers)? ";
                cin >> citizen.address.zipcode;
                if (cin.fail() || citizen.address.zipcode < 10000 || citizen.address.zipcode > 99999) {
                    cin.clear();
                    //Ignores if the user enters any letters
                    cin.ignore(numeric_limits<streamsize>::max(), '\n');
                    cout << "Invalid zipcode; must be 5 numbers only.\n";
                } else
                    break;
            }
            cin.ignore();

            //Adds new node and indexes into the index array and SSN
            insertNewNode(indexPosition, indexLocation, citizen);

            //End of adding a new citizen
            cout << "\nThis citizen is record: #" << (indexLocation[indexPosition].locationInNode + 1) << " with SSN: " << indexLocation[indexPosition].socialSecurityNum << ".\n";
            cout << "A new citizen has been added to the database.\n\n";
            dataFile.seekp(getBytePosition(indexLocation[indexPosition].locationInNode), ios::beg);
            dataFile.write(reinterpret_cast<char *>(&citizen), sizeof(citizen));

        } else {
            cout << "\nSorry, but the database is full.\n";
            cout << "You will now be redirected to the main menu.\n\n";
            dataFile.close();
            return 0;
        }

        dataFile.clear();
        dataFile.close();

    } else {
        //File failed to open
        didFileFail(dataFile, fileName);
        return 1;
    }
    return 0;
}





//menu Choice 2: Able to change the data of any citizen and either save or exit without saving changes DONE
int editRecord(fstream &dataFile, string fileName, Record citizen, IndexNode indexLocation[]) {

    cout << "\t\t\tEditing a New Citizen Record\n\n";

    if (indexLocation[0].locationInNode == -1) {
        cout << "The database is empty.\n\n";
        return 0;
    }

    //Variable Control
    int menuChoice = 0;
    bool testLoop = true;
    int totalRecords = 0;
    int indexPosition = 0;
    int testSocialSecNum = 0;
    dataFile.open(fileName, ios::in | ios::out | ios::binary | ios::beg);


    if (!dataFile.fail()) {

        //Get the size of the array by reading from the indexes filled
        //This size is then used to search for the SSN next
        for (int position = 0; position < ARR_SIZE; position++) {
            if (indexLocation[position].locationInNode != -1) {
                totalRecords++;
            }
        }
        //Make sure it's counting the size correct
        cout << "The current size of the array is " << totalRecords << endl;



        while (testLoop) {

            cout << "Enter the SSN of the citizen to edit: ";
            cin >> testSocialSecNum;

            //Returns an error if the user fails to enter an acceptable answer
            if (cin.fail() || testSocialSecNum > 999999999 || testSocialSecNum < 100000000) {

                cin.clear();
                //Ignores if the user enters any letters
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                cout << "Invalid input; must be between 100000000 - 999999999.\n";
            } else {

                //Checks if the social security number already exists or not
                for (int i = 0; i <= totalRecords; i++) {
                    //cout << "Compare " << testSocialSecNum << " with " << indexLocation[i].socialSecurityNum << endl;

                    //Compares the SSN the user put to every SSN that is in the index
                    if (testSocialSecNum == indexLocation[i].socialSecurityNum) {
                        //Because it exists, then it will exit to work with it next.
                        //indexPosition now holds where the SSN is stored
                        //from the indexLocation array
                        //cout << "That social security number exists.\n"; //TEST
                        indexPosition = i;
                        testLoop = false;
                        break;

                    } else if (i == totalRecords) {
                        //Will only reach this if the SSN did not already exist
                        cout << "The social security number doesn't exist.\n";
                        testLoop = true;
                    }
                }
            }
        }


        //Because we now know the position in the index where the SSN is stored when it got
        //it from the indexLocation, we pass it to read the contents
        dataFile.seekg(getBytePosition(indexLocation[indexPosition].locationInNode), ios::beg);
        dataFile.read(reinterpret_cast<char *>(&citizen), sizeof(citizen));


        //Displays the contents that were found in the position as a menu to be edited
        //Case corresponds to the position choice in the menu
        do {
            cout << endl;
            cout << "Record #" << (indexPosition + 1) << endl;
            cout << "1) Name: " << citizen.name << endl;
            cout << "2) SSN: " << citizen.socialSecurityNum << endl;
            cout << "3) DOB: " << citizen.dateOfBirth << endl;
            cout << "4) City: " << citizen.address.city << endl;
            cout << "5) State: " << citizen.address.state <<  endl;
            cout << "6) Zipcode: " << citizen.address.zipcode << endl;
            cout << "7) EXIT AND SAVE" << endl;
            cout << "8) EXIT WITHOUT SAVING" << endl;

            //Ask the user what part they want to change and make sure they put a correct value
            cout << "\nEnter the number for the value you want to change: ";
            do {
                cin >> menuChoice;
                if (cin.fail() || menuChoice < 1 || menuChoice > 8) {
                    //Required in case the user enters a letter (crash)
                    cin.clear();
                    cin.ignore(numeric_limits<streamsize>::max(), '\n');
                    cout << "Invalid input; please enter a number from 1 to 7: ";
                }
            } while (cin.fail() || menuChoice < 1 || menuChoice > 8);

            //Executes after the user entered a correct value for the menu option chosen
            cin.ignore();
            switch (menuChoice) {
                case 1:
                    cout << "What is the new citizen's name? ";
                    cin.getline(citizen.name, NAME_SIZE);
                    cin.clear();
                    break;


                case 2: testLoop = true;
                    //Makes sure the user enters a correct social security number
                    while (testLoop) {
                        cout << "What is the new social security number (9 digits)? ";
                        cin >> citizen.socialSecurityNum;

                        //Returns an error if the user fails to enter an acceptable answer
                        if (cin.fail() || citizen.socialSecurityNum > 999999999 || citizen.socialSecurityNum < 100000000) {

                            cin.clear();
                            //Ignores if the user enters any letters
                            cin.ignore(numeric_limits<streamsize>::max(), '\n');
                            cout << "Invalid input; must be between 100000000 - 999999999.\n";

                        } else {

                            //Checks if the social security number already exists or not
                            for (int i = 0; i <= totalRecords; i++) {

                                //Compares the SSN the user put to every SSN that is in the database
                                if (indexLocation[i].socialSecurityNum == citizen.socialSecurityNum) {
                                    if (citizen.socialSecurityNum == indexLocation[indexPosition].socialSecurityNum) {
                                        cout << "You can't assign the same social security number.\n";
                                    } else
                                        cout << "That social security number already exists.\n";
                                    testLoop = true;
                                    break;
                                    
                                } else if (i == totalRecords) {
                                    //Will only reach this if the SSN did not already exists
                                    testLoop = false;
                                }
                            }
                        }
                    }
                    break;


                case 3:
                    //Makes sure the user enters a correct date of birth
                    while (1) {
                        cout << "What is the birth date (MMDDYY)? ";
                        cin >> citizen.dateOfBirth;
                        if (cin.fail() || citizen.dateOfBirth < 10100 || citizen.dateOfBirth > 123115) {
                            cin.clear();
                            //Ignores if the user enters any letters
                            cin.ignore(numeric_limits<streamsize>::max(), '\n');
                            cout << "Invalid date of birth or future/past date.\n";
                        } else
                            break;
                    }
                    break;


                case 4:
                    cout << "What city are they currently residing? ";
                    cin.getline(citizen.address.city, ADDR_SIZE);
                    break;


                case 5:
                    cout << "What state are they currently residing? ";
                    cin.getline(citizen.address.state, ADDR_SIZE);
                    break;


                case 6:
                    while (1) {
                        cout << "What is their current zipcode (5 numbers)? ";
                        cin >> citizen.address.zipcode;
                        if (cin.fail() || citizen.address.zipcode < 10000 || citizen.address.zipcode > 99999) {
                            cin.clear();
                            //Ignores if the user enters any letters
                            cin.ignore(numeric_limits<streamsize>::max(), '\n');
                            cout << "Invalid zipcode; must be 5 numbers only.\n";
                        } else
                            break;
                    }
                    break;


                case 7:
                    //Saves any changes before exiting the program
                    cout << "Edit will exit and any changes have been saved.\n\n";
                    dataFile.seekp(getBytePosition(indexPosition), ios::beg);
                    dataFile.write(reinterpret_cast<char *>(&citizen), sizeof(citizen));
                    dataFile.close();
                    cout << endl;
                    break;


                case 8:
                    //No data was saved into the file
                    cout << "You exited without saving any data.\n\n";
                    dataFile.clear();
                    dataFile.close();
                    break;
            }
        } while (menuChoice != 7 && menuChoice != 8);  //Will exit if an 8 or 7 is entered
    } else {
        didFileFail(dataFile, fileName);
        return 0;
    }
    return 0;
}





//Menu Choice 3: Will display the contents of a single record
int displayRecord(fstream &dataFile, string fileName, Record citizen, IndexNode indexLocation[]) {

    cout << "\t\t\tViewing a Citizen Record\n\n";


    if (indexLocation[0].locationInNode == -1) {
        cout << "The database is empty.\n\n";
        return 0;
    }

    
    //Variable Control
    bool testLoop = true;
    int indexPosition = 0;
    int testSocialSecNum = 0;
    int totalRecords = 0;
    dataFile.open(fileName, ios::in | ios::binary | ios::beg);

    if (!dataFile.fail()) {

        //Get the size of the array by reading from the indexes filled
        //This size is then used to search for the SSN next
        for (int position = 0; position < ARR_SIZE; position++) {
            if (indexLocation[position].locationInNode != -1) {
                totalRecords++;
            }
        }
        //cout << "Size of the array " << totalRecords << endl;

        //Loops until the user enters a SSN that exists
        while (testLoop) {

            cout << "Enter the SSN of the citizen to view (-1 to exit): ";
            cin >> testSocialSecNum;

            if (testSocialSecNum == -1) {
                dataFile.clear();
                dataFile.close();
                cout << "\n\n";
                return 0;
            }

            //Returns an error if the user fails to enter an acceptable answer
            if (cin.fail() || testSocialSecNum > 999999999 || testSocialSecNum < 100000000) {

                cin.clear();
                //Ignores if the user enters any letters
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                cout << "Invalid input; must be between 100000000 - 999999999.\n";
            } else {

                //Checks if the social security number already exists or not
                for (int i = 0; i <= totalRecords; i++) {

                    //Compares the SSN the user put to every SSN that is in the index
                    if (testSocialSecNum == indexLocation[i].socialSecurityNum) {
                        //Because it exists, then it will exit to work with it next.
                        //indexPosition now holds where the SSN is stored
                        //from the indexLocation array
                        //cout << "That social security number exists.\n"; //TEST
                        indexPosition = i;
                        testLoop = false;
                        break;

                    } else if (i == totalRecords) {
                        //Will only reach this if the SSN did not already exist
                        cout << "The social security number doesn't exist.\n";
                        testLoop = true;
                    }
                }
            }
        }


        //Gets the position of the data by random access memory
        dataFile.seekg(getBytePosition(indexLocation[indexPosition].locationInNode), ios::beg);
        dataFile.read(reinterpret_cast<char *>(&citizen), sizeof(citizen));


        //Displays the user's data
        cout << endl;
        cout << "Record #" << (indexLocation[indexPosition].locationInNode + 1) << endl;
        cout << "Name: " << citizen.name << endl;
        cout << "SSN: " << citizen.socialSecurityNum << endl;
        cout << "DOB: " << citizen.dateOfBirth << endl;
        cout << "Address: " << citizen.address.city << ", " << citizen.address.state << " " << citizen.address.zipcode << endl;
        cout << endl;

        dataFile.clear();
        dataFile.close();
    } else {
        didFileFail(dataFile, fileName);
        return 1;
    }
    return 0;
}





//Menu Choice: 4 Displays all of the citizens in the database DONE
int displayAllRecords(fstream &dataFile, string fileName, Record citizen, IndexNode indexLocation[]) {

    cout << "\t\t\tViewing a Citizen's Record\n\n";

    if (indexLocation[0].locationInNode == -1) {
        cout << "The database is empty.\n\n";
        return 0;
    }


    //Variable Control
    int indexPosition = 0;
    dataFile.open(fileName, ios::in | ios::binary | ios::beg);

    
    if (!dataFile.fail()) {

        //Will continue to read from the file until it breaks
        while (1) {

            //Gets the position of the data by random access memory
            dataFile.seekg(getBytePosition(indexLocation[indexPosition].locationInNode), ios::beg);
            dataFile.read(reinterpret_cast<char *>(&citizen), sizeof(citizen));

            if (!dataFile.eof()) {

                if (indexLocation[indexPosition].locationInNode > -1) {

                    //Displays the user's data
                    cout << endl;
                    cout << "Record #" << (indexLocation[indexPosition].locationInNode + 1) << endl;
                    cout << "Name: " << citizen.name << endl;
                    cout << "SSN: " << citizen.socialSecurityNum << endl;
                    cout << "DOB: " << citizen.dateOfBirth << endl;
                    cout << "Address: " << citizen.address.city << ", " << citizen.address.state << " " << citizen.address.zipcode << endl;

                    //Increments to go to the next available byte in memory
                    indexPosition++;
                } else
                    break;
            } else
                break;
        }
        dataFile.clear();
        dataFile.close();
    } else {
        didFileFail(dataFile, fileName);
        return 1;
    }
    cout << endl;
    return 0;
}






//Menu Choice 5 will delete a record and compress the binary file
int deleteRecord(fstream & dataFile, string fileName, Record citizen, IndexNode indexLocation[], Record tempArray[]) {

    cout << "\t\t\tDeleting a Citizen's Record\n\n";

    if (indexLocation[0].locationInNode == -1) {
        cout << "The database is empty.\n\n";
        return 0;
    }
    cout << "WARNING: DELETING A RECORD CANNOT BE UNDONE.\n\n";


    //Variable Control
    Record eCitizen;    //Temporary empty citizen for the temporary array to be filled by
    for (int index = 0; index < ARR_SIZE; index++) {
        tempArray[index] = eCitizen;
    }
    int deletedIndex = 0;
    int userChoice = 0;
    bool testLoop = true;
    int testSocialSecNum = 0;
    int indexPosition = 0;
    int totalRecords = 0;
    dataFile.open(fileName, ios::in | ios::out | ios::binary | ios::beg);

    if (!dataFile.fail()) {

        //Get the size of the array by reading from the indexes filled
        //This size is then used to search for the SSN next
        for (int position = 0; position < ARR_SIZE; position++) {
            if (indexLocation[position].locationInNode != -1) {
                totalRecords++;
            }
        }
        //cout << "Size of the array " << totalRecords << endl;

        //Loops until the user enters a SSN that exists
        while (testLoop) {

            cout << "Enter the SSN of the citizen to delete (-1 to exit): ";
            cin >> testSocialSecNum;

            if (testSocialSecNum == -1) {
                dataFile.close();
                cout << "\n\n";
                return 0;
            }

            //Returns an error if the user fails to enter an acceptable answer
            if (cin.fail() /* testSocialSecNum > 999999999 || testSocialSecNum < 100000000*/) {

                cin.clear();
                //Ignores if the user enters any letters
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                cout << "Invalid input; must be between 100000000 - 999999999.\n";
            } else {

                //Checks if the social security number already exists or not
                for (int i = 0; i <= totalRecords; i++) {

                    //Compares the SSN the user put to every SSN that is in the index
                    if (testSocialSecNum == indexLocation[i].socialSecurityNum) {
                        //Because it exists, then it will exit to work with it next.
                        //indexPosition now holds where the SSN is stored
                        //from the indexLocation array
                        //cout << "That social security number exists.\n"; //TEST
                        indexPosition = i;
                        testLoop = false;
                        break;

                    } else if (i == totalRecords) {
                        //Will only reach this if the SSN did not already exist
                        cout << "The social security number doesn't exist.\n";
                        testLoop = true;
                    }
                }
            }
        }


        //Gets the position of the data by random access memory
        dataFile.seekg(getBytePosition(indexLocation[indexPosition].locationInNode), ios::beg);
        dataFile.read(reinterpret_cast<char *>(&citizen), sizeof(citizen));


        //Displays the user's data
        cout << endl;
        cout << "Record #" << (indexLocation[indexPosition].locationInNode + 1) << endl;
        cout << "Name: " << citizen.name << endl;
        cout << "SSN: " << citizen.socialSecurityNum << endl;
        cout << "DOB: " << citizen.dateOfBirth << endl;
        cout << "Address: " << citizen.address.city << ", " << citizen.address.state << " " << citizen.address.zipcode << endl;
        cout << endl;

        //Keeps asking the user if he wants to delete the record or not
        while (1) {
            cout << "\nARE YOU SURE YOU WANT TO DELETE THIS RECORD (1 FOR YES, 0 FOR NO)? ";
            cin >> userChoice;
            if (cin.fail() || userChoice < 0 || userChoice > 1) {

                cin.clear();
                //Ignores if the user enters any letters
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                cout << "Invalid input; only a 1 or 0 are allowed.";
            } else if (0) {
                return 0;
            } else {
                break;
            }
        }
        //not to confuse the indexes
        deletedIndex = indexPosition;

        /*************************************************************************************************/
        /****************************REINDEXING AND DELETION OCCURS HERE *********************************/
        /*************************************************************************************************/

        //Clears the data associated with the deleted index
        citizen.reset();

        //To get the last position the array copied
        int lastPosition = 0;
        //Will move every position ahead of the deleted one back one spot
        for (int position = deletedIndex; position < totalRecords; position++) {

            indexLocation[position].locationInNode = --indexLocation[position + 1].locationInNode;
            indexLocation[position].socialSecurityNum = indexLocation[position + 1].socialSecurityNum;
            lastPosition = position;
        }

        //Clears the last position in the index
        indexLocation[lastPosition].locationInNode = -1;
        indexLocation[lastPosition].socialSecurityNum = 0;


        //Assigns the value of -1 to the locations that were moved so they wont be added
        //later into the new binary file
        for (int index = lastPosition; lastPosition < ARR_SIZE; lastPosition++) {
            indexLocation[index].locationInNode = -1;
        }

        for (int index = 0; index < deletedIndex; index++) {
            dataFile.seekg(getBytePosition(index), ios::beg);
            dataFile.read(reinterpret_cast<char *>(&citizen), sizeof(citizen));
            tempArray[index] = citizen;
        }

        //Skips over the array that was deleted
        deletedIndex++;

        //Adds a copy of each object that was ahead of the deleted file
        for (int index = deletedIndex; index < totalRecords; index++) {
            dataFile.seekg(getBytePosition(index), ios::beg);
            dataFile.read(reinterpret_cast<char *>(&citizen), sizeof(citizen));
            tempArray[index - 1] = citizen;
        }

        //Gets the size of the temporary array based on what was left after deletion
        int tempArraySize = --totalRecords;

        //Close to file to clear its contents and add the new array into the file
        dataFile.close();
        dataFile.open(fileName, ios::out | ios::binary | ios::beg);

        //Copies the contents of the temporary array into the data file wihtout the deleted ones
        for (int byte = 0; byte < tempArraySize; byte++) {

            if (indexLocation[byte].locationInNode > -1) {
                dataFile.seekp(getBytePosition(byte), ios::beg);
                citizen = tempArray[byte];
                dataFile.write(reinterpret_cast<char *>(&citizen), sizeof(citizen));
            }
        }

        /*************************************************************************************************/
        /*****************************REINDEXING AND DELETION ENDS HERE **********************************/
        /*************************************************************************************************/

        cout << "A citizen was deleted from the database.\n\n";
        dataFile.close();
    } else {
        didFileFail(dataFile, fileName);
        return 1;
    }
    return 0;
}
