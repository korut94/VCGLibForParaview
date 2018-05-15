// Copyright 2018 Andrea Mantovani

#ifndef VTKUNIFORMRANDOMSAMPLING_H
#define VTKUNIFORMRANDOMSAMPLING_H

#include "vtkDataSet.h"
#include "vtkPointSetAlgorithm.h"

#include "vcg/complex/complex.h"

class VTK_EXPORT vtkUniformRandomSamplingFilter : public vtkPointSetAlgorithm
{
public:
  static vtkUniformRandomSamplingFilter *New();
  vtkTypeMacro(vtkUniformRandomSamplingFilter, vtkPointSetAlgorithm);

  vtkSetMacro(NumberOfSamples, int);
  vtkGetMacro(NumberOfSamples, int);
  vtkSetMacro(Radius, float);
  vtkGetMacro(Radius, float);

  void PrintSelf(ostream &os, vtkIndent indent);

protected:
  vtkUniformRandomSamplingFilter();
  ~vtkUniformRandomSamplingFilter();

  int RequestData(vtkInformation *request,
                  vtkInformationVector **inputVector,
                  vtkInformationVector *outputVector);

private:
  int NumberOfSamples;
  float Radius;

  vtkUniformRandomSamplingFilter(const vtkUniformRandomSamplingFilter&) = delete;
  vtkUniformRandomSamplingFilter(vtkUniformRandomSamplingFilter&&) = delete;
  vtkUniformRandomSamplingFilter& operator=(const vtkUniformRandomSamplingFilter&) = delete;

  int fillCoordsIdsFromDataSet(vtkDataSet *data,
                               std::vector<vcg::Point3f> &coords,
                               std::vector<vcg::Point3i> &ids);
};

#endif // VTKUNIFORMRANDOMSAMPLING_H


