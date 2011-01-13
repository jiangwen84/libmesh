
// $Id$

// The libMesh Finite Element Library.
// Copyright (C) 2002-2008 Benjamin S. Kirk, John W. Peterson, Roy H. Stogner
  
// This library is free software; you can redistribute it and/or
// modify it under the terms of the GNU Lesser General Public
// License as published by the Free Software Foundation; either
// version 2.1 of the License, or (at your option) any later version.
  
// This library is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
// Lesser General Public License for more details.
  
// You should have received a copy of the GNU Lesser General Public
// License along with this library; if not, write to the Free Software
// Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA

#ifndef __periodic_boundaries_h__
#define __periodic_boundaries_h__

// ------------------------------------------------------------
// Periodic boundary conditions information

// Local Includes -----------------------------------
#include "libmesh_config.h"

#ifdef LIBMESH_ENABLE_PERIODIC

// C++ Includes   -----------------------------------
#include <map>

// Local Includes -----------------------------------
#include "point.h"
#include "vector_value.h" // RealVectorValue

namespace libMesh {

// Forward Declarations
class Elem;
class MeshBase;

/**
 * The definition of a periodic boundary.
 */
class PeriodicBoundary
{
public:
  // The boundary ID of this boundary and it's counterpart
  unsigned int myboundary,
	       pairedboundary;

  PeriodicBoundary() :
    myboundary(-1),
    pairedboundary(-1),
    translation_vector(),
    variable(0),
    var_set(false)
  {
  }

  ~PeriodicBoundary() {}

  PeriodicBoundary(const PeriodicBoundary & o, bool inverse = false) :
    myboundary(o.myboundary),
    pairedboundary(o.pairedboundary),
    translation_vector(o.translation_vector),
    variable(0),
    var_set(o.var_set)
  {
    if (inverse)
    {
      std::swap(myboundary, pairedboundary);
      translation_vector *= -1.0;
    }
  }

  PeriodicBoundary(const RealVectorValue & vector) :
    myboundary(-1),
    pairedboundary(-1),
    translation_vector (vector),
    variable(0),
    var_set(false)
  {
  }

  virtual Point get_corresponding_pos(const Point & pt)
  {
    return pt + translation_vector;
  }

  void set_variable(unsigned int var)
  {
    variable = var;
    var_set = true;
  }

  bool is_my_variable(unsigned int var_num)
  {
    bool a = !var_set || (var_set && variable == var_num);
    return a;
  }

protected:
  // One of these days we'll support rotated boundaries
  // RealTensor rotation_matrix;

  // The vector which is added to points in myboundary
  // to produce corresponding points in pairedboundary
  RealVectorValue translation_vector;

  unsigned int variable;
  bool var_set;
};


/** 
 * The constraint matrix storage format. 
 * We're using a class instead of a typedef to allow forward
 * declarations and future flexibility.  Is there some issue with
 * deriving from standard containers, i.e. don't do it because they
 * don't have virtual destructors?
 */
class PeriodicBoundaries : public std::map<unsigned int, PeriodicBoundary *>
{
public:
  PeriodicBoundary *boundary(unsigned int id);

  PeriodicBoundaries() {}

  ~PeriodicBoundaries();

  // The periodic neighbor of \p e in direction \p side, if it
  // exists.  NULL otherwise
  const Elem *neighbor(unsigned int boundary_id, const MeshBase &mesh, const Elem *e, unsigned int side);

private:
};



// ------------------------------------------------------------
// PeriodicBoundary inline member functions

inline
PeriodicBoundary *PeriodicBoundaries::boundary(unsigned int id)
{
  iterator i = this->find(id);
  if (i == this->end())
    return NULL;
  return i->second;
}

} // namespace libMesh

#endif // LIBMESH_ENABLE_PERIODIC

#endif // __periodic_boundaries_h__
