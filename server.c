// Author: ECecillo
// Date: 2023/05/21
// Brief: A simple HTTP server that sends a HTML file to the client.

#include <stdio.h>
#include <sys/socket.h>
#include <unistd.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <string.h>

#define FILE_PATH "public/"

typedef struct {
  char* path;
  char* file_type;
} Request;

// Define a struct to hold the mapping between file extensions and MIME types.
typedef struct {
  const char* extension;
  const char* mime_type;
} mime_map;

mime_map mime_types[] = {
  {"html", "text/html"},
  {"css", "text/css"},
  {"js", "application/javascript"},
  {"png", "image/png"},
  {"jpg", "image/jpeg"},
  {"jpeg", "image/jpeg"},
  {"gif", "image/gif"},
};

char* buildPathToStaticContent(char* content_asked) { 
  char* full_path = malloc(strlen(FILE_PATH) + strlen(content_asked) + 1);
  strcpy(full_path, FILE_PATH);
  strcat(full_path, content_asked);
  return full_path;
}

Request parse_request_header_to_get_path(const char* request_buffer_to_parse) {
  char method[16], version[16];
  char* path = malloc(1024);
  Request request;

  if(sscanf(request_buffer_to_parse, "%15s %1023s %15s", method, path, version) == 3) {
    request.path = path;
    // Extract file type from path
    char* extension = strrchr(path, '.');
    if(extension) {
      request.file_type = malloc(strlen(extension) + 1);
      strcpy(request.file_type, extension + 1);
    } else {
      request.file_type = malloc(strlen("html") + 1);
      strcpy(request.file_type, "html");
    }
  } else {
    free(path);
    request.path = NULL;
    request.file_type= NULL;
  }
  return request;
}

char* read_html (char* file_to_read, long* len) {
  FILE *file_pointer; 

  file_pointer = fopen(file_to_read, "r"); 
  if(!file_pointer) {
    char* error_message = malloc(strlen("Error opening file\n") + 1);
    strcpy(error_message, "Error opening file\n");
    return error_message; 
  }

  // Go the end of the file to a find the length
  if(fseek(file_pointer, 0L, SEEK_END) != 0) {
    fclose(file_pointer);
    return ("Error seeking to the end of the file\n"); 
  }

  // Get the size of the file
  long file_size_buffer= ftell(file_pointer);
  if(file_size_buffer == -1) {
    fclose(file_pointer);
    return ("Error reading file\n"); 
  }
  *len = file_size_buffer;

  // Alloc. memory to store what we read.
  char *file_buffer = malloc(sizeof(char) * (file_size_buffer + 1));

  // Going back to the start of the file
  if(fseek(file_pointer, 0L, SEEK_SET) != 0) {
    free(file_buffer);
    fclose(file_pointer);
    return ("Error seeking to the start of the file\n"); 
  }

  // Read the file into our buffer
  size_t new_length = fread(file_buffer, sizeof(char), file_size_buffer, file_pointer);
  if(ferror(file_pointer) != 0) {
    free(file_buffer);
    fclose(file_pointer);
    return ("Error reading file\n"); 
  } else {
    file_buffer[new_length++] = '\0'; // Just to be safe.
  }

  fclose(file_pointer);
  return file_buffer;
}

const char* mimeTypeBasedOnFileType(const char* file_type) {
  // Iterate over the array and find the matching MIME type for the given file extension.
  for (int i = 0; i < sizeof(mime_types) / sizeof(mime_map); i++) {
    if (strcmp(file_type, mime_types[i].extension) == 0) {
      return mime_types[i].mime_type;
    }
  }
  // Default content type
  return "text/plain";
}

char* buildHTTPHeader(char* html, long len, const char* file_type) {

  const char * content_type = mimeTypeBasedOnFileType(file_type);
  char header_format[] = "HTTP/1.1 200 OK\nContent-Type: %s\nContent-Length: %ld\n\n";
  int header_length = snprintf(NULL, 0, header_format, content_type, len);

  char *html_header = malloc(header_length + 1 + len);
  // Write the header to the buffer
  sprintf(html_header, header_format, content_type,len);
  strcat(html_header, html);
  return html_header;
}

