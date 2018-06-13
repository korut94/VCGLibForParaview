#ifndef UTILSVCGFACTORY_H
#define UTILSVCGFACTORY_H

#include "vtkCellIterator.h"
#include "vtkDataSet.h"
#include "vtkOutputWindow.h"

#include "vcg/complex/algorithms/create/platonic.h"

namespace utils
{
class vcgFactory
{
private:
  vcgFactory() = default;

  static constexpr const char *MESSAGE_NO_TRIANGULAR_FACE = "Some cells of the mesh "
    "are not triangular proper face. Please consider to apply the Paraview's "
    "filter 'Triangulate' over your mesh and retry.";

  // The template parameter is not meaningful for the method itself, it just
  // to make the method a template. Using a common no template method the
  // build fails declaring that there are multiple definition of
  // ExtractVertexesFromDataSet method.
  template <typename MeshType>
  static int ExtractVertexesFromDataSet(vtkDataSet *data,
                                        std::vector<vcg::Point3f> &coords,
                                        std::vector<vcg::Point3i> &ids);

public:
  template <typename MeshType>
  static int BuildVCGMeshFromVTKDataSet(MeshType &mesh, vtkDataSet *data);
};

template <typename MeshType>
int vcgFactory::ExtractVertexesFromDataSet(
  vtkDataSet *data, 
  std::vector<vcg::Point3f> &coords, 
  std::vector<vcg::Point3i> &ids) {
  vtkOutputWindow *outputWindow = vtkOutputWindow::GetInstance();

  vcg::Point3i topology;
  double pointBuffer[3];

  vtkCellIterator *itr = data->NewCellIterator();
  vtkIdList *inFaceIdsList = vtkIdList::New();

  inFaceIdsList->Initialize();
#ifdef NDEBUG
  outputWindow->DisplayText("Number of faces: ");
  outputWindow->DisplayText(std::to_string(data->GetNumberOfCells()).c_str());
  outputWindow->DisplayText("\n");
#endif

  for (itr->InitTraversal(); !itr->IsDoneWithTraversal(); itr->GoToNextCell()) {
    if (itr->GetCellType() != VTK_TRIANGLE) {
      outputWindow->DisplayErrorText(MESSAGE_NO_TRIANGULAR_FACE);
      inFaceIdsList->Delete();
      itr->Delete();

      return 0;
    }

    data->GetCellPoints(itr->GetCellId(), inFaceIdsList);

    topology = vcg::Point3i(inFaceIdsList->GetId(0), inFaceIdsList->GetId(2), inFaceIdsList->GetId(1));
#ifdef NDEBUG
    outputWindow->DisplayText((std::string("T: ") +
                               std::to_string(topology.X()) + " " +
                               std::to_string(topology.Y()) + " " +
                               std::to_string(topology.Z()) + "\n").c_str());
#endif
    ids.push_back(topology);
  }

  inFaceIdsList->Delete();
  itr->Delete();

  for (vtkIdType i = 0; i < data->GetNumberOfPoints(); ++i) {
    data->GetPoint(i, pointBuffer);
#ifdef NDEBUG
    outputWindow->DisplayText((std::to_string(i) + ": [" +
                               std::to_string(pointBuffer[0]) + ", " +
                               std::to_string(pointBuffer[1]) + ", " +
                               std::to_string(pointBuffer[2]) + "] \n").c_str());
#endif
    coords.push_back(vcg::Point3f(pointBuffer[0], pointBuffer[1], pointBuffer[2]));
  }

  return 1;
}

template <typename MeshType>
int vcgFactory::BuildVCGMeshFromVTKDataSet(MeshType &mesh, vtkDataSet *data) {
  std::vector<vcg::Point3f> coordinateVector;
  std::vector<vcg::Point3i> indexVector;

  int exitCode = 1;

  if (ExtractVertexesFromDataSet<MeshType>(data, coordinateVector, indexVector) == 1) {
    vcg::tri::BuildMeshFromCoordVectorIndexVector(mesh, coordinateVector, indexVector);
    vcg::tri::Clean<MeshType>::RemoveDuplicateVertex(mesh);
    vcg::tri::Clean<MeshType>::RemoveUnreferencedVertex(mesh);
  } else {
    exitCode = 0;
  }

  return exitCode;
}
} // namespace utils 

#endif // UTILSVCGFACTORY_H