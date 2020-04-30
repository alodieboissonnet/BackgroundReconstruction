/*
This method aims to filling obstructed areas on an image by copying pixels from other images.
*/


#include "copy_method.hpp"

using namespace copy;

///////////////////////////////////////////////////////////////////////////
/////////////////////////////COPY METHOD //////////////////////////////////
///////////////////////////////////////////////////////////////////////////

Mat copy_method(int nb_images, Mat images[], Mat obstructions[]) {
	int rows = images[0].rows;
	int cols = images[0].cols;

	Mat result(Size(cols, rows), CV_32FC3);

	// areas to be removed
	Mat obstruction = obstructions[nb_images];

	// we fill obstructed areas of the first image by coping pixels from another not obstructed image
	for (int i = 0; i < images[0].rows; i++)
	{
		for (int j = 0; j < images[0].cols; j++)
		{
			// we look for the first image with no obstruction at (i,j)
			int k = 0;
			while (k < nb_images && obstructions[k].at<float>(i, j) < 0) {
				k++;
			}

			// all images are obstructed: we choose a pixel from the first image
			if (k == nb_images){
				result.at<Vec3f>(i, j) = images[0].at<Vec3f>(i, j);
			}
			// we found a non-obstructed image at (i,j)
			else {
				result.at<Vec3f>(i, j) = images[k].at<Vec3f>(i, j);
			}
		}
	}

	// we convert the result image to the correct type to store it
	result.convertTo(result, CV_8UC3);
	return result;
}