char* serve_static_content_to_client(char* request_buffer_to_parse) {
  Request request = parse_request_header_to_get_path(request_buffer_to_parse);
  char* content_asked = request.path;

  // Remove the leading '/' from the requested path to convert it to a file path.
  char* file_path = content_asked + 1;
  char* full_path = buildPathToStaticContent(file_path);
  printf("Before %s", file_path);

  long len = 0;
  // If the request path is "" or index.html then set the full path to index.html.
  if (strcmp(file_path, "") == 0 ) {
    // Clean memory before re-allocating.
    free(full_path);
    printf("Serving Index");
    full_path = buildPathToStaticContent("index.html");
  }

  printf("Serving %s \n", full_path); 
  char* html = read_html(full_path, &len);

  // If read_html() returned an error message, read the error.html file instead.
  if (strncmp(html, "Error", 5) == 0) {
    free(html);
    printf("%s", request.file_type);
    if(strcmp(request.file_type, "html") == 0) {
      printf("Serving Error Page");
      full_path = buildPathToStaticContent("error.html");
      html = read_html(full_path, &len);
    } else {
      char *header = "HTTP/1.1 404 NOT FOUND\nContent-Type: text/html\nContent-Length: 0\n\n";
      char *html_header = malloc(strlen(header) + 1);
      strcpy(html_header, header);
      free(content_asked);
      free(full_path); 
      return html_header; 
    }
  }

  // Handle the case where reading error.html also failed.
  if (strncmp(html, "Error", 5) == 0) {
    printf("Failed to read error.html: %s", html);
    free(html);
    free(content_asked);
    free(full_path);  
    return NULL;
  } 
  char *html_header = buildHTTPHeader(html, len, request.file_type);
  free(html);
  free(content_asked);
  free(full_path);  
  return html_header; 
}


#define PORT 8080
int main(int argc, char const *argv[])
{
  int server_fd, new_socket; long valread;
  struct sockaddr_in server_address;
  int addrlen = sizeof(server_address);

  // Creating socket file descriptor
  if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0)
  {
    perror("In socket");
    exit(EXIT_FAILURE);
  } 

  // Define the spec of the server socket.
  // Define the address based on the spec of our TCP/IP socket. 
  memset(server_address.sin_zero, '\0', sizeof server_address.sin_zero);
  server_address.sin_family = AF_INET;
  server_address.sin_addr.s_addr = INADDR_ANY;
  server_address.sin_port = htons( PORT );

  if (bind(server_fd, (struct sockaddr *)&server_address, sizeof(server_address))<0)
  {
    perror("In bind");
    exit(EXIT_FAILURE);
  }
  if (listen(server_fd, 10) < 0)
  {
    perror("In listen");
    exit(EXIT_FAILURE);
  }

  while(1)
  {
    printf("\n+++++++ Waiting for new connection ++++++++\n\n");
    if ((new_socket = accept(server_fd, (struct sockaddr *)&server_address, (socklen_t*)&addrlen))<0)
    {
      perror("In accept");
      exit(EXIT_FAILURE);
    }

    char buffer[30000] = {0};
    if((valread = read( new_socket , buffer, 30000)) < 0)
    {
      perror("In read");
      exit(EXIT_FAILURE);
    }

    // Retrieve only the first line of the request to know what the client wants.
    //char *new_line_position = strchr(buffer, '\n');
    //char* content_path = parse_request_header_to_get_path(buffer);
    //printf("%s\n",content_path);

    char* html_header = serve_static_content_to_client(buffer);
    if(html_header == NULL) {
      printf("Failed to serve static content to client\n");
      continue;
    }
    // Print out the incoming request Header from the client
    //    printf("%s\n",buffer );
    write(new_socket , html_header , strlen(html_header));
    printf("------------------HTML file sent-------------------\n");
    close(new_socket);

    //free(content_path);
    free(html_header);
  }
  return 0;
} 
