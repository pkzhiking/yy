/*
 * inst.h
 *
 *  Created on: Jul 17, 2012
 *      Author: saturnman
 */

#ifndef INST_H_
#define INST_H_
#include "gcc-plugin.h"
#include "system.h"
#include "coretypes.h"
#include "tm.h"
#include "tree.h"
#include "cp-tree.h"
#include "c-family/c-common.h"
#include "c-family/c-objc.h"
#include "tree-inline.h"
#include "tree-mudflap.h"
#include "toplev.h"
#include "flags.h"
#include "output.h"
#include "timevar.h"
#include "diagnostic.h"
#include "cgraph.h"
#include "tree-iterator.h"
#include "vec.h"
#include "target.h"
#include "gimple.h"
#include "bitmap.h"
tree
build_fn_call_stmt(tree fn);
void insert_into_stmt_list(tree stmt,tree* tree_stmt_list);
void build_fn_and_add_to_stmt_list(void* stmt_list_tree,void* fn_tree);
#endif /* INST_H_ */
