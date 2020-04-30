/*
This files aims at creating a new image by choosing randomly pixels from images.
*/

#include "random_method.hpp"

using namespace rd;

///////////////////////////////////////////////////////////////////////////
//////////////////////////// RANDOM METHOD ////////////////////////////////
///////////////////////////////////////////////////////////////////////////

Mat random_method(int nb_images, Mat images[], Mat obstructions[]) {
	int rows = images[0].rows;
	int cols = images[0].cols;

	Mat result(Size(cols, rows), CV_32FC3);

	for (int i=0; i<rows; i++){
		for (int j=0; j<cols; j++){
			int random = rand() % nb_images;

			// if the chosen pixel belongs to an obtructed area, we take another one randomly
			// in the case of the pixel is always obstructed, we stop the loop after 2*nb_images tries
			int k = 0;
			while (k < 2*nb_images && obstructions[random].at<float>(i,j) < 0){
				random = rand() % nb_images;
				k++;
			}

			result.at<Vec3f>(i,j) = images[random].at<Vec3f>(i,j);
		}
	}

	// we convert the result image to the correct type to store it
	result.convertTo(result, CV_8UC3);
	return result;
}
