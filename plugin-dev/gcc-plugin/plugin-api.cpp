/*
 *  plugin-api.h
 *
 *  Created on: Apr 2, 2012
 *      Author: Guo Jiuliang
 *  Description: Using GCC plugin interface as entry point of our analyzer
 */

extern "C"
{
	int plugin_is_GPL_compatible;
}
#include "gcc-plugin.h"
#include "plugin-version.h"
#include "ApplicationDelegate.h"
#include "ASTTreeBuilder.h"
#include <iostream>
using std::endl;
using std::cerr;
//api for building tree object
extern "C" struct ASTTreeBuilder *astTreeBuilder;
extern "C" void walker_init();
extern "C"
{
	int plugin_init(struct plugin_name_args*,struct plugin_gcc_version*);
	void plugin_callback(void*,void*);
}

int plugin_init(struct plugin_name_args* plugin_info,
				struct plugin_gcc_version* version)
{
	if(!plugin_default_version_check(version,&gcc_version))
		{
			cerr<<"Plugin version is not compatible with gcc version."<<endl;
			return 1;
		}
	register_callback(plugin_info->base_name,PLUGIN_PRE_GENERICIZE,plugin_callback,NULL);
	astTreeBuilder->init = walker_init;
	astTreeBuilder->init();
	return 0;
}
void plugin_callback(void* gcc_data,void* user_data)
{
	//delegate start to function
	ApplicationDelegate app(gcc_data,user_data);
	app.run();
}

