/*
 *---------------------------------------------------------------------------*
 *                                                                           *
 * Copyright (C) 2010 Michael Richter (alias neldar)                         *
 * This program is free software; you can redistribute it and/or modify      *
 * it under the terms of the GNU General Public License version 3 as         *
 * published by the Free Software Foundation.                                *
 *                                                                           *
 *---------------------------------------------------------------------------*
*/


#include "bmlwriter.h"

void errorPrint(ssize_t bytes_read, ssize_t total_read, ssize_t bytes_written, ssize_t total_written, ssize_t remaining){
	printf("bytes_read(%i) total_read(%i)\n", bytes_read, total_read);
	printf("bytes_written(%i) total_written(%i)\n", bytes_written, total_written);
	printf("bytesRemaining(%lld)\n", (unsigned long long)remaining);
	
	if (total_written > 0)
		printf("WARNING: BML partition is partial written!\n");
}

/* Read from FD into the buffer BUF of size SIZE.
 * Return the number of bytes read if successful, -1 (setting errno) on failure.
 */
ssize_t iread (int fd, char *buf, size_t size){
	while (1)
	{
		ssize_t nread = read(fd, buf, size);
		if (! (nread < 0 && errno == EINTR))
			return nread;
	}
}

/* Wrapper around iread function to accumulate full blocks.
*/
static ssize_t iread_fullblock (int fd, char *buf, size_t size)
{
  ssize_t nread = 0;

  while (0 < size)
    {
      ssize_t ncurr = iread (fd, buf, size);
      if (ncurr < 0)
        return ncurr;
      if (ncurr == 0)
        break;
      nread += ncurr;
      buf   += ncurr;
      size  -= ncurr;
    }

  return nread;
}

int copy(int destfd, int sourcefd, int bufsize) {
	char buf[bufsize];
	ssize_t bytes_read, bytes_written, total_read, total_written  = 0;
	struct stat srcFileInfo;


	if (fstat(sourcefd, &srcFileInfo) != 0)
		return ERROR_FSTAT;

	do {
		bytes_read = iread_fullblock(sourcefd, buf, 4096);

		//read fail
		if (bytes_read < 0) {
			errorPrint(bytes_read, total_read, bytes_written, total_written, (srcFileInfo.st_size - total_read));
			if (total_written == 0)
				return ERROR_READ; //only read fail
			else
				return FATAL_READ_PARTIAL_WRITE; //read fail with partial write
		}
		
		total_read += bytes_read;
		
		//partial read fail
		if (bytes_read < 4096) {
			if (total_read == srcFileInfo.st_size){
				//finish is in sight - fill buffer
				memset(&buf[bytes_read], 0, 4096 - bytes_read);
			}
			else {
				errorPrint(bytes_read, total_read, bytes_written, total_written, (srcFileInfo.st_size - total_read));
				if (total_written == 0)
					return ERROR_PARTIAL_READ; //partial read before file ends
				else
					return FATAL_PARTIAL_READ_PARTIAL_WRITE; //partial read before file ends with partial write
			}
		}
		
		total_written += bytes_written = write(destfd, buf, 4096);
		if (bytes_written < 4096){ //partial write or fail
			errorPrint(bytes_read, total_read, bytes_written, total_written, (srcFileInfo.st_size - total_read));
			return FATAL_WRITE; //write fail
		}
	} while(bytes_read == 4096 && (srcFileInfo.st_size - total_read != 0));
	
	//finish
	printf("Successful: %i bytes written.\n", total_written);
	return SUCCESS;
}

int main(int argc, char** argv) {
	int dstfd, srcfd = 0;
	int ret = -1;
	
	printf("bmlwriter v1\n");

	if (argc != 3){
		printf("usage:\nbmlwriter sourcefile bmlpartition\n");
		return ERROR_INVALID_ARGC;
	}

	if (argv[1][0] == '-' && argv[1][1] == 0)
		srcfd = 0;
	else {
		srcfd = open(argv[1], O_RDONLY | O_LARGEFILE);
		if (srcfd < 0)
			return ERROR_SRC_OPEN;
	}


	dstfd = open(argv[2], O_RDWR | O_LARGEFILE);
	if (dstfd < 0)
		return ERROR_DST_OPEN;
		
	printf("source file: (%s)\n",argv[1]);
	printf("bml partition: (%s)\n",argv[2]);

	if (ioctl(dstfd, BML_UNLOCK_ALL, 0))
		return ERROR_UNLOCK_FAIL;

	ret = copy(dstfd, srcfd, 4096);
	if (ret != SUCCESS){
		printf("bmlwriter failed with exit(%i)\n", ret);
		if(errno != 0)
			printf("bmlwriter (errno: %i): %s\n", errno, strerror(errno));
		return ret;
	}

	fsync(dstfd);
	close(dstfd);
	close(srcfd);
	
	return SUCCESS;
}
