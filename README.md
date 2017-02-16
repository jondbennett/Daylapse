# Daylapse
Raspberry Pi Timelapse Creator for very long timelapse projects

I put a Raspberry Pi and Pi Camera in a weather-proof box and mounted it to the top of my house so that I could create a one-year timelapse of our garden. You can see the timelapse movie [on YouTube](https://www.youtube.com/watch?v=xY_Os_A_1po).

# This project is a total hack.

The way this thing works is that it captures 25 frames per day, evenly spaced between sunrise and sunset. The time between frames is longer in the summer when days are longer, and the time between frames decreases with the shorter days of winter. It uses "raspistill" to actually take the images, and stores them in directories based on the date.

In order for the sunrise and sunset calculations to work, you will have to edit the location (latitude and longitude) in main.cpp to reflect the location of the camera:
```
// Location
double lat = 39.52;
double lon = -81.99;
```

The number of frames per day is configurable by editing main.cpp:
```
#define FRAMES_PER_SECOND	(25.)
#define SECONDS_PER_DAY		(2.)
```

I use Code::Blocks on the Pi as my c++ IDE, but you can work up a makefile if  you want.

# Usage
The normal way to use this is to setup a daily cron job for some very early time in the morning (like 3:00 AM) that calls the "Daylapse" program. Daylapse will wait until sunrise and then start taking photos. The time between photos is calculated based on the number of intended frames per seconds, and the number of seconds per day. After taking all of the photos for a given day the program exits. Cron will restart it the next day.

It might make sense to connect to the Pi and download the images from time to time in order to keep the SD card from filling up.

I used the "OpenShot" video editor to make the final timelapse movie. I also used a [Timelapse Deflickering Script](https://github.com/cyberang3l/timelapse-deflicker) to reduce the possability of inducing seizures.

# Directions:

1.  Create a directory with the name of the year (for my project that was 2016). This is called "the year directory."
2.  Below the year directory make a directory named "Days". This is where you will place the individual day directories downloaded from the Pi.
3.  Make another below the year directory named "All_Frames."
4.  Copy the contents of the "Tools" directory into the year directory.
  * Link_Frames will create hard links in the "All_Frames" directory.
  * timelapse-deflicker.pl will do the deflickering
  * renumberJPG will do the sequential renumbering needed for OpenShot to create the video.
6.  Use "OpenShot" to create the actual mpeg.

