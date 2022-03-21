# Multivariate Deep Learning
Multivariate Deep Learning Application
## Presiquitive
- Docker
- docker-compose 
- Node.js

### Install Docker
- [Docker for Windows](https://docs.docker.com/docker-for-windows/install/)
- [Docker for MacOS](https://docs.docker.com/docker-for-mac/install/)
- [Docker for Ubuntu](https://docs.docker.com/install/linux/docker-ce/ubuntu/)

Following installation step from previous links to install Docker on your system.

### Install Docker-Compose
On Windows and MacOS version of Docker desktop have included Docker-compose.  

#### [Docker-compose for linux](https://docs.docker.com/compose/install/)
Run this command to download the current stable release of Docker Compose:

```
sudo curl -L "https://github.com/docker/compose/releases/download/1.24.1/docker-compose-$(uname -s)-$(uname -m)" -o /usr/local/bin/docker-compose
```
Apply executable permissions to the binary:
```
sudo chmod +x /usr/local/bin/docker-compose
```


# Run the server on your local system
```
docker-compose up
```
# Set up Node.js
- Go to https://nodejs.org/en/download/, download and install version suitable for your machine.
- Download this github repository and unzip the socketServer8.zip
- Use command line to go to the unziped file and execute `node index.js`
- A message like this will be displayed `Server nodejs run at x.y.z.w:3484` where x.y.z.w is your machine ip address, remember this ip addrees
- Node.js server is good to go

# Set up device
- Download Arduino IDE from https://www.arduino.cc/en/Main/Software
- Open config_sensor.ino by Arduino IDE
- On line 30 and 31, change the network SSID and password to your settings
- On line 33 change the ip address with the one from your machine
- Inject the code to the device

