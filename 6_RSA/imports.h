#pragma once
#include "../SHARED/Foper.cpp"
#include "../SHARED/rsa.h"
#include "../THIRD-PARTY/INCLUDE/openssl/rsa.h"
#include "../THIRD-PARTY/INCLUDE/openssl/pem.h"
#include "../THIRD-PARTY/INCLUDE/openssl/bio.h"
#include <memory>
#include <locale>
#pragma comment (lib,"../third-party/lib/libcrypto_32MT.lib")