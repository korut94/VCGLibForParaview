// Copyright 2018 Andrea Mantovani

#include "vcgUniformRandomSamplingFilter.h"

#include "src/utils/vcgFactory.h"
#include "src/utils/vcgDefaultMesh.h"

#include "vtkInformation.h"
#include "vtkInformationVector.h"
#include "vtkObjectFactory.h"
#include "vtkPoints.h"
#include "vtkPointSet.h"

#include "vcg/complex/algorithms/point_sampling.h"

#include <algorithm>

vtkStandardNewMacro(vcgUniformRandomSamplingFilter);

vcgUniformRandomSamplingFilter::vcgUniformRandomSamplingFilter()
  : NumberOfSamples(_DEFAULT_NUMBER_OF_SAMPLES),
    Radius(0.f) {}

vcgUniformRandomSamplingFilter::~vcgUniformRandomSamplingFilter() {}


void vcgUniformRandomSamplingFilter::PrintSelf(ostream &os, vtkIndent indent) {
  this->Superclass::PrintSelf(os, indent);
}

int vcgUniformRandomSamplingFilter::RequestData(vtkInformation *request,
                                                vtkInformationVector **inputVector,
                                                vtkInformationVector *outputVector) {
  using namespace vcg;
  using Mesh = utils::VCGDefaultMesh;

  Mesh mesh;

  vtkInformation *inInfo = inputVector[0]->GetInformationObject(0);
  vtkInformation *outInfo = outputVector->GetInformationObject(0);

  vtkDataSet *input = vtkDataSet::SafeDownCast(inInfo->Get(vtkDataObject::DATA_OBJECT()));
  vtkPointSet *output = vtkPointSet::SafeDownCast(outInfo->Get(vtkDataObject::DATA_OBJECT()));

  if (utils::vcgFactory::FromVTKDataSetBuildVCGMesh(mesh, input) == 0) { return 0; }

  tri::SurfaceSampling<Mesh, tri::TrivialSampler<Mesh>>::SamplingRandomGenerator().initialize(time(0));

  std::vector<Point3f> pointVector;

  if (NumberOfSamples == 0 && Radius == 0.f) { NumberOfSamples = _DEFAULT_NUMBER_OF_SAMPLES; }
  tri::PoissonSampling<Mesh>(mesh, pointVector, NumberOfSamples, Radius);

  vtkPoints *samplingSet = vtkPoints::New();

  for (auto &&point : pointVector) {
    samplingSet->InsertNextPoint(point.X(), point.Y(), point.Z());
  }

  output->SetPoints(samplingSet);
  samplingSet->Delete();

  return 1;
}
