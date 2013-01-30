/*
 * gcc-tree-walker.c
 *
 *  Created on: Mar 31, 2012
 *      Author: Guo Jiuliang
 * Description: mainly taken from GCC source tree to walk through generic tree
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

#include "config.h"
#include "analyzer-config.h"
#include "stdio.h"
#include "system.h"
#include "coretypes.h"
#include "tm.h"
#include "tree.h"
#include "cp-tree.h"
#include "gcc-tree-walker.h"
#include "c-common.h"
#include "splay-tree.h"
#include "diagnostic-core.h"
#include "toplev.h"
#include "tree-pass.h"
#include "langhooks.h"
#include "tree-iterator.h"

/* If non-NULL, return one past-the-end of the matching SUBPART of
   the WHOLE string.  */
#define skip_leading_substring(whole,  part) \
   (strncmp (whole, part, strlen (part)) ? NULL : whole + strlen (part))

static unsigned int queue (dump_info_p, const_tree, int);
static void dump_index (dump_info_p, unsigned int);
static void dequeue_and_dump (dump_info_p);
static void dump_new_line (dump_info_p);
static void dump_maybe_newline (dump_info_p);
/* c common part */
/* Dump information common to statements from STMT.  */

void
gcc_dump_stmt (dump_info_p di, const_tree t)
{
  if (EXPR_HAS_LOCATION (t))
    gcc_dump_int (di, "line", EXPR_LINENO (t));
}

/* Dump any C-specific tree codes and attributes of common codes.  */

bool
gcc_c_dump_tree (void *dump_info, tree t)
{
  enum tree_code code;
  dump_info_p di = (dump_info_p) dump_info;

  /* Figure out what kind of node this is.  */
  code = TREE_CODE (t);

  switch (code)
    {
    case FIELD_DECL:
      if (DECL_C_BIT_FIELD (t))
	gcc_dump_string (di, "bitfield");
      break;

    default:
      break;
    }

  return false;
}
/* c++ specific */
static void dump_access (dump_info_p, tree);

static void dump_op (dump_info_p, tree);

/* Dump a representation of the accessibility information associated
   with T.  */

static void
dump_access (dump_info_p di, tree t)
{
  if (TREE_PROTECTED(t))
    dump_string_field (di, "accs", "prot");
  else if (TREE_PRIVATE(t))
    dump_string_field (di, "accs", "priv");
  else
    dump_string_field (di, "accs", "pub");
}

/* Dump a representation of the specific operator for an overloaded
   operator associated with node t.  */

static void
dump_op (dump_info_p di, tree t)
{
  switch (DECL_OVERLOADED_OPERATOR_P (t)) {
    case NEW_EXPR:
      gcc_dump_string (di, "new");
      break;
    case VEC_NEW_EXPR:
      gcc_dump_string (di, "vecnew");
      break;
    case DELETE_EXPR:
      gcc_dump_string (di, "delete");
      break;
    case VEC_DELETE_EXPR:
      gcc_dump_string (di, "vecdelete");
      break;
    case UNARY_PLUS_EXPR:
      gcc_dump_string (di, "pos");
      break;
    case NEGATE_EXPR:
      gcc_dump_string (di, "neg");
      break;
    case ADDR_EXPR:
      gcc_dump_string (di, "addr");
      break;
    case INDIRECT_REF:
      gcc_dump_string(di, "deref");
      break;
    case BIT_NOT_EXPR:
      gcc_dump_string(di, "not");
      break;
    case TRUTH_NOT_EXPR:
      gcc_dump_string(di, "lnot");
      break;
    case PREINCREMENT_EXPR:
      gcc_dump_string(di, "preinc");
      break;
    case PREDECREMENT_EXPR:
      gcc_dump_string(di, "predec");
      break;
    case PLUS_EXPR:
      if (DECL_ASSIGNMENT_OPERATOR_P (t))
	gcc_dump_string (di, "plusassign");
      else
	gcc_dump_string(di, "plus");
      break;
    case MINUS_EXPR:
      if (DECL_ASSIGNMENT_OPERATOR_P (t))
	gcc_dump_string (di, "minusassign");
      else
	gcc_dump_string(di, "minus");
      break;
    case MULT_EXPR:
      if (DECL_ASSIGNMENT_OPERATOR_P (t))
	gcc_dump_string (di, "multassign");
      else
	gcc_dump_string (di, "mult");
      break;
    case TRUNC_DIV_EXPR:
      if (DECL_ASSIGNMENT_OPERATOR_P (t))
	gcc_dump_string (di, "divassign");
      else
	gcc_dump_string (di, "div");
      break;
    case TRUNC_MOD_EXPR:
      if (DECL_ASSIGNMENT_OPERATOR_P (t))
	 gcc_dump_string (di, "modassign");
      else
	 gcc_dump_string (di, "mod");
      break;
    case BIT_AND_EXPR:
      if (DECL_ASSIGNMENT_OPERATOR_P (t))
	 gcc_dump_string (di, "andassign");
      else
	 gcc_dump_string (di, "and");
      break;
    case BIT_IOR_EXPR:
      if (DECL_ASSIGNMENT_OPERATOR_P (t))
	 gcc_dump_string (di, "orassign");
      else
	 gcc_dump_string (di, "or");
      break;
    case BIT_XOR_EXPR:
      if (DECL_ASSIGNMENT_OPERATOR_P (t))
	 gcc_dump_string (di, "xorassign");
      else
	 gcc_dump_string (di, "xor");
      break;
    case LSHIFT_EXPR:
      if (DECL_ASSIGNMENT_OPERATOR_P (t))
	 gcc_dump_string (di, "lshiftassign");
      else
	 gcc_dump_string (di, "lshift");
      break;
    case RSHIFT_EXPR:
      if (DECL_ASSIGNMENT_OPERATOR_P (t))
	 gcc_dump_string (di, "rshiftassign");
      else
	 gcc_dump_string (di, "rshift");
      break;
    case EQ_EXPR:
      gcc_dump_string (di, "eq");
      break;
    case NE_EXPR:
      gcc_dump_string (di, "ne");
      break;
    case LT_EXPR:
      gcc_dump_string (di, "lt");
      break;
    case GT_EXPR:
      gcc_dump_string (di, "gt");
      break;
    case LE_EXPR:
      gcc_dump_string (di, "le");
      break;
    case GE_EXPR:
      gcc_dump_string (di, "ge");
      break;
    case TRUTH_ANDIF_EXPR:
      gcc_dump_string (di, "land");
      break;
    case TRUTH_ORIF_EXPR:
      gcc_dump_string (di, "lor");
      break;
    case COMPOUND_EXPR:
      gcc_dump_string (di, "compound");
      break;
    case MEMBER_REF:
      gcc_dump_string (di, "memref");
      break;
    case COMPONENT_REF:
      gcc_dump_string (di, "ref");
      break;
    case ARRAY_REF:
      gcc_dump_string (di, "subs");
      break;
    case POSTINCREMENT_EXPR:
      gcc_dump_string (di, "postinc");
      break;
    case POSTDECREMENT_EXPR:
      gcc_dump_string (di, "postdec");
      break;
    case CALL_EXPR:
      gcc_dump_string (di, "call");
      break;
    case NOP_EXPR:
      if (DECL_ASSIGNMENT_OPERATOR_P (t))
	gcc_dump_string (di, "assign");
      break;
    default:
      break;
  }
}

