// Copyright 2018 Andrea Mantovani

#include "vcgHausdorffDistanceFilter.h"

#include "vtkInformation.h"
#include "vtkInformationVector.h"
#include "vtkObjectFactory.h"

vtkStandardNewMacro(vcgHausdorffDistanceFilter);

vcgHausdorffDistanceFilter::vcgHausdorffDistanceFilter() {}
vcgHausdorffDistanceFilter::~vcgHausdorffDistanceFilter() {}

void vcgHausdorffDistanceFilter::PrintSelf(ostream &os, vtkIndent indent) {
  this->Superclass::PrintSelf(os, indent);
}

int vcgHausdorffDistanceFilter::RequestData(vtkInformation *request,
                                            vtkInformationVector **inputVector,
                                            vtkInformationVector *outputVector) {
  return 1;
}
