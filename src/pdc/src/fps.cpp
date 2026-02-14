using namespace std::chrono;

static milliseconds startTime; // Time the next frame started
static milliseconds endTime; // Time the last frame ended

uint FPS = 0;
float FPSLimit = 0; // User defined limit

static int FPSData[60]; // Contains the last 60 fps values
static int FPSI; // Index of the fps array 


void GetFPS(){
	endTime = duration_cast<milliseconds>(system_clock::now().time_since_epoch()); // Get current time
	deltaTime = endTime.count()-startTime.count(); // Get time since last update
	deltaTime /= 1000; // Convert to milliseconds

	// Lock frame rate if there is a defined limit
	if (FPSLimit && deltaTime < 1.0f/FPSLimit){
		SleepFor(1/FPSLimit*1000-deltaTime*1000); // Pause until ready for next frame
		deltaTime = 1/FPSLimit;
	}
	
	// Averages the fps for the last 60 frames to get an estimate on the current fps of the game
	if (deltaTime != 0)
		FPSData[FPSI] = 1/deltaTime;
	else
		FPSData[FPSI] = 0.0001;
	for (int i = 0; i < 60; i++)
		FPS+=FPSData[i]+1;
	FPS/=60;
	FPSI++;
	if (FPSI >= 60)
		FPSI = 0;

	startTime = duration_cast<milliseconds>(system_clock::now().time_since_epoch());
}

void InitFPS(){
	deltaTime = 1;
	startTime = duration_cast<milliseconds>(system_clock::now().time_since_epoch());
}