bool
gcc_cp_dump_tree (void* dump_info, tree t)
{
  enum tree_code code;
  dump_info_p di = (dump_info_p) dump_info;

  /* Figure out what kind of node this is.  */
  code = TREE_CODE (t);

  if (DECL_P (t))
    {
      if (DECL_LANG_SPECIFIC (t) && DECL_LANGUAGE (t) != lang_cplusplus)
	gcc_dump_string_field (di, "lang", language_to_string (DECL_LANGUAGE (t)));
    }

  switch (code)
    {
    case IDENTIFIER_NODE:
      if (IDENTIFIER_OPNAME_P (t))
	{
	  gcc_dump_string_field (di, "note", "operator");
	  return true;
	}
      else if (IDENTIFIER_TYPENAME_P (t))
	{
	  dump_child ("tynm", TREE_TYPE (t));
	  return true;
	}
      break;

    case OFFSET_TYPE:
      gcc_dump_string_field (di, "note", "ptrmem");
      dump_child ("ptd", TYPE_PTRMEM_POINTED_TO_TYPE (t));
      dump_child ("cls", TYPE_PTRMEM_CLASS_TYPE (t));
      return true;

    case RECORD_TYPE:
      if (TYPE_PTRMEMFUNC_P (t))
	{
	  gcc_dump_string_field (di, "note", "ptrmem");
	  dump_child ("ptd", TYPE_PTRMEM_POINTED_TO_TYPE (t));
	  dump_child ("cls", TYPE_PTRMEM_CLASS_TYPE (t));
	  return true;
	}
      /* Fall through.  */

    case UNION_TYPE:
      /* Is it a type used as a base? */
      if (TYPE_CONTEXT (t) && TREE_CODE (TYPE_CONTEXT (t)) == TREE_CODE (t)
	  && CLASSTYPE_AS_BASE (TYPE_CONTEXT (t)) == t)
	{
	  dump_child ("bfld", TYPE_CONTEXT (t));
	  return true;
	}

      if (! MAYBE_CLASS_TYPE_P (t))
	break;

      dump_child ("vfld", TYPE_VFIELD (t));
      if (CLASSTYPE_TEMPLATE_SPECIALIZATION(t))
	gcc_dump_string(di, "spec");

      if (!dump_flag (di, TDF_SLIM, t) && TYPE_BINFO (t))
	{
	  int i;
	  tree binfo;
	  tree base_binfo;

	  for (binfo = TYPE_BINFO (t), i = 0;
	       BINFO_BASE_ITERATE (binfo, i, base_binfo); ++i)
	    {
	      dump_child ("base", BINFO_TYPE (base_binfo));
	      if (BINFO_VIRTUAL_P (base_binfo))
		gcc_dump_string_field (di, "spec", "virt");
	      dump_access (di, base_binfo);
	    }
	}
      break;

    case FIELD_DECL:
      dump_access (di, t);
      if (DECL_MUTABLE_P (t))
	gcc_dump_string_field (di, "spec", "mutable");
      break;

    case VAR_DECL:
      if (TREE_CODE (CP_DECL_CONTEXT (t)) == RECORD_TYPE)
	dump_access (di, t);
      if (TREE_STATIC (t) && !TREE_PUBLIC (t))
	gcc_dump_string_field (di, "link", "static");
      break;

    case FUNCTION_DECL:
      if (!DECL_THUNK_P (t))
	{
	  if (DECL_OVERLOADED_OPERATOR_P (t)) {
	    gcc_dump_string_field (di, "note", "operator");
	    dump_op (di, t);
	  }
	  if (DECL_FUNCTION_MEMBER_P (t))
	    {
	      gcc_dump_string_field (di, "note", "member");
	      dump_access (di, t);
	    }
	  if (DECL_PURE_VIRTUAL_P (t))
	    gcc_dump_string_field (di, "spec", "pure");
	  if (DECL_VIRTUAL_P (t))
	    gcc_dump_string_field (di, "spec", "virt");
	  if (DECL_CONSTRUCTOR_P (t))
	    gcc_dump_string_field (di, "note", "constructor");
	  if (DECL_DESTRUCTOR_P (t))
	    gcc_dump_string_field (di, "note", "destructor");
	  if (DECL_CONV_FN_P (t))
	    gcc_dump_string_field (di, "note", "conversion");
	  if (DECL_GLOBAL_CTOR_P (t))
	    gcc_dump_string_field (di, "note", "global init");
	  if (DECL_GLOBAL_DTOR_P (t))
	    gcc_dump_string_field (di, "note", "global fini");
	  if (DECL_FRIEND_PSEUDO_TEMPLATE_INSTANTIATION (t))
	    gcc_dump_string_field (di, "note", "pseudo tmpl");
	}
      else
	{
	  tree virt = THUNK_VIRTUAL_OFFSET (t);

	  gcc_dump_string_field (di, "note", "thunk");
	  if (DECL_THIS_THUNK_P (t))
	    gcc_dump_string_field (di, "note", "this adjusting");
	  else
	    {
	      gcc_dump_string_field (di, "note", "result adjusting");
	      if (virt)
		virt = BINFO_VPTR_FIELD (virt);
	    }
	  gcc_dump_int (di, "fixd", THUNK_FIXED_OFFSET (t));
	  if (virt)
	    gcc_dump_int (di, "virt", tree_low_cst (virt, 0));
	  dump_child ("fn", DECL_INITIAL (t));
	}
      break;

    case NAMESPACE_DECL:
      if (DECL_NAMESPACE_ALIAS (t))
	dump_child ("alis", DECL_NAMESPACE_ALIAS (t));
      else if (!dump_flag (di, TDF_SLIM, t))
	dump_child ("dcls", cp_namespace_decls (t));
      break;

    case TEMPLATE_DECL:
      dump_child ("rslt", DECL_TEMPLATE_RESULT (t));
      dump_child ("inst", DECL_TEMPLATE_INSTANTIATIONS (t));
      dump_child ("spcs", DECL_TEMPLATE_SPECIALIZATIONS (t));
      dump_child ("prms", DECL_TEMPLATE_PARMS (t));
      break;

    case OVERLOAD:
      dump_child ("crnt", OVL_CURRENT (t));
      dump_child ("chan", OVL_CHAIN (t));
      break;

    case TRY_BLOCK:
      gcc_dump_stmt (di, t);
      if (CLEANUP_P (t))
	dump_string_field (di, "note", "cleanup");
      dump_child ("body", TRY_STMTS (t));
      dump_child ("hdlr", TRY_HANDLERS (t));
      break;

    case EH_SPEC_BLOCK:
      gcc_dump_stmt (di, t);
      dump_child ("body", EH_SPEC_STMTS (t));
      dump_child ("raises", EH_SPEC_RAISES (t));
      break;

    case PTRMEM_CST:
      dump_child ("clas", PTRMEM_CST_CLASS (t));
      dump_child ("mbr", PTRMEM_CST_MEMBER (t));
      break;

    case THROW_EXPR:
      /* These nodes are unary, but do not have code class `1'.  */
      dump_child ("op 0", TREE_OPERAND (t, 0));
      break;

//    case AGGR_INIT_EXPR:
//      {
//	int i = 0;
//	tree arg;
//	aggr_init_expr_arg_iterator iter;
//	gcc_dump_int (di, "ctor", AGGR_INIT_VIA_CTOR_P (t));
//	dump_child ("fn", AGGR_INIT_EXPR_FN (t));
//	FOR_EACH_AGGR_INIT_EXPR_ARG (arg, iter, t)
//	  {
//	    char buffer[32];
//	    sprintf (buffer, "%u", i);
//	    dump_child (buffer, arg);
//	    i++;
//	  }
//	dump_child ("decl", AGGR_INIT_EXPR_SLOT (t));
//      }
//      break;

    case HANDLER:
      gcc_dump_stmt (di, t);
      dump_child ("parm", HANDLER_PARMS (t));
      dump_child ("body", HANDLER_BODY (t));
      break;

    case MUST_NOT_THROW_EXPR:
      gcc_dump_stmt (di, t);
      dump_child ("body", TREE_OPERAND (t, 0));
      break;

    case USING_STMT:
      gcc_dump_stmt (di, t);
      dump_child ("nmsp", USING_STMT_NAMESPACE (t));
      break;

    case CLEANUP_STMT:
      gcc_dump_stmt (di, t);
      dump_child ("decl", CLEANUP_DECL (t));
      dump_child ("expr", CLEANUP_EXPR (t));
      dump_child ("body", CLEANUP_BODY (t));
      break;

    case IF_STMT:
      gcc_dump_stmt (di, t);
      dump_child ("cond", IF_COND (t));
      dump_child ("then", THEN_CLAUSE (t));
      dump_child ("else", ELSE_CLAUSE (t));
      break;

    case BREAK_STMT:
    case CONTINUE_STMT:
      gcc_dump_stmt (di, t);
      break;

    case DO_STMT:
      gcc_dump_stmt (di, t);
      dump_child ("body", DO_BODY (t));
      dump_child ("cond", DO_COND (t));
      break;

    case FOR_STMT:
      gcc_dump_stmt (di, t);
      dump_child ("init", FOR_INIT_STMT (t));
      dump_child ("cond", FOR_COND (t));
      dump_child ("expr", FOR_EXPR (t));
      dump_child ("body", FOR_BODY (t));
      break;

    case RANGE_FOR_STMT:
      gcc_dump_stmt (di, t);
      dump_child ("decl", RANGE_FOR_DECL (t));
      dump_child ("expr", RANGE_FOR_EXPR (t));
      dump_child ("body", RANGE_FOR_BODY (t));
      break;

    case SWITCH_STMT:
      gcc_dump_stmt (di, t);
      dump_child ("cond", SWITCH_STMT_COND (t));
      dump_child ("body", SWITCH_STMT_BODY (t));
      break;

    case WHILE_STMT:
      gcc_dump_stmt (di, t);
      dump_child ("cond", WHILE_COND (t));
      dump_child ("body", WHILE_BODY (t));
      break;

    case STMT_EXPR:
      dump_child ("stmt", STMT_EXPR_STMT (t));
      break;

    case EXPR_STMT:
      gcc_dump_stmt (di, t);
      dump_child ("expr", EXPR_STMT_EXPR (t));
      break;

    default:
      break;
    }

  return gcc_c_dump_tree (di, t);
}

