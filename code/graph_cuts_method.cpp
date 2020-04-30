/*
This method aims at implementing the graph-cut optimization method described in the "Interactive Digital Photomontage" research article by the University of Washington
*/

#include "graph_cuts_method.hpp"

using namespace graph_cuts;

///////////////////////////////////////////////////////////////////////////
///////////////////////// GRAPH CUT METHOD ////////////////////////////////
///////////////////////////////////////////////////////////////////////////

//This function takes as an input an image I, and fills grax_x and grad_y with its horizontal and vertical gradient values
void img_to_gradient(const Mat &I, Mat &grad_x, Mat &grad_y)
{
	int m = I.rows, n = I.cols;
	grad_x = Mat(m, n, CV_32F);
	grad_y = Mat(m, n, CV_32F);

	for (int i = 0; i < m; i++)
	{
		for (int j = 0; j < n; j++)
		{
			float ix, iy;
			if (i == 0 || i == m - 1)
				iy = 0;
			else
				iy = (float(I.at<uchar>(i - 1, j)) - float(I.at<uchar>(i + 1, j))) / 2;
			if (j == 0 || j == n - 1)
				ix = 0;
			else
				ix = (float(I.at<uchar>(i, j + 1)) - float(I.at<uchar>(i, j - 1))) / 2;
			grad_x.at<float>(i, j) = ix;
			grad_y.at<float>(i, j) = iy;
		}
	}
}

//This function returns the "gradient distance" (the Y distance in the research article, in the "Graph-cut optimization" section, page 5)
float dist_gradient(Mat gradient[], Mat result_tmp, int i0, int j0, int i1, int j1, int img_new)
{
	int img0 = (int)result_tmp.at<float>(i0, j0);
	int img1 = img_new;

	vector<float> grad00(6);
	grad00[0] = gradient[img0 * 6].at<float>(i0, j0);
	grad00[1] = gradient[img0 * 6 + 1].at<float>(i0, j0);
	grad00[2] = gradient[img0 * 6 + 2].at<float>(i0, j0);
	grad00[3] = gradient[img0 * 6 + 3].at<float>(i0, j0);
	grad00[4] = gradient[img0 * 6 + 4].at<float>(i0, j0);
	grad00[5] = gradient[img0 * 6 + 5].at<float>(i0, j0);

	vector<float> grad01(6);
	grad01[0] = gradient[img1 * 6].at<float>(i0, j0);
	grad01[1] = gradient[img1 * 6 + 1].at<float>(i0, j0);
	grad01[2] = gradient[img1 * 6 + 2].at<float>(i0, j0);
	grad01[3] = gradient[img1 * 6 + 3].at<float>(i0, j0);
	grad01[4] = gradient[img1 * 6 + 4].at<float>(i0, j0);
	grad01[5] = gradient[img1 * 6 + 5].at<float>(i0, j0);

	vector<float> grad10(6);
	grad10[0] = gradient[img0 * 6].at<float>(i1, j1);
	grad10[1] = gradient[img0 * 6 + 1].at<float>(i1, j1);
	grad10[2] = gradient[img0 * 6 + 2].at<float>(i1, j1);
	grad10[3] = gradient[img0 * 6 + 3].at<float>(i1, j1);
	grad10[4] = gradient[img0 * 6 + 4].at<float>(i1, j1);
	grad10[5] = gradient[img0 * 6 + 5].at<float>(i1, j1);

	vector<float> grad11(6);
	grad11[0] = gradient[img1 * 6].at<float>(i1, j1);
	grad11[1] = gradient[img1 * 6 + 1].at<float>(i1, j1);
	grad11[2] = gradient[img1 * 6 + 2].at<float>(i1, j1);
	grad11[3] = gradient[img1 * 6 + 3].at<float>(i1, j1);
	grad11[4] = gradient[img1 * 6 + 4].at<float>(i1, j1);
	grad11[5] = gradient[img1 * 6 + 5].at<float>(i1, j1);

	return norm_vec(soustraction(grad00, grad01)) + norm_vec(soustraction(grad10, grad11));
}

//This method returns an integer matrix with random numbers (to be used as an input for the graph cut algorithm)
Mat random_map(int nb_images, Mat images[])
{

	int rows = images[0].rows;
	int cols = images[0].cols;

	Mat result(Size(cols, rows), CV_32F);

	for (int i = 0; i < images[0].rows; i++)
	{
		for (int j = 0; j < images[0].cols; j++)
		{
			result.at<float>(i, j) = rand() % nb_images;
		}
	}
	return result;
}

//This method returns a matrix that contains, for each pixel, the image corresponding to the median value (to be used as an input for the graph cut algorithm)
Mat median_map(int nb_images, Mat images[])
{

	int rows = images[0].rows;
	int cols = images[0].cols;

	Mat result(Size(cols, rows), CV_32F);

	//Computing a score for each pixel based on the distance to the median value of each color chanel
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
			result.at<float>(i,j) = (float)index_min;
		}
	}
	return result;
}

