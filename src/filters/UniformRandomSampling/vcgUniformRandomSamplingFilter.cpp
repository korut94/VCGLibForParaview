// Copyright 2018 Andrea Mantovani

#include "vcgUniformRandomSamplingFilter.h"

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

vtkStandardNewMacro(vcgUniformRandomSamplingFilter);

vcgUniformRandomSamplingFilter::vcgUniformRandomSamplingFilter()
  : NumberOfSamples(1000),
    Radius(0.f) {}

vcgUniformRandomSamplingFilter::~vcgUniformRandomSamplingFilter() {}

int vcgUniformRandomSamplingFilter::fillCoordsIdsFromDataSet(vtkDataSet *data,
                                                             std::vector<vcg::Point3f> &coords,
                                                             std::vector<vcg::Point3i> &ids) {
  vtkOutputWindow *outputWindow = vtkOutputWindow::GetInstance();

  vcg::Point3i topology;
  double pointBuffer[3];

  vtkCellIterator *itr = data->NewCellIterator();
  vtkIdList *inFaceIdsList = vtkIdList::New();

  inFaceIdsList->Initialize();

  outputWindow->DisplayText("Number of faces: ");
  outputWindow->DisplayText(std::to_string(data->GetNumberOfCells()).c_str());
  outputWindow->DisplayText("\n");

  for (itr->InitTraversal(); !itr->IsDoneWithTraversal(); itr->GoToNextCell()) {
    if (itr->GetCellType() != VTK_TRIANGLE) {
      outputWindow->DisplayErrorText((std::string()
                                      + "The current version of the filter supports "
                                      + "only triangular proper faces. \n").c_str());
      return 0;

    }

    data->GetCellPoints(itr->GetCellId(), inFaceIdsList);

    topology = vcg::Point3i(inFaceIdsList->GetId(0), inFaceIdsList->GetId(2), inFaceIdsList->GetId(1));

    outputWindow->DisplayText((std::string("T: ") +
                               std::to_string(topology.X()) + " " +
                               std::to_string(topology.Y()) + " " +
                               std::to_string(topology.Z()) + "\n").c_str());

    ids.push_back(topology);
  }

  inFaceIdsList->Delete();
  itr->Delete();

  for (vtkIdType i = 0; i < data->GetNumberOfPoints(); ++i) {
    data->GetPoint(i, pointBuffer);

    outputWindow->DisplayText((std::to_string(i) + ": [" +
                               std::to_string(pointBuffer[0]) + ", " +
                               std::to_string(pointBuffer[1]) + ", " +
                               std::to_string(pointBuffer[2]) + "] \n").c_str());

    coords.push_back(vcg::Point3f(pointBuffer[0], pointBuffer[1], pointBuffer[2]));
  }

  return 1;
}

void vcgUniformRandomSamplingFilter::PrintSelf(ostream &os, vtkIndent indent) {
  this->Superclass::PrintSelf(os, indent);
}

int vcgUniformRandomSamplingFilter::RequestData(vtkInformation *request,
                                                vtkInformationVector **inputVector,
                                                vtkInformationVector *outputVector) {
  using namespace vcg;

  vtkOutputWindow *window = vtkOutputWindow::GetInstance();

  VCGMesh mesh;
  VCGMesh sampledMesh;

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

  tri::SurfaceSampling<VCGMesh, tri::TrivialSampler<VCGMesh>>::SamplingRandomGenerator().initialize(time(0));

  std::vector<Point3f> pointVector;
  tri::PoissonSampling<VCGMesh>(mesh, pointVector, NumberOfSamples, Radius);

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
