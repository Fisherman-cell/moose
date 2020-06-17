//* This file is part of the MOOSE framework
//* https://www.mooseframework.org
//*
//* All rights reserved, see COPYRIGHT for full restrictions
//* https://github.com/idaholab/moose/blob/master/COPYRIGHT
//*
//* Licensed under LGPL 2.1, please see LICENSE for details
//* https://www.gnu.org/licenses/lgpl-2.1.html

#pragma once

#include "InterfaceMaterial.h"

/**
 * Interface material calculates a variable's jump value across an interface
 */
class InterfaceValueMaterial : public InterfaceMaterial
{
public:
  static InputParameters validParams();

  InterfaceValueMaterial(const InputParameters & parameters);

protected:
  virtual void computeQpProperties() override;
  virtual void initQpStatefulProperties() override;

  const std::string _mp_primary_name;
  const std::string _mp_secondary_name;
  const MaterialProperty<Real> & _mp_primary;
  const MaterialProperty<Real> & _mp_secondary;
  const VariableValue & _var_primary;
  const VariableValue & _var_secondary;
  const VariableValue & _nl_var_primary;
  const VariableValue & _nl_var_secondary;
  /// old values
  const bool _couple_old_values_and_properties;
  const MaterialProperty<Real> * _mp_primary_old;
  const MaterialProperty<Real> * _mp_secondary_old;
  const VariableValue * _var_primary_old;
  const VariableValue * _var_secondary_old;
  const VariableValue * _nl_var_primary_old;
  const VariableValue * _nl_var_secondary_old;
  /// the value type to be computed across the interface
  const MooseEnum _interface_value_type;
  const std::string _mp_out_base_name;
  const std::string _mp_var_out_base_name;
  MaterialProperty<Real> & _interface_value;
  MaterialProperty<Real> & _interface_value_2;
  const MaterialProperty<Real> & _interface_value_old;
  const MaterialProperty<Real> & _interface_value_2_old;
  MaterialProperty<Real> & _interface_value_prev;
  MaterialProperty<Real> & _interface_value_prev_2;
  MaterialProperty<Real> & _jump;
  // previous jump value
  MaterialProperty<Real> & _jump_prev;
};
