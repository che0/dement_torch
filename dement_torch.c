#include <stdio.h>
#include <pthread.h>
#include <curl/curl.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>


#define DEMENT_TORCH_VERSION "pn2_dement_torch/0"


static const char * get_string(const char * env_name) {
    char * env_value = getenv(env_name);
    if (env_value == NULL) {
        fprintf(stderr, "Missing env %s\n", env_name);
        exit(1);
    }
    
    return env_value;
}


static unsigned int get_number(const char * env_name) {
    const char * env_value = get_string(env_name);
    
    unsigned int result;
    int ret = sscanf(env_value, "%u", &result);
    if (ret != 1) {
        fprintf(stderr, "Env %s has bad format: \"%s\"\n", env_name, env_value);
        exit(1);
    }
    
    return result;
}


struct config {
    unsigned int thread_count;
    unsigned int requests;
    unsigned int sleep_ms;
    const char * url;
    unsigned int timeout_sec;
    unsigned int connect_timeout_sec;
    int skip_200;
};


size_t blackhole_callback(__attribute__((unused)) char *ptr, size_t size, size_t nmemb, __attribute__((unused)) void *userdata) {
    return size * nmemb;
}


void put_time(char * buf, size_t bufsize) {
    struct timespec cur_ts;
    clock_gettime(CLOCK_REALTIME, &cur_ts);
    
    struct tm cur_tm;
    localtime_r(&cur_ts.tv_sec, &cur_tm);
    
    char seconds_str[64];
    int ret = strftime(seconds_str, sizeof(seconds_str), "%Y-%m-%d %H:%M:%S", &cur_tm);
    if (ret == 0) {
        snprintf(buf, bufsize, "FAIL");
        return;
    }
    
    snprintf(buf, bufsize, "%s,%03ld", seconds_str, cur_ts.tv_nsec / 1000000);
}


/// returns string with double quotes changed to single (alters original)
char * strip_quotes(char * buf) {
    for (char * cur_byte = buf; *cur_byte != '\0'; cur_byte++) {
        if (*cur_byte == '"') { *cur_byte = '\''; }
    }
    return buf;
}


void do_one_request(const struct config * config) {
    CURL *curl;
    curl = curl_easy_init();
    curl_easy_setopt(curl, CURLOPT_URL, config->url);
    curl_easy_setopt(curl, CURLOPT_TIMEOUT, (long)config->timeout_sec);
    curl_easy_setopt(curl, CURLOPT_CONNECTTIMEOUT, (long)config->connect_timeout_sec);
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, blackhole_callback);
    
    char error_buf[CURL_ERROR_SIZE];
    curl_easy_setopt(curl, CURLOPT_ERRORBUFFER, error_buf);
    
    curl_easy_setopt(curl, CURLOPT_USERAGENT, DEMENT_TORCH_VERSION);
    
    CURLcode cret = curl_easy_perform(curl);
    
    char time_str[64];
    put_time(time_str, sizeof(time_str));
    
    if (cret != CURLE_OK)
    {
        printf("{\"asctime\":\"%s\",\"status_code\":599,\"message\":\"%s\"}\n", time_str, strip_quotes(error_buf));
        goto finish;
    }
    
    long response_code;
    curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &response_code);
    
    curl_off_t downloaded_bytes;
    cret = curl_easy_getinfo(curl, CURLINFO_SIZE_DOWNLOAD_T, &downloaded_bytes);
    if (cret != CURLE_OK) {
        downloaded_bytes = -1;
    }
    
    curl_off_t total_time_us;
    cret = curl_easy_getinfo(curl, CURLINFO_TOTAL_TIME_T, &total_time_us);
    if (cret != CURLE_OK) {
        total_time_us = -1;
    }
    
    curl_off_t connect_time_us;
    cret = curl_easy_getinfo(curl, CURLINFO_CONNECT_TIME_T, &connect_time_us);
    if (cret != CURLE_OK) {
        connect_time_us = -1;
    }
    
    if ((response_code != 200) || (!config->skip_200)) {
        printf("{\"asctime\":\"%s\",\"status_code\":%ld,\"downloaded_bytes\":%" CURL_FORMAT_CURL_OFF_T ","
            "\"connect_time_ms\":%" CURL_FORMAT_CURL_OFF_T ",\"total_time_ms\":%" CURL_FORMAT_CURL_OFF_T "}\n",
            time_str, response_code, downloaded_bytes,
            total_time_us / 1000, connect_time_us / 1000);
    }
    
    finish:
    curl_easy_cleanup(curl);
}


void * worker_thread(void * config_v) {
    const struct config * config = (const struct config *)config_v;
    
    for (unsigned int i = 0; i < config->requests; i++) {
        do_one_request(config);
        
        if (config->sleep_ms > 0) {
            int sleep_time_us = random() % (config->sleep_ms * 1000);
            usleep(sleep_time_us);
        }
    }
    
    return NULL;
}


int main() {
    struct config config;
    config.thread_count = get_number("THREADS");
    config.requests = get_number("REQUESTS");
    config.sleep_ms = get_number("SLEEP_MS");
    config.url = get_string("URL");
    config.timeout_sec = get_number("TIMEOUT_SEC");
    config.connect_timeout_sec = get_number("CONNECT_TIMEOUT_SEC");
    config.skip_200 = !!get_number("SKIP_200");
    
    curl_global_init(CURL_GLOBAL_ALL);
    pthread_t thread[config.thread_count];
    
    for (unsigned int i = 0; i < config.thread_count; i++) {
        int ret = pthread_create(&thread[i], NULL, worker_thread, &config);
        if (ret != 0) {
            fprintf(stderr, "Unable to create thread: %s\n", strerror(ret));
            exit(1);
        }
    }
    
    for (unsigned int i = 0; i < config.thread_count; i++) {
        int ret = pthread_join(thread[i], NULL);
        if (ret != 0) {
            fprintf(stderr, "Unable to join thread: %s\n", strerror(ret));
            exit(1);
        }
    }
    
    curl_global_cleanup();
    
    char time_str[64];
    put_time(time_str, sizeof(time_str));
    printf("%s Run complete.\n", time_str);
    return 0;
}