/* Add T to the end of the queue of nodes to dump.  Returns the index
   assigned to T.  */

static unsigned int
queue (dump_info_p di, const_tree t, int flags)
{
  dump_queue_p dq;
  dump_node_info_p dni;
  unsigned int index;

  /* Assign the next available index to T.  */
  index = ++di->index;

  /* Obtain a new queue node.  */
  if (di->free_list)
    {
      dq = di->free_list;
      di->free_list = dq->next;
    }
  else
    dq = XNEW (struct dump_queue);

  /* Create a new entry in the splay-tree.  */
  dni = XNEW (struct dump_node_info);
  dni->index = index;
  dni->binfo_p = ((flags & DUMP_BINFO) != 0);
  dq->node = splay_tree_insert (di->nodes, (splay_tree_key) t,
				(splay_tree_value) dni);

  /* Add it to the end of the queue.  */
  dq->next = 0;
  if (!di->queue_end)
    di->queue = dq;
  else
    di->queue_end->next = dq;
  di->queue_end = dq;

  /* Return the index.  */
  return index;
}

static void
dump_index (dump_info_p di, unsigned int index)
{
  fprintf (di->stream, "@%-6u ", index);
  di->column += 8;
  //astTreeBuilder->addNode(index);
}

/* If T has not already been output, queue it for subsequent output.
   FIELD is a string to print before printing the index.  Then, the
   index of T is printed.  */

void
gcc_queue_and_dump_index (dump_info_p di, const char *field, const_tree t, int flags)
{
  unsigned int index;
  splay_tree_node n;

  /* If there's no node, just return.  This makes for fewer checks in
     our callers.  */
  if (!t)
    return;

  /* See if we've already queued or dumped this node.  */
  n = splay_tree_lookup (di->nodes, (splay_tree_key) t);
  if (n)
    index = ((dump_node_info_p) n->value)->index;
  else
    /* If we haven't, add it to the queue.  */
    index = queue (di, t, flags);

  /* Print the index of the node.  */
  dump_maybe_newline (di);
  fprintf (di->stream, "%-4s: ", field);
  di->column += 6;
  dump_index (di, index);
  astTreeBuilder->addChild(field,index);
}

/* Dump the type of T.  */

void
gcc_queue_and_dump_type (dump_info_p di, const_tree t)
{
  gcc_queue_and_dump_index (di, "type", TREE_TYPE (t), DUMP_NONE);
}

/* Dump column control */
#define SOL_COLUMN 25		/* Start of line column.  */
#define EOL_COLUMN 55		/* End of line column.  */
#define COLUMN_ALIGNMENT 15	/* Alignment.  */

/* Insert a new line in the dump output, and indent to an appropriate
   place to start printing more fields.  */

static void
dump_new_line (dump_info_p di)
{
  fprintf (di->stream, "\n%*s", SOL_COLUMN, "");
  di->column = SOL_COLUMN;
}

/* If necessary, insert a new line.  */

