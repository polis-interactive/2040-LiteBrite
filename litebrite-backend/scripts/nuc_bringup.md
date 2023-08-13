## wake on power
- Enter bios (f2 on power up)
- advanced tab
	- power section
- after power loss / power on after power failure
- power on
- save and exit

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
sudo apt-get install git vim net-tools cmake g++ make build-essential pkg-config libssl-dev
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

## Copy system config

```
$ sudo cp /home/snuc/build/2040-LiteBrite/litebrite-backend/system/cron-reboot /etc/cron.d/reboot
$ sudo cp /home/snuc/build/2040-LiteBrite/litebrite-backend/system/server.service /etc/systemd/system/
$ sudo systemctl daemon-reload
$ sudo systemctl enable server.service
$ sudo systemctl start server.service
```