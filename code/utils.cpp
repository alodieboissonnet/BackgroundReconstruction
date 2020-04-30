/*
This file aims at providing useful methods for every other files.
*/

#include "utils.hpp"

using namespace utils_methods;

// we compute the mean of the norm of pixels from a 2x2 area around a pixel (i,j)
float norm_pixel(Mat img, int i, int j) {
	Vec3f value00 = img.at<Vec3f>(i, j);
	Vec3f value01 = img.at<Vec3f>(i, j+1);
	Vec3f value10 = img.at<Vec3f>(i+1, j);
	Vec3f value11 = img.at<Vec3f>(i+1, j+1);
	float norm00 = sqrt(pow(value00[0], 2.0f) + pow(value00[1], 2.0f) + pow(value00[2], 2.0f));
	float norm01 = sqrt(pow(value01[0], 2.0f) + pow(value01[1], 2.0f) + pow(value01[2], 2.0f));
	float norm10 = sqrt(pow(value10[0], 2.0f) + pow(value10[1], 2.0f) + pow(value10[2], 2.0f));
	float norm11 = sqrt(pow(value11[0], 2.0f) + pow(value11[1], 2.0f) + pow(value11[2], 2.0f));
	return (norm00 + norm01 + norm10 + norm11) / 4.f;
}


// we compute the mean of a vector
float mean_vec(vector<float> vec)
{
	float mean = 0;
	for (int i = 0; i < vec.size(); i++)
		mean += vec.at(i);
	return mean / vec.size();
}


// we compute the standard deviation of a vector
float std_vec(vector<float> vec)
{
	float mean = mean_vec(vec);
	float std = 0;
	for (int i = 0; i < vec.size(); i++)
		std += pow(vec.at(i) - mean, 2);
	return (float)sqrt(std / vec.size());
}


// we compute a distance between pixels of coordinates (i,j) from two different images
float is_close(Mat images[], int i, int j, int img0, int img1) {
	Vec3f value00 = images[img0].at<Vec3f>(i, j) - images[img1].at<Vec3f>(i, j);
	Vec3f value01 = images[img0].at<Vec3f>(i, j+1) - images[img1].at<Vec3f>(i, j+1);
	Vec3f value10 = images[img0].at<Vec3f>(i+1, j) - images[img1].at<Vec3f>(i+1, j);
	Vec3f value11 = images[img0].at<Vec3f>(i+1, j+1) - images[img1].at<Vec3f>(i+1, j+1);

	float dist2_00 = pow(value00[0], 2.0f) + pow(value00[1], 2.0f) + pow(value00[2], 2.0f);
	float dist2_01 = pow(value01[0], 2.0f) + pow(value01[1], 2.0f) + pow(value01[2], 2.0f);
	float dist2_10 = pow(value10[0], 2.0f) + pow(value10[1], 2.0f) + pow(value10[2], 2.0f);
	float dist2_11 = pow(value11[0], 2.0f) + pow(value11[1], 2.0f) + pow(value11[2], 2.0f);
	return (dist2_00 + dist2_01 + dist2_10 + dist2_11) / 4.f;
}


// we compute the distance to the two nearest neighbours of each image
float dist_nn(int nb_images, Mat images[], int img_index, int i, int j) {
	float min0 = FLT_MAX;
	float min1 = FLT_MAX;

	for (int k = 0; k < nb_images; k++){
		if (k != img_index){
			float d = is_close(images, i, j, img_index, k);
			if (d < min0){
				min1 = min0;
				min0 = d;
			}
			else if (d < min1){
				min1 = d;
			}
		}
	}

	return min0 + min1;
}


//Norm of a vector
float norm_vec(vector<float> v)
{
	float result = 0;
	for (int i = 0; i < v.size(); i++)
	{
		result += pow(v[i], 2.f);
	}
	return sqrt(result);
}


//Distance between a pixel and a target pixel in an image
float dist_target(int nb_images, Mat images[], int img_index, int i, int j, float beta, Mat target)
{
	Vec3f pixel = images[img_index].at<Vec3f>(i, j);
	Vec3f target_pixel = target.at<Vec3f>(i, j);
	float dist_target = beta * sqrt(pow(target_pixel[0] - pixel[0], 2.0f) + pow(target_pixel[1] - pixel[1], 2.0f) + pow(target_pixel[2] - pixel[2], 2.0f));
	return dist_target;
}


//Substraction of two vectors
vector<float> soustraction(vector<float> vec1, vector<float> vec2)
{
	vector<float> vec_sous(vec1.size());
	for (int i = 0; i < vec1.size(); i++)
	{
		vec_sous[i] = vec1[i] - vec2[i];
	}
	return vec_sous;
}


//Substraction of two vectors
vector<float> soustraction_2(Vec3f vec1, Vec3f vec2)
{
	vector<float> vec_sous(3);
	for (int i = 0; i < 3; i++)
	{
		vec_sous[i] = vec1[i] - vec2[i];
	}
	return vec_sous;
}

///////////////////////////////////////////////////////////////////////////
///////////////////////// OBSTRUCTION DETECTION ///////////////////////////
///////////////////////////////////////////////////////////////////////////

// we enlarge obstructed area to avoid any difficulties with transition
void transition(Mat &obstruction, int i, int j){
	for (int k=-3; k<4; k++){
		for (int l=-3; l<4; l++){
			if (i+k >= 0 && i+k < obstruction.rows && j+l >= 0 && j+l < obstruction.cols){
				obstruction.at<float>(i+k,j+l) = -1.f;
			}
		}
	}
}


