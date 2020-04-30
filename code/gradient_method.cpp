/*
This method aims at filling obstructed areas on an image by copying gradients from other images and computing missing pixels.
*/

#include "gradient_method.hpp"

using namespace gradient;

///////////////////////////////////////////////////////////////////////////
////////////////////////////// GRADIENT METHOD ////////////////////////////
///////////////////////////////////////////////////////////////////////////

Mat gradient_simple_method(int nb_images, Mat images[], Mat obstructions[]) {
	int rows = images[0].rows;
	int cols = images[0].cols;

	Mat result(Size(cols, rows), CV_32FC3);
	images[0].copyTo(result);

	// areas to be removed
	Mat obstruction = obstructions[nb_images];

	// gradient values for each direction and each color chanel
	float Ix_blue, Iy_blue;
	float Ix_green, Iy_green;
	float Ix_red, Iy_red;

	// we fill obstructed areas of the first image by copying gradients from other images and computing missing pixels
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
			if (k == nb_images)
			{
				result.at<Vec3f>(i, j) = images[0].at<Vec3f>(i, j);
			}
			// when there is no obstruction or where we can not compute the gradient, we copy pixels
			else if (k == 0 || i == 0 || j == 0){
				result.at<Vec3f>(i, j) = images[k].at<Vec3f>(i, j);
			}
			// we compute gradients with the difference method and then determine the value of missing pixels
			else
			{
				Ix_blue = (images[k].at<Vec3f>(i, j)[0] - images[k].at<Vec3f>(i - 1, j)[0]) * 0.5;
				Iy_blue = (images[k].at<Vec3f>(i, j)[0] - images[k].at<Vec3f>(i, j - 1)[0]) * 0.5;
				Ix_green = (images[k].at<Vec3f>(i, j)[1] - images[k].at<Vec3f>(i - 1, j)[1]) * 0.5;
				Iy_green = (images[k].at<Vec3f>(i, j)[1] - images[k].at<Vec3f>(i, j - 1)[1]) * 0.5;
				Ix_red = (images[k].at<Vec3f>(i, j)[2] - images[k].at<Vec3f>(i - 1, j)[2]) * 0.5;
				Iy_red = (images[k].at<Vec3f>(i, j)[2] - images[k].at<Vec3f>(i, j - 1)[2]) * 0.5;

				result.at<Vec3f>(i, j)[0] = Ix_blue + Iy_blue + 0.5 * (result.at<Vec3f>(i - 1, j)[0] + result.at<Vec3f>(i, j - 1)[0]);
				result.at<Vec3f>(i, j)[1] = Ix_green + Iy_green + 0.5 * (result.at<Vec3f>(i - 1, j)[1] + result.at<Vec3f>(i, j - 1)[1]);
				result.at<Vec3f>(i, j)[2] = Ix_red + Iy_red + 0.5 * (result.at<Vec3f>(i - 1, j)[2] + result.at<Vec3f>(i, j - 1)[2]);
			}
		}
	}

	// we convert the result image to the correct type to store it
	result.convertTo(result, CV_8UC3);
	return result;
}
