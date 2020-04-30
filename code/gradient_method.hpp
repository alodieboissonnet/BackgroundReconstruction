namespace gradient {
  class gradient {
    public :
      float value_from_gradient(Mat &Ic, const float &Ix, const float &Iy, int i, int j);
      void scharr(const Mat &I, float &Ix, float &Iy, int i, int j);
      Mat gradient_scharr_method(int nb_images, Mat images[], Mat obstructions[]);
      Mat gradient_simple_method(int nb_images, Mat images[], Mat obstructions[]);

  };
}
