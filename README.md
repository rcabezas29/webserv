# webserv

This repository contains a C++ implementation of a lightweight, high-performance HTTP web server, similar to NGINX, with support for file uploads. It is designed to follow the specifications of [RFC 2616](https://datatracker.ietf.org/doc/html/rfc2616) closely and includes implementations of the GET, POST, and DELETE methods. The server is built to handle high loads, maintaining at least a 99% availability during stress testing, and can listen on multiple ports. Additionally, it supports executing Common Gateway Interface (CGI) scripts, allowing for dynamic content generation.

## Features

- **File Uploads:** Clients can upload files to the server.
- **HTTP Methods:** Implements GET, POST, and DELETE methods according to [RFC 2616](https://datatracker.ietf.org/doc/html/rfc2616).
- **High Availability:** Designed to maintain a 99% availability rate during stress tests.
- **Multiple Ports:** Can listen to multiple ports simultaneously.
- **CGI Support:** Executes CGI scripts for dynamic content generation.
- **Configuration File:** Server behavior can be customized through a configuration file.

## Getting Started

### Building the Server

1. Clone the repository:

    ```sh
    git clone https://github.com/rcabezas29/webserv.git
    cd webserv
    ```

2. Build the project using Make:

    ```sh
    make
    ```

### Running the Server

To run the server, use:

```sh
./webserv [configuration file]
```

The server will start and listen on the ports specified in the configuration file. You can then use a web browser or a tool like curl to interact with the server.

### Configuration

The server is configured through a simple text file. An example configuration might look like [this](config/webserv.conf):

```
server {
    listen 4242;
    server_name name;
    error_page 301 /file_path.html;
    client_max_body_size 1000000000;
    location / {
        accept GET POST DELETE;
        index index.php;
        autoindex on;
        root www;
        upload_directory www;
        cgi .php cgi_bin/php-cgi;
    }
}
server {
    listen 8080;
    server_name dpuente;
    error_page 301 400 www/colorful.html;
    client_max_body_size 128;
    location / {
        accept GET POST;
        index colorful.html;
        autoindex off;
        root www;
        upload_directory www;
        cgi .php cgi_bin/php-cgi;
    }
    location /subpage {
        accept GET;
        index youpi.bla;
        autoindex on;
        root www/YoupiBanane;
    }
}
```

This file has all the rules that the program can manage, most of them has the same names and prototypes as specified for NGINX configuration.

