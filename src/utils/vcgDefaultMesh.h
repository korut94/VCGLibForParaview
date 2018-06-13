#ifndef UTILSVCGDEFAULTMESH_H
#define UTILSVCGDEFAULTMESH_H

#include "vcg/complex/complex.h"

namespace utils
{
class VCGDefaultEdge;
class VCGDefaultFace;
class VCGDefaultVertex;

struct VCGDefaultUsedTypes : public vcg::UsedTypes<
  vcg::Use<VCGDefaultVertex>::AsVertexType,
  vcg::Use<VCGDefaultEdge>::AsEdgeType,
  vcg::Use<VCGDefaultFace>::AsFaceType> {};

class VCGDefaultVertex : public vcg::Vertex<
  VCGDefaultUsedTypes,
  vcg::vertex::Coord3f,
  vcg::vertex::Normal3f,
  vcg::vertex::Color4b,
  vcg::vertex::BitFlags> {};

class VCGDefaultFace : public vcg::Face<
  VCGDefaultUsedTypes,
  vcg::face::VertexRef,
  vcg::face::Normal3f,
  vcg::face::FFAdj,
  vcg::face::BitFlags,
  vcg::face::Mark> {};

class VCGDefaultEdge : public vcg::Edge<VCGDefaultUsedTypes> {};

class VCGDefaultMesh : public vcg::tri::TriMesh<
  std::vector<VCGDefaultVertex>,
  std::vector<VCGDefaultFace>,
  std::vector<VCGDefaultEdge>> {};
} // namespace utils

#endif // UTILSVCGDEFAULTMESH_H