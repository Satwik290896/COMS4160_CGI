//! \file       image_utils.cc
//! \brief      Misc functions for image manipulation
//! \author     Hadi Fadaifard, 2022

#include "core/image_utils.h"
#include <boost/filesystem.hpp>
#include <spdlog/spdlog.h>
#include <cmath>

namespace olio {
namespace utils {

bool GammaCorrectImage64FC3(const cv::Mat &in_image, cv::Mat &out_image, Real gamma)
{
  // ======================================================================
  // *** Homework: Implement function to gamma correct an input image
  // with 64-bit floating point channels. This is the main type of
  // image that our renderer will produce. We will need to call this
  // function to gamma correct the image before saving it as a non-HDR
  // image (e.g., PNG format). Implement:
  // 0. Make sure to first read Sec. 3.2.2 of the textbook. Note that
  //    the intensity values for each pixel for CV_64F images is a
  //    double precision float. Most of the pixel intensities are in
  //    [0, 1]. There is no need to normalize the intensities (divide
  //    by max pixel intensity) before gamma correction:
  //      p' = p^(1/gamma)
  // 1. Remove placeholder code between START/END OF YOUR CODE lines
  // 2. If input image is not of type CV_64FC3, return false
  // 3. If input gamma is not greater than 0, return false
  // 4. If gamma == 1, set out_image = in_image and return true
  // 5. Allocate a temporary image of type cv::Mat with the same
  //    dimensions and type (CV_64FC3) as the input image
  // 6. Gamma correct the input image and save the result in the
  //    temporary image
  // 7. set out_image equal to the temporary image and return true
  // ======================================================================
  // ***** START OF YOUR CODE (DO NOT DELETE/MODIFY THIS LINE) *****
  //Check if gamma=0?? - Check with TA
  if((in_image.type() != CV_64FC3) || (gamma <= 0))
  {
    return false;
  }
  else{
    std::cout << "Output HEre!!!Output Here!!!" << std::endl;
    if (gamma == 1)
    {
      out_image = in_image;
    }
    else{
      cv::Mat Temp_image(in_image.rows, in_image.cols, in_image.type());
      cv::Vec3d V;
      for(int i = 0; i < in_image.rows; i++){
        for(int j = 0; j < in_image.cols; j++){
          V = in_image.at<cv::Vec3d>(i, j);
          V[0] = pow(V[0],1/gamma);
          V[1] = pow(V[1],1/gamma);
          V[2] = pow(V[2],1/gamma);
          Temp_image.at<cv::Vec3d>(i, j) = V;
        }
      }
      //V = in_image.at<cv::Vec3d>(0, 0);
      //std::cout << "Output HEre!!!Output Here!!! in_image:  " << V[1] << std::endl;
      out_image = Temp_image;
    }
    return true;
  }
  //return false;  //!< remove this line and add your own code
  // ***** END OF YOUR CODE (DO NOT DELETE/MODIFY THIS LINE) *****
}


bool GammaCorrectImage8UC3(const cv::Mat &in_image, cv::Mat &out_image, Real gamma)
{
  // ======================================================================
  // *** Homework: Implement function to gamma correct an input image
  // with 8-bit channels. Most non-HDR images that we read will be of
  // this type. Implement:
  // 0. Make sure to first read Sec. 3.2.2 of the textbook. Note that
  //    the range of intensity values for CV_8U images is [0, 255] and
  //    that they need to be normalized (divided by the max intensity)
  //    before gamma correction:
  //      p' = (p/MaxIntensity)^(1/gamma)
  // 1. Remove placeholder code between START/END OF YOUR CODE lines
  // 2. Ensure input image is of type CV_8UC3
  // 3. The code for this function is going to be similar to
  //    GammaCorrectImage64FC3(), except that we'll use a lookup table
  //    (LUT) to implement this function: since the range of values
  //    each channel of each pixel can take is very limited (256 of
  //    them), we can precompute the gamma corrected value for each
  //    and save the result in a 256-dimensional array. Review the
  //    thresholding example in the slides and implement this function
  //    with a lookup table. Make sure to perform similar checks for
  //    valid input as in GammaCorrectImage64FC3().
  // ======================================================================
  // ***** START OF YOUR CODE (DO NOT DELETE/MODIFY THIS LINE) *****
  //Check if gamma=0?? - Check with TA
  if((in_image.type() != CV_8UC3) || (gamma <= 0))
  {
    return false;
  }
  else{
    double LUT[256] = {0};
    for(int lut = 0; lut<256; lut++)
    {
      //Is Floor precsion correct?? - Ask TA
      LUT[lut] = floor(255.00 * pow((lut/255.00),(1/gamma)));
    }
    std::cout << "Output HEre!!! We entered" << std::endl;
    cv::Mat Temp_Image(in_image.rows, in_image.cols, in_image.type());
    cv::Vec3b V;
    for(int i=0; i< in_image.rows; i++){
      for(int j=0; j < in_image.cols; j++){
        V = in_image.at<cv::Vec3b>(i,j);
        V[0] = LUT[V[0]];
        V[1] = LUT[V[1]];
        V[2] = LUT[V[2]];
        Temp_Image.at<cv::Vec3b>(i,j) = V;
      }
    }
    out_image = Temp_Image;
    return true;
  }
  //return false;  //!< remove this line and add your own code
  // ***** END OF YOUR CODE (DO NOT DELETE/MODIFY THIS LINE) *****
}


bool GammaCorrectImage(const std::string &in_name, const std::string &out_name,
                       Real gamma)
{
  namespace fs = boost::filesystem;

  // check if input image is an exr image
  bool is_exr = fs::path(in_name).extension().string() == ".exr";
  if (is_exr) {
    spdlog::error("GammaCorrectImage: Input image is HDR -- "
                  "not gamma correcting...");
    return false;
  }

  // ======================================================================
  // *** Homework:
  // 0. Remove placeholder code between START/END OF YOUR CODE lines
  // 1. Read input image in_name using cv::imread(...) with this flag:
  //    cv::IMREAD_ANYDEPTH | cv::IMREAD_COLOR
  // 2. Check if image's bit/color depth is CV_8U. If not, return false
  // 3. Gamma correct the image by calling GammaCorrectImage8UC3()
  // 4. Write out the gamma corrected image to out_name with cv::imwrite()
  // ======================================================================
  // ***** START OF YOUR CODE (DO NOT DELETE/MODIFY THIS LINE) *****
  cv::Mat in_image = cv::imread (in_name, cv::IMREAD_ANYDEPTH | cv::IMREAD_COLOR);
  if(in_image.depth() == CV_8U)
  {
    cv::Mat out_image(in_image.rows, in_image.cols, in_image.type());
    bool B = GammaCorrectImage8UC3(in_image, out_image, gamma);

    if(B == true){
      cv::imwrite(out_name, out_image);
      return true;
    }
    else{
      //GammaCorrectImage8UC3 returns false - Check with TA
      return false;
    }
  }
  else{
    return false;
  }


  //return false;  //<! remove this line and add your own code
  // ***** END OF YOUR CODE (DO NOT DELETE/MODIFY THIS LINE) *****
}

}  // namespace utils
}  // namespace olio
