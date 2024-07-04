// ======================================================================
// Olio: Simple renderer
// Copyright (C) 2022 by Hadi Fadaifard
//
// Author: Hadi Fadaifard, 2022
// ======================================================================

//! \file       main.cc
//! \brief      main tests file
//! \author     Hadi Fadaifard, 2022

#include <iostream>

#define CATCH_CONFIG_MAIN
#include <catch2/catch.hpp>

#include "core/types.h"

using namespace std;
using namespace olio::core;

TEST_CASE("DoNothing") {
}

/*TEST_CASE("Lights", "[Vector3]") {
  SECTION("Constrcutors") {
    Vector3 v1;
    Vector3 v2{1, 2, 3};
    REQUIRE( v1[0] == 0 );
    REQUIRE( v2[0] == 1 );
  }
}*/
