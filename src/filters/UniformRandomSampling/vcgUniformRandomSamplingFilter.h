// Copyright 2018 Andrea Mantovani

#ifndef VCGUNIFORMRANDOMSAMPLING_H
#define VCGUNIFORMRANDOMSAMPLING_H

#include "vtkDataSet.h"
#include "vtkPointSetAlgorithm.h"

#include "vcg/complex/complex.h"

class VTK_EXPORT vcgUniformRandomSamplingFilter : public vtkPointSetAlgorithm
{
public:
  static vcgUniformRandomSamplingFilter *New();
  vtkTypeMacro(vcgUniformRandomSamplingFilter, vtkPointSetAlgorithm);

  vtkSetMacro(NumberOfSamples, int);
  vtkGetMacro(NumberOfSamples, int);
  vtkSetMacro(Radius, float);
  vtkGetMacro(Radius, float);

  void PrintSelf(ostream &os, vtkIndent indent);

protected:
  vcgUniformRandomSamplingFilter();
  ~vcgUniformRandomSamplingFilter();

  int RequestData(vtkInformation *request,
                  vtkInformationVector **inputVector,
                  vtkInformationVector *outputVector);

private:
  static const int _DEFAULT_NUMBER_OF_SAMPLES = 1000;

  int NumberOfSamples;
  float Radius;

  vcgUniformRandomSamplingFilter(const vcgUniformRandomSamplingFilter&) = delete;
  vcgUniformRandomSamplingFilter(vcgUniformRandomSamplingFilter&&) = delete;
  vcgUniformRandomSamplingFilter& operator=(const vcgUniformRandomSamplingFilter&) = delete;

  int fillCoordsIdsFromDataSet(vtkDataSet *data,
                               std::vector<vcg::Point3f> &coords,
                               std::vector<vcg::Point3i> &ids);
};

#endif // VCGUNIFORMRANDOMSAMPLING_H


