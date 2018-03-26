// Copyright 2018 Andrea Mantovani

#ifndef VTKUNIFORMRANDOMSAMPLING_H
#define VTKUNIFORMRANDOMSAMPLING_H

#include "vtkPointSetAlgorithm.h"

class VTK_EXPORT vtkUniformRandomSamplingFilter : public vtkPointSetAlgorithm
{
public:
  static vtkUniformRandomSamplingFilter *New();
  vtkTypeMacro(vtkUniformRandomSamplingFilter, vtkPointSetAlgorithm);

  void PrintSelf(ostream &os, vtkIndent indent);

protected:
  vtkUniformRandomSamplingFilter();
  ~vtkUniformRandomSamplingFilter();

  int RequestData(vtkInformation * request,
                  vtkInformationVector **inputVector,
                  vtkInformationVector *outputVector);

private:
  vtkUniformRandomSamplingFilter(const vtkUniformRandomSamplingFilter&) = delete;
  vtkUniformRandomSamplingFilter(vtkUniformRandomSamplingFilter&&) = delete;
  vtkUniformRandomSamplingFilter& operator=(const vtkUniformRandomSamplingFilter&) = delete;
};

#endif // VTKUNIFORMRANDOMSAMPLING_H
