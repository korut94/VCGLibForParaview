// Copyright 2018 Andrea Mantovani

#include "vcgUniformRandomSamplingFilter.h"

#include "src/utils/vcgFactory.h"
#include "src/utils/vcgDefaultMesh.h"

#include "vtkInformation.h"
#include "vtkInformationVector.h"
#include "vtkObjectFactory.h"
#include "vtkOutputWindow.h"
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

  vtkOutputWindow *window = vtkOutputWindow::GetInstance();

  Mesh mesh;

  vtkInformation *inInfo = inputVector[0]->GetInformationObject(0);
  vtkInformation *outInfo = outputVector->GetInformationObject(0);
  vtkDataSet *input = vtkDataSet::SafeDownCast(inInfo->Get(vtkDataObject::DATA_OBJECT()));
  vtkPointSet *output = vtkPointSet::SafeDownCast(outInfo->Get(vtkDataObject::DATA_OBJECT()));

  if (utils::vcgFactory::BuildVCGMeshFromVTKDataSet(mesh, input) == 0) { return 0; }

  tri::SurfaceSampling<Mesh, tri::TrivialSampler<Mesh>>::SamplingRandomGenerator().initialize(time(0));

  std::vector<Point3f> pointVector;

  if (NumberOfSamples == 0 && Radius == 0.f) { NumberOfSamples = _DEFAULT_NUMBER_OF_SAMPLES; }
  tri::PoissonSampling<Mesh>(mesh, pointVector, NumberOfSamples, Radius);

  window->DisplayText("Number of sample point: ");
  window->DisplayText(std::to_string(pointVector.size()).c_str());
  window->DisplayText("\n");

  vtkPoints *samplingSet = vtkPoints::New();

  for (auto &&point : pointVector) {
    window->DisplayText((std::to_string(point.X()) + " " +
                         std::to_string(point.Y()) + " " +
                         std::to_string(point.Z()) + "\n").c_str());
    samplingSet->InsertNextPoint(point.X(), point.Y(), point.Z());
  }

  output->SetPoints(samplingSet);

  return 1;
}
