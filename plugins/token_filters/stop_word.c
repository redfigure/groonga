/* -*- c-basic-offset: 2 -*- */
/*
  Copyright(C) 2014 Brazil

  This library is free software; you can redistribute it and/or
  modify it under the terms of the GNU Lesser General Public
  License version 2.1 as published by the Free Software Foundation.

  This library is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
  Lesser General Public License for more details.

  You should have received a copy of the GNU Lesser General Public
  License along with this library; if not, write to the Free Software
  Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
*/

#include <str.h>

#include <groonga.h>
#include <groonga/token_filter.h>

#include <string.h>

#define COLUMN_NAME "stop_word"

typedef struct {
  grn_obj *table;
  grn_token_mode mode;
  grn_obj *column;
  grn_obj value;
  grn_tokenizer_token token;
} grn_stop_word_token_filter;

static grn_obj *
stop_word_init(grn_ctx *ctx, int nargs, grn_obj **args, grn_user_data *user_data)
{
  grn_stop_word_token_filter *token_filter;

  token_filter = GRN_PLUGIN_MALLOC(ctx, sizeof(grn_stop_word_token_filter));
  if (!token_filter) {
    GRN_PLUGIN_ERROR(ctx, GRN_NO_MEMORY_AVAILABLE,
                     "[token-filter][stop-word] "
                     "failed to allocate grn_stop_word_token_filter");
    return NULL;
  }

  token_filter->table = args[0];
  token_filter->mode = GRN_UINT32_VALUE(args[1]);
  token_filter->column = grn_obj_column(ctx,
                                        token_filter->table,
                                        COLUMN_NAME,
                                        strlen(COLUMN_NAME));
  if (!token_filter->column) {
    char table_name[GRN_TABLE_MAX_KEY_SIZE];
    unsigned int table_name_size;

    table_name_size = grn_obj_name(ctx,
                                   token_filter->table,
                                   table_name,
                                   GRN_TABLE_MAX_KEY_SIZE);
    GRN_PLUGIN_ERROR(ctx, GRN_TOKEN_FILTER_ERROR,
                     "[token-filter][stop-word] "
                     "column for judging stop word doesn't exit: <%.*s.%s>",
                     table_name_size,
                     table_name,
                     COLUMN_NAME);
    GRN_PLUGIN_FREE(ctx, token_filter);
    return NULL;
  }

  user_data->ptr = token_filter;

  GRN_BOOL_INIT(&(token_filter->value), 0);
  grn_tokenizer_token_init(ctx, &(token_filter->token));

  return NULL;
}

static grn_obj *
stop_word_next(grn_ctx *ctx, int nargs, grn_obj **args, grn_user_data *user_data)
{
  grn_stop_word_token_filter *token_filter = user_data->ptr;
  grn_obj *current_token = args[0];
  int status = GRN_INT32_VALUE(args[1]);

  if (token_filter->mode == GRN_TOKEN_GET) {
    grn_id id;
    id = grn_table_get(ctx,
                       token_filter->table,
                       GRN_TEXT_VALUE(current_token),
                       GRN_TEXT_LEN(current_token));
    if (id != GRN_ID_NIL) {
      GRN_BULK_REWIND(&(token_filter->value));
      grn_obj_get_value(ctx,
                        token_filter->column,
                        id,
                        &(token_filter->value));
      if (GRN_BOOL_VALUE(&(token_filter->value))) {
        status |= GRN_TOKENIZER_TOKEN_SKIP;
      }
    }
  }

  grn_tokenizer_token_push(ctx,
                           &(token_filter->token),
                           GRN_TEXT_VALUE(current_token),
                           GRN_TEXT_LEN(current_token),
                           status);

  return NULL;
}

static grn_obj *
stop_word_fin(grn_ctx *ctx, int nargs, grn_obj **args, grn_user_data *user_data)
{
  grn_stop_word_token_filter *token_filter = user_data->ptr;
  if (!token_filter) {
    return NULL;
  }
  grn_tokenizer_token_fin(ctx, &(token_filter->token));
  grn_obj_unlink(ctx, token_filter->column);
  grn_obj_unlink(ctx, &(token_filter->value));
  GRN_PLUGIN_FREE(ctx, token_filter);
  return NULL;
}

grn_rc
GRN_PLUGIN_INIT(grn_ctx *ctx)
{
  return ctx->rc;
}

grn_rc
GRN_PLUGIN_REGISTER(grn_ctx *ctx)
{
  grn_rc rc;

  rc = grn_token_filter_register(ctx,
                                 "TokenFilterStopWord", -1,
                                 stop_word_init,
                                 stop_word_next,
                                 stop_word_fin);

  return rc;
}

grn_rc
GRN_PLUGIN_FIN(grn_ctx *ctx)
{
  return GRN_SUCCESS;
}