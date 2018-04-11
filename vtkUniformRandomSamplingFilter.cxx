// Copyright 2018 Andrea Mantovani

#include "vtkUniformRandomSamplingFilter.h"

#include "vtkCellIterator.h"
#include "vtkInformation.h"
#include "vtkInformationVector.h"
#include "vtkObjectFactory.h"
#include "vtkOutputWindow.h"
#include "vtkPoints.h"
#include "vtkPointSet.h"

#include "vcg/complex/algorithms/create/platonic.h"
#include "vcg/complex/algorithms/point_sampling.h"

#include "wrap/io_trimesh/export.h"

#include <algorithm>

class VCGEdge;
class VCGFace;
class VCGVertex;

struct VCGUsedTypes : public vcg::UsedTypes<
  vcg::Use<VCGVertex>::AsVertexType,
  vcg::Use<VCGEdge>::AsEdgeType,
  vcg::Use<VCGFace>::AsFaceType> {};

class VCGVertex : public vcg::Vertex<
  VCGUsedTypes,
  vcg::vertex::Coord3f,
  vcg::vertex::Normal3f,
  vcg::vertex::Color4b,
  vcg::vertex::BitFlags> {};

class VCGFace : public vcg::Face<
  VCGUsedTypes,
  vcg::face::VertexRef,
  vcg::face::Normal3f,
  vcg::face::FFAdj,
  vcg::face::BitFlags> {};

class VCGEdge : public vcg::Edge<VCGUsedTypes> {};

class VCGMesh : public vcg::tri::TriMesh<
  std::vector<VCGVertex>,
  std::vector<VCGFace>,
  std::vector<VCGEdge>> {};

vtkStandardNewMacro(vtkUniformRandomSamplingFilter);

vtkUniformRandomSamplingFilter::vtkUniformRandomSamplingFilter() {}

vtkUniformRandomSamplingFilter::~vtkUniformRandomSamplingFilter() {}

vcg::Point3i vtkUniformRandomSamplingFilter::retrieveTopologyFromCell(vtkCellIterator *cell,
                                                                      vtkIdList *globalIds) {
  vtkIdList *idList = cell->GetPointIds();
  vtkIdType inFaceIdBuffer[3];

  for (vtkIdType i = 0; i < 3; ++i) {
    // This guarantee to not take into account the same point more than one times
    globalIds->InsertUniqueId(idList->GetId(i));
    inFaceIdBuffer[i] = idList->GetId(i);
  }

  // Indexing the actual face putting the topology relationship between the
  // three vertexes as Point3i.
  return vcg::Point3i(inFaceIdBuffer[0], inFaceIdBuffer[1], inFaceIdBuffer[2]);
}

int vtkUniformRandomSamplingFilter::fillCoordsIdsFromDataSet(vtkDataSet *data,
                                                             std::vector<vcg::Point3f> &coords,
                                                             std::vector<vcg::Point3i> &ids) {
  vtkOutputWindow *outputWindow = vtkOutputWindow::GetInstance();

  vcg::Point3i topology;

  vtkCellIterator *itr = data->NewCellIterator();
  vtkIdList *globalIds = vtkIdList::New();
  globalIds->Initialize();

  outputWindow->DisplayText("Number of faces: ");
  outputWindow->DisplayText(std::to_string(data->GetNumberOfCells()).c_str());
  outputWindow->DisplayText("\n");

  for (itr->InitTraversal(); !itr->IsDoneWithTraversal(); itr->GoToNextCell()) {
    if (itr->GetPoints()->GetNumberOfPoints() != 3) {
      outputWindow->DisplayErrorText((std::string()
                                      + "The current version of the filter supports "
                                      + "only triangular proper faces. \n").c_str());
      return 0;

    }

    vcg::Point3i topology = retrieveTopologyFromCell(itr, globalIds);

    outputWindow->DisplayText((std::string("T: ") +
                               std::to_string(topology.X()) + " " +
                               std::to_string(topology.Y()) + " " +
                               std::to_string(topology.Z()) + "\n").c_str());

    ids.push_back(topology);
  }

  itr->Delete();

  double pointBuffer[3];

  for (vtkIdType i = 0; i < globalIds->GetNumberOfIds(); ++i) {
    data->GetPoint(globalIds->GetId(i), pointBuffer);

    outputWindow->DisplayText((std::to_string(globalIds->GetId(i)) + ": [" +
                               std::to_string(pointBuffer[0]) + ", " +
                               std::to_string(pointBuffer[1]) + ", " +
                               std::to_string(pointBuffer[2]) + "] \n").c_str());

    coords.push_back(vcg::Point3f(pointBuffer[0], pointBuffer[1], pointBuffer[2]));
  }

  return 1;
}

void vtkUniformRandomSamplingFilter::PrintSelf(ostream &os, vtkIndent indent) {
  this->Superclass::PrintSelf(os, indent);
}

int vtkUniformRandomSamplingFilter::RequestData(vtkInformation *request,
                                                vtkInformationVector **inputVector,
                                                vtkInformationVector *outputVector) {
  using namespace vcg;

  vtkOutputWindow *window = vtkOutputWindow::GetInstance();

  VCGMesh mesh;
  VCGMesh sampledMesh;

  tri::MeshSampler<VCGMesh> mrs(sampledMesh);

  std::vector<Point3f> coordinateVector;
  std::vector<Point3i> indexVector;

  vtkInformation *inInfo = inputVector[0]->GetInformationObject(0);
  vtkInformation *outInfo = outputVector->GetInformationObject(0);
  vtkDataSet *input = vtkDataSet::SafeDownCast(inInfo->Get(vtkDataObject::DATA_OBJECT()));
  vtkPointSet *output = vtkPointSet::SafeDownCast(outInfo->Get(vtkDataObject::DATA_OBJECT()));

  if (fillCoordsIdsFromDataSet(input, coordinateVector, indexVector) == 0) { return 0; }

  tri::BuildMeshFromCoordVectorIndexVector(mesh, coordinateVector, indexVector);
  tri::Clean<VCGMesh>::RemoveDuplicateVertex(mesh);
  tri::Clean<VCGMesh>::RemoveUnreferencedVertex(mesh);
  tri::io::ExporterPLY<VCGMesh>::Save(mesh, "/home/andreamnt94/mesh.ply");

  tri::SurfaceSampling<VCGMesh, tri::TrivialSampler<VCGMesh>>::SamplingRandomGenerator().initialize(time(0));

  std::vector<Point3f> pointVector; float radius = 0.f;
  // tri::PoissonSampling<VCGMesh>(mesh, pointVector, 1000, radius);

  tri::SurfaceSampling<VCGMesh, tri::MeshSampler<VCGMesh>>::VertexUniform(mesh, mrs, 1000);
  tri::io::ExporterPLY<VCGMesh>::Save(sampledMesh, "/home/andreamnt94/point_cloud.ply");

  window->DisplayText("Number of sample point: ");
  window->DisplayText(std::to_string(pointVector.size()).c_str());
  window->DisplayText("\n");

  vtkPoints *samplingSet = vtkPoints::New();

  for (auto &&point : pointVector) {
    window->DisplayText((std::to_string(point.X()) + " " +
                         std::to_string(point.Y()) + " " +
                         std::to_string(point.Z()) + "\n").c_str());
    samplingSet->InsertNextPoint(point.X(), point.Y(), point.Z());
  }

  output->SetPoints(samplingSet);

  return 1;
}
