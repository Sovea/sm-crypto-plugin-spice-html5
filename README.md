### sm-crypto-plugin-spice-html5

This project is a simple sm-crypto plugin written using Node.js C++ Addons & [sm2-EVP](https://github.com/Sovea/sm2-EVP.git) & express, created for the spice NTLS reconstruction project, especially for the for [spice-html5-gm](https://github.com/Sovea/spice-html5.git).

The main features of this project is to help spice-html5 to handle the **Ticket** when connecting to spice server. Include obtaining the pubKey from server, encrypt the password use SM2 and send to it with the required format.

#### How to use it:
```sh
# install modules
npm install

# install node-gyp
npm install -g node-gyp

node-gyp configure

# node-gyp build
node-gyp build

# start express service
node app.js
```