//* This file is part of the MOOSE framework
//* https://www.mooseframework.org
//*
//* All rights reserved, see COPYRIGHT for full restrictions
//* https://github.com/idaholab/moose/blob/master/COPYRIGHT
//*
//* Licensed under LGPL 2.1, please see LICENSE for details
//* https://www.gnu.org/licenses/lgpl-2.1.html

#include "NeighborCoupleable.h"

#include "FEProblem.h"
#include "MooseError.h" // mooseDeprecated
#include "MooseVariableFE.h"
#include "Problem.h"
#include "SubProblem.h"

NeighborCoupleable::NeighborCoupleable(const MooseObject * moose_object,
                                       bool nodal,
                                       bool neighbor_nodal)
  : Coupleable(moose_object, nodal), _neighbor_nodal(neighbor_nodal)
{
}

NeighborCoupleable::~NeighborCoupleable() {}

const VariableValue &
NeighborCoupleable::coupledNeighborValue(const std::string & var_name, unsigned int comp)
{
  MooseVariable * var = getVar(var_name, comp);
  if (_neighbor_nodal)
    return (_c_is_implicit) ? var->dofValuesNeighbor() : var->dofValuesOldNeighbor();
  else
    return (_c_is_implicit) ? var->slnNeighbor() : var->slnOldNeighbor();
}

const VariableValue &
NeighborCoupleable::coupledNeighborValueDot(const std::string & var_name, unsigned int comp)
{
  MooseVariable * var = getVar(var_name, comp);
  if (_neighbor_nodal)
    return var->dofValuesDotNeighbor();
  else
    return var->uDotNeighbor();
}

const VariableValue &
NeighborCoupleable::coupledNeighborValueDotDu(const std::string & var_name, unsigned int comp)
{
  MooseVariable * var = getVar(var_name, comp);
  if (_neighbor_nodal)
    return var->dofValuesDuDotDuNeighbor();
  else
    return var->duDotDuNeighbor();
}

const VariableValue &
NeighborCoupleable::coupledNeighborValueOld(const std::string & var_name, unsigned int comp)
{
  validateExecutionerType(var_name, "coupledNeighborValueOld");

  MooseVariable * var = getVar(var_name, comp);
  if (_neighbor_nodal)
    return (_c_is_implicit) ? var->dofValuesOldNeighbor() : var->dofValuesOlderNeighbor();
  else
    return (_c_is_implicit) ? var->slnOldNeighbor() : var->slnOlderNeighbor();
}

const VariableValue &
NeighborCoupleable::coupledNeighborValueOlder(const std::string & var_name, unsigned int comp)
{
  validateExecutionerType(var_name, "coupledNeighborValueOlder");

  MooseVariable * var = getVar(var_name, comp);
  if (_neighbor_nodal)
  {
    if (_c_is_implicit)
      return var->dofValuesOlderNeighbor();
    else
      mooseError("Older values not available for explicit schemes");
  }
  else
  {
    if (_c_is_implicit)
      return var->slnOlderNeighbor();
    else
      mooseError("Older values not available for explicit schemes");
  }
}

const VariableGradient &
NeighborCoupleable::coupledNeighborGradient(const std::string & var_name, unsigned int comp)
{
  if (_neighbor_nodal)
    mooseError("Nodal variables do not have gradients");

  MooseVariable * var = getVar(var_name, comp);
  return (_c_is_implicit) ? var->gradSlnNeighbor() : var->gradSlnOldNeighbor();
}

const VariableGradient &
NeighborCoupleable::coupledNeighborGradientOld(const std::string & var_name, unsigned int comp)
{
  if (_neighbor_nodal)
    mooseError("Nodal variables do not have gradients");

  validateExecutionerType(var_name, "coupledNeighborGradientOld");
  MooseVariable * var = getVar(var_name, comp);
  return (_c_is_implicit) ? var->gradSlnOldNeighbor() : var->gradSlnOlderNeighbor();
}

