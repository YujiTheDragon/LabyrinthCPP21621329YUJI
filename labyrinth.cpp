#include "labyrinth.h"
#include <cstdlib> // За rand().
#include <string>  // За string
#include <iostream>

using namespace std;

const string ANSI_RESET =  "\033[0m";
const string ANSI_GREEN =  "\033[32m";
const string ANSI_RED =    "\033[31m";
const string ANSI_YELLOW = "\033[33m";

static int Width, Height;
static bool Placement = false;
static char** LabCords;
static int PlayerX, PlayerY;
static int returnPointCounter;
static int foundReturnPoint1;
static int foundReturnPoint2;

static int randomInt(int max) {
    return rand() % max;
}
static bool randomBool() {
    return rand() % 2;
}

void PrintLabyrinth() {
    for (int i = 0; i < Width; i++) {
        for (int j = 0; j < Height; j++) {
            if (j == 0) {
                if (i >= 10)
                    cout << i << " ";
                else
                    cout << i << "  ";
            }

            if (LabCords[i][j] == 'X')
                cout << ANSI_GREEN << LabCords[i][j] << ANSI_RESET;
            else if (LabCords[i][j] == 'Y')
                cout << ANSI_RED << LabCords[i][j] << ANSI_RESET;
            else
                cout << LabCords[i][j];
        }
        cout << "\n";
    }
}

void Init(int sizeX, int sizeY, bool placement) {
    Width = sizeX;
    Height = sizeY;
    Placement = placement;

    LabCords = new char*[sizeX];
    for(int y = 0; y < sizeY; y++) {
        LabCords[y] = new char[sizeY];
    }

    int ExitIntLine = randomInt(sizeX),
        ExitIntCol = randomInt(sizeY),
        EntranceIntLine = randomInt(sizeX),
        EntranceIntCol = randomInt(sizeY);

    cout << "\nLABYRINTH!\n";
    for (int i = 0; i < sizeX; i = i + 1) {
        for (int j = 0; j < sizeY; j = j + 1) {
            if (ExitIntLine == i && ExitIntCol == j && Placement == false) {
                LabCords[i][j] = 'X';
                cout << ANSI_GREEN << "X" << ANSI_RESET;
            } else if (EntranceIntLine == i && EntranceIntCol == j && Placement == false) {
                LabCords[i][j] = 'Y';
                PlayerX = i;
                PlayerY = j;
                cout << ANSI_RED << "Y" << ANSI_RESET;
            } else if (i == 0 || i == sizeX - 1 || j == 0 || j == sizeY - 1 || randomBool()) {
                if (j == 0) {
                    if (i >= 10)
                        cout << i << " ";
                    else
                        cout << i << "  ";
                }
                LabCords[i][j] = '#';
                cout << "#";
            } else {
                LabCords[i][j] = ' ';
                cout << " ";
            }
        }
        cout << "\n";
    }

    /*
     * TODO FIX OUT OF BOUND EXCEPTION WHEN ENTERING A HIGH OR NEGATIVE VALUE
     */
    if (Placement == true) {
        int line = 0, line1 = 0, column = 0, column1 = 0;
        char tempchar;
        bool Done = false;

        do {
            cout << "Enter line for the exit:";
            cin >> line;

            if(line < 0 || line > Height) {
                cout << "ERROR: Line for exit must be between 0 and " << Height << "!\n";
                continue;
            }

            cout << "Enter column for the exit:";
            cin >> column;

            if(column < 0 || column > Width) {
                cout << "ERROR: Column for exit must be between 0 and " << Width << "!\n";
                continue;
            }

            tempchar = LabCords[line][column];
            LabCords[line][column] = 'X';

            cout << "Enter line for the player:";
            cin >> line1;

            if(line1 < 0 || line1 > Height) {
                cout << "ERROR: Line for player must be between 0 and " << Height << "!\n";
                continue;
            }

            cout << "Enter column for the player:";
            cin >> column1;

            if(column1 < 0 || column1 > Width) {
                cout << "ERROR: Column for player must be between 0 and " << Width << "!\n";
                continue;
            }

            if (LabCords[line1][column1] == 'X') {
                cout << "ERROR: Player cannot be placed on top of the exit!" << "\n";
                LabCords[line][column] = tempchar;
            } else {
                Done = true;
                LabCords[line1][column1] = 'Y';
                PlayerX = line1;
                PlayerY = column1;
            }
        } while (!Done);

        PrintLabyrinth();
    }

    for (int i = 0; i < sizeX; i = i + 1) {
        for (int j = 0; j < sizeY; j = j + 1) {
            if (LabCords[i][j] == 'X')
                cout << "X= " << i << "\n" << "Y= " << j << "\n";
            if (LabCords[i][j] == 'Y')
                cout << "X= " << i << "\n" << "Y= " << j << "\n";
        }
    }

    cout << ANSI_RESET;
}

