#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <string.h>
#include <time.h>
#include <unistd.h>
#include <sys/wait.h>
#include <curl/curl.h>
#include <sys/stat.h>
#include <sys/types.h>

#define MAX_PATH_LEN 1024
#define MAX_FILE_COUNT 1024

int main()
{
    // Download file zip
    CURL *curl;
    FILE *fp;
    CURLcode res;
    char *url = "https://drive.google.com/uc?id=1oDgj5kSiDO0tlyS7-20uz7t20X3atwrq&export=download";
    char outfilename[] = "binatang.zip";

    curl = curl_easy_init();
    if (curl)
    {
        fp = fopen(outfilename, "wb");
        curl_easy_setopt(curl, CURLOPT_URL, url);
        curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, fp);
        res = curl_easy_perform(curl);
        curl_easy_cleanup(curl);
        fclose(fp);
    }

    // check if file is downloaded
    fp = fopen(outfilename, "rb");
    if (fp)
    {
        printf("File is downloaded.\n");
        fclose(fp);
    }
    else
    {
        printf("Failed to download file.\n");
    }

    // Unzip file
    pid_t pid = fork();
    if (pid == 0)
    {
        // child process
        char *args[] = {"unzip", "binatang.zip", NULL};
        execvp("unzip", args);
    }
    else if (pid > 0)
    {
        // parent process
        wait(NULL);
        printf("Done unzipping.\n");
    }
    else
    {
        // fork failed
        printf("Fork failed.\n");
        exit(1);
    }

    // Random acak
    srand(time(NULL));
    DIR *dir;
    struct dirent *ent;
    char path[MAX_PATH_LEN];

    //random
    char files[MAX_FILE_COUNT][MAX_PATH_LEN];
    int file_count = 0;

    // open folder and read files
    if ((dir = opendir(".")) != NULL)
    {
        while ((ent = readdir(dir)) != NULL)
        {
            // filter JPG files
            if (ent->d_type == DT_REG && strstr(ent->d_name, ".jpg") != NULL)
            {
                snprintf(files[file_count], MAX_PATH_LEN, "%s", ent->d_name);
                file_count++;
            }
        }
        closedir(dir);
    }

    // select random file
    if (file_count > 0)
    {
        int rand_index = rand() % file_count;
        printf("Selected file: %s\n", files[rand_index]);
    }
    else
    {
        printf("No JPG files found in current directory.\n");
    }

    //Move file
    pid = fork();
    if (pid < 0) {
        fprintf(stderr, "Error: fork() failed\n");
        exit(EXIT_FAILURE);
    }
    else if (pid == 0) {
        // Child process
        execl("/bin/mkdir", "mkdir", "HewanAir", NULL);
        exit(EXIT_SUCCESS);
    }
    else {
        // Parent process
        wait(NULL);

        pid = fork();

        if (pid < 0) {
            fprintf(stderr, "Error: fork() failed\n");
            exit(EXIT_FAILURE);
        }
        else if (pid == 0) {
            // Child process
            execl("/bin/mkdir", "mkdir", "HewanDarat", NULL);
            exit(EXIT_SUCCESS);
        }
        else {
            // Parent process
            wait(NULL);

            pid = fork();

            if (pid < 0) {
                fprintf(stderr, "Error: fork() failed\n");
                exit(EXIT_FAILURE);
            }
            else if (pid == 0) {
                // Child process
                execl("/bin/mkdir", "mkdir", "HewanAmphibi", NULL);
                exit(EXIT_SUCCESS);
            }
            else {
                // Parent process
                wait(NULL);

                // Move files to appropriate folders
                execl("/bin/bash", "bash", "-c", "mv *_air.jpg ./HewanAir; mv *_darat.jpg ./HewanDarat; mv *_amphibi.jpg ./HewanAmphibi", NULL);
            }
        }
    }

    return 0;
}