/*
    Lists all the functions called by RIA with a quick description
    Functions and classes in the pdc folder are not listed in this file
*/

int CreateWindows(); // Sets up the windows that RIA uses
void Init(void); // (main.cpp) initializes RIA by creating missing folders and setting up the font and shaders
int main(void); // (main.cpp)

void DrawApp(); // Draws the main app area
void DrawLocations(); // Draws the locations sidebar
void DrawMain(); // Draws the placed images and grid
void DrawTags(); // Draws the tag sidebar

void MainInput(); // Handles all the input for the main window and passes it to area specific functions if needed
void ReorderImages(); // Reorders the images to put the selected one on top
void ResetImages(); // Resets the selected image state
Vector2 MouseToScreenSpace(Vector2); // REMOVE

void DrawBoards(void); // Draws found images boards with their screenshots
void FindBoards(void); // Gets all the image boards in the folder
Image GetBoardScreenshot(string); // Reads the screenshot data from an image board save file
void TakeBoardScreenshot(void); // Takes a screenshot of the current image board

string GetName(string);
void LoadImageBoard();
void Save();
void Load();
void ResetImport();
void Maximize(GLFWwindow*, int);
string SortFormat(string s);
Image LoadWebp(string);
void OpenHelp();
void OpenTutorial();
void SaveImageBoard();