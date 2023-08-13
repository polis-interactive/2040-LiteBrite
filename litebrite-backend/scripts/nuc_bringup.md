## Default user
```
$ echo 'defualt user is snuc'
$ passwd
```

## Setup ssh

```
$ sudo apt-get install openssh-server
$ sudo systemctl enable ssh
$ sudo systemctl start ssh
$ echo 'now we are on remote'
$ scp litebrite-management/authorized_keys.client snuc@remote-ip:/home/snuc/.ssh
```

## General tools

```
sudo apt-get install git vim net-tools cmake g++ make build-essential
```

## setup openvpn

follow litebrite-management/openvpn readme

## build third_party

```
$ mkdir build
$ cd build
$ git clone https://github.com/polis-interactive/2040-LiteBrite
$ echo 'follow ./2040-LiteBrite/litebrite-backend/scripts/setup-third-party.sh'
```