const VariableGradient &
NeighborCoupleable::coupledNeighborGradientOlder(const std::string & var_name, unsigned int comp)
{
  if (_neighbor_nodal)
    mooseError("Nodal variables do not have gradients");

  validateExecutionerType(var_name, "coupledNeighborGradientOlder");
  MooseVariable * var = getVar(var_name, comp);
  if (_c_is_implicit)
    return var->gradSlnOlderNeighbor();
  else
    mooseError("Older values not available for explicit schemes");
}

const VectorVariableGradient &
NeighborCoupleable::coupledVectorNeighborGradient(const std::string & var_name, unsigned int comp)
{
  if (_neighbor_nodal)
    mooseError("Gradients are non-sensical with nodal compute objects");

  VectorMooseVariable * var = getVectorVar(var_name, comp);
  return (_c_is_implicit) ? var->gradSlnNeighbor() : var->gradSlnOldNeighbor();
}

const VectorVariableGradient &
NeighborCoupleable::coupledVectorNeighborGradientOld(const std::string & var_name,
                                                     unsigned int comp)
{
  if (_neighbor_nodal)
    mooseError("Gradients are non-sensical with nodal compute objects");

  validateExecutionerType(var_name, "coupledNeighborGradientOld");
  VectorMooseVariable * var = getVectorVar(var_name, comp);
  return (_c_is_implicit) ? var->gradSlnOldNeighbor() : var->gradSlnOlderNeighbor();
}

const VectorVariableGradient &
NeighborCoupleable::coupledVectorNeighborGradientOlder(const std::string & var_name,
                                                       unsigned int comp)
{
  if (_neighbor_nodal)
    mooseError("Gradients are non-sensical with nodal compute objects");

  validateExecutionerType(var_name, "coupledNeighborGradientOlder");
  VectorMooseVariable * var = getVectorVar(var_name, comp);
  if (_c_is_implicit)
    return var->gradSlnOlderNeighbor();
  else
    mooseError("Older values not available for explicit schemes");
}

const VariableSecond &
NeighborCoupleable::coupledNeighborSecond(const std::string & var_name, unsigned int comp)
{
  if (_neighbor_nodal)
    mooseError("Nodal variables do not have second derivatives");

  MooseVariable * var = getVar(var_name, comp);
  return (_c_is_implicit) ? var->secondSlnNeighbor() : var->secondSlnOldNeighbor();
}

const DenseVector<Number> &
NeighborCoupleable::coupledNeighborSolutionDoFs(const std::string & var_name, unsigned int comp)
{
  if (_neighbor_nodal)
    mooseError("nodal objects should not call coupledSolutionDoFs");

  MooseVariable * var = getVar(var_name, comp);
  return (_c_is_implicit) ? var->solutionDoFsNeighbor() : var->solutionDoFsOldNeighbor();
}

const DenseVector<Number> &
NeighborCoupleable::coupledNeighborSolutionDoFsOld(const std::string & var_name, unsigned int comp)
{
  if (_neighbor_nodal)
    mooseError("nodal objects should not call coupledSolutionDoFsOld");

  validateExecutionerType(var_name, "coupledNeighborSolutionDoFsOld");
  MooseVariable * var = getVar(var_name, comp);
  return (_c_is_implicit) ? var->solutionDoFsOldNeighbor() : var->solutionDoFsOlderNeighbor();
}

const DenseVector<Number> &
NeighborCoupleable::coupledNeighborSolutionDoFsOlder(const std::string & var_name,
                                                     unsigned int comp)
{
  if (_neighbor_nodal)
    mooseError("nodal objects should not call coupledSolutionDoFsOlder");

  validateExecutionerType(var_name, "coupledNeighborSolutionDoFsOlder");
  MooseVariable * var = getVar(var_name, comp);
  if (_c_is_implicit)
    return var->solutionDoFsOlderNeighbor();
  else
    mooseError("Older values not available for explicit schemes");
}
