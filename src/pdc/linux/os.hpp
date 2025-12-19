void SleepFor(int);

void SleepFor(int milliseconds){
	usleep(milliseconds*1000);
}