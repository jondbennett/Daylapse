
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <limits.h>
#include <sys/time.h>

// Return current time in seconds + fractional second
double currentTimeMs()
{
struct timeval tp;
double seconds = 0.;
double milliseconds = 0.;

	gettimeofday(&tp, 0);
	seconds = (double)tp.tv_sec;
	milliseconds = (double)(tp.tv_usec / 1000);

	return (seconds + (milliseconds / 1000));
}

void snapFrames(unsigned int _frameCount, double _frameDelay)
{
	// Sanity
	if(_frameDelay < 0)
		return;

	// Set up the first frame time
	double frame_time = currentTimeMs();
	double next_frame_time = 0.;
	char snapCommand[PATH_MAX + FILENAME_MAX];

	{
		time_t now = time(0);
		printf("snapFrames: starting at %s",asctime(localtime(&now)));
		printf("snapFrames: count = %d, delay = %f\n", _frameCount, _frameDelay);
		fflush(stdout);
	}

	/////////////////////////////////////////////////////
	// Main Loop
	/////////////////////////////////////////////////////
	for (unsigned int frame = 0; frame < _frameCount; frame++)
	{
		// Figure timing for next frame
		next_frame_time = frame_time + _frameDelay;

		// Figure latency
		int latency = (int)((_frameDelay * 1000) - 500);
		if(latency < 1000) latency = 1000;
		if(latency > 5000) latency = 5000;

		// Now work up the file name in GMT to avoid daylight saving time
		time_t frame_time_int = (int)frame_time;
		int frame_time_decimal = (int)((frame_time - frame_time_int) * 1000);
		struct tm* gmt = gmtime(&frame_time_int);

		// Create snapshot command
		sprintf(snapCommand,"raspistill -vf -hf -n -w 1920 -h 1080 -t %d -q 90 -o %04d_%02d_%02d_%02d_%02d_%02d_%03d.jpg",
				latency,
			gmt->tm_year + 1900,gmt->tm_mon + 1,gmt->tm_mday,
			gmt->tm_hour,gmt->tm_min,gmt->tm_sec, frame_time_decimal);

		{
			time_t now = time(0);
//			printf("snapFrames: snaping frame %d (%s) at %s", frame, snapCommand, asctime(localtime(&now)));
			printf("snapFrames: snaping frame %d at %s", frame, asctime(localtime(&now)));
			fflush(stdout);
		}

		// Execute snapshot command
		system(snapCommand);

		// Tight sleep / delay

		// Use sleep if we have more than a second to wait
		int delay;
		delay = (int)(next_frame_time - frame_time);
		if(delay > 1)
			sleep(delay - 1);

		// Less than a second to go, so start a tight
		// loop waiting for the time to change
		while(currentTimeMs() < next_frame_time);
		frame_time = next_frame_time;
	}
}
