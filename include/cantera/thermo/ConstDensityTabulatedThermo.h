/*
 * ConstDensityTabulatedThermo.h
 *
 *  Created on: 04.06.2016
 *      Author: mmayur
 */

/**
 *  @file ConstDensityTabulatedThermo.h
 * Definition file for a derived class of ThermoPhase that assumes either
 * an ideal gas or ideal solution approximation and handles
 * variable pressure standard state methods for calculating
 * thermodynamic properties (see \ref thermoprops and
 * class \link Cantera::ConstDensityTabulatedThermo ConstDensityTabulatedThermo\endlink).
 */
/*
 * Copyright (2005) Sandia Corporation. Under the terms of
 * Contract DE-AC04-94AL85000 with Sandia Corporation, the
 * U.S. Government retains certain rights in this software.
 */
#ifndef CT_CONSTDENSITYTABULATEDTHERMO_H
#define CT_CONSTDENSITYTABULATEDTHERMO_H

#include "ConstDensityThermo.h"
#include "cantera/base/utilities.h"
#include <vector>
#include <iterator>
#include <iostream>
#include <fstream>

namespace Cantera
{

//!   Overloads the virtual methods of class ThermoPhase to implement the
//!   incompressible equation of state.
/**
 * <b> Specification of Solution Thermodynamic Properties </b>
 *
 * The density is assumed to be constant, no matter what the concentration of the solution.
 *
 * @ingroup thermoprops
 */
class ConstDensityTabulatedThermo : public ConstDensityThermo
{
public:
	//! Constructor.
	ConstDensityTabulatedThermo() {}

	//! Copy Constructor
	/*!
	 * @param right Object to be copied
	 */
	ConstDensityTabulatedThermo(const ConstDensityTabulatedThermo& right);

	//! Assignment Operator
	/*!
	 * @param right Object to be copied
	 */
	ConstDensityTabulatedThermo& operator=(const ConstDensityTabulatedThermo& right);

	//! Duplication routine for objects which inherit from ThermoPhase
	/*!
	 *  This virtual routine can be used to duplicate objects
	 *  derived from ThermoPhase even if the application only has
	 *  a pointer to ThermoPhase to work with.
	 */
	virtual ThermoPhase* duplMyselfAsThermoPhase() const;

    virtual std::string type() const {
        return "ConstDensityTabultedThermo";
    }

	//! This method returns an array of generalized concentrations
	/*!
	 * \f$ C^a_k\f$ are defined such that \f$ a_k = C^a_k /
	 * C^0_k, \f$ where \f$ C^0_k \f$ is a standard concentration
	 * defined below and \f$ a_k \f$ are activities used in the
	 * thermodynamic functions.  These activity (or generalized)
	 * concentrations are used
	 * by kinetics manager classes to compute the forward and
	 * reverse rates of elementary reactions. Note that they may
	 * or may not have units of concentration --- they might be
	 * partial pressures, mole fractions, or surface coverages,
	 * for example.
	 *
	 * @param c Output array of generalized concentrations. The
	 *           units depend upon the implementation of the
	 *           reaction rate expressions within the phase.
	 */
	virtual void getActivityConcentrations(doublereal* c) const;

	//! Get the array of non-dimensional molar-based activity coefficients at
	//! the current solution temperature, pressure, and solution concentration.
	/*!
	 * @param ac Output vector of activity coefficients. Length: m_kk.
	 */
	virtual void getActivityCoefficients(doublereal* ac) const;

	//! Return the standard concentration for the kth species
	/*!
	 * The standard concentration \f$ C^0_k \f$ used to normalize
	 * the activity (i.e., generalized) concentration. In many cases, this quantity
	 * will be the same for all species in a phase - for example,
	 * for an ideal gas \f$ C^0_k = P/\hat R T \f$. For this
	 * reason, this method returns a single value, instead of an
	 * array.  However, for phases in which the standard
	 * concentration is species-specific (e.g. surface species of
	 * different sizes), this method may be called with an
	 * optional parameter indicating the species.
	 *
	 * @param k Optional parameter indicating the species. The default
	 *         is to assume this refers to species 0.
	 * @return
	 *   Returns the standard Concentration in units of m3 kmol-1.
	 */
	virtual doublereal standardConcentration(size_t k=0) const;

	//! Initialize the ThermoPhase object after all species have been set up
	/*!
	 * @internal Initialize.
	 *
	 * This method is provided to allow
	 * subclasses to perform any initialization required after all
	 * species have been added. For example, it might be used to
	 * resize internal work arrays that must have an entry for
	 * each species.  The base class implementation does nothing,
	 * and subclasses that do not require initialization do not
	 * need to overload this method.  When importing a CTML phase
	 * description, this method is called from ThermoPhase::initThermoXML(),
	 * which is called from importPhase(),
	 * just prior to returning from function importPhase().
	 */
	virtual void initThermoXML(XML_Node& phaseNode, const std::string& id_);
	virtual void setMoleFractions(const doublereal* const x);
	virtual void setMoleFractions_NoNorm(const doublereal* const x);
	virtual void setMassFractions(const doublereal* const y);
	virtual void setMassFractions_NoNorm(const doublereal* const y);
	virtual void setConcentrations(const doublereal* const conc);

    //! Set equation of state parameter values from XML entries.
    /*!
     *
     * This method is called by function importPhase()  when processing a phase
     * definition in an input file. It should be overloaded in subclasses to set
     * any parameters that are specific to that particular phase
     * model. Note, this method is called before the phase is
     * initialized with elements and/or species.
     *
     * @param eosdata An XML_Node object corresponding to
     *                the "thermo" entry for this phase in the input file.
     */
	virtual void setParametersFromXML(const XML_Node& eosdata);

	//! Species thermodynamics interpolation functions
	doublereal interp_h(doublereal x) const;
	doublereal interp_s(doublereal x) const;

	//! Current modifiable species index
	size_t m_kk_mod;

protected:

	//! Current intercalating species mole fraction
	mutable doublereal m_xlast;

	//! File name for ConstDensityTabulatedThermo data
	std::string m_dataFile;

	//! Vector pairs for ConstDensityTabulatedThermo
	std::vector<std::pair<doublereal,doublereal> > molefrac_h;
	std::vector<std::pair<doublereal,doublereal> > molefrac_s;

	//! Function to update the reference state thermo functions
	//! Changed from private to protected so that the methods of the parent class could use this overridden function
	void _updateThermo() const;
};
}

//#endif

#endif /* INCLUDE_CANTERA_THERMO_CONSTDENSITYTABULATEDTHERMO_H_ */
