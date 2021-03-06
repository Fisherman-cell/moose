//* This file is part of the MOOSE framework
//* https://www.mooseframework.org
//*
//* All rights reserved, see COPYRIGHT for full restrictions
//* https://github.com/idaholab/moose/blob/master/COPYRIGHT
//*
//* Licensed under LGPL 2.1, please see LICENSE for details
//* https://www.gnu.org/licenses/lgpl-2.1.html

#pragma once

#include "DenseMatrix.h"
#include "PertinentGeochemicalSystem.h"
#include <libmesh/dense_vector.h>

/**
 * Class to swap basis species with equilibrium species
 */
class GeochemistrySpeciesSwapper
{
public:
  /**
   * @param basis_size Number of basis species in the geochemical model
   * @param stoi_tol Swapping involves inverting matrices via a singular value decomposition. During
   * this process: (1) if abs(singular value) < stoi_tol * L1norm(singular values), then the matrix
   * is deemed singular (so the basis swap is deemed invalid); (2) if abs(any stoichiometric
   * coefficient) < stoi_tol then it is set to zero.  A suggested reasonable value is stoi_tol=1E-6
   */
  GeochemistrySpeciesSwapper(unsigned basis_size, Real stoi_tol);

  /**
   * Check that replacing the named basis species with the named equilibrium species is valid.  In
   * performing this check, the "swap" matrix is inverted, so the check is somewhat expensive.
   * Therefore, if the swap will be performed, assuming it's valid, the performSwap method should be
   * used instead.
   * A mooseError or mooseException will result if the swap is invalid
   * @param mgd The ModelGeochemicalDatabase that holds all information regarding basis
   * components, equilibrium species, stoichiometries, etc
   * @param replace_this The basis component that will be removed from the basis and added to the
   * equilibrium species list
   * @param with_this The equilibrium species that will be removed from the equilibrium species list
   * and added to the basis component list
   */
  void checkSwap(const ModelGeochemicalDatabase & mgd,
                 const std::string & replace_this,
                 const std::string & with_this);

  /**
   * Check that replacing the indexed basis species with the indexed equilibrium species is valid.
   * In performing this check, the "swap" matrix is inverted, so the check is somewhat expensive.
   * Therefore, if the swap will be performed, assuming it's valid, the performSwap method should be
   * used instead.
   * A mooseError or mooseException will result if the swap is invalid
   * @param mgd The ModelGeochemicalDatabase that holds all information regarding basis
   * components, equilibrium species, stoichiometries, etc
   * @param basis_index_to_replace The index of the basis component that will be removed from the
   * basis and added to the equilibrium species list
   * @param eqm_index_to_insert The index of the equilibrium species that will be removed from the
   * equilibrium species list and added to the basis component list
   */
  void checkSwap(const ModelGeochemicalDatabase & mgd,
                 unsigned basis_index_to_replace,
                 unsigned eqm_index_to_insert);

  /**
   * Check that replacing the named basis species with the named equilibrium species is valid, and
   * then perform this swap by altering the mgd data structure.
   * A mooseError or mooseException will result if the swap is invalid
   * @param mgd The ModelGeochemicalDatabase that holds all information regarding basis
   * components, equilibrium species, stoichiometries, etc
   * @param replace_this The basis component that will be removed from the basis and added to the
   * equilibrium species list
   * @param with_this The equilibrium species that will be removed from the equilibrium species list
   * and added to the basis component list
   */
  void performSwap(ModelGeochemicalDatabase & mgd,
                   const std::string & replace_this,
                   const std::string & with_this);

  /**
   * Check that replacing the indexed basis species with the indexed equilibrium species is valid,
   * and then perform this swap by altering the mgd data structure.
   * A mooseError or mooseException will result if the swap is invalid
   * @param mgd The ModelGeochemicalDatabase that holds all information regarding basis
   * components, equilibrium species, stoichiometries, etc
   * @param basis_index_to_replace The index of the basis component that will be removed from the
   * basis and added to the equilibrium species list
   * @param eqm_index_to_insert The index of the equilibrium species that will be removed from the
   * equilibrium species list and added to the basis component list
   */
  void performSwap(ModelGeochemicalDatabase & mgd,
                   unsigned basis_index_to_replace,
                   unsigned eqm_index_to_insert);