//Implements one iteration of the graph cut algorithm
void MatToGraph(int nb_images, Mat images[], Mat gradient[], Mat &result_tmp, int img_new, float alpha, float beta, float gamma, Mat target)
{
	int rows = images[0].rows;
	int cols = images[0].cols;

	Graph<float, float, float> graph(rows * cols, 4 * rows * cols - (rows + cols));
	graph.add_node(rows * cols);

	for (int i = 0; i < rows; i++)
	{
		for (int j = 0; j < cols; j++)
		{
			graph.add_tweights(i * cols + j, dist_target(nb_images, images, img_new, i, j, beta, target), dist_target(nb_images, images, (int)result_tmp.at<float>(i, j), i, j, beta, target));
			if (j != cols - 1)
			{
				float weight = alpha * (norm_vec(soustraction_2(images[(int)result_tmp.at<float>(i, j)].at<Vec3f>(i, j), images[img_new].at<Vec3f>(i, j))) + norm_vec(soustraction_2(images[(int)result_tmp.at<float>(i, j+1)].at<Vec3f>(i, j + 1), images[img_new].at<Vec3f>(i, j + 1))));
				weight += gamma * dist_gradient(gradient, result_tmp, i, j, i, j + 1, img_new);
				//cout << weight << " ";

				graph.add_edge(i * cols + j, i * cols + (j + 1), weight, weight);
			}
			if (i != rows - 1)
			{
				float weight = alpha * (norm_vec(soustraction_2(images[(int)result_tmp.at<float>(i, j)].at<Vec3f>(i, j), images[img_new].at<Vec3f>(i, j))) + norm_vec(soustraction_2(images[(int)result_tmp.at<float>(i+1, j)].at<Vec3f>(i + 1, j), images[img_new].at<Vec3f>(i + 1, j))));
				weight += gamma * dist_gradient(gradient, result_tmp, i, j, i + 1, j, img_new);
				//cout << weight << " ";

				graph.add_edge(i * cols + j, (i + 1) * cols + j, weight, weight);
			}
		}
		//cout << endl;
	}

	int flow = graph.maxflow();
	cout << "Flow = " << flow << endl;
	cout << endl;
	for (int i = 0; i < rows; i++)
	{
		for (int j = 0; j < cols; j++)
		{
			if (graph.what_segment(i * cols + j) != Graph<float, float, float>::SOURCE)
			{
				result_tmp.at<float>(i, j) = img_new;
			}
		}
	}
}

//Implements the graph-cut algorithm
Mat graph_cuts_method(int nb_images, Mat images[], Mat gradient[], float alpha, float beta, float gamma, int epochs, Mat obstructions[])
{

	//Computing gradients
	cout << "Computing gradients..." << endl;
	Mat bgr[3];

	for (int i = 0; i < nb_images; i++)
	{
		//cout << i << endl;
		split(images[i], bgr);
		img_to_gradient(bgr[0], gradient[i * 6], gradient[i * 6 + 1]);
		img_to_gradient(bgr[1], gradient[i * 6 + 2], gradient[i * 6 + 3]);
		img_to_gradient(bgr[2], gradient[i * 6 + 4], gradient[i * 6 + 5]);
	}

	///////////////////////////////////////////////////
	////// Choose starting map for the algotithm //////
	///////////////////////////////////////////////////

	cout << "Initializing starting map..." << endl;
	Mat result_tmp;

	//result_tmp = random_map(nb_images,images);
	//result_tmp = median_map(nb_images,images);
	result_tmp = Mat::zeros(images[0].rows, images[0].cols, CV_32F);

    ///////////////////////////////////////////
	////// Choose pixelwise target image //////
    ///////////////////////////////////////////

	cout << "Initializing target image..." << endl;
	Mat target;

	target = median_method(nb_images, images);
	//target = random_method(nb_images, images, obstructions);

	target.convertTo(target, CV_32FC3);

    ///////////////////////////////////////////
	////// Iterate graph cut algorithm   //////
    ///////////////////////////////////////////

	cout << "Beginning graph cut algotithm..." << endl;

	for (int k = 0; k < nb_images*epochs; k++)
	{
		cout << "Epoch : " << (k/nb_images)+1 << " Image : " << k%nb_images+1 << endl;
		MatToGraph(nb_images, images, gradient, result_tmp, k % nb_images, alpha, beta, gamma, target);
	}

    /////////////////////////////////
	////// Draw result image   //////
    /////////////////////////////////

	cout << "Drawing result..." << endl;

	Mat result(Size(images[0].cols, images[0].rows), CV_32FC3);

	for (int i = 0; i < images[0].rows; i++)
	{
		for (int j = 0; j < images[0].cols; j++)
		{
			result.at<Vec3f>(i, j) = images[(int)result_tmp.at<float>(i, j)].at<Vec3f>(i, j);
		}
	}
	result.convertTo(result, CV_8UC3);
	return result;
}