void printSearch(char** VisitedLabCords) {
    for (int i = 0; i < Width; i++) {
        for (int j = 0; j < Height; j++) {
            if (j == 0) {
                if (i >= 10)
                    cout << i << " ";
                else
                    cout << i << "  ";
            }

            if (VisitedLabCords[i][j] == 'X')
                cout << ANSI_GREEN << VisitedLabCords[i][j] << ANSI_RESET;
            else if (VisitedLabCords[i][j] == 'Y')
                cout << ANSI_RED << VisitedLabCords[i][j] << ANSI_RESET;
            else if (VisitedLabCords[i][j] == '0')
                cout << ANSI_YELLOW << VisitedLabCords[i][j] << ANSI_RESET;
            else
                cout << VisitedLabCords[i][j];
        }
        cout << "\n";
    }
}

int LookAround(char** VisitedLabCords, int StartX, int StartY) {
    if (StartX - 1 > 0 && VisitedLabCords[StartX - 1][StartY] == ' ')
        return 1;
    else if (StartY + 1 < Height && VisitedLabCords[StartX][StartY + 1] == ' ')
        return 2;
    else if (StartX + 1 < Width && VisitedLabCords[StartX + 1][StartY] == ' ')
        return 3;
    else if (StartY - 1 > 0 && VisitedLabCords[StartX][StartY - 1] == ' ')
        return 4;

    return 0;
}

bool LookAroundExit(char** VisitedLabCords, int StartX, int StartY) {
    if (StartX - 1 > 0 && VisitedLabCords[StartX - 1][StartY] == 'X')
        return true;
    else if (StartY + 1 < Height && VisitedLabCords[StartX][StartY + 1] == 'X')
        return true;
    else if (StartX + 1 < Width && VisitedLabCords[StartX + 1][StartY] == 'X')
        return true;
    else if (StartY - 1 > 0 && VisitedLabCords[StartX][StartY - 1] == 'X')
        return true;

    return false;
}

int returnPointsCount(char** VisitedLabCords, int StartX, int StartY) {
    returnPointCounter = 0;
    if (StartX + 1 < Width && VisitedLabCords[StartX + 1][StartY] == ' ')
        returnPointCounter++;
    if (StartX - 1 > 0 && VisitedLabCords[StartX - 1][StartY] == ' ')
        returnPointCounter++;
    if (StartY + 1 < Height && VisitedLabCords[StartX][StartY + 1] == ' ')
        returnPointCounter++;
    if (StartY - 1 > 0 && VisitedLabCords[StartX][StartY - 1] == ' ')
        returnPointCounter++;
    return returnPointCounter;
}

bool returnPointsCheck(int Width, int Height, int** returnPoints) {
    for (int i = 0; i < Width; i++) {
        for (int j = 0; j < Height; j++) {
            if (returnPoints[i][j] == 1 || returnPoints[i][j] == 2 || returnPoints[i][j] == 3) {
                foundReturnPoint1 = i;
                foundReturnPoint2 = j;
                return true;
            }
        }
    }
    return false;
}

