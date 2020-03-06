#/bin/bash 

physicalNumber=0
coreNumber=0
logicalNumber=0
HTNumber=0

logicalNumber=$(grep "processor" /proc/cpuinfo|sort -u|wc -l)
physicalNumber=$(grep "physical id" /proc/cpuinfo|sort -u|wc -l)
coreNumber=$(grep "cpu cores" /proc/cpuinfo|uniq|awk -F':' '{print $2}'|xargs)
HTNumber=$((logicalNumber / (physicalNumber * coreNumber)))

echo "****** CPU Information ******"
echo "Logical CPU Number : ${logicalNumber}"
echo "Physical CPU Number : ${physicalNumber}"
echo "CPU Core Number : ${coreNumber}"
echo "HT Number : ${HTNumber}"

echo "*****************************"

#设置出错就暂停
set -e

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

TOP_DIR=$PWD

echo $TOP_DIR
echo $PATH
PATH="/media/user1/V3SSDK_BUILDROOT/v3ssdk/buildroot/out/host/bin/:$PATH"

APP_DIR=$TOP_DIR
BR_CROSS_COMPILE=arm-buildroot-linux-gnueabihf-



function build_library()
{

	cd $APP_DIR/ncnn/
	if [ ! -d "build" ]; then
		mkdir build
	fi
	cd build
	cmake -DCMAKE_TOOLCHAIN_FILE=../toolchains/arm-buildroot-gnueabihf.toolchain.cmake ..
	make -j${logicalNumber}

	cd $APP_DIR/libfacedetection/
	if [ ! -d "build" ]; then
		mkdir build
	fi
	cd build
	cmake ../
	make -j${logicalNumber}

}

function build_demos()
{


	cd $APP_DIR/demo-camera/
	echo "*****************************************"
	pwd
	make CROSS_COMPILE=$BR_CROSS_COMPILE -j ${logicalNumber} demo-camera


	#cd $APP_DIR/gpio/
	#echo "*****************************************"
	#pwd
	#make CROSS_COMPILE=$BR_CROSS_COMPILE -j ${logicalNumber}

	#cd $APP_DIR/demo-codec/
	#echo "*****************************************"
	#pwd
	#make CROSS_COMPILE=$BR_CROSS_COMPILE -j ${logicalNumber}

	cd $APP_DIR/demo-qt
	echo "*****************************************"
	pwd
	qmake
	make -j${logicalNumber}
	
	cd $APP_DIR/demo-guid
	echo "*****************************************"
	pwd
	qmake
	make -j${logicalNumber}
	
	cd $APP_DIR/demo-mjpgsrv
	echo "*****************************************"
	pwd
	qmake
	make -j${logicalNumber}

	cd $APP_DIR/demo-mjpgcloud
	echo "*****************************************"
	pwd
	qmake
	make -j${logicalNumber}

	cd $APP_DIR/demo-eth
	echo "*****************************************"
	pwd
	qmake
	make -j${logicalNumber}
	
	cd $APP_DIR/demo-router
	echo "*****************************************"
	pwd
	qmake
	make -j${logicalNumber}

	cd $APP_DIR/opencv
	echo "*****************************************"
	pwd
	make CROSS_COMPILE=$BR_CROSS_COMPILE -j ${logicalNumber} test

	cd $APP_DIR/curl
	echo "*****************************************"
	pwd
	make CROSS_COMPILE=$BR_CROSS_COMPILE -j ${logicalNumber} test


	#cd $APP_DIR/cam4g
	#echo "*****************************************"
	#pwd
	#make CROSS_COMPILE=$BR_CROSS_COMPILE -j ${logicalNumber}

	cd $APP_DIR/opencv2fb
	echo "*****************************************"
	pwd
	make CROSS_COMPILE=$BR_CROSS_COMPILE -j ${logicalNumber}


	cd $APP_DIR/serialport_linux/src
	echo "*****************************************"
	pwd
	make CROSS_COMPILE=$BR_CROSS_COMPILE -j ${logicalNumber} 

	cd $APP_DIR/facedetapp
	echo "*****************************************"
	pwd
	qmake
	make -j${logicalNumber}

	cd $APP_DIR/qrcode
	echo "*****************************************"
	pwd
	qmake
	make -j${logicalNumber}

	cd $APP_DIR/facenet/
	echo "*****************************************"
	pwd
	if [ ! -d "build" ]; then
		mkdir build
	fi
	echo "####################################"
	ls
	cd build
	pwd
	cmake ../
	make -j${logicalNumber}

	cd $APP_DIR/mjpg_srv
	echo "*****************************************"
	pwd
	make CROSS_COMPILE=$BR_CROSS_COMPILE -j ${logicalNumber}

	cd $APP_DIR/
	cp -v demo-guid/demo-guid ./sdcard/
	cp -v demo-router/demo-router ./sdcard/
	cp -v demo-mjpgsrv/demo-mjpgsrv ./sdcard/
	cp -v facedetapp/facenet ./sdcard/demo-facenet
	cp -v facedetapp/testcam ./sdcard/testcam
	cp -v qrcode/qrcode ./sdcard/demo-qrcode
	cp -v demo-mjpgcloud/cloud.cfg ./sdcard/cloud.cfg
	cp -v demo-eth/demo-eth ./sdcard/
        cp -v demo-mjpgcloud/demo-cloud ./sdcard/demo-cloud

}

function clean_demos()
{
	cd $TOP_DIR
	source env-qt

	cd $APP_DIR/demo-camera/
	make clean

	cd $APP_DIR/demo-qt
	./run_qmake.sh
	make clean

}

build_library
build_demos
