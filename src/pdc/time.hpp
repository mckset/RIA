using namespace std::chrono;

static milliseconds startTime; // Time the next frame started
static milliseconds endTime; // Time the last frame ended

float deltaTime = 0; // Time since last screen draw


void GetDeltaTime(){
	endTime = duration_cast<milliseconds>(system_clock::now().time_since_epoch()); // Get current time
	deltaTime = endTime.count()-startTime.count(); // Get time since last update
	deltaTime /= 1000; // Convert to milliseconds
	startTime = duration_cast<milliseconds>(system_clock::now().time_since_epoch());
}