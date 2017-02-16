
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <math.h>
#include <time.h>
#include <string.h>

// Uses Paul Schlyter's excellent sunriseset.code
//
#include "sunriset.h"
#include "snapframes.h"

time_t convertToTimeT(int _year, int _month, int _day, double _rsTime)
{
	struct tm gmt;

	// Convert the double to hours and minutes
	int hour = (int)_rsTime;
	int minute = (int)((_rsTime - hour) * 60);

	// Fill in the gmt struct
	memset(&gmt, 0, sizeof(gmt));
	gmt.tm_year = _year - 1900;
	gmt.tm_mon = _month - 1;
	gmt.tm_mday = _day;

	gmt.tm_hour = hour;
	gmt.tm_min = minute;

	return timegm(&gmt);
}

bool waitUntilSunrise(time_t _sunrise)
{
	time_t now = time(0);
	time_t sleepSeconds;

	if(_sunrise < now)
	{
#ifdef DEBUG
		printf("waitUntilSunrise in debug mode returning true\n");
		return true;
#else
		return false;
#endif
	}

	printf("waitUntilSunrise()\n");
	printf("Current time: %s",asctime(localtime(&now)));
	printf("Local Sunrise: %s",asctime(localtime(&_sunrise)));
	fflush(stdout);

	// I do it this way because a single sleep for the time between
	// "now" and "sunrise" could be on the order of several thousand seconds
	// and the sleep timer has proven to be inaccurate at that range.
	do
	{
		now = time(0);
		sleepSeconds = _sunrise - now;
		if(sleepSeconds > 60)
			sleepSeconds = 60;
		if(sleepSeconds < 0)
			sleepSeconds = 0;
		if(sleepSeconds > 0)
			sleep(sleepSeconds);
	}
	while(sleepSeconds > 0);

	printf("waitUntilSunrise: Complete at %s",asctime(localtime(&now)));
	fflush(stdout);

	return true;
}

#define FRAMES_PER_SECOND	(25.)
#define SECONDS_PER_DAY		(2.)

void snapDayFrames(double _dayLen)
{
	// Figure the number of frames for this day
	int dayFrames = round(FRAMES_PER_SECOND * SECONDS_PER_DAY);

	// Figure frame delay
	double frameDelay = (_dayLen * 60. * 60.)/dayFrames;

	printf("snapDayFrames: calling snapFrames(%d,  %0.3f)\n", dayFrames, frameDelay);
	fflush(stdout);


	// And snap the frames
	snapFrames(dayFrames, frameDelay);
}

int main()
{
	// Prep inputs for the sun_rise_set function
	time_t now = time(0);
	struct tm tmNow = *gmtime(&now);

	// Date
	int year = tmNow.tm_year + 1900;
	int month = tmNow.tm_mon + 1;
	int day = tmNow.tm_mday;

	// Location
	double lat = 39.52;
	double lon = -81.99;

	// Output
	double rise, set;
	time_t riseTime;
	time_t setTime;

	int rs = sun_rise_set( year, month, day, lon, lat,
						   &rise, &set );
	riseTime = convertToTimeT(year, month, day, rise);
	setTime = convertToTimeT(year, month, day, set);

	double daylen  = day_length(year,month,day,lon,lat);

	switch( rs )
	{
	case 0:
		printf("Local Sunrise: %s",asctime(localtime(&riseTime)));
		printf("Local Sunset: %s",asctime(localtime(&setTime)));
		printf("Day length = %5.2fh\n", daylen);
		fflush(stdout);

		// Wait for sunrise today or exit if we missed it
		if(!waitUntilSunrise(riseTime))
		{
			printf("ERROR: Sunrise for today has already passed\n");
			exit(1);
		}

		// Take the photos and we're done
		snapDayFrames(daylen);
		break;

	case +1:
		printf( "ERROR: Sun above horizon, no sunset today!\n" );
		break;

	case -1:
		printf( "ERROR: Sun below horizon, no sunrise today!\n" );
		break;
	}

	{
		time_t now = time(0);
		printf("dayLapse: complete at %s", asctime(localtime(&now)));
		fflush(stdout);
	}

	return 0;
}





