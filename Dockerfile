# ive nginx start

# Setup an operating system
FROM ubuntu:18.04

# Install tooling and their dependencies
RUN apt update && apt install -y libboost-all-dev libssl-dev  g++ nginx python3.6 python3-pip curl

# Install Node.js
# Use 14 instead (fallback to 12 if something goes wrong)
RUN curl -sL https://deb.nodesource.com/setup_14.x -o nodesource_setup.sh && bash nodesource_setup.sh && apt install -y nodejs

# Web server dependencies
RUN pip3 install flask_restful flask_cors psutil

# Backend code to compile
COPY discoveryserver/ /home/discoveryserver/

# Frontend for interaction (and sending commands)
COPY app/ /home/app/

# Nginx configuration
# https://www.digitalocean.com/community/tutorials/how-to-create-a-self-signed-ssl-certificate-for-nginx-in-ubuntu-16-04
# Step1: Generate and copy them to where Nginx would use
COPY nginx-selfsigned.crt /etc/ssl/certs/
COPY nginx-selfsigned.key /etc/ssl/private/
COPY dhparam.pem /etc/ssl/certs/
# Step2: Configure Nginx so it could find and use them
COPY ssl.conf /etc/nginx/conf.d/
# Step3: Use pre-written Nginx config
COPY default /etc/nginx/sites-available/

# Nginx start
RUN service nginx restart

# Compile frontend
WORKDIR /home/app/
RUN npm i && npm run build

# Compile backend
WORKDIR /home/discoveryserver/src/
RUN g++ main.cpp WebSocketMainWS.cpp WebSocketWS.cpp -I ../lib/SimpleWebSocketServer/ -lboost_system -lssl -lcrypto -lpthread -o WebSocketWS

# Ready to start!
COPY run.sh /home/discoveryserver/src/
RUN chmod +x run.sh

# Clean up to reduce the container size
RUN apt clean && rm -rf /var/lib/apt/lists/*

# Ready to start!
CMD "./run.sh"

# For the host to use the service
EXPOSE 80 443
