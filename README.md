# Daylapse
Raspberry Pi Timelapse Creator for very long timelapse projects

Put a Raspberry Pi in a weather proof box and mounted it to the top of my house in order to create a one-year long timelapse of our garden [on YouTube](https://www.youtube.com/watch?v=xY_Os_A_1po).

This project is a total hack. I use Code::Blocks on the pi as my c++ IDE, but you can work up a makefile if  you want.

The way this thing works is that it captures 25 frames per day, evenly spaced between sunrise and sunset. The time between frames is longer in the summer when days are longer, and the time betweenf rames decreases with the shorter days of winter. It uses "raspistill" to actually take the images, and stores them in directories based on the date. In order for the sunrise and sunset calculations to work, you will have to edit the location (latitude and longitude) in main.cpp to reflect the location of the camera.

```
// Location
double lat = 39.52;
double lon = -81.99;
```

The normal way to use this is to setup a daily cron job for some early time (like 3:00 AM) that calls the "DayLapse" program.
It might make sense to connect to the Pi and download the images from time to time in order to keep the SD card from filling up.

I used the "OpenShot" video editor to make the final timelapse movie.

The number of frames per day is configurable by editing main.cpp:
```
#define FRAMES_PER_SECOND	(25.)
#define SECONDS_PER_DAY		(2.)
```

