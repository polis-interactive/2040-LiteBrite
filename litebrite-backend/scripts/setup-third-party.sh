
# INSTALL SQLLITE

# pinned version for some reason on my laptop?
$ sudo apt-get install libsqlite3-0=3.37.2-2 libsqlite3-dev sqlite3


# SETUP CROW

$ cd ~/build
$ git clone https://github.com/CrowCpp/Crow.git
$ cd Crow/
$ mkdir build && cd build
$ cmake .. -DCROW_BUILD_EXAMPLES=OFF -DCROW_BUILD_TESTS=OFF -DASIO_INCLUDE_DIR=/usr/local/lib/boost_1_81_0
$ make -j6
$ sudo make install

# setup sqlite_orm

$ cd ~/build
$ git clone https://github.com/fnc12/sqlite_orm.git sqlite_orm
$ cd sqlite_orm
$ mkdir build && cd build
$ cmake .. -DSQLITE_ORM_ENABLE_CXX_17=ON -DCMAKE_BUILD_TYPE=RELEASE -DBUILD_TESTING=OFF
$ make -j6
$ sudo make install

# setup libsodium

$ cd ~/build
$ git clone -b stable https://github.com/jedisct1/libsodium
$ cd libsodium
$ ./configure
$ make -j6 && make check
$ sudo make install