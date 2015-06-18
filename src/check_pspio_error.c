/*
 Copyright (C) 2011 J. Alberdi, M. Oliveira, Y. Pouillon, and M. Verstraete
 Copyright (C) 2015 M. Oliveira

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

*/

/**
 * @file check_pspio_error.c
 * @brief checks pspio_error.c and pspio_error.h 
 */

#include <stdio.h>
#include <check.h>

#include "pspio_error.h"


void error_setup(void)
{
  pspio_error_free();
}

void error_teardown(void)
{
  pspio_error_free();
}


START_TEST(test_error_empty)
{
  ck_assert_int_eq(pspio_error_get_last(NULL), PSPIO_SUCCESS);
  ck_assert_int_eq(pspio_error_len(), 0);

  pspio_error_flush(stdout);

  ck_assert_int_eq(pspio_error_get_last(NULL), PSPIO_SUCCESS);
  ck_assert_int_eq(pspio_error_len(), 0);

}
END_TEST

START_TEST(test_error_single)
{
  pspio_error_add(PSPIO_EVALUE, "test_1_1.c", 1234, "dummy1");
  ck_assert_int_eq(pspio_error_get_last(NULL), PSPIO_EVALUE);
  ck_assert_int_eq(pspio_error_len(), 1);

  pspio_error_flush(stdout);

  ck_assert_int_eq(pspio_error_get_last(NULL), PSPIO_SUCCESS);
  ck_assert_int_eq(pspio_error_len(), 0);
}
END_TEST

START_TEST(test_error_double)
{
  pspio_error_add(PSPIO_EGSL, "test_2_1.c", 1234, "dummy21");
  ck_assert_int_eq(pspio_error_get_last(NULL), PSPIO_EGSL);
  ck_assert_int_eq(pspio_error_len(), 1);

  pspio_error_add(PSPIO_ENOSUPPORT, "test_2_2.c", 202, "dummy22");
  ck_assert_int_eq(pspio_error_get_last(NULL), PSPIO_ENOSUPPORT);
  ck_assert_int_eq(pspio_error_len(), 2);

  pspio_error_flush(stdout);

  ck_assert_int_eq(pspio_error_get_last(NULL), PSPIO_SUCCESS);
  ck_assert_int_eq(pspio_error_len(), 0);
}
END_TEST

START_TEST(test_error_triple)
{
  pspio_error_add(PSPIO_EVALUE, "test_3_1.c", 311, "dummy31");
  ck_assert_int_eq(pspio_error_get_last(NULL), PSPIO_EVALUE);
  ck_assert_int_eq(pspio_error_len(), 1);

  pspio_error_add(PSPIO_ENOFILE, "test_3_2.c", 322, "dummy32"); 
  ck_assert_int_eq(pspio_error_get_last(NULL), PSPIO_ENOFILE);
  ck_assert_int_eq(pspio_error_len(), 2);

  pspio_error_add(PSPIO_ERROR, "test_3_3.c", 333, "dummy33"); 
  ck_assert_int_eq(pspio_error_get_last(NULL), PSPIO_ERROR);
  ck_assert_int_eq(pspio_error_len(), 3);

  pspio_error_flush(stdout);

  ck_assert_int_eq(pspio_error_get_last(NULL), PSPIO_SUCCESS);
  ck_assert_int_eq(pspio_error_len(), 0);
}
END_TEST

START_TEST(test_error_get_last)
{
  pspio_error_add(PSPIO_EGSL,    "test_4_1.c", 411, "dummy41");
  pspio_error_add(PSPIO_ENOFILE, "test_4_2.c", 422, "dummy42");
  pspio_error_add(PSPIO_ERROR,   "test_4_3.c", 433, "dummy43");
  ck_assert_int_eq(pspio_error_len(), 3);

  ck_assert_int_eq(pspio_error_get_last("dummy41"), PSPIO_EGSL);
  ck_assert_int_eq(pspio_error_get_last("dummy42"), PSPIO_ENOFILE);
  ck_assert_int_eq(pspio_error_get_last("dummy43"), PSPIO_ERROR);
  ck_assert_int_eq(pspio_error_get_last("dummy44"), PSPIO_SUCCESS);
}
END_TEST


Suite * make_error_suite(void)
{
  Suite *s;
  TCase *tc_empty, *tc_single, *tc_double, *tc_triple, *tc_last;

  s = suite_create("Error");

  tc_empty = tcase_create("Empty chain");
  tcase_add_checked_fixture(tc_empty, error_setup, error_teardown);
  tcase_add_test(tc_empty, test_error_empty);
  suite_add_tcase(s, tc_empty);

  tc_single = tcase_create("Single error chain");
  tcase_add_checked_fixture(tc_single, error_setup, error_teardown);
  tcase_add_test(tc_single, test_error_single);
  suite_add_tcase(s, tc_single);

  tc_double = tcase_create("Double error chain");
  tcase_add_checked_fixture(tc_double, error_setup, error_teardown);
  tcase_add_test(tc_double, test_error_double);
  suite_add_tcase(s, tc_double);

  tc_triple = tcase_create("Triple error chain");
  tcase_add_checked_fixture(tc_triple, error_setup, error_teardown);
  tcase_add_test(tc_triple, test_error_triple);
  suite_add_tcase(s, tc_triple);

  tc_last = tcase_create("Get last");
  tcase_add_checked_fixture(tc_last, error_setup, error_teardown);
  tcase_add_test(tc_last, test_error_get_last);
  suite_add_tcase(s, tc_last);

  return s;
}
