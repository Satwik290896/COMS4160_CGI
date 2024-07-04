//! \file       image_utils.h
//! \brief      Misc functions for image manipulation
//! \author     Hadi Fadaifard, 2022

#pragma once

#include <iostream>
#include <opencv2/opencv.hpp>
#include "core/types.h"

namespace olio {
namespace utils {

bool ThresholdImage8UC3_naive(const cv::Mat &in_image, cv::Mat &out_image,
			      uchar threshold);
bool ThresholdImage8UC3(const cv::Mat &in_image, cv::Mat &out_image,
			uchar threshold);
bool GammaCorrectImage64FC3(const cv::Mat &in_image, cv::Mat &out_image,
			    Real gamma);
bool GammaCorrectImage8UC3(const cv::Mat &in_image, cv::Mat &out_image,
			   Real gamma);
bool GammaCorrectImage(const std::string &in_name, const std::string &out_name,
                       Real gamma);

}  // namespace utils
}  // namespace olio
