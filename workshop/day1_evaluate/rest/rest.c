#include <stdio.h>
#include <curl/curl.h>

int main(void)
{
  CURL *curl;
  CURLcode res;

  /* Initialize CURL globally */
  curl_global_init(CURL_GLOBAL_ALL);

  /* Get a curl handle */
  curl = curl_easy_init();
  if(curl) {
    /* Set the URL for the POST request */
    curl_easy_setopt(curl, CURLOPT_URL, "http://185.84.161.188:3000/api/data");

    /* Set the HTTP header to specify JSON content */
    struct curl_slist *headers = NULL;
    headers = curl_slist_append(headers, "Content-Type: application/json");
    curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);

    /* Specify the POST data */
    curl_easy_setopt(curl, CURLOPT_POSTFIELDS, "{\"test\":\"test\"}");

    /* Perform the request, res gets the return code */
    res = curl_easy_perform(curl);

    /* Check for errors */
    if(res != CURLE_OK)
      fprintf(stderr, "curl_easy_perform() failed: %s\n",
              curl_easy_strerror(res));

    /* Clean up */
    curl_slist_free_all(headers); /* Free the list of headers */
    curl_easy_cleanup(curl);
  }

  /* Clean up CURL globally */
  curl_global_cleanup();

  return 0;
}

