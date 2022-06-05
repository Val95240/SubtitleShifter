# Subtitle Shifter

When downloading subtitles for movies or TV shows, they are often not synced perfectly with the version of the media I have.
This simple Qt5 tool can shift an SRT file to get the correct timing.


## Installation

The code needs the following libraries to compile:
- [Qt5](https://www.qt.io/)

To compile, simply clone the directoy and run the `build` script.


## Usage

Load a SRT file and input the time at which the first subtitle *should* appear.
Without adjusting the last subtitle time, each line will only be shifted to the new timing.
If a last subtitle time is given, the subtitles will be squashed or elongated in order to fit these two constraints.

Press the `Generate` button to save the new SRT file in the destination file (which can be edited in the second field of the app).
