/*
 * gcc-genericize.c
 *
 *  Created on: Mar 31, 2012
 *      Author: Guo Jiuliang
 * Description: mainly taken from GCC source tree to make this genericizing process
 *              call our own tree walker
 */

/* Tree-dumping functionality for intermediate representation.
   Copyright (C) 1999, 2000, 2002, 2003, 2004, 2005, 2006, 2007, 2008, 2009,
   2010, 2011 Free Software Foundation, Inc.
   Written by Mark Mitchell <mark@codesourcery.com>

This file is part of GCC.

GCC is free software; you can redistribute it and/or modify it under
the terms of the GNU General Public License as published by the Free
Software Foundation; either version 3, or (at your option) any later
version.

GCC is distributed in the hope that it will be useful, but WITHOUT ANY
WARRANTY; without even the implied warranty of MERCHANTABILITY or
FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License
for more details.

You should have received a copy of the GNU General Public License
along with GCC; see the file COPYING3.  If not see
<http://www.gnu.org/licenses/>.  */

#include "gcc-plugin.h"
#include "stdio.h"
#include "system.h"
#include "coretypes.h"
#include "tm.h"
#include "tree.h"
#include "cp-tree.h"
#include "c-family/c-common.h"
#include "tree-iterator.h"
#include "gimple.h"
#include "hashtab.h"
#include "pointer-set.h"
#include "flags.h"
#include "tree-flow.h"
#include "tree-pass.h"
#include "cfgloop.h"
#include "cgraph.h"
#include "options.h"
#include "gcc-tree-walker.h"
#include "basic-block.h"
#include "tree-inline.h"
#include "diagnostic-core.h"
#include "langhooks.h"
#include "langhooks-def.h"
#include "ASTTreeBuilder.h"
extern void gcc_dump_node (const_tree t, int flags, FILE *stream);
extern const char* get_src_full_filename();
void register_src_file(tree t);
void gcc_cp_genericize (tree fndecl);
/* Local declarations.  */

enum bc_t { bc_break = 0, bc_continue = 1 };

/* Stack of labels which are targets for "break" or "continue",
linked through TREE_CHAIN.  */
static tree bc_label[2];
static int saved_state;
static struct dump_file_info* saved_dfi;
//extern cp_parser * the_parser;
void register_state(struct dump_file_info* dfi,int state)
{
saved_dfi = dfi;
saved_state = state;
}
void restore_state()
{
saved_dfi->state = saved_state;
}
struct cp_genericize_data
{
struct pointer_set_t *p_set;
VEC (tree, heap) *bind_expr_stack;
};
FILE *
gcc_dump_begin (int phase, int *flag_ptr);
void
gcc_dump_end (int phase ATTRIBUTE_UNUSED, FILE *stream)
{
fclose (stream);
astTreeBuilder->finishFunction();
}
static inline bool
is_invisiref_parm (const_tree t)
{
return ((TREE_CODE (t) == PARM_DECL || TREE_CODE (t) == RESULT_DECL)
  && DECL_BY_REFERENCE (t));
}
/* Genericize an IF_STMT by turning it into a COND_EXPR.  */

