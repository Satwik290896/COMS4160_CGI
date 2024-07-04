// ======================================================================
// Olio: Simple renderer
// Copyright (C) 2022 by Hadi Fadaifard
//
// Author: Hadi Fadaifard, 2022
// ======================================================================

//! \file       node.h
//! \brief      Node class
//! \author     Hadi Fadaifard, 2022

#pragma once

#include <memory>
#include <string>
#include <thread>
#include <mutex>
#include <set>

#include "core/types.h"

namespace olio {
namespace core {

// forward declarations
class Node;

//! \brief Friend function of \see Node class allowing access to
//!        class' protected destructor; to be passed to shared_ptr
//!        constructor when instantiating a Node instance
//! \param[in] node Node to delete
void NodeDeleter(Node *node);

#define OLIO_NODE_BASE(name) \
  EIGEN_MAKE_ALIGNED_OPERATOR_NEW \
  using Ptr = std::shared_ptr<name>; \
  using ConstPtr = std::shared_ptr<name const>; \
  using WeakPtr = std::weak_ptr<name>; \
  template<typename... Args> \
  static std::shared_ptr<name> Create(Args... args) { \
    return Ptr(new name(args...), NodeDeleter);} \
  std::shared_ptr<name> GetPtr() { \
    return std::dynamic_pointer_cast<name>(shared_from_this());} \
  std::shared_ptr<name const> GetPtr() const { \
    return std::dynamic_pointer_cast<name const>(shared_from_this());}

#define OLIO_NODE(name) \
  OLIO_NODE_BASE(name) \
  std::shared_ptr<Node> Clone() const override { \
    return std::shared_ptr<Node>(new name(*this), NodeDeleter);}

//! \class Node
//! \brief Node class
class Node : public std::enable_shared_from_this<Node> {
public:
  OLIO_NODE_BASE(Node)
  using NodeMask = uint;
  using NodePath = std::vector<Node::Ptr>;

  //! \brief Constructor
  explicit Node(const std::string &name=std::string());

  //! \brief Move constructor
  //! \param[in] other other Node instance to move from
  Node(Node &&other) = delete;
  Node& operator=(const Node &) = delete;
  Node& operator=(Node &&) = delete;

  //! \brief Clone node.
  //! \return pointer to cloned node
  virtual std::shared_ptr<Node> Clone() const;

  //! \brief Set node name
  //! \param[in] name Node name
  virtual void SetName(const std::string &name);

  //! \brief Get node's unique id
  //! \return node id
  virtual size_t GetGlobalNodeId() const final {return global_node_id_;}

  //! \brief Get node name
  //! \return node name
  virtual std::string GetName() const {return name_;}

  //! \brief Friend function of \see Node class allowing access to
  //!        class' protected destructor; to be passed to shared_ptr
  //!        constructor when instantiating a Node instance
  //! \param[in] node Node to delete
  friend void NodeDeleter(Node *node);
protected:
  //! \brief Copy constructor to be used only by Clone()
  //! \param[in] other other Node instance to copy from
  Node(const Node &other);

  //! \brief Destructor; made protected to prevent allocation of Node
  //!        on the stack or via new/malloc.
  virtual ~Node();

  // protected data members
  size_t global_node_id_{0};  //!< unique node id
  std::string name_;          //!< node name

  // static data members
  static size_t node_count_;  //!< total number of nodes ever created
  static std::mutex node_count_mutex_;  //!< mutex used to set unique node id
private:
  //! \brief Private function called by constructor to assign a
  //!        unique id to each node
  void SetGlobalNodeId();
};


//! \brief less than operator comparing two nodes
//! \param[in] a pointer to node a
//! \param[in] b pointer to node b
//! \return true if a < b
bool operator<(const Node::Ptr &a, const Node::Ptr &b);

//! \brief less than operator comparing two nodes
//! \param[in] a pointer to node a
//! \param[in] b pointer to node b
//! \return true if a < b
bool operator<(const Node::WeakPtr &a, const Node::WeakPtr &b);

}  // namespace core
}  // namespace olio
