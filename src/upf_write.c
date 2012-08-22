/*
 Copyright (C) 2011-2012 J. Alberdi, M. Oliveira, Y. Pouillon, and M. Verstraete

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

/** 
 * @file upf_write.c
 * @brief routines to write UPF files 
 */
#include <string.h>
#include <ctype.h>
#include "upf.h"
#include "util.h"

#if defined HAVE_CONFIG_H
#include "config.h"
#endif


int upf_write_header(FILE *fp, const pspio_pspdata_t *pspdata){
  int is, l;
  double occ;
  char label[5];
  int has_nlcc, exchange, correlation;
  char shortname[4], longname[20];

  //Write init tag
  fprintf(fp, "<PP_HEADER>\n");
  
  //Write the version number
  fprintf(fp, "   0                   Version Number\n");
 
  //Write the atomic symbol
  fprintf(fp, "  %2s                   Element\n", pspdata->symbol);

  //Write the kind of pseudo-potentials US|NC|PAW
  //At the moment we only support norm-conversing psp
  fprintf(fp, "   NC                  Norm - Conserving pseudopotential\n");
  
  //Write the if there are nonlinear core corrections
  HANDLE_FUNC_ERROR(pspio_xc_has_nlcc(pspdata->xc, &has_nlcc));
  if (has_nlcc) {
    fprintf(fp, "    T                  Nonlinear Core Correction\n");
  } else {
    fprintf(fp, "    F                  Nonlinear Core Correction\n");
  }

  //Write exchange-correlation functional
  HANDLE_FUNC_ERROR(pspio_xc_get_id(pspdata->xc, &exchange, &correlation));
  HANDLE_FUNC_ERROR(upf_from_libxc(exchange, correlation, longname, shortname));
  fprintf(fp, " %20s  %4s Exchange-Correlation functional\n", longname, shortname);

  //Write the Z valence
  fprintf(fp, "%17.11f      Z valence\n", pspdata->zvalence);

  //Write the total energy
  fprintf(fp, "%17.11f      Total energy\n", pspdata->total_energy);
  
  //Write the suggested cutoff for wfc and rho
  fprintf(fp, "%11.7f%11.7f Suggested cutoff for wfc and rho\n", 0.0, 0.0);
  
  //Write the max angular momentun component
  fprintf(fp, "%5d                  Max angular momentum component\n", pspdata->kb_l_max);
 
  //Write the number of points in mesh
  fprintf(fp, "%5d                  Number of points in mesh\n", pspdata->mesh->np);
  
  //Write the number of wavefunctions and projectors 
  fprintf(fp, "%5d%5d             Number of Wavefunctions, Number of Projectors\n", pspdata->n_states, pspdata->n_kbproj);

  //Write wavefunctions info
  fprintf(fp, " Wavefunctions         nl  l   occ\n");
  for (is=0; is<pspdata->n_states; is++) {
    HANDLE_FUNC_ERROR(pspio_state_get_label(pspdata->states[is], &label[0]));
    HANDLE_FUNC_ERROR(pspio_state_get_l(pspdata->states[is], &l));
    HANDLE_FUNC_ERROR(pspio_state_get_occ(pspdata->states[is], &occ));    
    fprintf(fp, "                       %2s%3d%6.2f\n", label, l, occ);    
  }

  //Write end tag
  fprintf(fp, "</PP_HEADER>\n");

  return PSPIO_SUCCESS;
}


int upf_write_mesh(FILE *fp, const pspio_pspdata_t *pspdata){
  int i;

  //Write init tag
  fprintf(fp, "<PP_MESH>\n");

  //Print mesh points
  fprintf(fp, "  <PP_R>\n");
  for (i=0; i<pspdata->mesh->np; i++) {
    if (i != 0 && i % 4 == 0) fprintf(fp, "\n");
    fprintf(fp, " %18.11E", pspdata->mesh->r[i]);
  }
  fprintf(fp, "\n  </PP_R>\n");

  //Print Rab
  fprintf(fp, "  <PP_RAB>\n");
  for (i=0; i<pspdata->mesh->np; i++) {
    if (i != 0 && i % 4 == 0) fprintf(fp, "\n");
    fprintf(fp, " %18.11E", pspdata->mesh->rab[i]);
  }
  fprintf(fp, "\n  </PP_RAB>\n");

  //Write end tag
  fprintf(fp, "</PP_MESH>\n");

  return PSPIO_SUCCESS;
}


int upf_write_nlcc(FILE *fp, const pspio_pspdata_t *pspdata){
  int i;
  double rho;

  //Write init tag
  fprintf(fp, "<PP_NLCC>\n");

  //Print density
  for (i=0; i<pspdata->mesh->np; i++) {
    HANDLE_FUNC_ERROR(pspio_xc_nlcc_eval(pspdata->xc, pspdata->mesh->r[i], &rho));
    if (i != 0 && i % 4 == 0) fprintf(fp, "\n");
    fprintf(fp, " %18.11E", rho);
  }

  //Write end tag
  fprintf(fp, "\n</PP_NLCC>\n");

  return PSPIO_SUCCESS;
}