static void
genericize_if_stmt (tree *stmt_p)
{
tree stmt, cond, then_, else_;
location_t locus = EXPR_LOCATION (*stmt_p);

stmt = *stmt_p;
cond = IF_COND (stmt);
then_ = THEN_CLAUSE (stmt);
else_ = ELSE_CLAUSE (stmt);

if (!then_)
then_ = build_empty_stmt (locus);
if (!else_)
else_ = build_empty_stmt (locus);

if (integer_nonzerop (cond) && !TREE_SIDE_EFFECTS (else_))
stmt = then_;
else if (integer_zerop (cond) && !TREE_SIDE_EFFECTS (then_))
stmt = else_;
else
stmt = build3 (COND_EXPR, void_type_node, cond, then_, else_);
if (CAN_HAVE_LOCATION_P (stmt) && !EXPR_HAS_LOCATION (stmt))
SET_EXPR_LOCATION (stmt, locus);
*stmt_p = stmt;
}
static tree
cp_genericize_r (tree *stmt_p, int *walk_subtrees, void *data)
{
tree stmt = *stmt_p;
struct cp_genericize_data *wtd = (struct cp_genericize_data *) data;
struct pointer_set_t *p_set = wtd->p_set;

if (is_invisiref_parm (stmt)
  /* Don't dereference parms in a thunk, pass the references through. */
  && !(DECL_THUNK_P (current_function_decl)
   && TREE_CODE (stmt) == PARM_DECL))
{
  *stmt_p = convert_from_reference (stmt);
  *walk_subtrees = 0;
  return NULL;
}

/* Map block scope extern declarations to visible declarations with the
 same name and type in outer scopes if any.  */
if (cp_function_chain->extern_decl_map
  && (TREE_CODE (stmt) == FUNCTION_DECL || TREE_CODE (stmt) == VAR_DECL)
  && DECL_EXTERNAL (stmt))
{
  struct cxx_int_tree_map *h, in;
  in.uid = DECL_UID (stmt);
  h = (struct cxx_int_tree_map *)
  htab_find_with_hash (cp_function_chain->extern_decl_map,
			   &in, in.uid);
  if (h)
{
  *stmt_p = h->to;
  *walk_subtrees = 0;
  return NULL;
}
}

/* Other than invisiref parms, don't walk the same tree twice.  */
if (pointer_set_contains (p_set, stmt))
{
  *walk_subtrees = 0;
  return NULL_TREE;
}

if (TREE_CODE (stmt) == ADDR_EXPR
  && is_invisiref_parm (TREE_OPERAND (stmt, 0)))
{
  *stmt_p = convert (TREE_TYPE (stmt), TREE_OPERAND (stmt, 0));
  *walk_subtrees = 0;
}
else if (TREE_CODE (stmt) == RETURN_EXPR
   && TREE_OPERAND (stmt, 0)
   && is_invisiref_parm (TREE_OPERAND (stmt, 0)))
/* Don't dereference an invisiref RESULT_DECL inside a RETURN_EXPR.  */
*walk_subtrees = 0;
else if (TREE_CODE (stmt) == OMP_CLAUSE)
switch (OMP_CLAUSE_CODE (stmt))
  {
  case OMP_CLAUSE_LASTPRIVATE:
/* Don't dereference an invisiref in OpenMP clauses.  */
if (is_invisiref_parm (OMP_CLAUSE_DECL (stmt)))
  {
	*walk_subtrees = 0;
	if (OMP_CLAUSE_LASTPRIVATE_STMT (stmt))
	  cp_walk_tree (&OMP_CLAUSE_LASTPRIVATE_STMT (stmt),
			cp_genericize_r, data, NULL);
  }
break;
  case OMP_CLAUSE_PRIVATE:
  case OMP_CLAUSE_SHARED:
  case OMP_CLAUSE_FIRSTPRIVATE:
  case OMP_CLAUSE_COPYIN:
  case OMP_CLAUSE_COPYPRIVATE:
/* Don't dereference an invisiref in OpenMP clauses.  */
if (is_invisiref_parm (OMP_CLAUSE_DECL (stmt)))
  *walk_subtrees = 0;
break;
  case OMP_CLAUSE_REDUCTION:
gcc_assert (!is_invisiref_parm (OMP_CLAUSE_DECL (stmt)));
break;
  default:
break;
  }
else if (IS_TYPE_OR_DECL_P (stmt))
*walk_subtrees = 0;

/* Due to the way voidify_wrapper_expr is written, we don't get a chance
 to lower this construct before scanning it, so we need to lower these
 before doing anything else.  */
else if (TREE_CODE (stmt) == CLEANUP_STMT)
*stmt_p = build2 (CLEANUP_EH_ONLY (stmt) ? TRY_CATCH_EXPR
					 : TRY_FINALLY_EXPR,
		  void_type_node,
		  CLEANUP_BODY (stmt),
		  CLEANUP_EXPR (stmt));

else if (TREE_CODE (stmt) == IF_STMT)
{
  genericize_if_stmt (stmt_p);
  /* *stmt_p has changed, tail recurse to handle it again.  */
  return cp_genericize_r (stmt_p, walk_subtrees, data);
}

/* COND_EXPR might have incompatible types in branches if one or both
 arms are bitfields.FILE *
my_dump_begin (int phase, int *flag_ptr)  Fix it up now.  */
else if (TREE_CODE (stmt) == COND_EXPR)
{
  tree type_left
= (TREE_OPERAND (stmt, 1)
   ? is_bitfield_expr_with_lowered_type (TREE_OPERAND (stmt, 1))
   : NULL_TREE);
  tree type_right
= (TREE_OPERAND (stmt, 2)
   ? is_bitfield_expr_with_lowered_type (TREE_OPERAND (stmt, 2))
   : NULL_TREE);
  if (type_left
  && !useless_type_conversion_p (TREE_TYPE (stmt),
				 TREE_TYPE (TREE_OPERAND (stmt, 1))))
{
  TREE_OPERAND (stmt, 1)
	= fold_convert (type_left, TREE_OPERAND (stmt, 1));
  gcc_assert (useless_type_conversion_p (TREE_TYPE (stmt),
					 type_left));
}
  if (type_right
  && !useless_type_conversion_p (TREE_TYPE (stmt),
				 TREE_TYPE (TREE_OPERAND (stmt, 2))))
{
  TREE_OPERAND (stmt, 2)
	= fold_convert (type_right, TREE_OPERAND (stmt, 2));
  gcc_assert (useless_type_conversion_p (TREE_TYPE (stmt),
					 type_right));
}
}

else if (TREE_CODE (stmt) == BIND_EXPR)
{
  VEC_safe_push (tree, heap, wtd->bind_expr_stack, stmt);
  cp_walk_tree (&BIND_EXPR_BODY (stmt),
		cp_genericize_r, data, NULL);
  VEC_pop (tree, wtd->bind_expr_stack);
}

else if (TREE_CODE (stmt) == USING_STMT)
{
  tree block = NULL_TREE;

  /* Get the innermost inclosing GIMPLE_BIND that has a non NULL
	 BLOCK, and append an IMPORTED_DECL to its
 BLOCK_VARS chained list.  */
  if (wtd->bind_expr_stack)
{
  int i;
  for (i = VEC_length (tree, wtd->bind_expr_stack) - 1; i >= 0; i--)
	if ((block = BIND_EXPR_BLOCK (VEC_index (tree,
						 wtd->bind_expr_stack, i))))
	  break;
}
  if (block)
{
  tree using_directive;
  gcc_assert (TREE_OPERAND (stmt, 0));

  using_directive = make_node (IMPORTED_DECL);
  TREE_TYPE (using_directive) = void_type_node;

  IMPORTED_DECL_ASSOCIATED_DECL (using_directive)
	= TREE_OPERAND (stmt, 0);
  DECL_CHAIN (using_directive) = BLOCK_VARS (block);
  BLOCK_VARS (block) = using_directive;
}
  /* The USING_STMT won't appear in GENERIC.  */
  *stmt_p = build1 (NOP_EXPR, void_type_node, integer_zero_node);
  *walk_subtrees = 0;
}

else if (TREE_CODE (stmt) == DECL_EXPR
   && TREE_CODE (DECL_EXPR_DECL (stmt)) == USING_DECL)
{
  /* Using decls inside DECL_EXPRs are just dropped on the floor.  */
  *stmt_p = build1 (NOP_EXPR, void_type_node, integer_zero_node);
  *walk_subtrees = 0;
}

pointer_set_insert (p_set, *stmt_p);

return NULL;
}
FILE *
gcc_dump_begin (int phase, int *flag_ptr)
{
char name[256];
struct dump_file_info *dfi;
FILE *stream;
if (phase == TDI_none)
return NULL;

//name = "test.original";
strcpy(name,get_src_full_filename());
strcat(name,".original");
//name = get_dump_file_name (phase);
//name = global_options.x_dump_base_name;
dfi = get_dump_file_info (phase);
#ifdef PLUGIN_DEBUG
stream = fopen (name, dfi->state < 0 ? "w" : "a");
#else
stream = fopen("/dev/null",dfi->state < 0 ? "w" : "a");
#endif
register_state(dfi,dfi->state);
if (!stream)
error ("could not open dump file %qs: %m", name);
else
dfi->state = 1;

//free (name);

if (flag_ptr)
*flag_ptr = dfi->flags;

return stream;
}
void
gcc_genericize (tree fndecl)
{
  FILE *dump_orig;
  int local_dump_flags;
  struct cgraph_node *cgn;

  /* Dump the C-specific tree IR.  */
  dump_orig = gcc_dump_begin (TDI_original, &local_dump_flags);

  /*
	  if(dump_orig)
	{
		printf("should start dumping original tree.\n");
	}else{
		printf("no dumping original tree flag.\n");
	}
	*/

  if (dump_orig)
	{
	  fprintf (dump_orig, "\n;; Function %s",
		   lang_hooks.decl_printable_name (fndecl, 2));

	  astTreeBuilder->addFunctionName(lang_hooks.decl_printable_name (fndecl, 2));

	  fprintf (dump_orig, " (%s)\n",
		   (!DECL_ASSEMBLER_NAME_SET_P (fndecl) ? "null"
		: IDENTIFIER_POINTER (DECL_ASSEMBLER_NAME (fndecl))));
	  fprintf (dump_orig, ";; enabled by -%s\n", dump_flag_name (TDI_original));
	  fprintf (dump_orig, "\n");

	  //if (local_dump_flags & TDF_RAW)
	gcc_dump_node (DECL_SAVED_TREE (fndecl),
		   TDF_SLIM | local_dump_flags, dump_orig);
 //     else
//	print_c_tree (dump_orig, DECL_SAVED_TREE (fndecl));
	  fprintf (dump_orig, "\n");
	  restore_state();
	  gcc_dump_end (TDI_original, dump_orig);
	}

  /* Dump all nested functions now.  */
  cgn = cgraph_node (fndecl);
  for (cgn = cgn->nested; cgn ; cgn = cgn->next_nested)
	gcc_genericize (cgn->decl);
}