static void
dump_maybe_newline (dump_info_p di)
{
  int extra;

  /* See if we need a new line.  */
  if (di->column > EOL_COLUMN)
    dump_new_line (di);
  /* See if we need any padding.  */
  else if ((extra = (di->column - SOL_COLUMN) % COLUMN_ALIGNMENT) != 0)
    {
      fprintf (di->stream, "%*s", COLUMN_ALIGNMENT - extra, "");
      di->column += COLUMN_ALIGNMENT - extra;
    }
}

/* Dump pointer PTR using FIELD to identify it.  */

void
gcc_dump_pointer (dump_info_p di, const char *field, void *ptr)
{
  dump_maybe_newline (di);
  fprintf (di->stream, "%-4s: %-8lx ", field, (unsigned long) ptr);
  astTreeBuilder->addULongProperty(field,(unsigned long) ptr);
  di->column += 15;
}

/* Dump integer I using FIELD to identify it.  */

void
gcc_dump_int (dump_info_p di, const char *field, int i)
{
  dump_maybe_newline (di);
  fprintf (di->stream, "%-4s: %-7d ", field, i);
  astTreeBuilder->addIntProperty(field,i);
  di->column += 14;
}

/* Dump the floating point value R, using FIELD to identify it.  */

static void
gcc_dump_real (dump_info_p di, const char *field, const REAL_VALUE_TYPE *r)
{
  char buf[32];
  real_to_decimal (buf, r, sizeof (buf), 0, true);
  dump_maybe_newline (di);
  fprintf (di->stream, "%-4s: %s ", field, buf);
  astTreeBuilder->addRealProperty(field,buf);
  di->column += strlen (buf) + 7;
}

/* Dump the fixed-point value F, using FIELD to identify it.  */

static void
gcc_dump_fixed (dump_info_p di, const char *field, const FIXED_VALUE_TYPE *f)
{
  char buf[32];
  fixed_to_decimal (buf, f, sizeof (buf));
  dump_maybe_newline (di);
  fprintf (di->stream, "%-4s: %s ", field, buf);
  di->column += strlen (buf) + 7;
  astTreeBuilder->addFixedProperty(field,buf);
}


/* Dump the string S.  */

void
gcc_dump_string (dump_info_p di, const char *string)
{
	//why call this?
  dump_maybe_newline (di);
  fprintf (di->stream, "%-13s ", string);
  astTreeBuilder->addString(string);
  if (strlen (string) > 13)
    di->column += strlen (string) + 1;
  else
    di->column += 14;
}

/* Dump the string field S.  */

void
gcc_dump_string_field (dump_info_p di, const char *field, const char *string)
{
  dump_maybe_newline (di);
  fprintf (di->stream, "%-4s: %-7s ", field, string);
  astTreeBuilder->addStringProperty(field,string);
  if (strlen (string) > 7)
    di->column += 6 + strlen (string) + 1;
  else
    di->column += 14;
}

/* Dump the next node in the queue.  */

