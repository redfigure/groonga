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
#ifndef GROONGA_TOKEN_H
#define GROONGA_TOKEN_H

#ifdef __cplusplus
extern "C" {
#endif  /* __cplusplus */

/*
  grn_token_mode describes propose for tokenization.

  `GRN_TOKEN_GET`: Tokenization for search.

  `GRN_TOKEN_ADD`: Tokenization for adding token to index.

  `GRN_TOKEN_DEL`: Tokenization for deleting token from index.

  @since 4.0.7
 */
typedef enum {
  GRN_TOKEN_GET = 0,
  GRN_TOKEN_ADD,
  GRN_TOKEN_DEL
} grn_token_mode;

/*
 * grn_token_status is a flag set for tokenizer status codes.
 * If a document or query contains no tokens, push an empty string with
 * GRN_TOKEN_LAST as a token.
 *
 * @since 4.0.7
 */
typedef unsigned int grn_token_status;

/*
 * GRN_TOKEN_CONTINUE means that the next token is not the last one.
 *
 * @since 4.0.7
 */
#define GRN_TOKEN_CONTINUE           (0)
/*
 * GRN_TOKEN_LAST means that the next token is the last one.
 *
 * @since 4.0.7
 */
#define GRN_TOKEN_LAST               (0x01L<<0)
/*
 * GRN_TOKEN_OVERLAP means that ...
 *
 * @since 4.0.7
 */
#define GRN_TOKEN_OVERLAP            (0x01L<<1)
/*
 * GRN_TOKEN_UNMATURED means that ...
 *
 * @since 4.0.7
 */
#define GRN_TOKEN_UNMATURED          (0x01L<<2)
/*
 * GRN_TOKEN_REACH_END means that ...
 *
 * @since 4.0.7
 */
#define GRN_TOKEN_REACH_END          (0x01L<<3)
/*
 * GRN_TOKEN_SKIP means that the token is skipped
 *
 * @since 4.0.7
 */
#define GRN_TOKEN_SKIP               (0x01L<<4)
/*
 * GRN_TOKEN_SKIP_WITH_POSITION means that the token and postion is skipped
 *
 * @since 4.0.7
 */
#define GRN_TOKEN_SKIP_WITH_POSITION (0x01L<<5)
/*
 * GRN_TOKEN_FORCE_PREIX that the token is used common prefix search
 *
 * @since 4.0.7
 */
#define GRN_TOKEN_FORCE_PREFIX       (0x01L<<6)

typedef struct _grn_token grn_token;

GRN_PLUGIN_EXPORT grn_obj *grn_token_get_data(grn_ctx *ctx,
                                              grn_token *token);
GRN_PLUGIN_EXPORT grn_rc grn_token_set_data(grn_ctx *ctx,
                                            grn_token *token,
                                            const char *str_ptr,
                                            int str_length);
GRN_PLUGIN_EXPORT grn_token_status grn_token_get_status(grn_ctx *ctx,
                                                        grn_token *token);
GRN_PLUGIN_EXPORT grn_rc grn_token_set_status(grn_ctx *ctx,
                                              grn_token *token,
                                              grn_token_status status);

#ifdef __cplusplus
}  /* extern "C" */
#endif  /* __cplusplus */

#endif  /* GROONGA_TOKEN_H */