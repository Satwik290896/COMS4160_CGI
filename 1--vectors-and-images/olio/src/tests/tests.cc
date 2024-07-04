//! \file       tests.cc
//! \brief      Main test file containing the entry point for the tests
//!             executable

#include <iostream>

#define CATCH_CONFIG_MAIN
#include <catch2/catch.hpp>

#include <opencv2/opencv.hpp>
#include <spdlog/spdlog.h>
#include "core/types.h"
#include "core/vector.h"
#include "core/image_utils.h"
#include "core/circle.h"

using namespace std;
using namespace olio;

TEST_CASE("DoNothing") {
}


// // ======================================================================
// // *** Uncomment test case for homework question 1 ***
// // ======================================================================
TEST_CASE("TestVector3", "[Vector3]") {
  SECTION("Constrcutors") {
    Vector3 v1;
    Vector3 v2{1, 2, 3};
    REQUIRE( v1[0] == 0 );
    REQUIRE( v2[0] == 1 );
  }

  SECTION("operator[]") {
    Vector3 v1;
    v1[1] = 5;
    REQUIRE( v1[1] == 5 );
  }

  SECTION("X(), Y(), Z()") {
    Vector3 v1{2, 3, 4};
    REQUIRE( v1.X() == 2 );
    REQUIRE( v1.Y() == 3 );
    REQUIRE( v1.Z() == 4 );
  }

  SECTION("SetX, SetY, SetZ") {
    Vector3 v1;
    v1.SetX(1);
    v1.SetY(2);
    v1.SetZ(3);
    REQUIRE( v1[0] == 1 );
    REQUIRE( v1[1] == 2 );
    REQUIRE( v1[2] == 3 );
  }

  SECTION("Norm") {
    Vector3 v1{1, 0, 0};
    REQUIRE( v1.Norm() == 1 );

    Vector3 v2{3, 4, 5};
    REQUIRE( fabs(v2.Norm() - 7.0710678119) < kEpsilon );
  }

  SECTION("Dot") {
    Vector3 v1{1, 2, 3};
    Vector3 v2{3, 4, 5};
    REQUIRE( v1.Dot(v2) == 26 );
  }

  SECTION("Negation operator") {
    Vector3 v1{1, 2, 3};
    Vector3 v2{-v1};
    REQUIRE( v2[0] == -1 );
    REQUIRE( v2[1] == -2 );
    REQUIRE( v2[2] == -3 );
  }

  SECTION("Addition operator") {
    Vector3 v1{1, 2, 3};
    Vector3 v2{3, 4, 5};
    Vector3 v3{v1 + v2};
    REQUIRE( v3[0] == 4 );
    REQUIRE( v3[1] == 6 );
    REQUIRE( v3[2] == 8 );
  }

  SECTION("Subtraction operator") {
    Vector3 v1{1, 2, 3};
    Vector3 v2{3, 4, 6};
    Vector3 v3{v1 - v2};
    REQUIRE( v3[0] == -2 );
    REQUIRE( v3[1] == -2 );
    REQUIRE( v3[2] == -3 );
  }

  SECTION("IsApprox") {
    Vector3 v1{1, 2, 3};
    Vector3 v2{1 + kEpsilon, 2 + kEpsilon, 3 + kEpsilon};
    REQUIRE( v1.IsApprox(v2, kEpsilon) );
  }

  SECTION("Multiplication") {
    Vector3 v1{1, 2, 3};
    REQUIRE( (v1 * 3).IsApprox(Vector3{3, 6, 9}, kEpsilon) );
    REQUIRE( (3 * v1).IsApprox(Vector3{3, 6, 9}, kEpsilon) );
  }

  SECTION("Division") {
    Vector3 v1{1, 2, 3};
    REQUIRE( (v1 / 3).IsApprox(Vector3{1 / 3.0, 2 / 3.0, 1.0}, kEpsilon) );
  }

  SECTION("Cross") {
    Vector3 v1{1, 2, 3};
    Vector3 v2{3, 4, 5};
    REQUIRE( v1.Cross(v2).IsApprox(Vector3{-2, 4, -2}, kEpsilon) );

    Vector3 v3{1, 0, 0};
    Vector3 v4{0, 1, 0};
    REQUIRE( v3.Cross(v4).IsApprox(Vector3{0, 0, 1}, kEpsilon) );
  }
}


