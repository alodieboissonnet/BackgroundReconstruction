/*
This files aims at creating a new image by computing the median of the data set for each pixel.
*/

#include "median_method.hpp"

using namespace median;

///////////////////////////////////////////////////////////////////////////
//////////////////////////// MEDIAN METHOD ////////////////////////////////
///////////////////////////////////////////////////////////////////////////

Mat median_method(int nb_images, Mat images[]) {
	int rows = images[0].rows;
	int cols = images[0].cols;

	Mat result(Size(cols, rows), CV_32FC3);

	// to choose the best median pixel, we compute a score for each pixel based on the distance to the median value of each color chanel
	float scores[nb_images];

	for (int i = 0; i < images[0].rows; i++) {
		for (int j = 0; j < images[0].cols; j++) {
			vector<float> values_blue;
			vector<float> values_green;
			vector<float> values_red;

			for (int k = 0; k < nb_images; k++) {
				values_blue.push_back(images[k].at<Vec3f>(i, j)[0]);
				values_green.push_back(images[k].at<Vec3f>(i, j)[1]);
				values_red.push_back(images[k].at<Vec3f>(i, j)[2]);
			}

			sort(values_blue.begin(), values_blue.end());
			sort(values_green.begin(), values_green.end());
			sort(values_red.begin(), values_red.end());

			// median value of each color chanel
			Vec3f value_new;
			value_new[0] = values_blue[nb_images / 2];
			value_new[1] = values_green[nb_images / 2];
			value_new[2] = values_red[nb_images / 2];

			// score of each pixel to choose the best median pixel
			int index_min = 0;
			for (int k = 0; k < nb_images; k++) {
				scores[k] = sqrt(pow(value_new[0] - images[k].at<Vec3f>(i,j)[0], 2) + pow(value_new[1] - images[k].at<Vec3f>(i,j)[1], 2) + pow(value_new[2] - images[k].at<Vec3f>(i,j)[2], 2));
				if (scores[k] < scores[index_min]) {
					index_min = k;
				}
			}
			result.at<Vec3f>(i,j) = images[index_min].at<Vec3f>(i,j);
		}
	}

	// we convert the result image to the correct type to store it
	result.convertTo(result, CV_8UC3);
	return result;
}
