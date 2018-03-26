// Copyright 2018 Andrea Mantovani

#include "vtkUniformRandomSamplingFilter.h"

#include "vtkInformation.h"
#include "vtkInformationVector.h"
#include "vtkObjectFactory.h"
#include "vtkOutputWindow.h"
#include "vtkPointSet.h"

#include "vcg/complex/complex.h"

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
  vtkPointSet *pointSet = vtkPointSet::SafeDownCast(input->Get(vtkDataObject::DATA_OBJECT()));

  double bufferPoint[3];
  std::vector<vcg::Point3f> coordinateVector;
  std::vector<vcg::Point3i> indexVector;

  for (vtkIdType i = 0; i < pointSet->GetNumberOfPoints(); ++i) {
    pointSet->GetPoint(i, bufferPoint);
    coordinateVector.push_back(vcg::Point3f(bufferPoint[0], bufferPoint[1], bufferPoint[2]));
  }

  outputWindow->DisplayText(std::to_string(coordinateVector.size()).c_str());

  return 1;
}
