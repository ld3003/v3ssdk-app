
#获取当前文件所在路径
if [ -L $0 ]
then
	BASE_DIR=`dirname $(readlink $0)`
else
	BASE_DIR=`dirname $0`
fi
basepath=$(cd $BASE_DIR; pwd)
echo $basepath

#进入当前文件所在目录
cd $basepath

TOP_DIR=`pwd`

#启动docker编译
sudo docker run -it -v $TOP_DIR:/$TOP_DIR ld3003/v3s_toolchain /bin/bash $TOP_DIR/buildapp.sh
