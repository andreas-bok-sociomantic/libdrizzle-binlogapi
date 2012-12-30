/*  vim:expandtab:shiftwidth=2:tabstop=2:smarttab: 
 *
 *  Drizzle Client & Protocol Library
 *
 * Copyright (C) 2012 Drizzle Developer Group
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are
 * met:
 *
 *     * Redistributions of source code must retain the above copyright
 * notice, this list of conditions and the following disclaimer.
 *
 *     * Redistributions in binary form must reproduce the above
 * copyright notice, this list of conditions and the following disclaimer
 * in the documentation and/or other materials provided with the
 * distribution.
 *
 *     * The names of its contributors may not be used to endorse or
 * promote products derived from this software without specific prior
 * written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
 * A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
 * OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
 * LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 * DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 * THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 */

#include <yatl/lite.h>

#include <libdrizzle-5.1/libdrizzle.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <inttypes.h>
#include <string.h>

int main(int argc, char *argv[])
{
  (void) argc;
  (void) argv;
  drizzle_stmt_st *stmt;

  drizzle_st *con= drizzle_create_tcp("localhost", DRIZZLE_DEFAULT_TCP_PORT, "root", NULL, NULL, 0);
  ASSERT_NOT_NULL_(con, "Drizzle connection object creation error");

  drizzle_return_t ret= drizzle_connect(con);
  if (ret == DRIZZLE_RETURN_COULD_NOT_CONNECT)
  {
    const char *error= drizzle_error(con);
    drizzle_quit(con);
    SKIP_IF_(ret == DRIZZLE_RETURN_COULD_NOT_CONNECT, "%s(%s)", error, drizzle_strerror(ret));
  }
  ASSERT_EQ_(DRIZZLE_RETURN_OK, ret, "drizzle_connect(): %s(%s)", drizzle_error(con), drizzle_strerror(ret));

  drizzle_query_str(con, "DROP SCHEMA IF EXISTS libdrizzle", &ret);
  ASSERT_EQ_(DRIZZLE_RETURN_OK, ret, "CREATE SCHEMA libdrizzle (%s)", drizzle_error(con));

  drizzle_query_str(con, "CREATE SCHEMA libdrizzle", &ret);
  ASSERT_EQ_(DRIZZLE_RETURN_OK, ret, "CREATE SCHEMA libdrizzle (%s)", drizzle_error(con));

  drizzle_result_st *result= drizzle_select_db(con, "libdrizzle", &ret);
  ASSERT_TRUE(result);
  ASSERT_EQ_(DRIZZLE_RETURN_OK, ret, "USE libdrizzle");

  drizzle_query_str(con, "create table libdrizzle.t1 (a varchar(50))", &ret);
  ASSERT_EQ_(DRIZZLE_RETURN_OK, ret, "create table libdrizzle.t1 (a int): %s", drizzle_error(con));

  drizzle_query_str(con, "insert into libdrizzle.t1 values ('hello'),('drizzle'),('people')", &ret);
  ASSERT_EQ_(DRIZZLE_RETURN_OK, ret, "%s", drizzle_error(con));

  const char *query= "select * from libdrizzle.t1 where a = ?";
  stmt= drizzle_stmt_prepare(con, query, strlen(query), &ret);
  ASSERT_EQ_(DRIZZLE_RETURN_OK, ret, "%s", drizzle_error(con));

  /* Query should have 1 param */
  ASSERT_EQ_(1, drizzle_stmt_param_count(stmt), "Retrieved bad param count");

  char val[]= "hello";
  ret = drizzle_stmt_set_string(stmt, 0, val, strlen(val));
  if (ret != DRIZZLE_RETURN_OK)
  {
    printf("Bind failure\n");
    return EXIT_FAILURE;
  }

  ret = drizzle_stmt_execute(stmt);
  if (ret != DRIZZLE_RETURN_OK)
  {
    printf("Execute failure\n");
    return EXIT_FAILURE;
  }
  ret = drizzle_stmt_buffer(stmt);
  if (ret != DRIZZLE_RETURN_OK)
  {
    printf("Buffer failure\n");
    return EXIT_FAILURE;
  }
  /* Result should have 1 row */
  if (drizzle_stmt_row_count(stmt) != 1)
  {
    printf("Retrieved bad row count\n");
    return EXIT_FAILURE;
  }
  uint32_t i= 0;
  while (drizzle_stmt_fetch(stmt) != DRIZZLE_RETURN_ROW_END)
  {
    const char* char_val;
    size_t len;
    char_val= drizzle_stmt_get_string(stmt, 0, &len, &ret);
    ASSERT_EQ_(DRIZZLE_RETURN_OK, ret, "drizzle_stmt_get_string");
    i++;
    if (strncmp(val, char_val, len) != 0)
    {
      printf("Retrieved unexpected string value\n");
      return EXIT_FAILURE;
    }
  }
  /* Should have cycled through 1 row */
  if (i != 1)
  {
    printf("Retrieved bad number of rows\n");
    return EXIT_FAILURE;
  }
  ret = drizzle_stmt_close(stmt);
  if (ret != DRIZZLE_RETURN_OK)
  {
    printf("Statement close failure ret: %d, err: %d, msg: %s\n", ret, drizzle_errno(con), drizzle_error(con));
    return EXIT_FAILURE;
  }

  drizzle_query_str(con, "DROP TABLE libdrizzle.t1", &ret);
  ASSERT_EQ_(DRIZZLE_RETURN_OK, ret, "DROP TABLE libdrizzle.t1");

  drizzle_query_str(con, "DROP SCHEMA IF EXISTS libdrizzle", &ret);
  ASSERT_EQ_(DRIZZLE_RETURN_OK, ret, "DROP SCHEMA libdrizzle (%s)", drizzle_error(con));

  ret= drizzle_quit(con);
  ASSERT_EQ_(DRIZZLE_RETURN_OK, ret, "%s", drizzle_strerror(ret));

  return EXIT_SUCCESS;
}
