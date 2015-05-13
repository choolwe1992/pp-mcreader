# Overview #
Mobile Currency Reader that scans currency using Android phones and says the amount value of the currency.  This project was completed as part of the Project Possibility SS12 event held in Feb/March 2010 at UCLA. More information on Project Possibility and its mission can be found at http://www.projectpossibility.org/.

# Problem #
All denominations of U.S. bills have the same size and shape, making them hard for the blind to differentiate.

# Possible Solutions #
  * Print out more accessible currency
    * It's expensive to change bills
  * Ask someone
    * People aren't always trustworthy
  * Fold the bills differently based on their value
    * Doesn't help when receiving money
  * **Use an electronic money reader**

# Usage #
  1. Hold the bill up against a flat surface
  1. Point the camera towards the bill and touch the screen to take a picture
  1. Hold the phone until you hear a shutter sound
  1. It will beep while processing; this might take a minute
  1. It will report either an error message or the denomination of the bill

# Architecture #
![http://pp-mcreader.googlecode.com/files/CurrencyReader.gif](http://pp-mcreader.googlecode.com/files/CurrencyReader.gif)

Due to the limited processing resources on current mobile devices, the system uses a client-server architecture.  The mobile device client is responsible for taking images and uploading them to the server.  The back-end server then performs the heavyweight image recognition using computer vision techniques.  Once a match has been identified, the server responds to the client with the identified value.  Upon receiving a response, the client indicates the bill denomination to the user via audio cues.