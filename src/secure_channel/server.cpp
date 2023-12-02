#include <czmq.h>
#include "botan_all.h"


using namespace Botan;

void encryptdata()
{
    try
    {
        std::string text = "abc";

        // LibraryInitializer init;

        AutoSeeded_RNG rng;

        RSA_PrivateKey key(rng, 1024);

        std::string pub = X509::PEM_encode(key);

        std::string priv = PKCS8::PEM_encode(key);

        DataSource_Memory key_pub(pub);

        DataSource_Memory key_priv(priv);

        X509_PublicKey *pub_rsa = X509::load_key(key_pub);

        PKCS8_PrivateKey *priv_rsa = PKCS8::load_key(key_priv, rng);

        PK_Encrypting_Key *enckey = dynamic_cast<PK_Encrypting_Key*>(pub_rsa);

        PK_Decrypting_Key *deckey = dynamic_cast<PK_Decrypting_Key*>(priv_rsa);

        PK_Encryptor *enc = get_pk_encryptor(*enckey, "EME1(SHA-256)");

        PK_Decryptor *dec = get_pk_decryptor(*deckey, "EME1(SHA-256)");

        
    //     QByteArray array = text.toLatin1();

    //     byte msgtoencrypt[array.count()];

    //     for (int i = 0; i < array.count(); i++)
    //     {
    //         msgtoencrypt[i] = array[i];
    //     }

    //     SecureVector<byte> ciphertext = enc->encrypt(msgtoencrypt, sizeof(msgtoencrypt), rng);

    //     SecureVector<byte> plaintext = dec->decrypt(ciphertext, ciphertext.size());

    //     QByteArray encrypted;

    //     for (uint i = 0; i < ciphertext.size(); i++)
    //     {
    //         encrypted[i] = ciphertext[i];
    //     }

    //     QByteArray result;

    //     for (uint i = 0; i < plaintext.size(); i++)
    //     {
    //         result[i] = plaintext[i];
    //     }

    //     if (array == result)
    //     {
    //         qDebug() << "Ok";
    //     }
    //     else
    //     {
    //         qDebug() << "Error";
    //     }

    //     qDebug() << QString(encrypted);
    //     qDebug() << QString(array);
    //     qDebug() << QString(result);
    // }
    catch(std::exception &e)
    {
        printf(e.what());
    }
}

int main (void) 
{
    encryptdata();
    //  Create and bind server socket
    // zsock_t *server = zsock_new (ZMQ_PUSH);
    // zsock_bind (server, "tcp://*:9000");


    // Botan::AutoSeeded_RNG rng;
    // Botan::RSA_PrivateKey keygen(rng, 4096);
    
    // auto private_key = Botan::PKCS8::PEM_encode(keygen);
    // auto public_key  = Botan::X509::PEM_encode(keygen);
}