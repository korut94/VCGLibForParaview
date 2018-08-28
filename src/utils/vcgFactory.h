// Copyright 2018 Andrea Mantovani

// https://github.com/korut94/VCGLibForParaview/tree/master/src/utils

#ifndef UTILSVCGFACTORY_H
#define UTILSVCGFACTORY_H

#include "vtkCellIterator.h"
#include "vtkCellArray.h"
#include "vtkDataSet.h"
#include "vtkPolyData.h"
#include "vtkPolygon.h"
#include "vtkOutputWindow.h"
#include "vtkPoints.h"
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

public:
  template <typename MeshType>
  static int FromVCGMeshExtractVTKPoints(const MeshType &mesh, vtkPoints *points);

  template <typename MeshType>
  static int FromVCGMeshExtractFacesVertices(const MeshType &mesh, int* faces, double *vertices);

  template <typename MeshType>
  static int FromVCGMeshBuildVTKPolyData(const MeshType &mesh, vtkPolyData *polydata);

  template <typename MeshType>
  static int FromVTKDataSetBuildVCGMesh(MeshType &mesh, vtkDataSet *data);

  // The template parameter is not meaningful for the method itself, it just
  // to make the method a template. Using a common no template method the
  // build fails declaring that there are multiple definition of
  // ExtractVertexesFromDataSet method.
  template <typename MeshType>
  static int FromVTKDataSetExtractVCGVertexes(vtkDataSet *data,
                                              std::vector<vcg::Point3f> &coords,
                                              std::vector<vcg::Point3i> &ids);
};

template <typename MeshType>
int vcgFactory::FromVTKDataSetExtractVCGVertexes(
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
int vcgFactory::FromVTKDataSetBuildVCGMesh(MeshType &mesh, vtkDataSet *data) {
  std::vector<vcg::Point3f> coordinateVector;
  std::vector<vcg::Point3i> indexVector;

  int exitCode = 1;

  if (FromVTKDataSetExtractVCGVertexes<MeshType>(data, coordinateVector, indexVector) == 1) {
    vcg::tri::BuildMeshFromCoordVectorIndexVector(mesh, coordinateVector, indexVector);
    vcg::tri::Clean<MeshType>::RemoveDuplicateVertex(mesh);
    vcg::tri::Clean<MeshType>::RemoveUnreferencedVertex(mesh);
  } else {
    exitCode = 0;
  }

  return exitCode;
}

template <typename MeshType>
int vcgFactory::FromVCGMeshExtractVTKPoints(const MeshType &mesh, vtkPoints *points) {
    for (auto &&vertex : mesh.vert) {
      auto point = vertex.P();
      points->InsertNextPoint(point.X(), point.Y(), point.Z());
    }

    return 1;
}

template <typename MeshType>
int vcgFactory::FromVCGMeshExtractFacesVertices(const MeshType &mesh, int* faces, double *vertices) {
	// Only works with triangular meshes only (no face with more than 3 vertices)

	int i = 0;
	vcg::SimpleTempData<typename const MeshType::VertContainer, int> indices(mesh.vert);
	for (auto &&vertex : mesh.vert) {
		indices[vertex] = i++;
	}
	i = 0;
	for (auto &&f : mesh.face) {
		for (int k = 0; k < f.VN(); ++k)
			faces[3 * i + k] = indices[f.cV(k)];
		i++;
	}

	i = 0;
	for (auto &&vertex : mesh.vert) {
		auto point = vertex.P();
		vertices[3 * i] = point.X();
		vertices[3 * i + 1] = point.Y();
		vertices[3 * i + 2] = point.Z();
		i++;
	}

	return 1;
}

template <typename MeshType>
int vcgFactory::FromVCGMeshBuildVTKPolyData(const MeshType &mesh, vtkPolyData *polydata) {

  // Only for triangular polygons

	vtkSmartPointer<vtkCellArray> polygons = vtkSmartPointer<vtkCellArray>::New();
	vtkSmartPointer<vtkPolygon> polygon = vtkSmartPointer<vtkPolygon>::New();
	vtkSmartPointer<vtkPoints> points = vtkSmartPointer<vtkPoints>::New();

	int* faces = new int[3 * mesh.fn];
	double *vertices = new double[3 * mesh.vn];
	int res = FromVCGMeshExtractFacesVertices(mesh, faces, vertices);

	for (int i = 0; i < mesh.vn; i++)
		points->InsertNextPoint(vertices[3 * i], vertices[3 * i + 1], vertices[3 * i + 2]);

	polygon->GetPointIds()->SetNumberOfIds(3);
	for (int i = 0; i < mesh.fn; i++) {
		polygon->GetPointIds()->SetId(0, faces[3 * i]);
		polygon->GetPointIds()->SetId(1, faces[3 * i + 1]);
		polygon->GetPointIds()->SetId(2, faces[3 * i + 2]);
		polygons->InsertNextCell(polygon);
	}

	polydata->SetPoints(points);
	polydata->SetPolys(polygons);

	delete faces;
	delete vertices;

	return 1;
}
} // namespace utils

#endif // UTILSVCGFACTORY_H
