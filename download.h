#include <curl/curl.h>

void download(const char*, const char*);
FILE *download_to_tmpf(const char *url);