static void
dequeue_and_dump (dump_info_p di)
{
  //printf(stderr,"code name:%s",tree_code_name[
  dump_queue_p dq;
  splay_tree_node stn;
  dump_node_info_p dni;
  tree t;
  unsigned int index;
  enum tree_code code;
  enum tree_code_class code_class;
  const char* code_name;

  /* Get the next node from the queue.  */
  dq = di->queue;
  stn = dq->node;
  t = (tree) stn->key;
  //print tree code name
  //printf("code name:%s\n",tree_code_name[(int) TREE_CODE(t)]);
  dni = (dump_node_info_p) stn->value;
  index = dni->index;

  /* Remove the node from the queue, and put it on the free list.  */
  di->queue = dq->next;
  if (!di->queue)
    di->queue_end = 0;
  dq->next = di->free_list;
  di->free_list = dq;

  /* Print the node index.  */
  dump_index (di, index);
  /* And the type of node this is.  */
  if (dni->binfo_p)
    code_name = "binfo";
  else
    code_name = tree_code_name[(int) TREE_CODE (t)];
  fprintf (di->stream, "%-16s ", code_name);
  astTreeBuilder->addNode(index,code_name);
  //fprintf 
  di->column = 25;

  /* Figure out what kind of node this is.  */
  code = TREE_CODE (t);
  code_class = TREE_CODE_CLASS (code);

  /* Although BINFOs are TREE_VECs, we dump them specially so as to be
     more informative.  */
  if (dni->binfo_p)
    {
      unsigned ix;
      tree base;
      VEC(tree,gc) *accesses = BINFO_BASE_ACCESSES (t);

      dump_child ("type", BINFO_TYPE (t));
      //astTreeBuilder->addChild("type",)
      if (BINFO_VIRTUAL_P (t))
	gcc_dump_string_field (di, "spec", "virt");

      gcc_dump_int (di, "bases", BINFO_N_BASE_BINFOS (t));
      for (ix = 0; BINFO_BASE_ITERATE (t, ix, base); ix++)
	{
	  tree access = (accesses ? VEC_index (tree, accesses, ix)
			 : access_public_node);
	  const char *string = NULL;

	  if (access == access_public_node)
	    string = "pub";
	  else if (access == access_protected_node)
	    string = "prot";
	  else if (access == access_private_node)
	    string = "priv";
	  else
	    gcc_unreachable ();

	  gcc_dump_string_field (di, "accs", string);
	  gcc_queue_and_dump_index (di, "binf", base, DUMP_BINFO);
	}

      goto done;
    }

  /* We can knock off a bunch of expression nodes in exactly the same
     way.  */
  if (IS_EXPR_CODE_CLASS (code_class))
    {
      /* If we're dumping children, dump them now.  */
      gcc_queue_and_dump_type (di, t);

      switch (code_class)
	{
	case tcc_unary:
	  dump_child ("op 0", TREE_OPERAND (t, 0));
	  break;

	case tcc_binary:
	case tcc_comparison:
	  dump_child ("op 0", TREE_OPERAND (t, 0));
	  dump_child ("op 1", TREE_OPERAND (t, 1));
	  break;

	case tcc_expression:
	case tcc_reference:
	case tcc_statement:
	case tcc_vl_exp:
	  /* These nodes are handled explicitly below.  */
	  break;

	default:
	  gcc_unreachable ();
	}
    }
  else if (DECL_P (t))
    {
      expanded_location xloc;
      /* All declarations have names.  */
      if (DECL_NAME (t))
	dump_child ("name", DECL_NAME (t));
      if (DECL_ASSEMBLER_NAME_SET_P (t)
	  && DECL_ASSEMBLER_NAME (t) != DECL_NAME (t))
	dump_child ("mngl", DECL_ASSEMBLER_NAME (t));
      if (DECL_ABSTRACT_ORIGIN (t))
        dump_child ("orig", DECL_ABSTRACT_ORIGIN (t));
      /* And types.  */
      gcc_queue_and_dump_type (di, t);
      dump_child ("scpe", DECL_CONTEXT (t));
      /* And a source position.  */
      xloc = expand_location (DECL_SOURCE_LOCATION (t));
      if (xloc.file)
	{
	  const char *filename = strrchr (xloc.file, '/');
	  if (!filename)
	    filename = xloc.file;
	  else
	    /* Skip the slash.  */
	    ++filename;

	  dump_maybe_newline (di);
	  fprintf (di->stream, "srcp: %s:%-6d ", filename,
		   xloc.line);
	  char buf[32];
	  sprintf(buf,"%s:%-6d",filename,xloc.line);
	  astTreeBuilder->addStringProperty("srcp",buf);
	  di->column += 6 + strlen (filename) + 8;
	}
      /* And any declaration can be compiler-generated.  */
      if (CODE_CONTAINS_STRUCT (TREE_CODE (t), TS_DECL_COMMON)
	  && DECL_ARTIFICIAL (t))
	gcc_dump_string_field (di, "note", "artificial");
      if (DECL_CHAIN (t) && !dump_flag (di, TDF_SLIM, NULL))
	dump_child ("chain", DECL_CHAIN (t));
    }
  else if (code_class == tcc_type)
    {
      /* All types have qualifiers.  */
      int quals = lang_hooks.tree_dump.type_quals (t);

      if (quals != TYPE_UNQUALIFIED)
	{
	  fprintf (di->stream, "qual: %c%c%c     ",
		   (quals & TYPE_QUAL_CONST) ? 'c' : ' ',
		   (quals & TYPE_QUAL_VOLATILE) ? 'v' : ' ',
		   (quals & TYPE_QUAL_RESTRICT) ? 'r' : ' ');
	  char buf[32];
	  sprintf (buf, "%c%c%c     ",
		   (quals & TYPE_QUAL_CONST) ? 'c' : ' ',
		   (quals & TYPE_QUAL_VOLATILE) ? 'v' : ' ',
		   (quals & TYPE_QUAL_RESTRICT) ? 'r' : ' ');
	  astTreeBuilder->addStringProperty("qual",buf);
	  di->column += 14;
	}

      /* All types have associated declarations.  */
      dump_child ("name", TYPE_NAME (t));

      /* All types have a main variant.  */
      if (TYPE_MAIN_VARIANT (t) != t)
	dump_child ("unql", TYPE_MAIN_VARIANT (t));

      /* And sizes.  */
      dump_child ("size", TYPE_SIZE (t));

      /* All types have alignments.  */
      gcc_dump_int (di, "algn", TYPE_ALIGN (t));
    }
  else if (code_class == tcc_constant)
    /* All constants can have types.  */
    gcc_queue_and_dump_type (di, t);
    if(TREE_CODE(t)==DECL_EXPR){
        //printf("found decl_expr\n");
        dump_child ("op 0", DECL_EXPR_DECL (t));
    }
  /* Give the language-specific code a chance to print something.  If
     it's completely taken care of things, don't bother printing
     anything more ourselves.  */
  //if (lang_hooks.tree_dump.dump_tree (di, t))
   // goto done;
    if (gcc_cp_dump_tree(di,t))
    	goto done;
  /* Now handle the various kinds of nodes.  */
  switch (code)
    {
      int i;

    case IDENTIFIER_NODE:
      gcc_dump_string_field (di, "strg", IDENTIFIER_POINTER (t));
      gcc_dump_int (di, "lngt", IDENTIFIER_LENGTH (t));
      break;

    case TREE_LIST:
      dump_child ("purp", TREE_PURPOSE (t));
      dump_child ("valu", TREE_VALUE (t));
      dump_child ("chan", TREE_CHAIN (t));
      break;

    case STATEMENT_LIST:
      {
	tree_stmt_iterator it;
	for (i = 0, it = tsi_start (t); !tsi_end_p (it); tsi_next (&it), i++)
	  {
	    char buffer[32];
	    sprintf (buffer, "%u", i);
	    dump_child (buffer, tsi_stmt (it));
	  }
      }
      break;

    case TREE_VEC:
      gcc_dump_int (di, "lngt", TREE_VEC_LENGTH (t));
      for (i = 0; i < TREE_VEC_LENGTH (t); ++i)
	{
	  char buffer[32];
	  sprintf (buffer, "%u", i);
	  dump_child (buffer, TREE_VEC_ELT (t, i));
	}
      break;

    case INTEGER_TYPE:
    case ENUMERAL_TYPE:
      gcc_dump_int (di, "prec", TYPE_PRECISION (t));
      gcc_dump_string_field (di, "sign", TYPE_UNSIGNED (t) ? "unsigned": "signed");
      dump_child ("min", TYPE_MIN_VALUE (t));
      dump_child ("max", TYPE_MAX_VALUE (t));

      if (code == ENUMERAL_TYPE)
	dump_child ("csts", TYPE_VALUES (t));
      break;

    case REAL_TYPE:
      dump_int (di, "prec", TYPE_PRECISION (t));
      break;

    case FIXED_POINT_TYPE:
      gcc_dump_int (di, "prec", TYPE_PRECISION (t));
      gcc_dump_string_field (di, "sign", TYPE_UNSIGNED (t) ? "unsigned": "signed");
      gcc_dump_string_field (di, "saturating",
			 TYPE_SATURATING (t) ? "saturating": "non-saturating");
      break;

    case POINTER_TYPE:
      dump_child ("ptd", TREE_TYPE (t));
      break;

    case REFERENCE_TYPE:
      dump_child ("refd", TREE_TYPE (t));
      break;
    case METHOD_TYPE:
      dump_child ("clas", TYPE_METHOD_BASETYPE (t));
      /* Fall through.  */

    case FUNCTION_TYPE:
      dump_child ("retn", TREE_TYPE (t));
      dump_child ("prms", TYPE_ARG_TYPES (t));
      break;

    case ARRAY_TYPE:
      dump_child ("elts", TREE_TYPE (t));
      dump_child ("domn", TYPE_DOMAIN (t));
      break;

    case RECORD_TYPE:
    case UNION_TYPE:
      if (TREE_CODE (t) == RECORD_TYPE)
	gcc_dump_string_field (di, "tag", "struct");
      else
	gcc_dump_string_field (di, "tag", "union");

      dump_child ("flds", TYPE_FIELDS (t));
      //add debug chain
      //dump_child ("fldschan",TREE_CHAIN(TYPE_FIELDS(t)));
      dump_child ("fncs", TYPE_METHODS (t));
      gcc_queue_and_dump_index (di, "binf", TYPE_BINFO (t),
			    DUMP_BINFO);
      break;

    case CONST_DECL:
      dump_child ("cnst", DECL_INITIAL (t));
      break;

    case DEBUG_EXPR_DECL:
      dump_int (di, "-uid", DEBUG_TEMP_UID (t));
      /* Fall through.  */
    //debug
    //case DECL_EXPR:
      //if(DECL_EXPR_DECL (t) != NULL){
        //dump_child ("decl_obj", DECL_EXPR_DECL (t));
      //}
    case VAR_DECL:
    case PARM_DECL:
    case FIELD_DECL:
    case RESULT_DECL:
      if (TREE_CODE (t) == PARM_DECL)
	dump_child ("argt", DECL_ARG_TYPE (t));
      else
	dump_child ("init", DECL_INITIAL (t));
      dump_child ("size", DECL_SIZE (t));
      gcc_dump_int (di, "algn", DECL_ALIGN (t));

      if (TREE_CODE (t) == FIELD_DECL)
	{
      //dump_child ("chan",TREE_CHAIN(t));
	  if (DECL_FIELD_OFFSET (t))
	    dump_child ("bpos", bit_position (t));
      dump_child ("chan",TREE_CHAIN(t));

        //dump_child ("chan",TREE_CHAIN(t));

	}else if (TREE_CODE(t) == VAR_DECL)
    {
      //if (DECL_FIELD_OFFSET (t))
	    //dump_child ("bpos", bit_position (t));
      //dump_child ("chan",TREE_CHAIN(t));
	  //temporary solution: if scope is of struct_decl I chain it up
      tree decl_context = DECL_CONTEXT(t);
      if (decl_context && TREE_CODE(DECL_CONTEXT(t))==RECORD_TYPE)
      {
        dump_child ("chan",TREE_CHAIN(t));
      }
        gcc_dump_int (di, "used", TREE_USED (t));
      //dump_child ("chan",TREE_CHAIN(t));
	  if (DECL_REGISTER (t))
	    gcc_dump_string_field (di, "spec", "register");

    }
      //debug add
      //dump_child ("chan",next);
      else if (TREE_CODE (t) == PARM_DECL)
	{
	  gcc_dump_int (di, "used", TREE_USED (t));
      //dump_child ("chan",TREE_CHAIN(t));
	  if (DECL_REGISTER (t))
	    gcc_dump_string_field (di, "spec", "register");
	}
      break;
    case FUNCTION_DECL:
      dump_child ("args", DECL_ARGUMENTS (t));
      if (DECL_EXTERNAL (t))
	gcc_dump_string_field (di, "body", "undefined");
      if (TREE_PUBLIC (t))
	gcc_dump_string_field (di, "link", "extern");
      else
	gcc_dump_string_field (di, "link", "static");
      if (DECL_SAVED_TREE (t) && !dump_flag (di, TDF_SLIM, t))
	dump_child ("body", DECL_SAVED_TREE (t));
      break;

    case INTEGER_CST:
      if (TREE_INT_CST_HIGH (t))
	dump_int (di, "high", TREE_INT_CST_HIGH (t));
      gcc_dump_int (di, "low", TREE_INT_CST_LOW (t));
      break;

    case STRING_CST:
      fprintf (di->stream, "strg: %-7s ", TREE_STRING_POINTER (t));
      gcc_dump_int (di, "lngt", TREE_STRING_LENGTH (t));
      break;

    case REAL_CST:
      gcc_dump_real (di, "valu", TREE_REAL_CST_PTR (t));
      break;

    case FIXED_CST:
      gcc_dump_fixed (di, "valu", TREE_FIXED_CST_PTR (t));
      break;

    case TRUTH_NOT_EXPR:
    case ADDR_EXPR:
    case INDIRECT_REF:
    case CLEANUP_POINT_EXPR:
    case SAVE_EXPR:
    case REALPART_EXPR:
    case IMAGPART_EXPR:
      /* These nodes are unary, but do not have code class `1'.  */
      dump_child ("op 0", TREE_OPERAND (t, 0));
      break;

    case TRUTH_ANDIF_EXPR:
    case TRUTH_ORIF_EXPR:
    case INIT_EXPR:
    case MODIFY_EXPR:
    case COMPOUND_EXPR:
    case PREDECREMENT_EXPR:
    case PREINCREMENT_EXPR:
    case POSTDECREMENT_EXPR:
    case POSTINCREMENT_EXPR:
      /* These nodes are binary, but do not have code class `2'.  */
      dump_child ("op 0", TREE_OPERAND (t, 0));
      dump_child ("op 1", TREE_OPERAND (t, 1));
      break;

    case COMPONENT_REF:
      dump_child ("op 0", TREE_OPERAND (t, 0));
      dump_child ("op 1", TREE_OPERAND (t, 1));
      dump_child ("op 2", TREE_OPERAND (t, 2));
      break;

    case ARRAY_REF:
    case ARRAY_RANGE_REF:
      dump_child ("op 0", TREE_OPERAND (t, 0));
      dump_child ("op 1", TREE_OPERAND (t, 1));
      dump_child ("op 2", TREE_OPERAND (t, 2));
      dump_child ("op 3", TREE_OPERAND (t, 3));
      break;

    case COND_EXPR:
      gcc_dump_stmt (di, t);
      dump_child ("op 0", TREE_OPERAND (t, 0));
      dump_child ("op 1", TREE_OPERAND (t, 1));
      dump_child ("op 2", TREE_OPERAND (t, 2));
      break;

    case TRY_FINALLY_EXPR:
      dump_child ("op 0", TREE_OPERAND (t, 0));
      dump_child ("op 1", TREE_OPERAND (t, 1));
      break;

    case CALL_EXPR:
      {
	int i = 0;
	tree arg;
	call_expr_arg_iterator iter;
	dump_child ("fn", CALL_EXPR_FN (t));
	FOR_EACH_CALL_EXPR_ARG (arg, iter, t)
	  {
	    char buffer[32];
	    sprintf (buffer, "%u", i);
	    dump_child (buffer, arg);
	    i++;
	  }
      }
      break;

    case CONSTRUCTOR:
      {
	unsigned HOST_WIDE_INT cnt;
	tree index, value;
	gcc_dump_int (di, "lngt", VEC_length (constructor_elt,
					  CONSTRUCTOR_ELTS (t)));
	FOR_EACH_CONSTRUCTOR_ELT (CONSTRUCTOR_ELTS (t), cnt, index, value)
	  {
	    dump_child ("idx", index);
	    dump_child ("val", value);
	  }
      }
      break;

    case BIND_EXPR:
      gcc_dump_stmt (di, t);
      dump_child ("vars", TREE_OPERAND (t, 0));
      dump_child ("body", TREE_OPERAND (t, 1));
      break;

    case LOOP_EXPR:
      dump_child ("body", TREE_OPERAND (t, 0));
      break;

    case EXIT_EXPR:
      dump_child ("cond", TREE_OPERAND (t, 0));
      break;

    case RETURN_EXPR:
      dump_child ("expr", TREE_OPERAND (t, 0));
      break;

    case TARGET_EXPR:
      dump_child ("decl", TREE_OPERAND (t, 0));
      dump_child ("init", TREE_OPERAND (t, 1));
      dump_child ("clnp", TREE_OPERAND (t, 2));
      /* There really are two possible places the initializer can be.
	 After RTL expansion, the second operand is moved to the
	 position of the fourth operand, and the second operand
	 becomes NULL.  */
      dump_child ("init", TREE_OPERAND (t, 3));
      break;

    case CASE_LABEL_EXPR:
      dump_child ("name", CASE_LABEL (t));
      if (CASE_LOW (t))
	{
	  dump_child ("low ", CASE_LOW (t));
	  if (CASE_HIGH (t))
	    dump_child ("high", CASE_HIGH (t));
	}
      break;
    case LABEL_EXPR:
      dump_child ("name", TREE_OPERAND (t,0));
      break;
    case GOTO_EXPR:
      dump_child ("labl", TREE_OPERAND (t, 0));
      break;
    case SWITCH_EXPR:
      dump_child ("cond", TREE_OPERAND (t, 0));
      dump_child ("body", TREE_OPERAND (t, 1));
      if (TREE_OPERAND (t, 2))
        {
      	  dump_child ("labl", TREE_OPERAND (t,2));
        }
      break;
    case OMP_CLAUSE:
      {
	int i;
	fprintf (di->stream, "%s\n", omp_clause_code_name[OMP_CLAUSE_CODE (t)]);
	for (i = 0; i < omp_clause_num_ops[OMP_CLAUSE_CODE (t)]; i++)
	  dump_child ("op: ", OMP_CLAUSE_OPERAND (t, i));
      }
      break;
    default:
      /* There are no additional fields to print.  */
      break;
    }

 done:
  if (dump_flag (di, TDF_ADDRESS, NULL))
    gcc_dump_pointer (di, "addr", (void *)t);

  /* Terminate the line.  */
  fprintf (di->stream, "\n");
}

