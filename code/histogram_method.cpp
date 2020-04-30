/*
This files aims at creating a new image by computing a histogram for each pixel and color channel.
*/

#include "histogram_method.hpp"

using namespace histogram;

///////////////////////////////////////////////////////////////////////////
/////////////////////////// HISTOGRAM METHOD //////////////////////////////
///////////////////////////////////////////////////////////////////////////

Mat histogram_method(int nb_images, Mat images[], int nb_intervals)
{

	int rows = images[0].rows;
	int cols = images[0].cols;

	Mat result(Size(cols, rows), CV_32FC3);

	//On each pixel location, we'll score images based on the appearances of their 3 color values at this location in the 3 histograms
	int scores[nb_images];

	for (int i = 0; i < images[0].rows; i++)
	{
		for (int j = 0; j < images[0].cols; j++)
		{

			//Initializing the 3 histograms
			int histogram_blue[nb_intervals];
			int histogram_green[nb_intervals];
			int histogram_red[nb_intervals];

			for (int k = 0; k < nb_intervals; k++)
			{
				histogram_blue[k] = 0;
				histogram_green[k] = 0;
				histogram_red[k] = 0;
			}

			//Filling the histograms
			for (int k = 0; k < nb_images; k++)
			{
				histogram_blue[(int)(images[k].at<Vec3f>(i, j)[0] * nb_intervals / 256)]++;
				histogram_green[(int)(images[k].at<Vec3f>(i, j)[1] * nb_intervals / 256)]++;
				histogram_red[(int)(images[k].at<Vec3f>(i, j)[2] * nb_intervals / 256)]++;
			}

			//Scoring the images for this particular pixel
			int index_max = 0;
			for (int k = 0; k < nb_images; k++)
			{
				scores[k] = histogram_blue[(int)(images[k].at<Vec3f>(i, j)[0] * nb_intervals / 256)] + histogram_green[(int)(images[k].at<Vec3f>(i, j)[0] * nb_intervals / 256)] + histogram_red[(int)(images[k].at<Vec3f>(i, j)[0] * nb_intervals / 256)];
				if (scores[k] > scores[index_max])
				{
					index_max = k;
				}
			}

			//Picking the best pixel, and filling it in the result
			result.at<Vec3f>(i, j) = images[index_max].at<Vec3f>(i, j);
		}
	}

	result.convertTo(result, CV_8U);
	return result;
}
