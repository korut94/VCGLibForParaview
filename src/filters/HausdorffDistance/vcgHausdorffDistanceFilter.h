// Copyright 2018 Andrea Mantovani

#ifndef VCGHAUSDORFFDISTANCEFILTER_H
#define VCGHAUSDORFFDISTANCEFILET_H

#include "vtkPointSetAlgorithm.h"

class VTK_EXPORT vcgHausdorffDistanceFilter : public vtkPointSetAlgorithm
{
public:
  static vcgHausdorffDistanceFilter* New();
  vtkTypeMacro(vcgHausdorffDistanceFilter, vtkPointSetAlgorithm);

  void PrintSelf(ostream &os, vtkIndent indent);

protected:
  vcgHausdorffDistanceFilter();
  ~vcgHausdorffDistanceFilter();

  int RequestData(vtkInformation *request,
                  vtkInformationVector **inputVector,
                  vtkInformationVector *outputVector);

private:
  vcgHausdorffDistanceFilter(const vcgHausdorffDistanceFilter&) = delete;
  vcgHausdorffDistanceFilter(vcgHausdorffDistanceFilter&&) = delete;
  vcgHausdorffDistanceFilter& operator=(const vcgHausdorffDistanceFilter&) = delete;
};

#endif // VCGHAUSDORFFDISTANCEFILTER_H

