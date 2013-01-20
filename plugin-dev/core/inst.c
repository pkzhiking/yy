/*
 * inst.c
 *
 *  Created on: Jul 17, 2012
 *      Author: saturnman
 */
#include "inst.h"
void build_fn_and_add_to_stmt_list(void* stmt_list_tree, void* fn_tree) {
	tree call_stmt = build_fn_call_stmt(fn_tree);
	insert_into_stmt_list(call_stmt, &stmt_list_tree);
}
tree build_fn_call_stmt(tree fn) {
	tree result_tree = NULL_TREE;
	//tree call_expr = build_stmt (location_t loc, enum tree_code code, ...)
	VEC(tree,gc) *args;
	args = make_tree_vector();
	tree call_expr = finish_call_expr(fn, args, 0, 1, 3);
	tree expr_stmt = build_stmt(NULL, EXPR_STMT, call_expr);
	tree cleanup_point_expr =
			build1(CLEANUP_POINT_EXPR,void_type_node,expr_stmt);
	//tree current_stmt_list = find_cur_stmt_list();
	///my_add_stmt(cleanup_point_expr,&current_stmt_list);
	//insert_into_stmt_list(cleanup_point_expr,current_stmt_list);
	return cleanup_point_expr;
}
void insert_into_stmt_list(tree t, tree* stmt_list_p) {
	tree list = *stmt_list_p;
	tree_stmt_iterator i;

	if (!list) {
		if (t && TREE_CODE (t) == STATEMENT_LIST) {
			*stmt_list_p = t;
			return;
		}
		*stmt_list_p = list = alloc_stmt_list();
	}
	if (list->stmt_list.tail==-1){
	        list->stmt_list.tail = 0;
	}
	i = tsi_last(list);
	tsi_link_after(&i, t, TSI_CONTINUE_LINKING);
}

