import os
import sys
import getpass

print "use last configure, yes or no?"
use_last_config = raw_input()

#if the configure is changed, configure it again, and store the configure into file ".default_config"
if("no" == use_last_config): 
    print "please input gcc install dir"
    gcc_install_dir = raw_input()

    print "please input downloaded gcc dir"
    gcc_dl_dir = raw_input()

    print "please input gmp install dir"
    gmp_install_dir = raw_input()

    print "please input tomcat dir"
    tomcat = raw_input()

    print "please input the database server ip address"
    ip = raw_input()

    print ip

    print "please input mysql user name"
    user = raw_input()

    print "please input password of user"
    password = getpass.getpass()

    print "please input related database name"
    database = raw_input()
    f = open(".default_config", 'wb')
    str = gcc_install_dir + "\n" + gcc_dl_dir + "\n" + gmp_install_dir + "\n" + tomcat + "\n" + ip + "\n" + user + "\n" + password + "\n" + database
    f.write(str)
    f.close()
    pass

#if configure is not changed since last time, use the default config from file ".default_config"
else:
    f = open(".default_config", 'r')
    gcc_install_dir = f.readline()
    gcc_install_dir = gcc_install_dir.rstrip('\n')
    gcc_dl_dir = f.readline()
    gcc_dl_dir = gcc_dl_dir.rstrip('\n')
    gmp_install_dir = f.readline()
    gmp_install_dir = gmp_install_dir.rstrip('\n')
    tomcat = f.readline()
    tomcat = tomcat.rstrip('\n')
    ip = f.readline()
    ip = ip.rstrip('\n')
    user = f.readline()
    user = user.rstrip('\n')
    password = f.readline()
    password = password.rstrip('\n')
    database = f.readline()
    database = password.rstrip('\n')
    f.close()
    pass

tomcat_dir = tomcat.replace('/', '\/')

gcc_install_dir = gcc_install_dir.replace('/','\/')
gcc_dl_dir = gcc_dl_dir.replace('/', '\/')
gmp_install_dir = gmp_install_dir.replace('/', '\/')

change_make_file = "change_makefile"
makefile_target_file = "plugin-dev/Makefile"
makefile_temp_file = "plugin-dev/Makefile.temp"

f = open(change_make_file, 'wb')
change_makefile_sed = "s/GCCINSTALL_DIR =[\.\/$a-zA-Z0-9\"$-]*/GCCINSTALL_DIR =" + gcc_install_dir + "/g\n" +"s/GCCDOWNLOAD_DIR =[-\/\ a-zA-Z0-9\"$\t\.]*/GCCDOWNLOAD_DIR =" + gcc_dl_dir + "/g\n" + "s/GMPINSTALL_DIR =[-\/-\ a-zA-Z0-9\"$\t\.]*/GMPINSTALL_DIR =" + gmp_install_dir + "/g\n"
print change_makefile_sed
f.write(change_makefile_sed)
f.close()

exec_change_makefile_cmd = "sed -f " + change_make_file + " " + makefile_target_file + " > " + makefile_temp_file
print exec_change_makefile_cmd
os.system(exec_change_makefile_cmd)
exec_cp_makefile_cmd = "cp " + makefile_temp_file + " " + makefile_target_file
print exec_cp_makefile_cmd
os.system(exec_cp_makefile_cmd)

rm_change_make_file_cmd = "rm " + change_make_file
print rm_change_make_file_cmd
os.system(rm_change_make_file_cmd)

rm_makefile_temp_file_cmd = "rm " + makefile_temp_file
print rm_makefile_temp_file_cmd
os.system(rm_makefile_temp_file_cmd)

change_config_file = "change_config"
config_file = "config.h"
temp_config_file = "temp_config"
cur_dir = os.getcwd()
current_dir = cur_dir.replace('/', '\/')
f = open(change_config_file, 'wb')
change_config_sed = "s/define CURRENT_PROJS_DIR[-\"\ a-zA-Z0-9$\/]*/define CURRENT_PROJS_DIR " + "\"" + current_dir + "\"/g\n" + "s/USER [-\/\ a-zA-Z0-9\"$\t\.]*/USER \"" + user + "\"/g\n" + "s/PASSWORD [-\"\ a-zA-Z0-9$\/]*/PASSWORD \"" + password + "\"/g\n" + "s/DATABASE [-\/\ a-zA-Z0-9\"$\t\.]*/DATABASE \"" + database + "\"/g\n" + "s/IP [\.0-9a-zA-Z\"$\/\ ]*/IP \"" + ip + "\"/g"
print change_config_sed
f.write(change_config_sed)
f.close()

exec_change_config_cmd = "sed -f " + change_config_file + " " + config_file + " > " + temp_config_file
print exec_change_config_cmd
os.system(exec_change_config_cmd)
exec_cp_config_cmd = "cp " + temp_config_file + " " + config_file
print exec_cp_config_cmd
os.system(exec_cp_config_cmd)

copy_to_plugin_dev_pre = "cp config.h plugin-dev-pre/include/"
print copy_to_plugin_dev_pre
os.system(copy_to_plugin_dev_pre)
copy_to_plugin_dev = "cp config.h plugin-dev/core/include/Config.h"
print copy_to_plugin_dev
os.system(copy_to_plugin_dev)
copy_to_plugin_dev2 = "cp config.h plugin-dev2/core/include/"
print copy_to_plugin_dev2
os.system(copy_to_plugin_dev2)