  /**
   * Check that replacing the named basis species with the named equilibrium species is valid, and
   * then perform this swap by altering the mgd data structure, and calculate the bulk composition
   * of the new system.
   * A mooseError or mooseException will result if the swap is invalid
   * @param mgd The ModelGeochemicalDatabase that holds all information regarding basis
   * components, equilibrium species, stoichiometries, etc
   * @param[in/out] bulk_composition Upon entry, the bulk composition in the original basis.  Upon
   * exit, the bulk composition in the basis after the swap
   * @param replace_this The basis component that will be removed from the basis and added to the
   * equilibrium species list
   * @param with_this The equilibrium species that will be removed from the equilibrium species list
   * and added to the basis component list
   */
  void performSwap(ModelGeochemicalDatabase & mgd,
                   DenseVector<Real> & bulk_composition,
                   const std::string & replace_this,
                   const std::string & with_this);

  /**
   * Check that replacing the indexed basis species with the indexed equilibrium species is valid,
   * and then perform this swap by altering the mgd data structure, and calculate the bulk
   * composition of the new system.
   * A mooseError or mooseException will result if the swap is invalid
   * @param mgd The ModelGeochemicalDatabase that holds all information regarding basis
   * components, equilibrium species, stoichiometries, etc
   * @param[in/out] bulk_composition Upon entry, the bulk composition in the original basis.  Upon
   * exit, the bulk composition in the basis after the swap
   * @param basis_index_to_replace The index of the basis component that will be removed from the
   * basis and added to the equilibrium species list
   * @param eqm_index_to_insert The index of the equilibrium species that will be removed from the
   * equilibrium species list and added to the basis component list
   */
  void performSwap(ModelGeochemicalDatabase & mgd,
                   DenseVector<Real> & bulk_composition,
                   unsigned basis_index_to_replace,
                   unsigned eqm_index_to_insert);

private:
  /**
   * Construct the swap matrix, and its inverse, that describes the swap between the indexed basis
   * species and the indexed equilibrium index.  The inverse of the swap matrix is used in alterMGD
   * to modify its datastructures to implement the swap.
   * A mooseError or mooseException will result if the swap is invalid
   * @param mgd The ModelGeochemicalDatabase that holds all information regarding basis
   * components, equilibrium species, stoichiometries, etc
   * @param basis_index_to_replace The index of the basis component that will be removed from the
   * basis and added to the equilibrium species list
   * @param eqm_index_to_insert The index of the equilibrium species that will be removed from the
   * equilibrium species list and added to the basis component list
   */
  void constructInverseMatrix(const ModelGeochemicalDatabase & mgd,
                              unsigned basis_index_to_replace,
                              unsigned eqm_index_to_insert);

  /**
   * Modify the ModelGeochemicalDatabase mgd to swap the indexed basis
   * species and the indexed equilibrium index.
   * @param mgd The ModelGeochemicalDatabase that holds all information regarding basis
   * components, equilibrium species, stoichiometries, etc
   * @param basis_index_to_replace The index of the basis component that will be removed from the
   * basis and added to the equilibrium species list
   * @param eqm_index_to_insert The index of the equilibrium species that will be removed from the
   * equilibrium species list and added to the basis component list
   */
  void alterMGD(ModelGeochemicalDatabase & mgd,
                unsigned basis_index_to_replace,
                unsigned eqm_index_to_insert);

  /**
   * Calculate the bulk composition in the new basis based on the bulk composition in the original
   * basis
   * @param basis_size Size of basis, which is only used for error checking
   * @param[in/out] bulk_composition Upon entry, the bulk composition in the original basis.  Upon
   * exit, the bulk composition in the basis after the swap
   */
  void alterBulkComposition(unsigned basis_size, DenseVector<Real> & bulk_composition) const;

  /// tolerance for matrix inversion and stoichiometric coefficients
  Real _stoi_tol;

  /// swap matrix
  DenseMatrix<Real> _swap_matrix;

  /// inverse of swap matrix
  DenseMatrix<Real> _inv_swap_matrix;

  /// used in SVD decomposition of swap matrix
  DenseVector<Real> _swap_sigma;

  /// used in SVD decomposition of swap matrix
  DenseMatrix<Real> _swap_U;

  /// used in SVD decomposition of swap matrix
  DenseMatrix<Real> _swap_VT;
};
