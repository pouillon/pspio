!! Copyright (C) 2015-2016 Micael Oliveira <micaeljtoliveira@gmail.com>
!!                         Yann Pouillon <notifications@materialsevolution.es>
!!
!! This file is part of Libpspio.
!!
!! Libpspio is free software: you can redistribute it and/or modify it under
!! the terms of the GNU Lesser General Public License as published by the Free
!! Software Foundation, version 3 of the License, or (at your option) any later
!! version.
!!
!! Libpspio is distributed in the hope that it will be useful, but WITHOUT ANY
!! WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
!! FOR A PARTICULAR PURPOSE.  See the GNU Lesser General Public License for
!! more details.
!!
!! You should have received a copy of the GNU Lesser General Public License
!! along with Libpspio.  If not, see <http://www.gnu.org/licenses/> or write to
!! the Free Software Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA
!! 02110-1301  USA.


!*********************************************************************!
! Global routines                                                     !
!*********************************************************************!

! alloc
integer function fpspio_qn_alloc(qn) result(ierr)
  type(fpspio_qn_t), intent(inout) :: qn

  ierr = pspio_qn_alloc(qn%ptr)

end function fpspio_qn_alloc

! init
integer function fpspio_qn_init(qn, n, l, j) result(ierr)
  type(fpspio_qn_t), intent(inout) :: qn
  integer,           intent(in)    :: n
  integer,           intent(in)    :: l
  real(8),           intent(in)    :: j

  ierr = pspio_qn_init(qn%ptr, n, l, j)

end function fpspio_qn_init

! copy
integer function fpspio_qn_copy(src, dst) result(ierr)
  type(fpspio_qn_t), intent(in)    :: src
  type(fpspio_qn_t), intent(inout) :: dst

  ierr = pspio_qn_copy(src%ptr, dst%ptr)

end function fpspio_qn_copy

! free
subroutine fpspio_qn_free(qn)
  type(fpspio_qn_t), intent(inout) :: qn

  call pspio_qn_free(qn%ptr)
  qn%ptr = C_NULL_PTR

end subroutine fpspio_qn_free

! associated
logical function fpspio_qn_associated(qn) result(is_associated)
  type(fpspio_qn_t), intent(in) :: qn

  is_associated = c_associated(qn%ptr)

end function fpspio_qn_associated


!*********************************************************************!
! Getters                                                             !
!*********************************************************************!

! n
integer function fpspio_qn_get_n(qn) result(n)
  type(fpspio_qn_t), intent(in)  :: qn

  n = pspio_qn_get_n(qn%ptr)

end function fpspio_qn_get_n

! l
integer function fpspio_qn_get_l(qn) result(l)
  type(fpspio_qn_t), intent(in)  :: qn

  l = pspio_qn_get_l(qn%ptr)

end function fpspio_qn_get_l

! j
real(8) function fpspio_qn_get_j(qn) result(j)
  type(fpspio_qn_t), intent(in)  :: qn

  j = pspio_qn_get_j(qn%ptr)

end function fpspio_qn_get_j


!*********************************************************************!
! Utility routines                                                    !
!*********************************************************************!

! cmp
integer function fpspio_qn_cmp(qn1, qn2) result(cmp)
  type(fpspio_qn_t) :: qn1
  type(fpspio_qn_t) :: qn2

  cmp = pspio_qn_cmp(qn1%ptr, qn2%ptr)

end function fpspio_qn_cmp

! label
integer function fpspio_qn_label(qn, label) result(ierr)
  type(fpspio_qn_t), intent(in)  :: qn
  character(len=*),  intent(out) :: label

  character(kind=c_char,len=1) :: c_label(len(label)+1)

  ierr = pspio_qn_label(qn%ptr, c_label)
  if (ierr == PSPIO_SUCCESS) call c_to_f_string(c_label, label)

end function fpspio_qn_label