rm_change_config_file_cmd = "rm " + change_config_file
print rm_change_config_file_cmd
os.system(rm_change_config_file_cmd)

rm_temp_config_file = "rm " + temp_config_file
print rm_temp_config_file
os.system(rm_temp_config_file)

tomcat_dir = tomcat_dir + "\/webapps"
exec_py_file = "plugin-dev/exec-plugin-dev.py"
change_html_target_file = "change_html_target"
temp_html_target_file = "temp_html"
f = open(change_html_target_file, 'wb')
change_html_target_sed = "s/html_target_dir =[-\/\"a-zA-Z0-9$\ \._\_]*/html_target_dir =\"" + tomcat_dir + "\"/g"
print change_html_target_sed
f.write(change_html_target_sed)
f.close()

exec_change_html_cmd = "sed -f " + change_html_target_file + " " + exec_py_file + " > " + temp_html_target_file
print exec_change_html_cmd
os.system(exec_change_html_cmd)
exec_cp_html_cmd = "cp " + temp_html_target_file + " " + exec_py_file
print exec_cp_html_cmd
os.system(exec_cp_html_cmd)

rm_change_html_file_cmd = "rm " + change_html_target_file
print rm_change_html_file_cmd
os.system(rm_change_html_file_cmd)

rm_temp_html_file = "rm " + temp_html_target_file
print rm_temp_html_file
os.system(rm_temp_html_file)

run_sh_file = "plugin-dev/run.sh"
change_run_sh_file = "change_run_sh"
temp_run_sh = "temp_run_sh"
f = open(change_run_sh_file, 'wb')
change_run_sh_sed = "s/cd[-\/a-z0-9A-Z\.$\"\ ]*/cd " + current_dir + "\/plugin-dev/g"
f.write(change_run_sh_sed)
f.close()

exec_change_run_sh_cmd = "sed -f " + change_run_sh_file + " " + run_sh_file + " > " + temp_run_sh
print exec_change_run_sh_cmd
os.system(exec_change_run_sh_cmd)
exec_cp_run_sh_cmd = "cp " + temp_run_sh + " " + run_sh_file
print exec_cp_run_sh_cmd
os.system(exec_cp_run_sh_cmd)

rm_change_run_sh_file_cmd = "rm " + change_run_sh_file
print rm_change_run_sh_file_cmd
os.system(rm_change_run_sh_file_cmd)

rm_temp_run_sh_cmd = "rm " + temp_run_sh
print rm_temp_run_sh_cmd
os.system(rm_temp_run_sh_cmd)

make_cmd = "make"
os.system(make_cmd)

sub_dir = "CPSA_web"
jar_file = "CPSA_web.war"

rmdir_cmd = "rm -rf " + sub_dir
print rmdir_cmd
os.system(rmdir_cmd)

mkdir_cmd = "mkdir " + sub_dir;
print mkdir_cmd
os.system(mkdir_cmd)

current_dir = os.getcwd()
complete_sub_dir = current_dir + "/" + sub_dir
print "chdir to " + complete_sub_dir
os.chdir(complete_sub_dir)

cp_cmd = "cp ../" + jar_file + " ."
print cp_cmd
os.system(cp_cmd)

extract_cmd = "jar -xvf " + jar_file
print extract_cmd
os.system(extract_cmd)

rm_huawei_cmd = "rm " + tomcat + "/webapps/huawei -rf"
print rm_huawei_cmd
os.system(rm_huawei_cmd)


os.chdir(cur_dir)
ls_cmd = "ls"
print ls_cmd
os.system(ls_cmd)
config_xml_file = "CPSA_web/WEB-INF/classes/config.xml"
change_xml_file = "change_xml"
temp_xml_file = "temp_xml"
test_path = current_dir + "/" + "plugin-dev/test"
run_sh_path = current_dir + "/" + "plugin-dev/run.sh"
test_path = test_path.replace('/', '\/')
run_sh_path = run_sh_path.replace('/', '\/')

f = open(change_xml_file, 'wb')
change_config_xml_sed = "s/ip\">[.0-9]*/ip\">" + ip + "/g\n" + "s/database_schema\">[^<]*/database_schema\">" + database + "/g\n"+ "s/db_user\">[^<]*/db_user\">" + user + "/g\n" + "s/db_password\">[^<]*/db_password\">" + password + "/g\n" + "s/test_path\">[^<]*/test_path\">" + test_path + "/g\n" + "s/run_path\">[^<]*/run_path\">" + run_sh_path + "/g\n" + "s/site_url\">[^<]*/site_url\">" + ip + ":8080\/huawei" + "/g"
f.write(change_config_xml_sed)
f.close()

exec_change_xml_sed_cmd = "sed -f " + change_xml_file + " " + config_xml_file + " > "  + temp_xml_file
print exec_change_xml_sed_cmd
os.system(exec_change_xml_sed_cmd)
exec_cp_config_xml_cmd = "cp " + temp_xml_file + " " + config_xml_file
print exec_cp_config_xml_cmd
os.system(exec_cp_config_xml_cmd)

rm_change_xml_file_cmd = "rm " + change_xml_file
print rm_change_xml_file_cmd
os.system(rm_change_xml_file_cmd)

rm_temp_xml_file_cmd = "rm " + temp_xml_file
print rm_temp_xml_file_cmd
os.system(rm_temp_xml_file_cmd)

create_ln_cmd = "ln -s " + complete_sub_dir + " " + tomcat + "/webapps/huawei"
print create_ln_cmd
os.system(create_ln_cmd)
