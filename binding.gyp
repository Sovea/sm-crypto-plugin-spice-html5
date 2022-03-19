{
    'variables': {
        "node_shared_openssl": "false",
    },
    'targets': [
        {
            'target_name': 'sm2_crypto_plugin',
            'sources': [ 
                'sm-plugin-src/sm-plugin.cc',
                'sm2-EVP/cpp/sm2.cpp',
            ],
            "libraries": [ "-lssl", "-lcrypto"],
        }
    ]
}