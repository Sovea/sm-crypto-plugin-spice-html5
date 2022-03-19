var express = require('express');
var router = express.Router();
var sm2_plugin_for_spice_html5 = require('../build/Release/sm2_crypto_plugin');
router.get('/', function (req, res, next) {
    res.render('index', { title: 'Express' });
});

const { spawn } = require('child_process');
router.post('/repair-pubKey', function (req, res, next) {
    const params = req.body;
    // test get_pubKey_from_DER
    const pubKey_buf = params.keyData.split(',');
    const pubKey_buf_array = new Uint8Array(pubKey_buf);
    const sm2_pubkey = sm2_plugin_for_spice_html5.get_pubKey_from_DER(pubKey_buf_array);
    res.json({ status: 200, pubKey: sm2_pubkey });
});

router.post('/encrypt-password-pubKey', function (req, res, next) {
    const params = req.body;
    // encrypt_password_with_pubKey
    const pubKey = params.pubKey;
    const password = params.password;
    const encryptedPassword = sm2_plugin_for_spice_html5.encrypt_password_with_pubKey(password, pubKey);
    res.json({ status: 200, encryptedPassword: encryptedPassword });
});

module.exports = router;
