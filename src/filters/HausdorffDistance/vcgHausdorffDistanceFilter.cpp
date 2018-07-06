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
  SetNumberOfOutputPorts(2);
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
  vtkInformation *outInfoSource = outputVector->GetInformationObject(0);
  vtkInformation *outInfoTarget = outputVector->GetInformationObject(1);

  vtkDataSet *sourceData = vtkDataSet::SafeDownCast(inSourceInfo->Get(vtkDataObject::DATA_OBJECT()));
  vtkDataSet *targetData = vtkDataSet::SafeDownCast(inTargetInfo->Get(vtkDataObject::DATA_OBJECT()));
  vtkPointSet *outputSource = vtkPointSet::SafeDownCast(outInfoSource->Get(vtkDataObject::DATA_OBJECT()));
  vtkPointSet *outputTarget = vtkPointSet::SafeDownCast(outInfoTarget->Get(vtkDataObject::DATA_OBJECT()));

  Mesh source;
  Mesh target;
  Mesh samplePointMesh;
  Mesh closestPointMesh;

  if (utils::vcgFactory::FromVTKDataSetBuildVCGMesh(source, sourceData) == 0 ||
      utils::vcgFactory::FromVTKDataSetBuildVCGMesh(target, targetData) == 0) {
    return 0;
  }

  tri::UpdateNormal<Mesh>::PerFaceNormalized(target);

  HausdorffSampler<Mesh> hs(&target);

  if (SaveSample) { hs.init(&samplePointMesh, &closestPointMesh); }

  hs.dist_upper_bound = source.bbox.Diag() * DiagonalPercent;

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
  
  if (SaveSample) {
    tri::UpdateBounding<Mesh>::Box(samplePointMesh);
    tri::UpdateBounding<Mesh>::Box(closestPointMesh);

    vtkPoints *sourceCloudPoint = vtkPoints::New();
    vtkPoints *targetCloudPoint = vtkPoints::New();

    utils::vcgFactory::FromVCGMeshExtractVTKPoints(samplePointMesh, sourceCloudPoint);
    utils::vcgFactory::FromVCGMeshExtractVTKPoints(closestPointMesh, targetCloudPoint);

    outputSource->SetPoints(sourceCloudPoint);
    outputTarget->SetPoints(targetCloudPoint);

    sourceCloudPoint->Delete();
    targetCloudPoint->Delete();
  }

  outputSource->GetFieldData()->AddArray(distanceRangeFieldData);
  outputSource->GetFieldData()->AddArray(hausdorffDistanceFieldData);
  outputSource->GetFieldData()->AddArray(rmsDistanceFieldData);

  return 1;
}
