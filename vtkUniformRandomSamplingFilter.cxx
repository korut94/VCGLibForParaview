/**
 * Copyright 2018 Andrea Mantovani
 *
 * This file is part of UniformRandomSampling.
 *
 * UniformRandomSampling is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * UniformRandomSampling is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with Foobar. If not, see <http://www.gnu.org/licenses/>.
 */

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
