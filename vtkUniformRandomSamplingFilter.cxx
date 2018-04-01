// Copyright 2018 Andrea Mantovani

#include "vtkUniformRandomSamplingFilter.h"

#include "vtkCellIterator.h"
#include "vtkInformation.h"
#include "vtkInformationVector.h"
#include "vtkObjectFactory.h"
#include "vtkOutputWindow.h"
#include "vtkPolyData.h"

#include "vcg/complex/complex.h"

#include <assert.h>

vtkStandardNewMacro(vtkUniformRandomSamplingFilter);

vtkUniformRandomSamplingFilter::vtkUniformRandomSamplingFilter() {}

vtkUniformRandomSamplingFilter::~vtkUniformRandomSamplingFilter() {}

void vtkUniformRandomSamplingFilter::PrintSelf(ostream &os, vtkIndent indent) {
  this->Superclass::PrintSelf(os, indent);
}

int vtkUniformRandomSamplingFilter::RequestData(vtkInformation *request,
                                                vtkInformationVector **inputVector,
                                                vtkInformationVector *outputVector) {
  vtkOutputWindow *outputWindow = vtkOutputWindow::GetInstance();

  vtkInformation *input = inputVector[0]->GetInformationObject(0);
  vtkPolyData *polyData = vtkPolyData::SafeDownCast(input->Get(vtkDataObject::DATA_OBJECT()));

  vtkCellIterator *itr = polyData->NewCellIterator();
  double pointBuffer[3];
  uint InFaceIdPointBuffer[3];
  std::vector<vcg::Point3f> coordinateVector;
  std::vector<vcg::Point3i> indexVector;

  outputWindow->DisplayText("Number of faces: ");
  outputWindow->DisplayText(std::to_string(itr->GetNumberOfFaces()).c_str());
  outputWindow->DisplayText("\n");

  for (itr->InitTraversal(); !itr->IsDoneWithTraversal(); itr->GoToNextCell()) {
    vtkPoints *pointSet = itr->GetPoints();
    vtkIdList *idList = itr->GetPointIds();

    outputWindow->DisplayText("Number of points in the current face: ");
    outputWindow->DisplayText(std::to_string(idList->GetNumberOfIds()).c_str());
    outputWindow->DisplayText("\n");

    // The actual version of the filter doesn't support no triangular proper
    // face.
    // TODO: Triangularize the mesh in input or require it is already trangularized.
    if (idList->GetNumberOfIds() != 3) { return 0; }

    for (vtkIdType i = 0; i < idList->GetNumberOfIds(); ++i) {
      pointSet->GetPoint(idList->GetId(i), pointBuffer);
      outputWindow->DisplayText((std::to_string(idList->GetId(i)) + ": (" +
                                 std::to_string(pointBuffer[0]) + ", " +
                                 std::to_string(pointBuffer[1]) + ", " +
                                 std::to_string(pointBuffer[2]) + ")").c_str());
      outputWindow->DisplayText("\n");

      coordinateVector.push_back(vcg::Point3f(pointBuffer[0], pointBuffer[1], pointBuffer[2]));
      InFaceIdPointBuffer[i] = static_cast<uint>(idList->GetId(i));
    }

    // Indexing the actual face putting the topology relationship between the
    // three vertexes as Point3i with the following meaning:
    // * first: the index of the left neighbour vertex
    // * second: the index of the actual vertex
    // * third: the index of the right neighbour vertex

    // Indexing of the vertex with id InFaceIdPointBuffer[0]
    indexVector.push_back(vcg::Point3i(InFaceIdPointBuffer[2],
                                       InFaceIdPointBuffer[0],
                                       InFaceIdPointBuffer[1]));
    // Indexing of the vertex with id InFaceIdPointBuffer[1]
    indexVector.push_back(vcg::Point3i(InFaceIdPointBuffer[0],
                                       InFaceIdPointBuffer[1],
                                       InFaceIdPointBuffer[2]));
    // Indexing of the vertex with id InFaceIdPointBuffer[2]
    indexVector.push_back(vcg::Point3i(InFaceIdPointBuffer[1],
                                       InFaceIdPointBuffer[2],
                                       InFaceIdPointBuffer[0]));



    outputWindow->DisplayText("\n");
  }

  itr->Delete();

  return 1;
}
