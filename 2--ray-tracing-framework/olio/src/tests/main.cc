// ======================================================================
// Olio: Simple renderer
//
// ======================================================================

//! \file       main.cc
//! \brief      main tests file

#include <iostream>

#define CATCH_CONFIG_MAIN
#include <catch2/catch.hpp>

#include "core/types.h"
#include "core/geometry/triangle.h"
#include "core/geometry/sphere.h"
#include "core/camera/camera.h"
#include "core/renderer/raytracer.h"
#include "core/ray.h"
#include "core/parser/raytra_parser.h"
#include <cmath>

using namespace std;
using namespace olio;
using namespace olio::core;

TEST_CASE("DoNothing") {
}

TEST_CASE("HitRecord") {
    SECTION("T1:HitRecrod::SetNormal1") {
        Vec3r O = Vec3r{1,0,0};
        Vec3r dr = Vec3r{2,1,0};
        Vec3r Normal = Vec3r{1,1,0};
        Ray ray(O,dr);

        REQUIRE(ray.GetOrigin() == O);
        REQUIRE(ray.GetDirection() == dr);

        HitRecord hitRecord;
        hitRecord.SetNormal(ray,Normal);

        REQUIRE(hitRecord.GetNormal() == -Normal);
        REQUIRE(hitRecord.IsFrontFace() == false);
    }

    SECTION("T2:HitRecrod::SetNormal1") {
        Vec3r O = Vec3r{1,0,0};
        Vec3r dr = Vec3r{2,1,0};
        Vec3r Normal = Vec3r{1,-3,0};
        Ray ray(O,dr);

        REQUIRE(ray.GetOrigin() == O);
        REQUIRE(ray.GetDirection() == dr);

        HitRecord hitRecord;
        hitRecord.SetNormal(ray,Normal);

        REQUIRE(hitRecord.GetNormal() == Normal);
        REQUIRE(hitRecord.IsFrontFace() == true);
    }

    SECTION("T3:HitRecord::SetNormal2") {
        Vec3r Normal = Vec3r{1,-3,0};
        bool faceNormal = false;

        HitRecord hitRecord;
        hitRecord.SetNormal(Normal,faceNormal);

        REQUIRE(hitRecord.GetNormal() == -Normal);
        REQUIRE(hitRecord.IsFrontFace() == false);
    }

    SECTION("T4:HitRecord::SetNormal2") {
        Vec3r Normal = Vec3r{1,-3,0};
        bool faceNormal = true;

        HitRecord hitRecord;
        hitRecord.SetNormal(Normal,faceNormal);

        REQUIRE(hitRecord.GetNormal() == Normal);
        REQUIRE(hitRecord.IsFrontFace() == true);
    }

}

TEST_CASE("TestSphere") {
    SECTION("Sphere::NormalHit") {
    /*s -1 0 0 1*/
    /*c 0 0 2 0 0 -1 0.035 .053886 0.0404145 800 600*/
    Sphere::Ptr sphere;
    Vec3r O{-1,0,0};
    Real radius = 1;
    sphere = Sphere::Create(O,radius,"spher");
    Camera::Ptr camera;
    Vec3r eye{0,0,2};
    Vec3r target{0,0,-1};
    Vec3r up{0,1,0};

    Real focal_length = 0.035;
    Real viewport_height = 0.0404145;
    Real viewport_width = 0.053886;
    Real fovy = 2 * atan2(viewport_height * 0.5f, focal_length) * kRADtoDEG;
    Real viewport_aspect = viewport_width / viewport_height;
    camera = Camera::Create(eye, target, up, fovy,viewport_aspect);

    int width = 800;
    int height = 600;
    cv::Mat rendered_image_ = cv::Mat(cv::Size(width, height), CV_64FC3,
			    cv::Scalar(0, 0, 0, 0));

    for (int i = 0; i < height; i++)
    {
      for (int j = 0; j < width; j++)
      {
          //cout << "index: "<< (height-i-0.5f)/height << "\n" << endl;
          Real ho = (j+0.5f)/width;
          Real lo = (height-(i+0.5f))/height;
          Ray ray = camera->GetRay(ho,lo);
          REQUIRE(ray.GetOrigin() == eye);

          HitRecord hit_record;
          if(sphere->Hit(ray,0,kInfinity,hit_record) == true)
          {
            //cout << "ray_t Hit: " << hit_record.GetRayT() << endl;
            Vec3r color{1,0,0};
            rendered_image_.at<Vec3r>(i, j) = color;
          }
          else{
            Vec3r color{0,0,0};
            rendered_image_.at<Vec3r>(i, j) = color;              
          }
          
      }
    }

    RayTracer rt;
    rt.SetOutputImage(rendered_image_);
    rt.WriteImage("OutputSphereNormalFunc.png");

        
    REQUIRE( camera->GetEye() == eye );
  }

}

