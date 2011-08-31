/*
 Copyright (C) 2011 J. Alberdi, M. Oliveira, Y. Pouillon, and M. Verstraete

 This program is free software; you can redistribute it and/or modify
 it under the terms of the GNU Lesser General Public License as published by
 the Free Software Foundation; either version 3 of the License, or 
 (at your option) any later version.

 This program is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU Lesser General Public License for more details.

 You should have received a copy of the GNU Lesser General Public License
 along with this program; if not, write to the Free Software
 Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA

 $Id$
*/

#include "pspio_potential.h"
#include <stdlib.h>

int pspio_potential_alloc(pspio_potential_t *potential, const int np){
  int ierr;

  ASSERT (np > 1, PSPIO_EVALUE);

  potential = (pspio_potential_t *) malloc (sizeof(pspio_potential_t));
  HANDLE_FATAL_ERROR (potential == NULL, PSPIO_ENOMEM);

  ierr = pspio_meshfunc_alloc(potential->v, np);
  if (ierr) {
    pspio_potential_free(potential);
    HANDLE_ERROR (ierr);
  }

  ierr = pspio_qn_alloc(potential->qn);
  if (ierr) {
    pspio_potential_free(potential);
    HANDLE_ERROR (ierr);
  }

  return PSPIO_SUCCESS;
}


int pspio_potential_set(pspio_potential_t *potential, pspio_qn_t *qn, pspio_mesh_t *mesh, double *v){

  ASSERT (potential != NULL, PSPIO_ERROR);

  HANDLE_FUNC_ERROR (pspio_qn_copy(qn, potential->qn));

  HANDLE_FUNC_ERROR (pspio_meshfunc_set(potential->v, mesh, v));

  return PSPIO_SUCCESS;
}


int pspio_potential_free(pspio_potential_t *potential){

  if (potential != NULL) {
    pspio_meshfunc_free(potential->v);
    free(potential);
  }
  return PSPIO_SUCCESS;
}
