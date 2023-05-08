#include "labyrinth.h"
#include <iostream>

using namespace std;

int Width = -1,
    Height = -1;

void clearScreen() {
    //cout << "\u001B[H\u001B[2J" << flush;
}

bool Placement = false;

void Menu() {
    int answer = 0;

    while (answer != 1 && answer != 2 && answer != 3) {
        clearScreen();
        cout << "WELCOME TO THE LABYRINTH!" << "\n";
        cout << "1.Generate a labyrinth randomly by declared size" << "\n";
        cout << "2.Generate a labyrinth and choose a start and end point" << "\n";
        cout << "3.Exit" << "\n";
        cout << "Your choice:";

        cin >> answer;

        switch (answer) {
        case 1:
        case 2:
            if (answer == 1)
                Placement = false;
            else
                Placement = true;

            do {
                cout << "Enter a number between 5 and 100" << "\n";
                cout << "Enter X: ";
                cin >> Width;

                cout << "Enter Y: ";
                cin >> Height;
            } while (Width > 100 || Width < 5 || Height > 100 || Height < 5);

            break;
        case 3:
            exit(0);
        }
    }
}

int main() {
    Menu();

    Init(Width, Height, Placement);
    PlayerController();
}
