#include <opencv2/highgui/highgui.hpp>
//#include <opencv2/features2d/features2d.hpp>
//#include <opencv2/calib3d/calib3d.hpp>
#include <opencv2/imgproc/imgproc.hpp>

#include <iostream>

using namespace std;
using namespace cv;

namespace utils_methods{
  class utils{
    public :
      float norm_pixel(Mat img, int i, int j);
      float mean_vec(vector<float> vec);
      float std_vec(vector<float> vec);
      float is_close(Mat images[], int i, int j, int img0, int img1, float threshold2);
      float is_close_2(Mat images[], int i, int j, int img0, int img1);
      float dist_nn(int nb_images, Mat images[], int img_index, int i, int j);
      float norm_vec(vector<float> v);
      float dist_target(int nb_images, Mat images[], int img_index, int i, int j, float beta, Mat target);
      vector<float> soustraction(vector<float> vec1, vector<float> vec2);
      vector<float> soustraction_2(Vec3f vec1, Vec3f vec2);
      void transition(Mat &obstruction, int i, int j);
      void obs(Mat obstructions[], int obstructions_count[], int nb_images, Mat images[]);
      void harmonization(int nb_images, Mat images[], Mat obstructions[]);
      void permutation_int(int *a, int *b);
      void permutation_mat(Mat *a, Mat *b);
      void quick_sort(Mat images[], Mat obstructions[], int obstructions_count[], int first, int last);
      void sort_images(Mat images[], Mat obstructions[], int obstructions_count[], int nb_images);
  };
}
