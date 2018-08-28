// Copyright 2018 Andrea Mantovani
// https://github.com/korut94/VCGLibForParaview/tree/master/src/utils

#ifndef UTILSVCGDEFAULTMESH_H
#define UTILSVCGDEFAULTMESH_H

// stuff to define the mesh
#include <vcg/complex/complex.h>

// io
#include <wrap/io_trimesh/import.h>
#include <wrap/io_trimesh/export_ply.h>

// local optimization
#include <vcg/complex/algorithms/local_optimization.h>
#include <vcg/complex/algorithms/local_optimization/tri_edge_collapse_quadric.h>

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
  vcg::vertex::VFAdj,
  vcg::vertex::BitFlags,
  vcg::vertex::Qualityf> {
public:
    vcg::math::Quadric<double> &Qd() {return q;}
private:
    vcg::math::Quadric<double> q;
  };

class VCGDefaultFace : public vcg::Face<
  VCGDefaultUsedTypes,
  vcg::face::VertexRef,
  vcg::face::Normal3f,
  vcg::face::FFAdj,
  vcg::face::VFAdj,
  vcg::face::BitFlags,
  vcg::face::Mark> {};

class VCGDefaultEdge : public vcg::Edge<VCGDefaultUsedTypes> {};

class VCGDefaultMesh : public vcg::tri::TriMesh<
  std::vector<VCGDefaultVertex>,
  std::vector<VCGDefaultFace>,
  std::vector<VCGDefaultEdge>> {};

typedef BasicVertexPair<VCGDefaultVertex> VCGDefaultVertexPair;

class VCGDefaultTriEdgeCollapse: public vcg::tri::TriEdgeCollapseQuadric< VCGDefaultMesh, VCGDefaultVertexPair, VCGDefaultTriEdgeCollapse, QInfoStandard<VCGDefaultVertex>  >
{
public:
	typedef  vcg::tri::TriEdgeCollapseQuadric< VCGDefaultMesh, VCGDefaultVertexPair, VCGDefaultTriEdgeCollapse, QInfoStandard<VCGDefaultVertex>  > TECQ;
    typedef  VCGDefaultMesh::VertexType::EdgeType EdgeType;
    inline VCGDefaultTriEdgeCollapse(  const VCGDefaultVertexPair &p, int i, vcg::BaseParameterClass *pp) :TECQ(p,i,pp){}
};

} // namespace utils

#endif // UTILSVCGDEFAULTMESH_H
