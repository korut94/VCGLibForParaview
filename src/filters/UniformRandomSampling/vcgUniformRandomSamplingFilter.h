// Copyright 2018 Andrea Mantovani

#ifndef VCGUNIFORMRANDOMSAMPLING_H
#define VCGUNIFORMRANDOMSAMPLING_H

#include "vtkPointSetAlgorithm.h"

class VTK_EXPORT vcgUniformRandomSamplingFilter : public vtkPointSetAlgorithm
{
public:
  static vcgUniformRandomSamplingFilter *New();
  vtkTypeMacro(vcgUniformRandomSamplingFilter, vtkPointSetAlgorithm);

  vtkGetMacro(NumberOfSamples, int);
  vtkGetMacro(Radius, float);

  vtkSetMacro(NumberOfSamples, int);
  vtkSetMacro(Radius, float);

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
};

#endif // VCGUNIFORMRANDOMSAMPLING_H


