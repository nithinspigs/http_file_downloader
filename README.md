# HTTP File Downloader

## Use

Compile http-client.c by running the command "make" in the http_file_downloader directory. Run the program by specifying the host name, port number (typically 80), and the file path as arguments, in that order. For example, if I want to download the image at the address [http://www.cs.columbia.edu/~jae/images/jae-half-profile.jpg](http://www.cs.columbia.edu/~jae/images/jae-half-profile.jpg), I can run the command

> ./http-client www.cs.columbia.edu 80 /~jae/images/jae-half-profile.jpg

Note that HTTP compatability and other errors occasionally occur, causing the download to fail. In these cases, the status line is printed to the command line.

## Summary
http-client is an executable file, compiled from http-client.c, that downloads files hosted online given a host name, port number (typically 80), and the file path. It works similarly to wget in this regard. The protocol used is HTTP 1.0.

## Necessary Header Files
    stdio.h
    stdlib.h
    string.h
    assert.h
    unistd.h
    sys/wait.h
    sys/stat.h
    arpa/inet.h
    sys/types.h
    sys/socket.h
    netdb.h
