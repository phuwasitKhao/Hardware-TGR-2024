

#include "../main_thr.h"
#include <stdio.h>
#include <curl/curl.h>
#include <string.h>
#include <stdlib.h>

#define JWT "eyJhbGciOiJFUzI1NiIsInR5cCI6IkpXVCJ9.eyJpc3MiOiJsb2NhbGhvc3Q6ODA4MCIsInN1YiI6ImI0NjliOTNjLWQwNGYtNGJhMS1iZmExLTg4NjYzZGY5ZTg3YSJ9.Gp4vlllGMwkbCE1x7UfhJHAsqSUaO6CA6fMfUUcCL_hW75rIld6pV2_rHepFejLMnsePZ51X78PDzIZFmEZ1DA" // Replace with your actual JWT token

void *rest_thr_fcn(void *ptr)
{
    printf("Starting REST thread\n");
    char message[256];

    while (1)
    {
        // Wait for the shared message to be updated
        pthread_mutex_lock(&rest_cond_mutex);
        pthread_cond_wait(&rest_cond, &rest_cond_mutex);
        strcpy(message, shared_message_sw);
        pthread_mutex_unlock(&rest_cond_mutex);

        // Print the message
        printf("REST: %s\n", message);

        if (strcmp(message, "output_") == 0) {
            upload_Sound(message);
        } 
        else if (strncmp(message, "id", 2) == 0) { // Check if message starts with "id"
            // Extract the ID part starting from the third character
            const char *id = &message[2];
            download_file(id); // Pass the ID to the download function
        }
    }
}

void upload_Sound(char *filename)
{
  CURL *curl;
  CURLcode res;

  /* Initialize CURL globally */
  curl_global_init(CURL_GLOBAL_ALL);

  /* Get a curl handle */
  curl = curl_easy_init();
  if (curl)
  {
    /* Set the URL for the POST request */
    curl_easy_setopt(curl, CURLOPT_URL, "http://services-obseeit.kku-topgun.xyz/api/v1/attachment/file");

    /* Create a CURL form to send the file */
    curl_mime *form = NULL;
    curl_mimepart *field = NULL;

    /* Initialize the form */
    form = curl_mime_init(curl);

    /* Add the file field */
    field = curl_mime_addpart(form);
    curl_mime_name(field, "file");       // Field name
    curl_mime_filedata(field, filename); // Path to the sound file

    /* Add another field (optional, for example, metadata) */
    field = curl_mime_addpart(form);
    curl_mime_name(field, "file_type");
    curl_mime_data(field, "sound", CURL_ZERO_TERMINATED);

    /* Set the form info as POST data */
    curl_easy_setopt(curl, CURLOPT_MIMEPOST, form);

    /* Set the HTTP header to include the JWT token */
    struct curl_slist *headers = NULL;
    char auth_header[256];
    snprintf(auth_header, sizeof(auth_header), "Authorization: Bearer %s", JWT);
    headers = curl_slist_append(headers, auth_header);
    curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);

    /* Perform the request, res gets the return code */
    res = curl_easy_perform(curl);

    /* Check for errors */
    if (res != CURLE_OK)
    {
      fprintf(stderr, "curl_easy_perform() failed: %s\n", curl_easy_strerror(res));
    }

    /* Clean up */
    curl_mime_free(form);         // Free the form
    curl_slist_free_all(headers); // Free the header list
    curl_easy_cleanup(curl);      // Clean up CURL handle
  }

  /* Clean up CURL globally */
  curl_global_cleanup();
}
// Helper function to determine the file extension based on the Content-Type
const char* get_file_extension(const char *content_type) {
    // Check if content_type is NULL
    if (content_type == NULL) {
        return "";
    }

    // Find the position of '/'
    const char *slash = strchr(content_type, '/');
    if (slash && *(slash + 1) != '\0') {
        // Allocate memory for the extension and prepend a dot
        char *extension = malloc(strlen(slash)); // Enough space for dot + subtype
        if (extension == NULL) {
            return ""; // Return empty string if memory allocation fails
        }

        // Copy the subtype part with a dot prefix
        snprintf(extension, strlen(slash), ".%s", slash + 1);

        // Convert specific subtypes to correct extensions if needed
        if (strcmp(extension, ".jpeg") == 0) {
            strcpy(extension, ".jpg"); // Use .jpg for JPEG images
        } else if (strcmp(extension, ".plain") == 0) {
            strcpy(extension, ".txt"); // Use .txt for plain text files
        } else if (strcmp(extension, ".x-csrc") == 0) {
            strcpy(extension, ".c"); // Use .c for C source code
        } else if (strcmp(extension, ".octet-strea") == 0) {
            strcpy(extension, ".mat"); // Use .m for MATLAB files
        } else if (strcmp(extension, ".x-python") == 0) {
            strcpy(extension, ".py"); // Use .py for Python files
        } else if (strcmp(extension, ".x-sh") == 0) {
            strcpy(extension, ".sh"); // Use .sh for shell scripts
        } else if (strcmp(extension, ".x-yaml") == 0) {
            strcpy(extension, ".yaml"); // Use .yaml for YAML files
        } else if (strcmp(extension, ".x-zip-compressed") == 0) {
            strcpy(extension, ".zip"); // Use .zip for ZIP archives
        } else if (strcmp(extension, ".pd") == 0) {
            strcpy(extension, ".pdf"); // Use .docx for Word documents
        } 
        return extension;
    }

    return ""; // Return empty if format is unknown
}

