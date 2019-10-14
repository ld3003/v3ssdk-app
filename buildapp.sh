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


TOP_DIR=$PWD

echo $TOP_DIR
echo $PATH
PATH="/media/user1/V3SSDK_BUILDROOT/v3ssdk/buildroot/out/host/bin/:$PATH"

APP_DIR=$TOP_DIR
BR_CROSS_COMPILE=arm-buildroot-linux-gnueabihf-



function build_library()
{

	cd $APP_DIR/ncnn/
	mkdir build
	cd build
	cmake -DCMAKE_TOOLCHAIN_FILE=../toolchains/arm-buildroot-gnueabihf.toolchain.cmake ..
	make -j${logicalNumber}

	cd $APP_DIR/libfacedetection/
	mkdir build
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


	cd $APP_DIR/gpio/
	echo "*****************************************"
	pwd
	make CROSS_COMPILE=$BR_CROSS_COMPILE -j ${logicalNumber}

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


	cd $APP_DIR/cam4g
	echo "*****************************************"
	pwd
	make CROSS_COMPILE=$BR_CROSS_COMPILE -j ${logicalNumber}

	cd $APP_DIR/opencv2fb
	echo "*****************************************"
	pwd
	make CROSS_COMPILE=$BR_CROSS_COMPILE -j ${logicalNumber}


	cd $APP_DIR/facedetectapp
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
	mkdir build
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
	cp -v qrcode/qrcode ./sdcard/demo-qrcode


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