// we determine obstructed areas thanks to the distance to nearest neighbours
void obs(Mat obstructions[], int obstructions_count[], int nb_images, Mat images[]) {
	for (int i = 0; i < images[0].rows; i++) {
		for (int j = 0; j < images[0].cols; j++) {
			if (i != images[0].rows-1 && j != images[0].cols-1){
				vector<float> values_dist;
				for (int k = 0; k < nb_images; k++){
					float dist_new = dist_nn(nb_images, images, k, i, j);
					values_dist.push_back(dist_new);
				}

				for (int k = 0; k < nb_images; k++) {
					if (abs(values_dist[k]) > 1000) {
						transition(obstructions[nb_images], i, j);
						transition(obstructions[k], i, j);
						obstructions[nb_images].at<float>(i, j) = -1.0f;
						obstructions[k].at<float>(i, j) = -1.0f;
						obstructions_count[k] ++;
					}

				}


				// we first looked at the difference between the norm of a pixel and the median of the norm of pixels
				// but it was not efficient when areas where more often obstructed than not obstructed

				/*
				vector<float> values_norm;
				for (int k = 0; k < nb_images; k++)
					values_norm.push_back(norm_pixel(images[k], i, j));

				sort(values_norm.begin(), values_norm.end());
				float med_norm = values_norm[values_norm.size() / 2];
				float std_norm = std_vec(values_norm);


				for (int k = 0; k < nb_images; k++)
				{
					if (abs(norm_pixel(images[k], i, j) - med_norm) > 50)
					{
						transition(obstruction, i, j);
						obstruction.at<float>(i, j) = -1.0f;
						images[k].at<Vec3f>(i, j) = Vec3f(-1.0f, -1.0f, -1.0f);
					}
				}
				*/

			}
		}
	}
}


///////////////////////////////////////////////////////////////////////////
///////////////////// IMAGE INTENSITY HARMONIZATION ///////////////////////
///////////////////////////////////////////////////////////////////////////

// we equalize the intensity of images outside of all obstruction areas to avoid visible transitions between images
void harmonization(int nb_images, Mat images[], Mat obstructions[]){
	int rows = images[0].rows;
	int cols = images[0].cols;

	// all areas to be removede
	Mat obstruction = obstructions[nb_images];

	float means[nb_images];
	int count = 0;
	for (int k = 0; k < nb_images; k++)
		means[k] = 0;

	// we compute the mean of images outside of all obstruction areas
	for (int i = 0; i < rows; i++) {
		for (int j = 0; j < cols; j++) {
			if (i != rows-1 && j != cols-1 && obstruction.at<float>(i, j) == 0) {
				for (int k = 0; k < nb_images; k++){
					means[k] += norm_pixel(images[k], i, j);
				}
				count++;
			}
		}
	}
	for (int k = 0; k < nb_images; k++) {
		means[k] = means[k] / count;
	}

	// we equalize the intensity of images outside of all obstruction areas
	for (int i = 1; i < nb_images; i++) {
		//cout << means[0] / means[i] << endl;
		images[i].convertTo(images[i], -1, means[0] / means[i], 0);
	}
}


///////////////////////////////////////////////////////////////////////////
//////////////////////////// IMAGES SORTING ///////////////////////////////
///////////////////////////////////////////////////////////////////////////

// we use a quick sort algorithm to sort images according to their numbers of obstructed pixels

// we swap integers
void permutation_int(int *a, int *b) {
    int tmp;
    tmp = *a;
    *a = *b;
    *b = tmp;
}


// we swap matrices
void permutation_mat(Mat *a, Mat *b) {
    Mat tmp;
    tmp = *a;
    *a = *b;
    *b = tmp;
}


// quick sort algorithm
void quick_sort(Mat images[], Mat obstructions[], int obstructions_count[], int first, int last) {
    int pivot, i, j;
    if(first < last) {
        pivot = first;
        i = first;
        j = last;
        while (i < j) {
            while(obstructions_count[i] <= obstructions_count[pivot] && i < last)
                i++;
            while(obstructions_count[j] > obstructions_count[pivot])
                j--;
            if(i < j) {
                permutation_int(&obstructions_count[i], &obstructions_count[j]);
								permutation_mat(&images[i], &images[j]);
								permutation_mat(&obstructions[i], &obstructions[j]);
            }
        }
        permutation_int(&obstructions_count[pivot], &obstructions_count[j]);
				permutation_mat(&images[pivot], &images[j]);
				permutation_mat(&obstructions[pivot], &obstructions[j]);
        quick_sort(images, obstructions, obstructions_count, first, j - 1);
        quick_sort(images, obstructions, obstructions_count, j + 1, last);
    }
}

// we sort images according to their numbers of obstructed pixels
void sort_images(Mat images[], Mat obstructions[], int obstructions_count[], int nb_images){
	quick_sort(images, obstructions, obstructions_count, 0, nb_images - 1);

	// you can uncomment the following lines to store sorting images in a directory
	/*
	for (int i = 0; i < nb_images; i ++){
		Mat img_tmp;
		images[i].convertTo(img_tmp, CV_8UC3);
		imwrite("../data/Set8/img" + to_string(i+1) + ".png", img_tmp);
	}
	*/
}
