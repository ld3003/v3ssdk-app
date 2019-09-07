set b=%cd%

docker run -it -v %b%:/app ld3003/v3s_toolchain /bin/bash /app/cleanapp.sh

pause
