import os
import os.path
import sys
import re
def do_move(src,dest,sid):
    orig_last_part = src[src.rfind('/')+1:]
    last_part = dest[dest.rfind('/')+1:]
    print 'last part '+last_part
    for root, dirs, files in os.walk(src):
        for f in files:
            fullpath = os.path.join(root, f)
            print '->'+fullpath
            file_name = fullpath[fullpath.rfind('/')+1:]
            copy_dest = dest+fullpath[fullpath.find(orig_last_part)+len(orig_last_part):]
	    dest_dir = copy_dest[:copy_dest.rfind('/')]
	    dest_dir = dest_dir.replace(' ', '_')
	    if(os.path.exists(dest_dir)):
		pass
	    else:
		create_dir = 'mkdir ' + dest_dir
		print 'run command ' + create_dir
		os.system(create_dir)
		pass
            if (fullpath[-8:]=='cpp.html' or full[-6:]=='c.html'):
		if(fullpath[-8:]=='cpp.html'):
		    new_name = 'cpp.' + sid + '.html'
		    new_file = fullpath.replace('(', '\(').replace(')', '\)').replace(' ', '_').replace('cpp.html', new_name)
		if(fullpath[-6:]=='c.html'):
		    new_name = 'c.' + sid + '.html'
		    new_file = fullpath.replace('(', '\(').replace(')', '\)').replace(' ', '_').replace('c.html', new_name)
		new_file = dest + new_file
		print 'new_file: ' + new_file
		move_cmd = 'mv ' + fullpath.replace('(', '\(').replace(')', '\)').replace(' ', '\ ') + ' ' + new_file
		print 'run command ' + move_cmd
		os.system(move_cmd)
		copy_cmd = 'mv ' + new_file + ' ' + dest_dir
                #copy_cmd = 'cp '+fullpath.replace('(','\(').replace(')','\)')+' '+ copy_dest
                print 'run command '+copy_cmd
                os.system(copy_cmd)
                pass 
        
if len(sys.argv)<4:
    print 'Usage: <src folder> <dest folder> <sid>'
else:
    create_mirror_cmd = "find "+sys.argv[1]+" -type d| sed 's;"+sys.argv[1]+"\/;"+sys.argv[2]+"/;' |sed 's/^/mkdir -p /'|sh -x"
    print 'exec '+create_mirror_cmd
    os.system(create_mirror_cmd)
    print 'create mirror finished'
    do_move(sys.argv[1],sys.argv[2],sys.argv[3])