void SearchAlg() {
    char** VisitedLabCords = new char*[Width];
    for(int i = 0; i < Width; i++) {
        VisitedLabCords[i] = new char[Height];
    }

    int StartX = 0;
    int StartY = 0;
    for (int i = 0; i < Width; i++) {
        for (int j = 0; j < Height; j++) {
            VisitedLabCords[i][j] = LabCords[i][j];
            if (LabCords[i][j] == 'Y') {
                StartX = i;
                StartY = j;
                cout << "FOUND Y" << "\n";
            }
        }
    }
    printSearch(VisitedLabCords);
    bool solved = false;
    bool notfound = false;

    int** returnPoints = new int*[Width];
    for(int i = 0; i < Width; i++){
        returnPoints[i] = new int[Height];
    }

    while (solved == false) {
        cout << "LOOP" << "\n";
        if (LookAroundExit(VisitedLabCords, StartX, StartY) == true) {
            solved = true;
            notfound = false;
            break;
        }
        if (LookAround(VisitedLabCords, StartX, StartY) == 0
            && returnPointsCheck(Width, Height, returnPoints) == false && solved == false) {
            solved = true;
            notfound = true;
        }
        if (LookAround(VisitedLabCords, StartX, StartY) == 0) {
            VisitedLabCords[StartX][StartY] = '0';
            StartX = foundReturnPoint1;
            StartY = foundReturnPoint2;
            VisitedLabCords[StartX][StartY] = 'Y';
            // returnPoints[StartX][StartY] = returnPoints[StartX][StartY] - 1;

        }
        returnPoints[StartX][StartY] = returnPointsCount(VisitedLabCords, StartX, StartY);
        switch (LookAround(VisitedLabCords, StartX, StartY)) {
        case 1:
            cout << "up" << "\n";
            VisitedLabCords[StartX - 1][StartY] = 'Y';
            VisitedLabCords[StartX][StartY] = '0' + returnPoints[StartX][StartY];
            StartX = StartX - 1;
            break;
        case 3:
            cout << "down" << "\n";
            VisitedLabCords[StartX + 1][StartY] = 'Y';
            VisitedLabCords[StartX][StartY] = '0' + returnPoints[StartX][StartY];
            StartX = StartX + 1;
            break;
        case 2:
            cout << "right" << "\n";
            VisitedLabCords[StartX][StartY + 1] = 'Y';
            VisitedLabCords[StartX][StartY] = '0' + returnPoints[StartX][StartY];
            StartY = StartY + 1;
            break;
        case 4:
            cout << "left" << "\n";
            VisitedLabCords[StartX][StartY - 1] = 'Y';
            VisitedLabCords[StartX][StartY] = '0' + returnPoints[StartX][StartY];
            StartY = StartY - 1;
            break;
        }
        printSearch(VisitedLabCords);
    }

    /* CHECKS AFTER LOOP */
    if (notfound == true) {
        cout << "Exit not found!" << "\n";
        solved = false;
    }

    else if (solved == true)
        cout << "Found the exit!" << "\n";

}

void PlayerController() {
    int Key;
    bool ExitReached = false;
    char tempchar = ' ';

    while (!ExitReached) {
        PrintLabyrinth();
        cout << "INSTRUCTIONS:" << "\n"
             << " To go up press 1" << "\n"
             << " To go right press 2" << "\n"
             << " To go down press 3" << "\n"
             << " To go left press 4" << "\n"
             << " To try the Search Algorithm press 5" << "\n";

        cin >> Key;

        switch (Key) {
        case 1:
            if (LabCords[PlayerX - 1][PlayerY] == '#')
                break;
            else if (LabCords[PlayerX - 1][PlayerY] == 'X') {
                ExitReached = true;
                cout << "NICE YOU FOUND THE EXIT!" << "\n";
                break;
            } else {
                LabCords[PlayerX][PlayerY] = tempchar;
                tempchar = LabCords[PlayerX][PlayerY];
                LabCords[PlayerX - 1][PlayerY] = 'Y';

                PlayerX -= 1;

                break;
            }
        case 2:
            if (LabCords[PlayerX][PlayerY + 1] == '#')
                break;
            else if (LabCords[PlayerX][PlayerY + 1] == 'X') {
                ExitReached = true;
                cout << "NICE YOU FOUND THE EXIT!" << "\n";
                break;
            } else {
                LabCords[PlayerX][PlayerY] = tempchar;
                tempchar = LabCords[PlayerX][PlayerY];
                LabCords[PlayerX][PlayerY + 1] = 'Y';

                PlayerY += 1;

                break;
            }
        case 3:
            if (LabCords[PlayerX + 1][PlayerY] == '#')
                break;
            else if (LabCords[PlayerX + 1][PlayerY] == 'X') {
                ExitReached = true;
                cout << "NICE YOU FOUND THE EXIT!" << "\n";
                break;
            } else {
                LabCords[PlayerX][PlayerY] = tempchar;
                tempchar = LabCords[PlayerX][PlayerY];
                LabCords[PlayerX + 1][PlayerY] = 'Y';

                PlayerX += 1;

                break;
            }
        case 4:
            if (LabCords[PlayerX][PlayerY - 1] == '#')
                break;
            else if (LabCords[PlayerX][PlayerY - 1] == 'X') {
                ExitReached = true;
                cout << "NICE YOU FOUND THE EXIT!" << "\n";
                break;
            } else {
                LabCords[PlayerX][PlayerY] = tempchar;
                tempchar = LabCords[PlayerX][PlayerY];
                LabCords[PlayerX][PlayerY - 1] = 'Y';

                PlayerY -= 1;

                break;
            }
        case 5:
            SearchAlg();
            break;
        }
    }
}
