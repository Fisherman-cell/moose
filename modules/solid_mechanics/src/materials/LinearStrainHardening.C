//* This file is part of the MOOSE framework
//* https://www.mooseframework.org
//*
//* All rights reserved, see COPYRIGHT for full restrictions
//* https://github.com/idaholab/moose/blob/master/COPYRIGHT
//*
//* Licensed under LGPL 2.1, please see LICENSE for details
//* https://www.gnu.org/licenses/lgpl-2.1.html

#include "LinearStrainHardening.h"
#include "IsotropicPlasticity.h"

registerMooseObject("SolidMechanicsApp", LinearStrainHardening);

InputParameters
LinearStrainHardening::validParams()
{
  InputParameters params = SolidModel::validParams();
  params += IsotropicPlasticity::validParams();

  return params;
}

LinearStrainHardening::LinearStrainHardening(const InputParameters & parameters)
  : SolidModel(parameters)
{

  createConstitutiveModel("IsotropicPlasticity");
}
