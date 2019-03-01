#ifndef __C_MODULES_UNIX_X86_64_LINUX_SECURE_SOCKET_H__
#define __C_MODULES_UNIX_X86_64_LINUX_SECURE_SOCKET_H__
#include <stdio.h>
#include <unistd.h>
#include <malloc.h>
#include <string.h>
#include <sys/socket.h>
#include <resolv.h>
#include <openssl/ssl.h>
#include <openssl/err.h>

#define FAIL -1

SSL_CTX* UNIX_X86_64_LINUX_init_server_ctx(void)
{
    SSL_library_init();
    SSL_METHOD *method;
    SSL_CTX *ctx;
    OpenSSL_add_all_algorithms();       /* load & register all cryptos, etc. */
    SSL_load_error_strings();           /* load all error messages */
    method = SSLv23_server_method();    /* create new server-method instance */
    ctx = SSL_CTX_new(method);          /* create new context from method */
    if (ctx == NULL)
        return NULL;
    return ctx;
}

void UNIX_X86_64_LINUX_load_certificates(SSL_CTX* ctx, char* cert_file, char* key_file)
{
    /* set the local certificate from cert_file */
    if (SSL_CTX_use_certificate_file(ctx, cert_file, SSL_FILETYPE_PEM) <= 0)
    {
        ERR_print_errors_fp(stderr);
        abort();
    }
    /* set the private key from key_file (may be the same as cert_file) */
    if (SSL_CTX_use_PrivateKey_file(ctx, key_file, SSL_FILETYPE_PEM) <= 0)
    {
        ERR_print_errors_fp(stderr);
        abort();
    }
    /* verify private key */
    if (!SSL_CTX_check_private_key(ctx))
    {
        fprintf(stderr, "Private key does not match the public certificate\n");
        abort();
    }
}

void UNIX_X86_64_LINUX_show_certs(SSL* ssl)
{   
    X509 *cert;
    char *line;
    cert = SSL_get_peer_certificate(ssl);   /* Get certificates (if available) */
    if (cert != NULL)
    {
        printf("Server certificates:\n");
        line = X509_NAME_oneline(X509_get_subject_name(cert), 0, 0);
        printf("Subject: %s\n", line);
        free(line);
        line = X509_NAME_oneline(X509_get_issuer_name(cert), 0, 0);
        printf("Issuer: %s\n", line);
        free(line);
        X509_free(cert);
    }
    else
        printf("No certificates.\n");
}

#endif