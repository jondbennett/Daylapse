/*
 * File:
 *      snapframes.h
 *
 * Description:
 *      C library that takes N pictures with raspistill
 *      with T interval in between each picture
 */
 
void snapFrames(unsigned int _frameCount, double _frameDelay, bool _dryrun);

