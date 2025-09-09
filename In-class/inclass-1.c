/*
 * filecopy.c
 * ------------------------------
 * Author: Gabriel Giancarlo
 * Student ID: 2405449
 * Class: Operating Systems 380
 *
 * Project Specification:
 * Write a C program (filecopy.c) that copies the contents of one file 
 * to a destination file. This program works by getting the name of the 
 * source and destination files from the command line using argv.
 * 
 * Requirements:
 * - Use only the following system calls:
 *     int open(const char *pathname, int flags);
 *     ssize_t read(int fd, void *buf, size_t count);
 *     ssize_t write(int fd, const void *buf, size_t count);
 *     int close(int fd);
 * - The program must copy the contents of the source file into the 
 *   destination file, overwriting it if it already exists.
 * - If the wrong number of arguments are provided, display a usage message.
 */

 #include <fcntl.h>     // for open flags (O_RDONLY, O_WRONLY, etc.)
 #include <unistd.h>    // for read, write, close
 #include <stdlib.h>    // for exit
 #include <stdio.h>     // for perror
 
 #define BUF_SIZE 4096  // Buffer size for file copying
 
 int main(int argc, char *argv[]) {
     // Check that the user provided exactly 2 arguments (source and destination)
     if (argc != 3) {
         write(2, "Usage: filecopy <source> <destination>\n", 39);
         exit(1); // Exit with error code
     }
 
     // Open the source file in read-only mode
     int src_fd = open(argv[1], O_RDONLY);
     if (src_fd < 0) { // If open fails, print error and exit
         perror("Error opening source file");
         exit(1);
     }
 
     // Open/create the destination file with write-only access
     // O_CREAT: create file if it doesn't exist
     // O_TRUNC: truncate file to 0 length if it already exists
     // Permissions: rw-r--r-- (0644)
     int dest_fd = open(argv[2], O_WRONLY | O_CREAT | O_TRUNC, 0644);
     if (dest_fd < 0) {
         perror("Error opening/creating destination file");
         close(src_fd); // Close source file before exiting
         exit(1);
     }
 
     char buffer[BUF_SIZE];
     ssize_t bytes_read, bytes_written;
 
     // Read chunks from source and write to destination until done
     while ((bytes_read = read(src_fd, buffer, BUF_SIZE)) > 0) {
         bytes_written = write(dest_fd, buffer, bytes_read);
         if (bytes_written != bytes_read) {
             perror("Error writing to destination file");
             close(src_fd);
             close(dest_fd);
             exit(1);
         }
     }
 
     // If read() returned -1, an error occurred
     if (bytes_read < 0) {
         perror("Error reading from source file");
     }
 
     // Close both files
     close(src_fd);
     close(dest_fd);
 
     return 0; // Successful completion
 }
 