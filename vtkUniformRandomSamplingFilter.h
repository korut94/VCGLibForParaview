/**
 * Copyright 2018 Andrea Mantovani
 *
 * This file is part of UniformRandomSampling.
 *
 * UniformRandomSampling is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * UniformRandomSampling is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with Foobar. If not, see <http://www.gnu.org/licenses/>.
 */

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