/* Return nonzero if FLAG has been specified for the dump, and NODE
   is not the root node of the dump.  */

int dump_flag (dump_info_p di, int flag, const_tree node)
{
  return (di->flags & flag) && (node != di->node);
}

/* Dump T, and all its children, on STREAM.  */

void
gcc_dump_node (const_tree t, int flags, FILE *stream)
{
  struct dump_info di;
  dump_queue_p dq;
  dump_queue_p next_dq;

  /* Initialize the dump-information structure.  */
  di.stream = stream;
  di.index = 0;
  di.column = 0;
  di.queue = 0;
  di.queue_end = 0;
  di.free_list = 0;
  di.flags = flags;
  di.node = t;
  di.nodes = splay_tree_new (splay_tree_compare_pointers, 0,
			     (splay_tree_delete_value_fn) &free);

  /* Queue up the first node.  */
  queue (&di, t, DUMP_NONE);

  /* Until the queue is empty, keep dumping nodes.  */
  while (di.queue)
    dequeue_and_dump (&di);

  /* Now, clean up.  */
  for (dq = di.free_list; dq; dq = next_dq)
    {
      next_dq = dq->next;
      free (dq);
    }
  splay_tree_delete (di.nodes);
}


/* Table of tree dump switches. This must be consistent with the
   tree_dump_index enumeration in tree-pass.h.  */