int upf_write_nonlocal(FILE *fp, const pspio_pspdata_t *pspdata){
  int ikb, l, i;
  double proj, ekb;

  //Write init tag
  fprintf(fp, "<PP_NONLOCAL>\n");

  //Write projectors
  for (ikb=0; ikb<pspdata->n_kbproj; ikb++) {
    HANDLE_FUNC_ERROR(pspio_projector_l(pspdata->kb_projectors[ikb], &l));
    fprintf(fp, "  <PP_BETA>\n");
    fprintf(fp, "%5d%5d             Beta    L\n", ikb+1, l);
    fprintf(fp, "%6d\n", pspdata->mesh->np);
    for (i=0; i<pspdata->mesh->np; i++) {
      HANDLE_FUNC_ERROR(pspio_projector_eval(pspdata->kb_projectors[ikb], pspdata->mesh->r[i], &proj));
      if (i != 0 && i % 4 == 0) fprintf(fp, "\n");
      proj *= 2.0*pspdata->mesh->r[i];
      fprintf(fp, " %18.11E", proj);
    }
    fprintf(fp, "\n  </PP_BETA>\n");
  }

  //Write the KB energies
  fprintf(fp, "  <PP_DIJ>\n");
  fprintf(fp, "%5d                  Number of nonzero Dij\n", pspdata->n_kbproj);
  for (ikb=0; ikb<pspdata->n_kbproj; ikb++) {
    HANDLE_FUNC_ERROR(pspio_projector_energy(pspdata->kb_projectors[ikb], &ekb));
    ekb /= 2.0;
    fprintf(fp, "%5d%5d%19.11E\n", ikb+1, ikb+1, ekb);
  }
  fprintf(fp, "  </PP_DIJ>\n");

  //Write end tag
  fprintf(fp, "</PP_NONLOCAL>\n");

  return PSPIO_SUCCESS;
}


int upf_write_local(FILE *fp, const pspio_pspdata_t *pspdata){
  int i;
  double vlocal;

  //Write init tag
  fprintf(fp, "<PP_LOCAL>\n");

  //Print vlocal
  for (i=0; i<pspdata->mesh->np; i++) {
    if (i != 0 && i % 4 == 0) fprintf(fp, "\n");
    HANDLE_FUNC_ERROR(pspio_potential_eval(pspdata->vlocal, pspdata->mesh->r[i], &vlocal));
    vlocal *= 2.0;
    fprintf(fp, " %18.11E", vlocal);
  }

  //Write end tag
  fprintf(fp, "\n</PP_LOCAL>\n");

  return PSPIO_SUCCESS;
}


int upf_write_pswfc(FILE *fp, const pspio_pspdata_t *pspdata){
  int is, i, l;
  double occ, wf;
  char label[5];

  //Write init tag
  fprintf(fp, "<PP_PSWFC>\n");

  //Write wavefunctions
  for (is=0; is<pspdata->n_states; is++) {
    HANDLE_FUNC_ERROR(pspio_state_get_label(pspdata->states[is], &label[0]));
    HANDLE_FUNC_ERROR(pspio_state_get_l(pspdata->states[is], &l));
    HANDLE_FUNC_ERROR(pspio_state_get_occ(pspdata->states[is], &occ));
    fprintf(fp, "%s %4d %5.2f          Wavefunction\n", label, l, occ);
    for (i=0; i<pspdata->mesh->np; i++) {
      if (i != 0 && i % 4 == 0) fprintf(fp, "\n");
      HANDLE_FUNC_ERROR(pspio_state_wf_eval(pspdata->states[is], pspdata->mesh->r[i], &wf));
      wf *= pspdata->mesh->r[i];
      fprintf(fp, " %18.11E", wf);
    }
    fprintf(fp, "\n");
  }

  //Write end tag
  fprintf(fp, "</PP_PSWFC>\n");

  return PSPIO_SUCCESS;
}


int upf_write_rhoatom(FILE *fp, const pspio_pspdata_t *pspdata){
  int i;
  double rho;

  //Write init tag
  fprintf(fp, "<PP_RHOATOM>\n");

  //Print valence density
  for (i=0; i<pspdata->mesh->np; i++) {
    if (i != 0 && i % 4 == 0) fprintf(fp, "\n");
    HANDLE_FUNC_ERROR(pspio_meshfunc_eval(pspdata->rho_valence, pspdata->mesh->r[i], &rho));
    rho *= 4.0*M_PI*pspdata->mesh->r[i]*pspdata->mesh->r[i];
    fprintf(fp, " %18.11E", rho);
  }

  //Write end tag
  fprintf(fp, "\n</PP_RHOATOM>\n");

  return PSPIO_SUCCESS;
}
