
# SETUP OAT

$ cd ~/build
$ git clone https://github.com/oatpp/oatpp.git
$ cd oatpp/
$ mkdir build && cd build
$ cmake .. -DOATPP_BUILD_TESTS=OFF -DCMAKE_BUILD_TYPE=Release
$ make -j6
$ make install