// // ======================================================================
// // *** Uncomment this test case for homework question 2 ***
// // ======================================================================
TEST_CASE("TestVec3d", "[Vec3d]") {
//   SECTION("Constrcutors") {
//     Vec3d v1;
//     REQUIRE( v1[0] == 0 );

//     Vec3d v2{1, 2, 3};
//     REQUIRE( v2[0] == 1 );
//   }

  SECTION("operator[]") {
    Vec3d v1;
    v1[1] = 5;
    REQUIRE( v1[1] == 5 );
  }

  SECTION("norm") {
    Vec3d v1{1, 0, 0};
    REQUIRE( v1.norm() == 1 );

    Vec3d v2{3, 4, 5};
    REQUIRE( fabs(v2.norm() - 7.0710678119) < kEpsilon );
  }

  SECTION("dot") {
    Vec3d v1{1, 2, 3};
    Vec3d v2{3, 4, 5};
    REQUIRE( v1.dot(v2) == 26 );
  }

  SECTION("Negation operator") {
    Vec3d v1{1, 2, 3};
    Vec3d v2{-v1};
    REQUIRE( v2[0] == -1 );
    REQUIRE( v2[1] == -2 );
    REQUIRE( v2[2] == -3 );
  }

  SECTION("Addition operator") {
    Vec3d v1{1, 2, 3};
    Vec3d v2{3, 4, 5};
    Vec3d v3{v1 + v2};
    REQUIRE( v3[0] == 4 );
    REQUIRE( v3[1] == 6 );
    REQUIRE( v3[2] == 8 );
  }

  SECTION("Subtraction operator") {
    Vec3d v1{1, 2, 3};
    Vec3d v2{3, 4, 6};
    Vec3d v3{v1 - v2};
    REQUIRE( v3[0] == -2 );
    REQUIRE( v3[1] == -2 );
    REQUIRE( v3[2] == -3 );
  }

  SECTION("isApprox") {
    Vec3d v1{1, 2, 3};
    Vec3d v2{1 + kEpsilon, 2 + kEpsilon, 3 + kEpsilon};
    REQUIRE( v1.isApprox(v2, kEpsilon) );
  }

  SECTION("Scaling") {
    Vec3d v1{1, 2, 3};
    REQUIRE( (v1 * 3).isApprox(Vec3d{3, 6, 9}, kEpsilon) );
    REQUIRE( (3 * v1).isApprox(Vec3d{3, 6, 9}, kEpsilon) );
  }

  SECTION("cross") {
    Vec3d v1{1, 2, 3};
    Vec3d v2{3, 4, 5};
    REQUIRE( v1.cross(v2).isApprox(Vec3d(-2, 4, -2), kEpsilon) );

    Vec3d v3{1, 0, 0};
    Vec3d v4{0, 1, 0};
    REQUIRE( v3.cross(v4).isApprox(Vec3d(0, 0, 1), kEpsilon) );
  }
}


// // ======================================================================
// // *** Uncomment this test case for homework question 3             ***
// // *** NOTE: make sure to add #include "core/circle.h" to beginning ***
// // ***       of this file                                           ***
// // ======================================================================
TEST_CASE("TestCircle", "[Circle]") {
  SECTION("Constrcutors and Getters") {
    Circle circle{Vec2d{0, 0}, 1, Vec3d{1, 0, 0}};
    REQUIRE( circle.GetCenter() == Vec2d{0, 0} );
    REQUIRE( circle.GetRadius() == 1.0f );
    REQUIRE( circle.GetColor() == Vec3d{1, 0, 0} );
  }

  SECTION("Setters") {
    Circle circle;
    circle.SetCenter(Vec2d{0, 0});
    circle.SetRadius(1);
    circle.SetColor(Vec3d{1, 0, 0});
    REQUIRE( circle.GetCenter() == Vec2d{0, 0} );
    REQUIRE( circle.GetRadius() == 1.0f );
    REQUIRE( circle.GetColor() == Vec3d{1, 0, 0} );
  }

    SECTION("Point Inside") {
    bool value;
    Circle circle;
    circle.SetCenter(Vec2d{0, 0});
    circle.SetRadius(1);
    circle.SetColor(Vec3d{1, 0, 0});
    value = circle.IsPointInside(Vec2d{0.5, 0});
    REQUIRE( circle.GetCenter() == Vec2d{0, 0} );
    REQUIRE( circle.GetRadius() == 1.0f );
    REQUIRE( circle.GetColor() == Vec3d{1, 0, 0} );
    REQUIRE( value == true );
  }

    SECTION("Point border Inside") {
    bool value;
    Circle circle;
    circle.SetCenter(Vec2d{0, 0});
    circle.SetRadius(1);
    circle.SetColor(Vec3d{1, 0, 0});
    value = circle.IsPointInside(Vec2d{1, 0});
    REQUIRE( circle.GetCenter() == Vec2d{0, 0} );
    REQUIRE( circle.GetRadius() == 1.0f );
    REQUIRE( circle.GetColor() == Vec3d{1, 0, 0} );
    REQUIRE( value == true );
  }

    SECTION("Point Outside") {
    bool value;
    Circle circle;
    circle.SetCenter(Vec2d{0, 0});
    circle.SetRadius(1);
    circle.SetColor(Vec3d{1, 0, 0});
    value = circle.IsPointInside(Vec2d{1, 1});
    cout << "Value: " << value << endl;
    REQUIRE( circle.GetCenter() == Vec2d{0, 0} );
    REQUIRE( circle.GetRadius() == 1.0f );
    REQUIRE( circle.GetColor() == Vec3d{1, 0, 0} );
    REQUIRE( value == false );
  }
}


// // ======================================================================
// // *** Uncomment this test case for homework question 4 ***
// // ======================================================================
TEST_CASE("TestGamma", "[GammaCorrection]") {
  Real gamma = 2.1;
  SECTION("CV_64FC3") {
    cv::Mat image(1, 1, CV_64FC3, cv::Scalar(0, .48, 1));
    cout << "image: " << image.type() << endl;
    cout << "image rows: " << image.rows << endl;
    cout << "image cols: " << image.cols << endl;
    cv::Mat out_image;
    utils::GammaCorrectImage64FC3(image, out_image, gamma);
    const auto &pixel = out_image.at<cv::Vec3d>(0, 0);
    // spdlog::info("{}, {}, {}", pixel[0], pixel[1], pixel[2]);
    REQUIRE( pixel[0] == 0 );
    REQUIRE( fabs(pixel[1] - 0.7050340841753) < kEpsilon );
    REQUIRE( pixel[2] == 1 );
  }

  SECTION("CV_8UC3") {
    cv::Mat image(1, 1, CV_8UC3, cv::Scalar(0, 125, 255));
    cv::Mat out_image;
    utils::GammaCorrectImage8UC3(image, out_image, gamma);
    const auto &pixel = out_image.at<cv::Vec3b>(0, 0);
    // spdlog::info("{}, {}, {}", pixel[0], pixel[1], pixel[2]);
    REQUIRE( pixel[0] == 0 );
    REQUIRE( pixel[1] == 181 );
    REQUIRE( pixel[2] == 255 );
  }
 }