static struct dump_file_info dump_files[TDI_end] =
{
  {NULL, NULL, NULL, 0, 0, 0},
  {".cgraph", "ipa-cgraph", NULL, TDF_IPA, 0,  0},
  {".tu", "translation-unit", NULL, TDF_TREE, 0, 1},
  {".class", "class-hierarchy", NULL, TDF_TREE, 0, 2},
  {".original", "tree-original", NULL, TDF_TREE, 0, 3},
  {".gimple", "tree-gimple", NULL, TDF_TREE, 0, 4},
  {".nested", "tree-nested", NULL, TDF_TREE, 0, 5},
  {".vcg", "tree-vcg", NULL, TDF_TREE, 0, 6},
  {".ads", "ada-spec", NULL, 0, 0, 7},
#define FIRST_AUTO_NUMBERED_DUMP 8

  {NULL, "tree-all", NULL, TDF_TREE, 0, 0},
  {NULL, "rtl-all", NULL, TDF_RTL, 0, 0},
  {NULL, "ipa-all", NULL, TDF_IPA, 0, 0},
};

/* Dynamically registered tree dump files and switches.  */
static struct dump_file_info *extra_dump_files;
static size_t extra_dump_files_in_use;
static size_t extra_dump_files_alloced;

/* Define a name->number mapping for a dump flag value.  */
struct dump_option_value_info
{
  const char *const name;	/* the name of the value */
  const int value;		/* the value of the name */
};

/* Table of dump options. This must be consistent with the TDF_* flags
   in tree.h */
static const struct dump_option_value_info dump_options[] =
{
  {"address", TDF_ADDRESS},
  {"asmname", TDF_ASMNAME},
  {"slim", TDF_SLIM},
  {"raw", TDF_RAW},
  {"graph", TDF_GRAPH},
  {"details", TDF_DETAILS},
  {"cselib", TDF_CSELIB},
  {"stats", TDF_STATS},
  {"blocks", TDF_BLOCKS},
  {"vops", TDF_VOPS},
  {"lineno", TDF_LINENO},
  {"uid", TDF_UID},
  {"stmtaddr", TDF_STMTADDR},
  {"memsyms", TDF_MEMSYMS},
  {"verbose", TDF_VERBOSE},
  {"eh", TDF_EH},
  {"alias", TDF_ALIAS},
  {"nouid", TDF_NOUID},
  {"enumerate_locals", TDF_ENUMERATE_LOCALS},
  {"all", ~(TDF_RAW | TDF_SLIM | TDF_LINENO | TDF_TREE | TDF_RTL | TDF_IPA
	    | TDF_STMTADDR | TDF_GRAPH | TDF_DIAGNOSTIC | TDF_VERBOSE
	    | TDF_RHS_ONLY | TDF_NOUID | TDF_ENUMERATE_LOCALS)},
  {NULL, 0}
};

unsigned int
dump_register (const char *suffix, const char *swtch, const char *glob,
	       int flags)
{
  static int next_dump = FIRST_AUTO_NUMBERED_DUMP;
  int num = next_dump++;

  size_t count = extra_dump_files_in_use++;

  if (count >= extra_dump_files_alloced)
    {
      if (extra_dump_files_alloced == 0)
	extra_dump_files_alloced = 32;
      else
	extra_dump_files_alloced *= 2;
      extra_dump_files = XRESIZEVEC (struct dump_file_info,
				     extra_dump_files,
				     extra_dump_files_alloced);
    }

  memset (&extra_dump_files[count], 0, sizeof (struct dump_file_info));
  extra_dump_files[count].suffix = suffix;
  extra_dump_files[count].swtch = swtch;
  extra_dump_files[count].glob = glob;
  extra_dump_files[count].flags = flags;
  extra_dump_files[count].num = num;

  return count + TDI_end;
}


