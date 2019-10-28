TOP_DIR=`pwd`

sudo docker run -it -v $TOP_DIR:/app ld3003/v3s_toolchain /bin/bash /app/cleanapp.sh
