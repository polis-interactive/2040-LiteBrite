
# INSTALL SQLLITE

# pinned version for some reason?
$ sudo apt-get install libsqlite3-0=3.37.2-2 libsqlite3-dev sqlite3


# SETUP CROW

$ cd ~/build
$ git clone https://github.com/CrowCpp/Crow.git
$ cd Crow/
$ mkdir build && cd build
$ cmake .. -DCROW_BUILD_EXAMPLES=OFF -DCROW_BUILD_TESTS=OFF -DASIO_INCLUDE_DIR=/usr/local/lib/boost_1_81_0
$ make -j6
$ make install
