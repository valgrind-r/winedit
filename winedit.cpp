#include <iostream>
#include <fstream>
#include <conio.h>
#include <windows.h>
#include <string>

// Function to get the console window size
COORD getConsoleWindowSize() {
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbi);
    COORD size;
    size.X = csbi.srWindow.Right - csbi.srWindow.Left + 1;
    size.Y = csbi.srWindow.Bottom - csbi.srWindow.Top + 1;
    return size;
}

// Function to print text at a specific position
void printAtPosition(HANDLE hConsoleOutput, int x, int y, const std::string& text) {
    COORD pos = { static_cast<short>(x), static_cast<short>(y) };
    SetConsoleCursorPosition(hConsoleOutput, pos);
    std::cout << text;
}

int main(int argc, char* argv[]) {
    if (argc != 2) {
        std::cout << "Usage: winedit <filename>" << std::endl;
        return -1;
    }

    std::string file = argv[1];
    std::ifstream infile(file);

    static HANDLE hConsoleOutput = GetStdHandle(STD_OUTPUT_HANDLE);
    std::string content = "";
    int x = 0, y = 1;
	std::string tmp;
	if (!infile) {
		tmp = "New file to write...(" + file + ")";
	} else {
		tmp = "Editing current file...(" + file + ")";
		if (infile.is_open()) {
			std::string line;
			while (std::getline(infile, line)) {
				content += line + '\n';
			}
			infile.close();
		}
	}
		
    while (true) {
        COORD consoleSize = getConsoleWindowSize();

        system("cls");

        // Print top and bottom message
		
		printAtPosition(hConsoleOutput, 0 ,0 ,tmp);
        printAtPosition(hConsoleOutput, 0, consoleSize.Y, "Ctrl+Q - quit discarding changes		Ctrl+S - save changes");
		COORD pos = {0,1};
		SetConsoleCursorPosition(hConsoleOutput, pos);

        // Print the content
        std::cout << content;

        char ch = _getch();

        if (ch == 27) { // Check if it's an escape sequence (arrow keys)
            ch = _getch(); // Get the next character

            if (ch == 91) { // Check for the '[' character
                ch = _getch(); // Get the final character

                // Identify the arrow key
                switch (ch) {
                    case 65: // Up Arrow
                        if (y > 0) {
                            --y;
                        }
                        break;
                    case 66: // Down Arrow
                        if (y < consoleSize.Y - 1) {
                            ++y;
                        }
                        break;
                    case 67: // Right Arrow
                        if (x < consoleSize.X - 1) {
                            ++x;
                        }
                        break;
                    case 68: // Left Arrow
                        if (x > 0) {
                            --x;
                        }
                        break;
                }
            }
        } else if (ch == 13) { // Enter key
            content.push_back('\n');
            ++y;
            x = 0;
        } else if (ch == 8) { // Backspace key
            if (x >= 1) {
                if(!content.empty()){
					content.pop_back();
				}
            } else if (x == 0 && y > 0) {
                --y;
                x = consoleSize.X - 1;
            }
        } else if (ch == 19){
			try{
				std::ofstream outFile(file);
				outFile << content;
				outFile.close();
				std::cout << "Saved successfully" << std::endl;
			} catch (...){
				std::cerr << "Error saving" << std::endl;
			}
			system("cls");
			break;
		} else if (ch == 17){
			system("cls");
			break;
		} else {
            content.push_back(ch);
            ++x;
        }
    }
	return 0;
}
