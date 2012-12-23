/*  vim:expandtab:shiftwidth=2:tabstop=2:smarttab: 
 *
 *  Drizzle Client & Protocol Library
 *
 * Copyright (C) 2012 Andrew Hutchings (andrew@linuxjedi.co.uk)
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

#include "config.h"

#include <libdrizzle-5.0/drizzle_client.h>
#include <stdio.h>
#include <stdlib.h>

#ifndef EXIT_SKIP
# define EXIT_SKIP 77
#endif

int main(int argc, char *argv[])
{
  (void) argc;
  (void) argv;
  drizzle_st *con;
  drizzle_return_t ret;
  drizzle_stmt_st *stmt;

  con = drizzle_create_tcp("localhost", 3306, "root", "", "libdrizzle", 0);
  if (con == NULL)
  {
    printf("Drizzle connection object creation error\n");
    return EXIT_FAILURE;
  }
  ret = drizzle_connect(con);
  if (ret != DRIZZLE_RETURN_OK)
  {
    printf("Drizzle connection failure\n");
    return EXIT_SKIP;
  }

  drizzle_query_str(con, "create table libdrizzle.t1 (a int)", &ret);
  if (ret != DRIZZLE_RETURN_OK)
  {
    printf("Create table failure\n");
    return EXIT_FAILURE;
  }

  drizzle_query_str(con, "insert into libdrizzle.t1 values (1),(2),(3)", &ret);
  if (ret != DRIZZLE_RETURN_OK)
  {
    printf("Insert failure\n");
    return EXIT_FAILURE;
  }

  const char *query= "select * from libdrizzle.t1 where a > ?";
  stmt= drizzle_stmt_prepare(con, query, strlen(query), &ret);
  if (ret != DRIZZLE_RETURN_OK)
  {
    printf("Prepare failure\n");
    return EXIT_FAILURE;
  }
  printf("Params: %" PRIu16 "\n", drizzle_stmt_param_count(stmt));

  uint32_t val= 1;
  ret = drizzle_stmt_bind_param(stmt, 0, DRIZZLE_COLUMN_TYPE_LONG, &val, 4, DRIZZLE_BIND_OPTION_NONE);
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
  drizzle_stmt_buffer(stmt);
  printf("Rows found: %" PRIu64 "\n", drizzle_stmt_row_count(stmt));
  while (drizzle_stmt_fetch(stmt) != DRIZZLE_RETURN_ROW_END)
  {
    uint32_t *res_val;
    res_val= (uint32_t*)drizzle_stmt_item_data(stmt, 1);
    printf("Got value: %" PRIu32 "\n", *res_val);
  }
  ret = drizzle_stmt_close(stmt);
  if (ret != DRIZZLE_RETURN_OK)
  {
    printf("Statement close failure\n");
    return EXIT_FAILURE;
  }

  drizzle_query_str(con, "drop table libdrizzle.t1", &ret);
  if (ret != DRIZZLE_RETURN_OK)
  {
    printf("Drop table failure\n");
    return EXIT_FAILURE;
  }


  drizzle_quit(con);
  return EXIT_SUCCESS;
}
