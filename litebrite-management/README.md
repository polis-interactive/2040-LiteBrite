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
