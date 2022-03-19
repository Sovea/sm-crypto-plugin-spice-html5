#include <node.h>
#include <iostream>
#include <openssl/x509.h>
#include <openssl/pem.h>
#include "../sm2-EVP/cpp/sm2.h"

#define SPICE_TICKET_KEY_PAIR_LENGTH 1024
#define SPICE_TICKET_PUBKEY_BYTES (SPICE_TICKET_KEY_PAIR_LENGTH / 8 + 34)

using namespace v8;
using namespace std;

SM2 sm2Handler;
void bio_to_string(BIO *bio, string &data) {
    char *temp;
    int readSize = (int)BIO_get_mem_data(bio, &temp);
    data = string(temp, readSize);
}

void get_pubKey_from_DER(const FunctionCallbackInfo<Value> &args) {
    Isolate *isolate = args.GetIsolate();
    // make sure the first argument is an Uint8Array
    assert(args[0]->IsUint8Array());

    // read first argument as an Int8Array.
    v8::Local<v8::Uint8Array> view = args[0].As<v8::Uint8Array>();

    // get contents as a void pointer
    void *rawData = view->Buffer()->GetContents().Data();

    // create a pointer to uint8_t and typecast
    uint8_t *data = static_cast<uint8_t *>(rawData);

    EVP_PKEY *evp_pkey;
    string sm2_pubkey;
    BIO *bioKey, *bioKey_pem;
    bioKey = BIO_new(BIO_s_mem());
    BIO_write(bioKey, data, SPICE_TICKET_PUBKEY_BYTES);
    evp_pkey = d2i_PUBKEY_bio(bioKey, NULL);

    // Recive pubKey from BIO
    bioKey_pem = BIO_new(BIO_s_mem());
    PEM_write_bio_PUBKEY(bioKey_pem, evp_pkey);
    bio_to_string(bioKey_pem, sm2_pubkey);
    // cout << "sm2_pubkey: \n" << sm2_pubkey << endl;
    args.GetReturnValue().Set(String::NewFromUtf8(isolate, sm2_pubkey.c_str()).ToLocalChecked());
}

void encrypt_password_with_pubKey(const FunctionCallbackInfo<Value> &args) {
    Isolate *isolate = args.GetIsolate();
    string password = string(*String::Utf8Value(isolate, args[0]));
    string pubKey = string(*String::Utf8Value(isolate, args[1]));
    int encryptedPW_len;
    string encryptedPW = "";
    sm2Handler.Encrypt(password, password.length(), encryptedPW, encryptedPW_len, pubKey);
    uint8_t *encryptedData = (uint8_t *)(encryptedPW.c_str());
    string result = "";
    for (int i = 0; i < 128; i++) result += std::to_string(unsigned(encryptedData[i])) + ",";
    // cout << "result: \n" << result << endl;
    args.GetReturnValue().Set(String::NewFromUtf8(isolate, result.c_str()).ToLocalChecked());
}

void Initialize(Local<Object> exports) {
    NODE_SET_METHOD(exports, "get_pubKey_from_DER", get_pubKey_from_DER);
    NODE_SET_METHOD(exports, "encrypt_password_with_pubKey", encrypt_password_with_pubKey);
}

NODE_MODULE(NODE_GYP_MODULE_NAME, Initialize);