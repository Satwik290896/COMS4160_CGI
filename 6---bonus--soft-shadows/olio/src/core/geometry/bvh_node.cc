// ======================================================================
// Olio: Simple renderer
// Copyright (C) 2022 by Hadi Fadaifard
//
// Author: Hadi Fadaifard, 2022
// ======================================================================

//! \file       bvh_node.cc
//! \brief      BVHNode class
//! \author     Hadi Fadaifard, 2022
#include <iostream>
#include "core/geometry/bvh_node.h"
#include <spdlog/spdlog.h>
#include "core/ray.h"
#include "core/material/material.h"

namespace olio {
namespace core {

using namespace std;
int global_axis = 0; 

BVHNode::BVHNode(const std::string &name) :
  Surface{}
{
  name_ = name.size() ? name : "BVHNode";
}


AABB
BVHNode::GetBoundingBox(bool force_recompute)
{
  // if bound is clean, just return existing bbox_
  if (!force_recompute && !IsBoundDirty())
    return bbox_;

  bbox_.Reset();
  if (left_)
    bbox_.ExpandBy(left_->GetBoundingBox(force_recompute));
  if (right_)
    bbox_.ExpandBy(right_->GetBoundingBox(force_recompute));
  // spdlog::info("{}: {}", GetName(), bbox_);
  bound_dirty_ = false;
  return bbox_;
}


bool
BVHNode::Hit(const Ray &ray, Real tmin, Real tmax, HitRecord &hit_record)
{
  // ======================================================================
  // *** Homework: Implement function
  // ======================================================================
  // ***** START OF YOUR CODE (DO NOT DELETE/MODIFY THIS LINE) *****
  if (bbox_.Hit (ray, tmin, tmax)) {
    // if we are a leaf: try to intersect the surface - record hit and
    // adjust tmax if necessary, and return true or false
    // OTHERWISE:
    /*HitRecord lrec, rrec;
    bool left_hit = ((left_ != nullptr) && (left_->Hit (ray, tmin, tmax, lrec)));
    bool right_hit = ((right_ != nullptr) && (right_->Hit (ray, lrec.GetRayT(), tmax, rrec)));

    if (left_hit && right_hit) {
      if (lrec.GetRayT() < rrec.GetRayT())
        hit_record = lrec;
      else
        hit_record = rrec;
      return true;
    }
    else if (left_hit) {
      hit_record = lrec;
      return true;
    }
    else if (right_hit) {
      hit_record = rrec;
      return true;
    }*/

    bool hit = false;
    //HitRecord lrec, rrec;
    if ((left_ != nullptr) && (left_->Hit (ray, tmin, tmax, hit_record))) {
      tmax = hit_record.GetRayT();
      hit = true;
    }
    if ((right_ != nullptr) && (right_->Hit (ray, tmin, tmax, hit_record))) {
      hit = true;
    }
    return hit;

  }
  return false;

 //!< remove this line and add your own code
  // ***** END OF YOUR CODE (DO NOT DELETE/MODIFY THIS LINE) *****
}


BVHNode::Ptr
BVHNode::BuildBVH(std::vector<Surface::Ptr> surfaces, const string &name)
{
  //spdlog::info("Building BVH ({})", name);

  // error checking
  auto surface_count = surfaces.size();
  if (!surface_count)
    return nullptr;

  // make sure we have valid bboxes for surfaces
  for (auto surface : surfaces) {
    if (surface)
      surface->GetBoundingBox();
  }

  // build bvh
  uint split_axis = 0;
  auto bvh_node = BuildBVH(surfaces, 0, surface_count, split_axis, name);

  // compute bboxes
  if (bvh_node)
    bvh_node->GetBoundingBox();

  //spdlog::info("Done building BVH ({})", name);
  return bvh_node;
}

// Compares two intervals according to starting times.
bool compareInterval(Surface::Ptr i1, Surface::Ptr i2)
{
    Vec3r min_temp_i1 = i1->GetBoundingBox(false).GetMin();
    Vec3r min_temp_i2 = i2->GetBoundingBox(false).GetMin();
    return (min_temp_i1[global_axis] < min_temp_i2[global_axis]);
}

BVHNode::Ptr
BVHNode::BuildBVH(std::vector<Surface::Ptr> &surfaces, size_t start,
                  size_t end, uint split_axis, const std::string &name)
{
  // ======================================================================
  // *** Homework: Implement function
  // ======================================================================
  // ***** START OF YOUR CODE (DO NOT DELETE/MODIFY THIS LINE) *****
  BVHNode::Ptr bvh_node = BVHNode::Create();
  size_t N = end - start;

  if (N == 1) {
    bvh_node->left_ = surfaces[start];
    bvh_node->right_ = nullptr;
    bvh_node->bbox_ = surfaces[start]->GetBoundingBox();
  }
  else if (N == 2) {
    bvh_node->left_ = surfaces[start];
    bvh_node->right_ = surfaces[start+1];

    AABB bbox_combine = bvh_node->left_->GetBoundingBox(false);
    bbox_combine.ExpandBy(bvh_node->right_->GetBoundingBox(false));
    bvh_node->bbox_ = bbox_combine;

    //bvh_node->bbox_ = BBoxCombine(surfaces[start]->BoundingBox(), surfaces[start+1]->BoundingBox());
  }
  else {
    //sort A by the surface or bbox centers along AXIS
    global_axis = split_axis;
    sort(surfaces.begin(), surfaces.end(), compareInterval);
    size_t mid = start + N/2;
    std::vector<Surface::Ptr> surfaces_l(mid), surfaces_r(N-mid);
    surfaces_l.assign(surfaces.begin(), surfaces.begin() + mid);
    surfaces_r.assign(surfaces.begin() + mid, surfaces.end());
    //std::cout << "surfaces size: " << surfaces.size() << "\n" << std::endl;
    //std::cout << "N: " << N << "\n" << std::endl;
    //std::cout << "surfaces_l size: " << surfaces_l.size() << "\n" << std::endl;
    //std::cout << "surfaces_r size: " << surfaces_r.size() << "\n" << std::endl;

    bvh_node->left_ = BVHNode::BuildBVH(surfaces_l, 0, mid, (split_axis+1)%3);
    bvh_node->right_ = BVHNode::BuildBVH(surfaces_r, 0, surfaces_r.size(), (split_axis+1)%3);

    AABB bbox_combine = bvh_node->left_->GetBoundingBox(false);
    bbox_combine.ExpandBy(bvh_node->right_->GetBoundingBox(false));
    bvh_node->bbox_ = bbox_combine;

    //bvh_node->bbox_ = BBoxCombine (left_->bbox_, right_->bbox_)
  }

  return bvh_node;
  //return nullptr;  //!< remove this line and add your own code
  // ***** END OF YOUR CODE (DO NOT DELETE/MODIFY THIS LINE) *****
}

}  // namespace core
}  // namespace olio
