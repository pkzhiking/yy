cd /home/damon/Workspace/yy/plugin-dev
echo $1 >> time.log
date >> time.log
python exec-plugin-dev.py $1
date >> time.log
