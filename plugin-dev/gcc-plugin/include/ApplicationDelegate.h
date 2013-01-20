/*
 *  ApplicationDelegate.h
 *
 *  Created on: Apr 2, 2012
 *      Author: Guo Jiuliang
 *  Description: Analyzer delegate interface
 */
#ifndef APPLICATION_DELEGATE_H
#define APPLICATION_DELEGATE_H
class ApplicationDelegate
{
	public:
	ApplicationDelegate(void* gcc_data,void* user_data);
	void run();
	private:
	void* mGCCData;
	void* mUserData;
};
#endif /* APPLICATION_DELEGATE_H */
