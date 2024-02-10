#include <dirent.h>
#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

int main() {
    DIR *directory;
    struct dirent *entry;
    directory = opendir("/proc");
    printf("  PID CMD\n");
    while ((entry = readdir(directory)) != NULL) {
        if (entry->d_type == DT_DIR) {
            // change directory to integer
            int pid = atoi(entry->d_name);
            char status_path[256];
            //set the status path of the process 
            snprintf(status_path, sizeof(status_path), "/proc/%d/status", pid);

            int status_file = open(status_path, O_RDONLY);
            if (status_file != -1) {

                char process_info[256];
                //read contents of status file
                ssize_t bytesRead = read(status_file, process_info, sizeof(process_info) - 1);
                process_info[bytesRead] = '\0';

                char *name_line = strtok(process_info, "\n");
                if (name_line != NULL) {
                    char processName[256] = "";
                    sscanf(name_line, "Name:\t%[^\n]", processName);
                    //printf("Process ID: %d, Process Name: %s\n", pid, processName);
                    printf("%5d %s\n", pid, processName);
                }
                close(status_file);
            }
        }
    }
    closedir(directory);
    return 0;
}
