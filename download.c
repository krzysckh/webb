#include "download.h"

void download(const char* url, const char* fname) {
	CURL* easyhandle = curl_easy_init();
	curl_easy_setopt(easyhandle, CURLOPT_URL, url);
	FILE* file = fopen(fname, "w");
	curl_easy_setopt(easyhandle, CURLOPT_WRITEDATA, file);
	curl_easy_perform(easyhandle);
	curl_easy_cleanup(easyhandle);
	fclose(file);
}

void download_to_f(const char *url, FILE *f) {
	CURL* easyhandle = curl_easy_init();
	curl_easy_setopt(easyhandle, CURLOPT_URL, url);
	curl_easy_setopt(easyhandle, CURLOPT_WRITEDATA, f);
	curl_easy_perform(easyhandle);
	curl_easy_cleanup(easyhandle);
}
