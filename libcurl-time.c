/********* Sample code generated by the curl command line tool **********
 * All curl_easy_setopt() options are documented at:
 * https://curl.se/libcurl/c/curl_easy_setopt.html
 ************************************************************************/

#define _XOPEN_SOURCE 600
#include <stdlib.h>
#include <string.h>

#include <curl/curl.h>

struct timespec utc_system_timestamp(char[]);
int64_t difftimespec_us(const struct timespec after, const struct timespec before);

struct MemoryStruct
{
  char *memory;
  size_t size;
};

static size_t
WriteMemoryCallback(void *contents, size_t size, size_t nmemb, void *userp)
{
  size_t realsize = size * nmemb;
  struct MemoryStruct *mem = (struct MemoryStruct *)userp;

  char *ptr = realloc(mem->memory, mem->size + realsize + 1);
  if (!ptr)
  {
    /* out of memory! */
    printf("not enough memory (realloc returned NULL)\n");
    return 0;
  }

  mem->memory = ptr;
  memcpy(&(mem->memory[mem->size]), contents, realsize);
  mem->size += realsize;
  mem->memory[mem->size] = 0;

  return realsize;
}

int main(int argc, char *argv[])
{
  CURLcode ret;
  CURL *hnd;
  CURLcode res;
  curl_off_t total;
  curl_off_t connect;

  struct MemoryStruct chunk;

  struct timespec ts_start, ts_end;
  clock_t start, end;
  double cpu_time_used;
  char *URL;
  int max_date_length = 31;
  char start_str[max_date_length], end_str[max_date_length];

  ts_start = utc_system_timestamp(start_str);
  printf("start %s ", start_str);

  if (argc > 1)
  {
    URL = argv[1];
    printf("URL %s ", URL);
  }
  else
  {
    printf("missing URL as argument\n");
    exit(EXIT_FAILURE);
  }

  start = clock();
  chunk.memory = malloc(1); /* will be grown as needed by the realloc above */
  chunk.size = 0;           /* no data at this point */

  curl_global_init(CURL_GLOBAL_ALL);

  hnd = curl_easy_init();
  curl_easy_setopt(hnd, CURLOPT_BUFFERSIZE, 102400L);
  curl_easy_setopt(hnd, CURLOPT_URL, URL);
  curl_easy_setopt(hnd, CURLOPT_NOPROGRESS, 1L);
  curl_easy_setopt(hnd, CURLOPT_TIMEOUT_MS, 6000L);
  curl_easy_setopt(hnd, CURLOPT_USERAGENT, "curl/7.81.0");
  curl_easy_setopt(hnd, CURLOPT_MAXREDIRS, 50L);
  curl_easy_setopt(hnd, CURLOPT_HTTP_VERSION, (long)CURL_HTTP_VERSION_2TLS);
  curl_easy_setopt(hnd, CURLOPT_SSL_VERIFYPEER, 0L);
  curl_easy_setopt(hnd, CURLOPT_SSL_VERIFYHOST, 0L);
  curl_easy_setopt(hnd, CURLOPT_FTP_SKIP_PASV_IP, 1L);
  curl_easy_setopt(hnd, CURLOPT_TCP_KEEPALIVE, 1L);
  curl_easy_setopt(hnd, CURLOPT_VERBOSE, 0L);

  /* send all data to this function  */
  curl_easy_setopt(hnd, CURLOPT_WRITEFUNCTION, WriteMemoryCallback);

  /* we pass our 'chunk' struct to the callback function */
  curl_easy_setopt(hnd, CURLOPT_WRITEDATA, (void *)&chunk);

  /* Here is a list of options the curl code used that cannot get generated
     as source easily. You may choose to either not use them or implement
     them yourself.

  CURLOPT_INTERLEAVEDATA set to a objectpointer
  CURLOPT_READDATA set to a objectpointer
  CURLOPT_READFUNCTION set to a functionpointer
  CURLOPT_SEEKDATA set to a objectpointer
  CURLOPT_SEEKFUNCTION set to a functionpointer
  CURLOPT_ERRORBUFFER set to a objectpointer
  CURLOPT_STDERR set to a objectpointer
  CURLOPT_HEADERFUNCTION set to a functionpointer
  CURLOPT_HEADERDATA set to a objectpointer

  */

  ret = curl_easy_perform(hnd);

  /* Check for errors */
  if (ret != CURLE_OK)
  {
    fprintf(stderr, "curl_easy_perform() failed: %s\n", curl_easy_strerror(ret));
    exit(EXIT_FAILURE);
  }

  end = clock();
  cpu_time_used = ((double)(end - start)) / CLOCKS_PER_SEC;

  printf("cpu_time_used %f ", cpu_time_used);

  res = curl_easy_getinfo(hnd, CURLINFO_APPCONNECT_TIME_T, &connect);

  if ((CURLE_OK == res) && connect)
    printf("connect(ms) %" CURL_FORMAT_CURL_OFF_T ".%06ld ", connect / 1000000,
           (long)(connect % 1000000));

  res = curl_easy_getinfo(hnd, CURLINFO_TOTAL_TIME_T, &total);

  if ((CURLE_OK == res) && total)

    printf("total(ms) %" CURL_FORMAT_CURL_OFF_T ".%06ld ", total / 1000000,
           (long)(total % 1000000));

  curl_easy_cleanup(hnd);
  free(chunk.memory);
  hnd = NULL;

  /* we are done with libcurl, so clean it up */
  curl_global_cleanup();

  ts_end = utc_system_timestamp(end_str);
  printf("elapsedTime(ms) %ld ", difftimespec_us(ts_end, ts_start));
  printf("end %s ", end_str);

  return (int)ret;
}

// https://stackoverflow.com/a/65786680
// I have modified this part to return the timespec variable
struct timespec utc_system_timestamp(char buf[])
{
  const int tmpsize = 21;
  struct timespec now;
  struct tm tm;
  int retval = clock_gettime(CLOCK_REALTIME, &now);

  if (retval == -1)
  {
    perror("clock_getres");
    exit(EXIT_FAILURE);
  }

  gmtime_r(&now.tv_sec, &tm);
  strftime(buf, tmpsize, "%Y-%m-%dT%H:%M:%S.", &tm);
  sprintf(buf + strlen(buf), "%04luZ", now.tv_nsec);
  return now;
}

// https://stackoverflow.com/a/64896093
// return microseconds
int64_t difftimespec_us(const struct timespec after, const struct timespec before)
{
  return ((int64_t)after.tv_sec - (int64_t)before.tv_sec) * (int64_t)1000000 + ((int64_t)after.tv_nsec - (int64_t)before.tv_nsec) / 1000;
}
