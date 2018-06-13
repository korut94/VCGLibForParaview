// Copyright 2018 Andrea Mantovani

#include "vcgHausdorffDistanceFilter.h"

#include "src/utils/vcgFactory.h"
#include "src/utils/vcgDefaultMesh.h"

#include "vtkDataSet.h"
#include "vtkDoubleArray.h"
#include "vtkFieldData.h"
#include "vtkInformation.h"
#include "vtkInformationVector.h"
#include "vtkObjectFactory.h"
#include "vtkPointSet.h"
#include "vtkSmartPointer.h"

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
  vtkInformation *outInfo = outputVector->GetInformationObject(0);

  vtkDataSet *sourceData = vtkDataSet::SafeDownCast(inSourceInfo->Get(vtkDataObject::DATA_OBJECT()));
  vtkDataSet *targetData = vtkDataSet::SafeDownCast(inTargetInfo->Get(vtkDataObject::DATA_OBJECT()));
  vtkPointSet *output = vtkPointSet::SafeDownCast(outInfo->Get(vtkDataObject::DATA_OBJECT()));

  Mesh source;
  Mesh target;

  if (utils::vcgFactory::BuildVCGMeshFromVTKDataSet(source, sourceData) == 0 ||
      utils::vcgFactory::BuildVCGMeshFromVTKDataSet(target, targetData) == 0) {
    return 0;
  }

  tri::UpdateNormal<Mesh>::PerFaceNormalized(target);

  HausdorffSampler<Mesh> hs(&target);
  hs.dist_upper_bound = 1000; // TODO: add property Max Distant

  if (SampleVert) {
    tri::SurfaceSampling<Mesh, HausdorffSampler<Mesh>>::VertexUniform(source, hs, NumberOfSamples);
  } if (SampleEdge) {
    tri::SurfaceSampling<Mesh, HausdorffSampler<Mesh>>::EdgeUniform(source, hs, NumberOfSamples, SampleFauxEdge);
  } if (SampleFace) {
    tri::SurfaceSampling<Mesh, HausdorffSampler<Mesh>>::Montecarlo(source, hs, NumberOfSamples);
  }

  double distanceRange[3] = {hs.getMinDist(), hs.getMeanDist(), hs.getMaxDist()};

  vtkSmartPointer<vtkDoubleArray> hausdorffDistanceFieldData = vtkSmartPointer<vtkDoubleArray>::New();
  hausdorffDistanceFieldData->SetNumberOfComponents(3);
  hausdorffDistanceFieldData->SetName("DistanceRange");
  hausdorffDistanceFieldData->InsertNextTuple(distanceRange);

  vtkSmartPointer<vtkDoubleArray> distanceRangeFieldData = vtkSmartPointer<vtkDoubleArray>::New();
  distanceRangeFieldData->SetNumberOfComponents(1);
  distanceRangeFieldData->SetName("HausdorffDistance");
  distanceRangeFieldData->InsertNextValue(hs.getMaxDist());

  vtkSmartPointer<vtkDoubleArray> rmsDistanceFieldData = vtkSmartPointer<vtkDoubleArray>::New();
  rmsDistanceFieldData->SetNumberOfComponents(1);
  rmsDistanceFieldData->SetName("RMSDistance");
  rmsDistanceFieldData->InsertNextValue(hs.getRMSDist());

  output->DeepCopy(sourceData);
  output->GetFieldData()->AddArray(distanceRangeFieldData);
  output->GetFieldData()->AddArray(hausdorffDistanceFieldData);
  output->GetFieldData()->AddArray(rmsDistanceFieldData);

  return 1;
}
