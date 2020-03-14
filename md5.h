#pragma once
#include <string.h>
#include <iostream>
#include <openssl/md5.h>

std::string mymd5(uint8_t* buffer, size_t length );
