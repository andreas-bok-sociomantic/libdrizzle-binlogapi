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

#include <libdrizzle/drizzle_client.h>
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[])
{
  (void) argc;
  (void) argv;
  drizzle_st *drizzle;
  drizzle_con_st *con;
  drizzle_return_t ret;
  drizzle_result_st *result;
  drizzle_row_t row;
  int num_fields;

  drizzle = drizzle_create();
  if (drizzle == NULL)
  {
    printf("Drizzle object creation error\n");
    return EXIT_FAILURE;
  }
  con = drizzle_con_add_tcp(drizzle, "localhost", 3306, "root", "", "libdrizzle", 0);
  if (con == NULL)
  {
    printf("Drizzle connection object creation error\n");
    return EXIT_FAILURE;
  }
  ret = drizzle_con_connect(con);
  if (ret != DRIZZLE_RETURN_OK)
  {
    printf("Drizzle connection failure\n");
    return EXIT_FAILURE;
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

  result= drizzle_query_str(con, "select * from libdrizzle.t1", &ret);
  if (ret != DRIZZLE_RETURN_OK)
  {
    printf("Select failure\n");
    return EXIT_FAILURE;
  }
  drizzle_result_buffer(result);
  num_fields= drizzle_result_column_count(result);

  printf("%d fields\n", num_fields);
  while ((row = drizzle_row_next(result)))
  {
    printf("Data: %s\n", row[0]);
  }

  drizzle_result_free(result);

  drizzle_query_str(con, "drop table libdrizzle.t1", &ret);
  if (ret != DRIZZLE_RETURN_OK)
  {
    printf("Drop table failure\n");
    return EXIT_FAILURE;
  }


  drizzle_con_quit(con);
  drizzle_free(drizzle);
  return EXIT_SUCCESS;
}
