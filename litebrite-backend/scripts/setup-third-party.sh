
# setup crow

$ cd ~/build
$ git clone https://github.com/CrowCpp/Crow.git
$ cd Crow/
# ONLY APPLY PATCH FOR DEBUG MODE!!!!
$ cp ~/polis/2040-LiteBrite/litebrite-backend/scripts/crow_hack.patch  ./
$ git apply crow_hack.patch
$ mkdir build && cd build
$ cmake .. -DCROW_BUILD_EXAMPLES=OFF -DCROW_BUILD_TESTS=OFF -DASIO_INCLUDE_DIR=/usr/local/lib/boost_1_81_0
$ make -j6
$ sudo make install
$ sudo chmod 755 /usr/local/include/crow/middlewares

# setup libsodium

$ cd ~/build
$ git clone -b stable https://github.com/jedisct1/libsodium
$ cd libsodium
$ ./configure
$ make -j6 && make check
$ sudo make install

# install sqlite

# pinned version for some reason on my laptop?
$ sudo apt-get install libsqlite3-0=3.37.2-2 libsqlite3-dev sqlite3

# setup sqlitecpp

$ cd ~/build
$ git clone https://github.com/SRombauts/SQLiteCpp
$ cd SQLiteCpp/
$ mkdir build && cd build
$ cmake .. -DSQLITECPP_BUILD_TESTS=OFF -DSQLITECPP_RUN_CPPLINT=OFF
$ make -j6
$ sudo make install
