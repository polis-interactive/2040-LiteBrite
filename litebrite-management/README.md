# Setting up the reverse proxy

Install ngnix / certbot
```
sudo apt-get install ngnix certbot python3-certbot-nginx python3-certbot-dns-route53
```

setup certbot
```
sudo certbot certonly --dns-route53 --dns-route53-propagation-seconds 30 -d 'lighting.polis.tv,*.lighting.polis.tv'
```

setup auto-renew
```
crontab -e
00 3 * * 1 /usr/bin/certbot renew --dns-route53 --dns-route53-propagation-seconds 30
```

Login to the server
```
ssh -i PolisOpenvpnCredentials.pem ubuntu@openvpn.polis.tv
```

Update lighting.polis.tv
```
scp -i ./PolisOpenvpnCredentials.pem ./lighting.polis.tv ubuntu@openvpn.polis.tv:/home/ubuntu/sites-available
# log into server
sudo cp ~/sites-available/lighting.polis.tv /etc/nginx/sites-available/lighting.polis.tv
sudo nginx -t
sudo service nginx reload
```

Update frontend
```
scp -i ./PolisOpenvpnCredentials.pem -r ../litebrite-frontend/.output/public ubuntu@openvpn.polis.tv:/home/ubuntu/
# log into server
sudo rm -rf /var/www/lighting.polis.tv/public/*
sudo cp -r ~/public /var/www/lighting.polis.tv
sudo nginx -t
sudo service nginx reload
```