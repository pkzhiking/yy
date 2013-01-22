import os
import os.path
import sys
import fileinput
cur_dir = os.getcwd()
project_dir = cur_dir[0:cur_dir.rfind('/')+1]
pre_dir = project_dir + "plugin-dev-pre/"
print pre_dir
proj1_dir = project_dir + "plugin-dev/"
proj2_dir = project_dir + "plugin-dev2/"
html_source_dir = proj1_dir + "test"
html_target_dir ="/home/damon/Software/tomcat6/webapps"
print html_target_dir
rm_intemediate_files_stmt = "rm -rf " + proj2_dir + "test/*"
print rm_intemediate_files_stmt
os.system(rm_intemediate_files_stmt)
exec_pre_stmt = pre_dir + "main " + sys.argv[1]
print exec_pre_stmt
os.system(exec_pre_stmt)
init_file = pre_dir + "files"
file = open(init_file)
allfiles = ""
while 1:
    line = file.readline()
    if not line:
	break
    else:
	if line[-1] == '\n':
	    line = line[0: -1]
	#allfiles += line + " "
	exec_proj1_stmt = "make run -k \"INPUT_FILE=" + line + "\""
	print exec_proj1_stmt
	os.system(exec_proj1_stmt)
#exec_proj1_stmt = "make run -k \"INPUT_FILES=" + allfiles + "\""
#print exec_proj1_stmt
#os.system(exec_proj1_stmt)
exec_proj2_stmt = proj2_dir + "binary/main " + sys.argv[1]
print exec_proj2_stmt
os.system(exec_proj2_stmt)
html_target_dir ="/home/damon/Software/tomcat6/webapps"+ "/huawei/result"
move_html_stmt = "python movehtml.py " + html_source_dir + ' ' + html_target_dir + " " + sys.argv[1]
print move_html_stmt
os.system(move_html_stmt)
