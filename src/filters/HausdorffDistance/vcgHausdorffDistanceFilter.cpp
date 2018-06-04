// Copyright 2018 Andrea Mantovani

#include "vcgHausdorffDistanceFilter.h"

#include "src/utils/vcgFactory.h"
#include "src/utils/vcgDefaultMesh.h"

#include "vtkDataSet.h"
#include "vtkInformation.h"
#include "vtkInformationVector.h"
#include "vtkObjectFactory.h"

#include "vcg/complex/algorithms/clean.h"
#include "vcg/complex/algorithms/point_sampling.h"

vtkStandardNewMacro(vcgHausdorffDistanceFilter);

vcgHausdorffDistanceFilter::vcgHausdorffDistanceFilter() {
  SetNumberOfInputPorts(2);
}

vcgHausdorffDistanceFilter::~vcgHausdorffDistanceFilter() {}

void vcgHausdorffDistanceFilter::PrintSelf(ostream &os, vtkIndent indent) {
  this->Superclass::PrintSelf(os, indent);
}

int vcgHausdorffDistanceFilter::RequestData(vtkInformation *request,
                                            vtkInformationVector **inputVector,
                                            vtkInformationVector *outputVector) {
  using namespace vcg;
                                          
  using Mesh = utils::VCGDefaultMesh;
  
  vtkInformation *inSourceInfo = inputVector[0]->GetInformationObject(0);
  vtkInformation *inTargetInfo = inputVector[1]->GetInformationObject(0);
  vtkDataSet *sourceData = vtkDataSet::SafeDownCast(inSourceInfo->Get(vtkDataObject::DATA_OBJECT()));
  vtkDataSet *targetData = vtkDataSet::SafeDownCast(inTargetInfo->Get(vtkDataObject::DATA_OBJECT()));

  Mesh source;
  Mesh target;

  if (utils::vcgFactory::BuildVCGMeshFromVTKDataSet(source, sourceData) == 0 ||
      utils::vcgFactory::BuildVCGMeshFromVTKDataSet(target, targetData) == 0) {
    return 0;
  }

  if (source.Tr != Matrix44m::Identity()) {
    tri::UpdatePosition<Mesh>::Matrix(source, source.Tr, true);
  }

  if (target.Tr != Matrix44m::Identity()) {
    tri::UpdatePosition<Mesh>::Matrix(target, target.Tr, true);
  }

  tri::UpdateNormal<Mesh>::PerFaceNormalized(target);

  HausdorffSampler<Mesh> hs(&target);
  hs.dist_upper_bound = 0; // TODO: add property Max Distant

  tri::SurfaceSampling<Mesh, HausdorffSampler<Mesh>>::Montecarlo(source, hs, 1000); // TODO: add property Sample Number

  // Realy needed??
  if (source.Tr != Matrix44m::Identity()) {
    tri::UpdatePosition<Mesh>::Matrix(source, source.Tr, true);
  }

  if (target.Tr != Matrix44m::Identity()) {
    tri::UpdatePosition<Mesh>::Matrix(target, target.Tr, true);
  }

  return 1;
}