TEST_CASE("TestTriangle") {
    SECTION("Triangle::NormalHit") {
    /*c 0 0 2 0 0 -1 0.035 .053886 0.0404145 800 600*/
    Triangle::Ptr triangle;
    Vec3r p0{-1,0,-1};
    Vec3r p1{1,0,-1};
    Vec3r p2{0,1,-1};
    triangle = Triangle::Create(p0,p1,p2,"trian");
    Camera::Ptr camera;
    Vec3r eye{0,0,2};
    Vec3r target{0,0,-1};
    Vec3r up{0,1,0};

    Real focal_length = 0.035;
    Real viewport_height = 0.0404145;
    Real viewport_width = 0.053886;
    Real fovy = 2 * atan2(viewport_height * 0.5f, focal_length) * kRADtoDEG;
    Real viewport_aspect = viewport_width / viewport_height;
    camera = Camera::Create(eye, target, up, fovy,viewport_aspect);

    int width = 800;
    int height = 600;
    cv::Mat rendered_image_ = cv::Mat(cv::Size(width, height), CV_64FC3,
			    cv::Scalar(0, 0, 0, 0));

    for (int i = 0; i < height; i++)
    {
      for (int j = 0; j < width; j++)
      {
          //cout << "index: "<< (height-i-0.5f)/height << "\n" << endl;
          Real ho = (j+0.5f)/width;
          Real lo = (height-(i+0.5f))/height;
          Ray ray = camera->GetRay(ho,lo);
          REQUIRE(ray.GetOrigin() == eye);

          HitRecord hit_record;
          if(triangle->Hit(ray,0,kInfinity,hit_record) == true)
          {
            //cout << "ray_t Hit: " << hit_record.GetRayT() << endl;
            Vec3r color{1,0,0};
            rendered_image_.at<Vec3r>(i, j) = color;
          }
          else{
            Vec3r color{0,0,0};
            rendered_image_.at<Vec3r>(i, j) = color;              
          }
          
      }
    }

    RayTracer rt;
    rt.SetOutputImage(rendered_image_);
    rt.WriteImage("OutputTriangleNormalFunc.png");

        
    REQUIRE( camera->GetEye() == eye );
  }

    SECTION("Triangle::StaticRayTriangleHit") {
    /*c 0 0 2 0 0 -1 0.035 .053886 0.0404145 800 600*/
    Triangle::Ptr triangle;
    Vec3r p0{-1,0,-1};
    Vec3r p1{1,0,-1};
    Vec3r p2{0,1,-1};
    //triangle = Triangle::Create(p0,p1,p2,"trian");
    Camera::Ptr camera;
    Vec3r eye{0,0,2};
    Vec3r target{0,0,-1};
    Vec3r up{0,1,0};

    Real focal_length = 0.035;
    Real viewport_height = 0.0404145;
    Real viewport_width = 0.053886;
    Real fovy = 2 * atan2(viewport_height * 0.5f, focal_length) * kRADtoDEG;
    Real viewport_aspect = viewport_width / viewport_height;
    camera = Camera::Create(eye, target, up, fovy,viewport_aspect);

    int width = 800;
    int height = 600;
    cv::Mat rendered_image_ = cv::Mat(cv::Size(width, height), CV_64FC3,
			    cv::Scalar(0, 0, 0, 0));

    for (int i = 0; i < height; i++)
    {
      for (int j = 0; j < width; j++)
      {
          //cout << "index: "<< (height-i-0.5f)/height << "\n" << endl;
          Real ho = (j+0.5f)/width;
          Real lo = (height-(i+0.5f))/height;
          Ray ray = camera->GetRay(ho,lo);
          REQUIRE(ray.GetOrigin() == eye);
          Real ray_t2;
          Vec2r uv;
          if(triangle->RayTriangleHit(p0,p1,p2,ray,0,kInfinity,ray_t2,uv) == true)
          {
              //cout << "ray_t2: " << ray_t2 << endl;
            Vec3r color{1,0,0};
            rendered_image_.at<Vec3r>(i, j) = color;
          }
          else{
            Vec3r color{0,0,0};
            rendered_image_.at<Vec3r>(i, j) = color;              
          }
          
      }
    }

    RayTracer rt;
    rt.SetOutputImage(rendered_image_);
    rt.WriteImage("OutputTriangleStaticFunc.png");

        
    REQUIRE( camera->GetEye() == eye );
  }
}