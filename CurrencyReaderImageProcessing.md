# Mobile Currency Reader #

## Compiling & Installing currencyreader.cpp ##

To compile the currencyreader.cpp program, cd into vlfeat.
You must first install vlfeat before you can compile currencyreader.cpp.
Find the package at http://www.vlfeat.org/.
Download the latest version for your system and extract it to
a folder called vl.  You should see files called sift.h and
generic.h in the vl folder.
Once that is done, you can return to your install directory, where
currencyreader.cpp is located, and run `make`.
This should compile the program and give you an executable called "cr".

## Running the Android application ##

To run the application follow the instructions on the Android
Developers website.  The files for the app can be found in
MCReader.

## Alternative Solution (In progress) ##

Assume: We have a raw imagine name image.jpg

  1. We input the image file into an already trained classifier.
    1. Training classifier could takes more than a day due to size of training data.
  1. Classifier will extract the banknote from the image and input into an object identifier.
  1. The object identifier will output what bill is.

## Somewhat different alternative solution (In progress) ##

Derived from above solution.

  1. We will train the five different classifiers. So we have about different classifiers for each banknote $1, $5, $10, $20, $100.
  1. Classifiers will tell you if it found object in the image.
  1. If Sum of classifier's output doesn't add up to one bill, then it is ERROR.

## Correlation Approach ##

Extract the bill and run normalized cross correlation with sample templates and return the denomination corresponding to the template that gave the maximum value.
Source code added in the Downloads Section.