# BackgroundReconstruction
Background reconstruction to remove obstacles from image in C++


This is the resulting code of our project "BACKGROUND RECONSTRUCTION TO REMOVE OBSTACLES FROM IMAGES" for the course INF573. The two members of our team are Alodie Boissonnet and Paul Jacob.

In order to run the code, make sure to have OpenCV installed.

The data images are provided in the "data" directory.
The results will be stored in the "results" directory.

Depending on the dataset that you are using, the different methods can give satisfying results or not. We recommand you to try datasets with different amounts, sizes, frequencies of obstructing objects.

All the settings that you should change to test our code with different parameters are located in the "main.cpp" file, in the main function.

# TO LAUNCH THE PROJECT:

- Delete the build directory
- Go in the source directory in the console
- Type "mkdir build", "cd build", "cmake .." then "make"
- Type "./main" to launch the program


## GENERAL SETTINGS:

- rootdir: The folder path to images. They should be stored in .JPG format, and called "img0.jpg", "img1.jpg"...

- nb_images: The number of images considered by the methods. The methods will take as an input the first images. For example if nb_images is set to 5, the methods will only take into account the images between "img0.jpg" and "img4.jpg".

- height: The resolution of the images for the algorithms. A higher resolution means more time for the methods to compute (especially the graph-cut one, which is time-consuming).


## METHODS TO TRY:

Change to "true" or "false" those booleans to try only the methods that you want to for your dataset.


## GRAPH CUT SETTINGS:

The best parameters depend a lot on the dataset that you are using, especially because of the relative size of the obstacles in the images, and their frequency. Feel free to change them.
