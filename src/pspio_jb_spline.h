/* Copyright (C) 2014-2016 Micael Oliveira <micaeljtoliveira@gmail.com>
 *                         Yann Pouillon <notifications@materialsevolution.es>
 *
 * This file is part of Libpspio.
 *
 * Libpspio is free software: you can redistribute it and/or modify it under
 * the terms of the GNU Lesser General Public License as published by the Free
 * Software Foundation, version 3 of the License, or (at your option) any later
 * version.
 *
 * Libpspio is distributed in the hope that it will be useful, but WITHOUT ANY
 * WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
 * FOR A PARTICULAR PURPOSE.  See the GNU Lesser General Public License for
 * more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with Libpspio.  If not, see <http://www.gnu.org/licenses/> or write to
 * the Free Software Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA
 * 02110-1301  USA.
 */


/**
 * @file jb_spline.h 
 * @brief header file for the handling of spline from JB
 */


#ifndef PSPIO_JB_SPLINE
#define PSPIO_JB_SPLINE

#include "pspio_error.h"

#if defined HAVE_CONFIG_H
#include "config.h"
#endif


/**********************************************************************
 * Data structures                                                    *
 **********************************************************************/

/**
 * Interpolation data structure
 */
typedef struct jb_spline_t jb_spline_t;


/**********************************************************************
 * Global routines                                                    *
 **********************************************************************/

/**
 * 
 */
int jb_spline_alloc(jb_spline_t **spline, int np);

/**
 * 
 */
int jb_spline_init(jb_spline_t **spline, const double *f, const double *r, 
		   int np);

/**
 * 
 */
int jb_spline_copy(jb_spline_t **dst, const jb_spline_t *src);

/**
 * Compute the second derivatives of a piecewise cubic spline using 'natural'
 * boundary conditions.
 */
double *jb_natural_spline_cubic_init(int n, const double* t, const double* y);

/**
 * 
 */
double *jb_spline_cubic_init (int n, const double *t, const double *y, int ibcbeg,
			      double ybcbeg, int ibcend, double ybcend);

/**
 * 
 */
void jb_spline_free(jb_spline_t *spline);


/**********************************************************************
 * Utility routines                                                   *
 **********************************************************************/

/**
 * 
 */
double jb_spline_eval(const jb_spline_t *spline, double r);

/**
 * 
 */
double jb_spline_eval_deriv(const jb_spline_t *spline, double r);

/**
 * 
 */
double jb_spline_eval_deriv2(const jb_spline_t *spline, double r);

/**
 * Evaluates a piecewise cubic spline at a point.
 */
void jb_spline_cubic_val(int n, const double *t, const double *y, const double *ypp, 
			 double tval, double *yval, double *ypval, double *yppval);

/**
 * Solves a pentadiagonal system of linear equations.
 */
double *penta(int n, double a1[], double a2[], double a3[], double a4[], 
	      double a5[], double b[]);

#endif