// Callback function to capture the Content-Type header
size_t header_callback(char *buffer, size_t size, size_t nitems, void *userdata)
{
  size_t num_bytes = size * nitems;
  if (strncasecmp(buffer, "Content-Type:", 13) == 0)
  {
    // Extract content type from header
    sscanf(buffer, "Content-Type: %s", (char *)userdata);
  }
  return num_bytes;
}

void download_file(const char *attach_id)
{
  CURL *curl;
  CURLcode res;
  FILE *fp;
  char url[512];
  char auth_header[512];
  char content_type[64] = ""; // Store Content-Type here
  struct curl_slist *headers = NULL;

  // Construct the URL
  snprintf(url, sizeof(url), "http://services-obseeit.kku-topgun.xyz/api/v1/attachment/file/%s", attach_id);

  // Initialize CURL globally
  curl_global_init(CURL_GLOBAL_ALL);

  // Get a curl handle
  curl = curl_easy_init();
  if (curl)
  {
    // Set the URL for the GET request
    curl_easy_setopt(curl, CURLOPT_URL, url);

    // Set the HTTP header to include the JWT token if provided
    snprintf(auth_header, sizeof(auth_header), "Authorization: Bearer %s", JWT);
    headers = curl_slist_append(headers, auth_header);
    curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);

    // Set a callback to capture headers and extract the Content-Type
    curl_easy_setopt(curl, CURLOPT_HEADERFUNCTION, header_callback);
    curl_easy_setopt(curl, CURLOPT_HEADERDATA, content_type);

    // Perform the request to get headers and response data
    res = curl_easy_perform(curl);

    // Check if there was an error with the request
    if (res != CURLE_OK)
    {
      fprintf(stderr, "curl_easy_perform() failed: %s\n", curl_easy_strerror(res));
    }
    else
    {
      // Create a filename based on attach_id and content type
      char filename[256];
      snprintf(filename, sizeof(filename), "downloads/%s%s", attach_id, get_file_extension(content_type));

      // Open the file to write the downloaded data
      fp = fopen(filename, "wb");
      if (fp)
      {
        // Set the file to write the data
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, fp);

        // Perform the request again to write data to file
        res = curl_easy_perform(curl);

        // Check for errors on the second request
        if (res != CURLE_OK)
        {
          fprintf(stderr, "curl_easy_perform() failed: %s\n", curl_easy_strerror(res));
        }
       

        // Close the file
        fclose(fp);
      }
      else
      {
        fprintf(stderr, "Failed to open file for writing: %s\n", filename);
      }
    }

    // Clean up
    curl_slist_free_all(headers); // Free the header list
    curl_easy_cleanup(curl);      // Clean up CURL handle
  }
  else
  {
    fprintf(stderr, "Failed to initialize CURL\n");
  }

  // Clean up CURL globally
  curl_global_cleanup();
}
