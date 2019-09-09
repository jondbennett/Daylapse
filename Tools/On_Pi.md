If you want to use the pi to do more than just take the photos, you
may need additional modules. In particular, if you want to run the deflicker
perl script you might need:

sudo apt install imagemagick
sudo apt-get install libimage-magick-perl


sudo cpan (if it asks to auto-configure say yes)
install File::Type
install Term::ProgressBar
install Image::ExifTool
