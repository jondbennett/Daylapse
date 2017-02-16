# Daylapse
Raspberry Pi Timelapse Creator for very long timelapse projects

I put a Raspberry Pi in a weather proof box and mounted it to the top of my house so that I could create a one-year timelapse of our garde, and place it [on YouTube](https://www.youtube.com/watch?v=xY_Os_A_1po).

This project is a total hack. I use Code::Blocks on the pi as my c++ IDE, but you can work up a makefile if  you want.

The way this thing works is that it captures 25 frames per day, evenly spaced between sunrise and sunset. The time between frames is longer in the summer when days are longer, and the time between frames decreases with the shorter days of winter. It uses "raspistill" to actually take the images, and stores them in directories based on the date. In order for the sunrise and sunset calculations to work, you will have to edit the location (latitude and longitude) in main.cpp to reflect the location of the camera.

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

The normal way to use this is to setup a daily cron job for some early time (like 3:00 AM) that calls the "DayLapse" program.
It might make sense to connect to the Pi and download the images from time to time in order to keep the SD card from filling up.

I used the "OpenShot" video editor to make the final timelapse movie. I also used a [Timelapse Deflickering Script](https://github.com/cyberang3l/timelapse-deflicker) to reduce the possability of inducing seizures.

So:

1.  Create a directory with the name of the year (for my project that was 2016). This is called "the year directory."
2.  Below the year directory make a directory named "Days". This is where you will place the individual day directories downloaded from the Pi.
3.  Make another below the year directory named "All_Frames."
4.  Copy the contents of the "Tools" directory into the year directory.
  * Link_Frames will create hard links in the "All_Frames" directory.
  * timelapse-deflicker.pl will do the deflickering
  * renumberJPG will do the sequential renumbering needed for OpenShot to create the video.
6.  Use "OpenShot" to create the actual mpeg.

