// ======================================================================
// Olio: Simple renderer
// Copyright (C) 2022 by Hadi Fadaifard
//
// Author: Hadi Fadaifard, 2022
// ======================================================================

//! \file       node.cc
//! \brief      Node class
//! \author     Hadi Fadaifard, 2022

#include "core/node.h"
#include <spdlog/spdlog.h>

namespace olio {
namespace core {

using namespace std;

// initialize static data members
size_t Node::node_count_ = 0;
std::mutex Node::node_count_mutex_;

Node::Node(const std::string &name) :
  std::enable_shared_from_this<Node>()
{
  SetGlobalNodeId();
  name_ = name.size() ? name : "Node";
}


Node::Node(const Node &other) :
  std::enable_shared_from_this<Node>(),
  name_{other.name_}
{
  // spdlog::info("Cloning {}", other.name_);
  SetGlobalNodeId();
}


std::shared_ptr<Node>
Node::Clone() const
{
  return std::shared_ptr<Node>(new Node(*this), NodeDeleter);
}


Node::~Node()
{
  // spdlog::info("Deleting {} ({})", name_, global_node_id_);
}


void
Node::SetGlobalNodeId()
{
  const std::lock_guard<std::mutex> lock(node_count_mutex_);
  global_node_id_ = node_count_++;
}


void
Node::SetName(const std::string &name)
{
  name_ = name;
}


bool
operator<(const Node::Ptr &a, const Node::Ptr &b)
{
  if (!a || !b)
    return false;
  return a->GetGlobalNodeId() < b->GetGlobalNodeId();
}


bool
operator<(const Node::WeakPtr &aweak, const Node::WeakPtr &bweak)
{
  auto a = aweak.lock();
  auto b = bweak.lock();
  if (!a || !b)
    return false;
  return a->GetGlobalNodeId() < b->GetGlobalNodeId();
}


void
NodeDeleter(Node *node)
{
  delete node;
}

}  // namespace core
}  // namespace olio