void
gcc_cp_genericize (tree fndecl)
{
  register_src_file(fndecl);
  tree t;
  struct cp_genericize_data wtd;

  /* Fix up the types of parms passed by invisible reference.  */
  for (t = DECL_ARGUMENTS (fndecl); t; t = DECL_CHAIN (t))
	if (TREE_ADDRESSABLE (TREE_TYPE (t)))
	  {
	/* If a function's arguments are copied to create a thunk,
	   then DECL_BY_REFERENCE will be set -- but the type of the
	   argument will be a pointer type, so we will never get
	   here.  */
	gcc_assert (!DECL_BY_REFERENCE (t));
	gcc_assert (DECL_ARG_TYPE (t) != TREE_TYPE (t));
	TREE_TYPE (t) = DECL_ARG_TYPE (t);
	DECL_BY_REFERENCE (t) = 1;
	TREE_ADDRESSABLE (t) = 0;
	relayout_decl (t);
	  }

  /* Do the same for the return value.  */
  if (TREE_ADDRESSABLE (TREE_TYPE (DECL_RESULT (fndecl))))
	{
	  t = DECL_RESULT (fndecl);
	  TREE_TYPE (t) = build_reference_type (TREE_TYPE (t));
	  DECL_BY_REFERENCE (t) = 1;
	  TREE_ADDRESSABLE (t) = 0;
	  relayout_decl (t);
	  if (DECL_NAME (t))
	{
	  /* Adjust DECL_VALUE_EXPR of the original var.  */
	  tree outer = outer_curly_brace_block (current_function_decl);
	  tree var;

	  if (outer)
		for (var = BLOCK_VARS (outer); var; var = DECL_CHAIN (var))
		  if (DECL_NAME (t) == DECL_NAME (var)
		  && DECL_HAS_VALUE_EXPR_P (var)
		  && DECL_VALUE_EXPR (var) == t)
		{
		  tree val = convert_from_reference (t);
		  SET_DECL_VALUE_EXPR (var, val);
		  break;
		}
	}
	}

  /* If we're a clone, the body is already GIMPLE.  */
  if (DECL_CLONED_FUNCTION_P (fndecl))
	return;

  /* We do want to see every occurrence of the parms, so we can't just use
	 walk_tree's hash functionality.  */
  wtd.p_set = pointer_set_create ();
  wtd.bind_expr_stack = NULL;
  cp_walk_tree (&DECL_SAVED_TREE (fndecl), cp_genericize_r, &wtd, NULL);
  pointer_set_destroy (wtd.p_set);
  VEC_free (tree, heap, wtd.bind_expr_stack);

  /* Do everything else.  */
  gcc_genericize (fndecl);

  gcc_assert (bc_label[bc_break] == NULL);
  gcc_assert (bc_label[bc_continue] == NULL);
}
void register_src_file(tree t)
{
	expanded_location xloc = expand_location (DECL_SOURCE_LOCATION (t));
	register_src_full_filename(xloc.file);
}
