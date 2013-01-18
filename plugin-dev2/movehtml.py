import os
import os.path
import sys
def do_move(src,dest):
    orig_last_part = src[src.rfind('/')+1:]
    last_part = dest[dest.rfind('/')+1:]
    print 'last part '+last_part
    for root, dirs, files in os.walk(src):
        for f in files:
            fullpath = os.path.join(root, f)
            print '->'+fullpath
            file_name = fullpath[fullpath.rfind('/')+1:]
            copy_dest = dest+fullpath[fullpath.find(orig_last_part)+len(orig_last_part):]
            if (fullpath[-4:]=='html'):
                copy_cmd = 'cp '+fullpath.replace('(','\(').replace(')','\)')+' '+ copy_dest
                print 'run command '+copy_cmd
                os.system(copy_cmd)
                pass 
        
if len(sys.argv)<3:
    print 'Usage: <src folder> <dest folder>'
else:
    create_mirror_cmd = "find "+sys.argv[1]+" -type d| sed 's;"+sys.argv[1]+"\/;"+sys.argv[2]+"/;' |sed 's/^/mkdir -p /'|sh -x"
    print 'exec '+create_mirror_cmd
    os.system(create_mirror_cmd)
    print 'create mirror finished'
    do_move(sys.argv[1],sys.argv[2])