namespace graph_cuts
{
class graph_cuts
{
public:
  void img_to_gradient(const Mat &I, Mat &grad_x, Mat &grad_y);
  float dist_gradient(Mat gradient[], Mat result_tmp, int i0, int j0, int i1, int j1);
  Mat random_map(int nb_images, Mat images[]);
  Mat median_map(int nb_images, Mat images[]);
  void MatToGraph(int nb_images, Mat images[], Mat &result_tmp, int img_new, bool gradient, float threshold2, float alpha, float beta);
  Mat graph_cuts_method(int nb_images, Mat images[], Mat gradient[], float alpha, float beta, float gamma, int epochs, Mat obstructions[]);
};
} // namespace graph_cuts
