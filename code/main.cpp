#include "../maxflow/graph.h"
#include "utils.cpp"
#include "median_method.cpp"
#include "histogram_method.cpp"
#include "copy_method.cpp"
#include "gradient_method.cpp"
#include "random_method.cpp"
#include "graph_cuts_method.cpp"

///////////////////////////////////////////////////////////////////////////
///////////////////// MAIN FUNCTION ///////////////////////////////////////
///////////////////////////////////////////////////////////////////////////


int main()
{

	//GENERAL SETTINGS : Feel free to change the parameters

	string rootdir = "../data/Set1/"; //all images must be stored in this folder, and they name should be "img1.jpg", "img2.jpg", etc.
	const int nb_images = 25;				  //nb_images stores the number of images that are considered by the program
	int height = 500;				  //height stores the size of the images that we consider. A higher height gives a better resolution, but more time to compute the results

	//METHODS TO TRY : Change to "false" the methods that you do not want to try at this time

	bool do_median = true;
	bool do_histogram = true;
	bool do_copy = true;
	bool do_gradient = true;
	bool do_random = true;
	bool do_graph_cut = true;

	//Loading the images
	Mat images[nb_images];
	for (int i = 1; i < nb_images + 1; i++)
	{
		string path = rootdir + "img" + to_string(i) + ".jpg";
		images[i - 1] = imread(path);
	}

	//Conversion
	for (int i = 0; i < nb_images; i++)
	{
		images[i].convertTo(images[i], CV_32FC3);
	}

	//Resize
	for (int i = 0; i < nb_images; i++)
	{
		double ratio = float(height) / images[i].size().height;
		resize(images[i], images[i], Size((int)(ratio * images[i].size().width), height), 0, 0, INTER_NEAREST);
	}

	//Determining obstructed areas on all images and store the number of obstructed pixels for each image
	//The last matrix of the tab obstructions gathers obstruction areas of all the data set
	Mat obstructions[nb_images + 1];
	int obstructions_count[nb_images];
	for (int i = 0; i < nb_images + 1; i++)
	{
		obstructions[i] = Mat::zeros(images[0].rows, images[0].cols, CV_32F);
		if (i != nb_images)
		{
			obstructions_count[i] = 0;
		}
	}
	obs(obstructions, obstructions_count, nb_images, images);

	//Equalizing the intensity of images outside of all obstruction areas to avoid visible transitions between images :
	harmonization(nb_images, images, obstructions);

	//Sorting images according to their numbers of obstructed pixels (uncomment if you want to do it) :
	sort_images(images, obstructions, obstructions_count, nb_images);

	//Median method

	if (do_median)
	{
		cout << "Median method..." << endl;
		Mat result_median = median_method(nb_images, images);
		imshow("Result Median", result_median);
		imwrite("../results/result_median.jpg", result_median);
	}

	//Histogram method according to different interval sizes

	if (do_histogram)
	{
		cout << "Histogram method..." << endl;

		Mat result5 = histogram_method(nb_images, images, 5);
		imshow("Result Histo 5", result5);
		imwrite("../results/result_histo_5.jpg", result5);

		Mat result10 = histogram_method(nb_images, images, 10);
		imshow("Result Histo 10", result10);
		imwrite("../results/result_histo_10.jpg", result10);

		Mat result20 = histogram_method(nb_images, images, 20);
		imshow("Result Histo 20", result20);
		imwrite("../results/result_histo_20.jpg", result20);

		Mat result40 = histogram_method(nb_images, images, 40);
		imshow("Result Histo 40", result40);
		imwrite("../results/result_histo_40.jpg", result40);
	}

	//Copy method

	if (do_copy)
	{
		cout << "Copy method..." << endl;
		Mat result_copy = copy_method(nb_images, images, obstructions);
		imshow("Result Copy", result_copy);
		imwrite("../results/result_copy.jpg", result_copy);
	}

	//Gradient method

	if (do_gradient)
	{
		cout << "Gradient method..." << endl;
		Mat result_gradient = gradient_simple_method(nb_images, images, obstructions);
		imshow("Result Gradient", result_gradient);
		imwrite("../results/result_gradient.jpg", result_gradient);
	}

	//Random method

	if (do_random)
	{
		cout << "Random method..." << endl;
		Mat result_random = random_method(nb_images, images, obstructions);
		imshow("Result Random", result_random);
		imwrite("../results/result_random.jpg", result_random);
	}

	//Graph cut method : feel free to change the parameters to get better results (best values depend a lot on the image set)

	//GRAPH CUT SETTINGS

	float alpha = 0.1; //Quantifies the importance of the change in color in neigbourgh pixels
	float beta = 1;  //Quantifies the importance of the target pixel (for example, the mean pixel)
	float gamma = 0.1; //Quantifies the importance of the change in gradient in neigbourgh pixels
	int epochs = 1;  //Quantifies the number of epochs for the algorithm

	if (do_graph_cut)
	{
		cout << "Graph cut method..." << endl;
		Mat gradient[nb_images * 6];
		Mat result_graph_cut = graph_cuts_method(nb_images, images, gradient, alpha, beta, gamma, epochs, obstructions);
		imshow("Result Graph Cuts", result_graph_cut);
		imwrite("../results/result_graph_cut_" + to_string(alpha) + "__" + to_string(beta) + "__" + to_string(gamma) + ".jpg", result_graph_cut);
	}

	waitKey(0);
	return 0;
}