/* Return the dump_file_info for the given phase.  */

struct dump_file_info *
get_dump_file_info (int phase)
{
  if (phase < TDI_end)
    return &dump_files[phase];
  else if ((size_t) (phase - TDI_end) >= extra_dump_files_in_use)
    return NULL;
  else
    return extra_dump_files + (phase - TDI_end);
}


/* Return the name of the dump file for the given phase.
   If the dump is not enabled, returns NULL.  */

char *
get_dump_file_name (int phase)
{
  char dump_id[10];
  struct dump_file_info *dfi;

  if (phase == TDI_none)
    return NULL;

  dfi = get_dump_file_info (phase);
  if (dfi->state == 0)
    return NULL;

  if (dfi->num < 0)
    dump_id[0] = '\0';
  else
    {
      char suffix;
      if (dfi->flags & TDF_TREE)
	suffix = 't';
      else if (dfi->flags & TDF_IPA)
	suffix = 'i';
      else
	suffix = 'r';

      if (snprintf (dump_id, sizeof (dump_id), ".%03d%c", dfi->num, suffix) < 0)
	dump_id[0] = '\0';
    }

  return concat (dump_base_name, dump_id, dfi->suffix, NULL);
}

/* Begin a tree dump for PHASE. Stores any user supplied flag in
   *FLAG_PTR and returns a stream to write to. If the dump is not
   enabled, returns NULL.
   Multiple calls will reopen and append to the dump file.  */

FILE *
_my_dump_begin (int phase, int *flag_ptr)
{
  char *name;
  struct dump_file_info *dfi;
  FILE *stream;

  if (phase == TDI_none || !dump_enabled_p (phase))
    return NULL;

  name = get_dump_file_name (phase);
  printf("%s\n", name);
  dfi = get_dump_file_info (phase);
  stream = fopen (name, dfi->state < 0 ? "w" : "a");
  if (!stream)
    error ("could not open dump file %qs: %m", name);
  else
    dfi->state = 1;
  free (name);

  if (flag_ptr)
    *flag_ptr = dfi->flags;

  return stream;
}

/* Returns nonzero if tree dump PHASE is enabled.  If PHASE is
   TDI_tree_all, return nonzero if any dump is enabled.  */

int
dump_enabled_p (int phase)
{
  if (phase == TDI_tree_all)
    {
      size_t i;
      for (i = TDI_none + 1; i < (size_t) TDI_end; i++)
	if (dump_files[i].state)
	  return 1;
      for (i = 0; i < extra_dump_files_in_use; i++)
	if (extra_dump_files[i].state)
	  return 1;
      return 0;
    }
  else
    {
      struct dump_file_info *dfi = get_dump_file_info (phase);
      return dfi->state;
    }
}

/* Returns nonzero if tree dump PHASE has been initialized.  */

int
dump_initialized_p (int phase)
{
  struct dump_file_info *dfi = get_dump_file_info (phase);
  return dfi->state > 0;
}

/* Returns the switch name of PHASE.  */

const char *
dump_flag_name (int phase)
{
  struct dump_file_info *dfi = get_dump_file_info (phase);
  return dfi->swtch;
}

/* Finish a tree dump for PHASE. STREAM is the stream created by
   dump_begin.  */

void
dump_end (int phase ATTRIBUTE_UNUSED, FILE *stream)
{
  fclose (stream);
}

/* Enable all tree dumps.  Return number of enabled tree dumps.  */

static int
dump_enable_all (int flags)
{
  int ir_dump_type = (flags & (TDF_TREE | TDF_RTL | TDF_IPA));
  int n = 0;
  size_t i;

  for (i = TDI_none + 1; i < (size_t) TDI_end; i++)
    if ((dump_files[i].flags & ir_dump_type))
      {
        dump_files[i].state = -1;
        dump_files[i].flags |= flags;
        n++;
      }

  for (i = 0; i < extra_dump_files_in_use; i++)
    if ((extra_dump_files[i].flags & ir_dump_type))
      {
        extra_dump_files[i].state = -1;
        extra_dump_files[i].flags |= flags;
	n++;
      }

  return n;
}

/* Parse ARG as a dump switch. Return nonzero if it is, and store the
   relevant details in the dump_files array.  */

static int
dump_switch_p_1 (const char *arg, struct dump_file_info *dfi, bool doglob)
{
  const char *option_value;
  const char *ptr;
  int flags;

  if (doglob && !dfi->glob)
    return 0;

  option_value = skip_leading_substring (arg, doglob ? dfi->glob : dfi->swtch);
  if (!option_value)
    return 0;

  if (*option_value && *option_value != '-')
    return 0;

  ptr = option_value;
  flags = 0;

  while (*ptr)
    {
      const struct dump_option_value_info *option_ptr;
      const char *end_ptr;
      unsigned length;

      while (*ptr == '-')
	ptr++;
      end_ptr = strchr (ptr, '-');
      if (!end_ptr)
	end_ptr = ptr + strlen (ptr);
      length = end_ptr - ptr;

      for (option_ptr = dump_options; option_ptr->name; option_ptr++)
	if (strlen (option_ptr->name) == length
	    && !memcmp (option_ptr->name, ptr, length))
	  {
	    flags |= option_ptr->value;
	    goto found;
	  }
      warning (0, "ignoring unknown option %q.*s in %<-fdump-%s%>",
	       length, ptr, dfi->swtch);
    found:;
      ptr = end_ptr;
    }

  dfi->state = -1;
  dfi->flags |= flags;

  /* Process -fdump-tree-all and -fdump-rtl-all, by enabling all the
     known dumps.  */
  if (dfi->suffix == NULL)
    dump_enable_all (dfi->flags);

  return 1;
}

int
dump_switch_p (const char *arg)
{
  size_t i;
  int any = 0;

  for (i = TDI_none + 1; i != TDI_end; i++)
    any |= dump_switch_p_1 (arg, &dump_files[i], false);

  /* Don't glob if we got a hit already */
  if (!any)
    for (i = TDI_none + 1; i != TDI_end; i++)
      any |= dump_switch_p_1 (arg, &dump_files[i], true);

  for (i = 0; i < extra_dump_files_in_use; i++)
    any |= dump_switch_p_1 (arg, &extra_dump_files[i], false);

  if (!any)
    for (i = 0; i < extra_dump_files_in_use; i++)
      any |= dump_switch_p_1 (arg, &extra_dump_files[i], true);


  return any;
}

/* Dump FUNCTION_DECL FN as tree dump PHASE.  */

void
dump_function (int phase, tree fn)
{
  FILE *stream;
  int flags;

  stream = (FILE*)gcc_dump_begin (phase, &flags);
  if (stream)
    {
      dump_function_to_file (fn, stream, flags);
      dump_end (phase, stream);
    }
}

bool
enable_rtl_dump_file (void)
{
  return dump_enable_all (TDF_RTL | TDF_DETAILS | TDF_BLOCKS) > 0;
}
