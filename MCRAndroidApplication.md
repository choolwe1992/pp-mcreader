Overview of the Android application part of the project

# Design Goals #
Create an accessible, easy-to-use interface with clear indicators for the state of the application.
  * Accessibility:
    * Use voice and sound to indicate errors
    * Use sound to indicate processing time so the user knows that the application is doing something
    * Use voice to communicate the result of the image processing
  * Ease of use:
    * One button interface: point and touch the screen
    * On error, fail quickly so the user can try again as soon as possible

# Class Structure #
This section is for people who are interested in making changes to our Android application.
Based on our admittedly limited understanding of Android development:
  * The activity is the main function that is called when the application starts
  * The preview is the surface that contains the camera preview box
    * It holds the camera as well as displays the camera preview
    * A function that takes the picture is called when the preview surface is touched
  * The handler is created solely to get messages from the worker thread, this allows us to have a separate thread for work and display
  * The worker thread sends the image data to our server

# Details #
  * Camera preview is started
  * Touching the surface takes a photo and sends to the server as HTTP POST
  * Plays a "waiting" sound to notify the user that processing is in progress
  * Reads the response from the server
  * Plays audio corresponding to the result of the processing.

## Relevant Links ##
[Android Developers](http://developer.android.com/index.html)

[Android Examples - Camera](http://marakana.com/forums/android/android_examples/39.html)