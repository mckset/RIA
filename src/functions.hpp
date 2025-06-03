/*
    Lists all the functions called by RIA with a quick description
    Functions and classes in the pdc folder are not listed in this file for the reason that they belong to the separate pdc project and will change over time
*/

//--------------------------------------------------------------------------------------------------
// main.cpp
//
int CreateWindows(); // Sets up the windows that RIA uses
void Init(void); // initializes RIA by creating missing folders and setting up the font and shaders
int main(int, char**); // Starts the program and handles exiting
//--------------------------------------------------------------------------------------------------


//--------------------------------------------------------------------------------------------------
// src/windows/main
//

// src/windows/main/boards.cpp
void DrawBoards(void); // Draws found images boards with their screenshots
void FindBoards(void); // Gets all the image boards in the folder
Image GetBoardScreenshot(string); // Reads the screenshot data from an image board save file
void TakeBoardScreenshot(void); // Takes a screenshot of the current image board

// src/windows/main/main_input.cpp
void MainInput(); // Handles all the input for the main window and passes it to area specific functions if needed
void ReorderImages(); // Reorders the images to put the selected one on top
void ResetImages(); // Resets the selected image state

// src/windows/main/main_render.cpp
void DrawApp(); // Draws the main app area
void DrawLocations(); // Draws the locations sidebar
void DrawMain(); // Draws the placed images and grid
void DrawTags(); // Draws the tag sidebar

// src/windows/main/left_menu_input.cpp
// src/windows/main/right_menu_input.cpp
void LeftMenuInput(void); // Handles the input for the left menu and adjacent buttons
void RightMenuInput(void); // Handles the input for the right menu and adjacent buttons

// src/windows/main/tutorial_render.cpp
// src/windows/main/tutorial_input.cpp
void DrawTutorial(void); // Displays information on the step in the tutorial
void OpenTutorial(void); // Sets the variables for opening the tutorial
void TutorialInput(void); // Input for the tutorial
//--------------------------------------------------------------------------------------------------


//--------------------------------------------------------------------------------------------------
// src/windows/import
//

// src/windows/import/import_render.cpp
//void DrawFolders(float, float*, Vector2, Table*); // Draws a given folder and its sub folders
void DrawImportFolders(void); // Draws all folders that are associated with an image pack
void DrawImportMain(void); // Draws found image packs
void DrawImportTags(void); // Draw all the tags that will be imported
void ResetImport(void); // Resets import window variables

// src/windows/import/import_input.cpp
void CreateImport(void); // Gets all the files and tags in an image pack and creates an import.dat file
//Tag FilterTag(Tag, vector<File>); // Checks a tag to see if it contains any given files and returns a tag with only matching files
void FoldersImportInput(void); // Handles the file import menu input
string GetFolderName(string); // Returns the first folder in the path
//void ImportTag(Tag, Tag*); // Checks if an image is already in the given tag to prevent double tagging
bool LoadImport(void); // Loads tags from an image pack
void MainImportInput(void); // Default input for the import window
void TagsImportInput(void); // Handles the tag import menu input
//--------------------------------------------------------------------------------------------------


//--------------------------------------------------------------------------------------------------
// src/functions
//

// src/functions/clipboard.cpp 
void Copy(void);
void Paste(void);

// src/functions/handler.cpp 
void DragDrop(GLFWwindow*, int, const char**); // Handles drag and drop events
static void Error(int, const char*); // Reports GLFW errors
void GetScrollWheel(GLFWwindow*, double, double); // Handles scrollwheel events
char KeyToChar(int); // Shifts a character to upper case when shift is held
void Maximize(GLFWwindow*, int); // Maximizes a window
void On_Resize(GLFWwindow*, int, int); // Handles resize events
void SetCursorPosition(GLFWwindow*, double, double); // Updates mouse cursor position
void SetKeyboardState(GLFWwindow*, int, int, int, int); // Handles keyboard events
void SetMouseState(GLFWwindow*, int, int, int); // Gets mouse button events
void SubClose(GLFWwindow *); // Hides sub windows

// src/functions/load.cpp 
//bool Duplicate(string img, vector<File>* list); // Checks for duplicate images in a list
void FixString(string*); // Fixes a path to match the os slash
Color GetColor(string); // Gets a color from a string
string GetName(string); // Gets the name from the end of a path
string GetString(void); // Reads a string from the save data
string GetStringOld(void); // Reads a string from old save data
void Load(void); // Main load function
void LoadImageBoard(void); // Load an image board
//vector<Tag> LoadTags(ifstream*, bool, bool); // Load tags
Image LoadWebp(string); // Load a webp image

// src/functions/os_linux.cpp or src/functions/os_windows.cpp
#ifdef _WIN32 
    void GetBoard(); // This is the worst thing I've ever had to deal with
#endif
void OpenShared();
void SaveBoard();
void HideConsole();

// src/functions/save.cpp
void Save(void); // Main save function
void SaveImageBoard(void); // Saves the current image board
//void SaveTags(ofstream*, vector<Tag>, bool); // Saves tag information

// src/functions/sort.cpp
string SortFormat(string s);
bool SortString(string, string);
//bool SortTag(Tag t1, Tag t2);
bool SortUint(uint, uint);

// src/functions/tag.cpp
void DrawTag(); // Draws the tag editor
void TagInput(); // Tag editor input
void ResetTagEdit(); // Resets tag editor variables
//--------------------------------------------------------------------------------------------------