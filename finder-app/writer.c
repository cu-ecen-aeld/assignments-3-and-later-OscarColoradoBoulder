#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <syslog.h>

int main(int argc, char *argv[]) {
  openlog(NULL, 0, LOG_USER);

  if (argc != 3) {
    fprintf(stderr,
            "Error: Invalid number of arguments (received %d, expected 2)\n",
            argc - 1);
    syslog(LOG_ERR, "Invalid number of arguments: received %d, expected 2",
           argc - 1);
    closelog();
    return 1;
  }

  const char *filepath = argv[1];
  const char *writestr = argv[2];
  FILE *fp = fopen(filepath, "w");
  if (!fp) {
    fprintf(stderr, "Error: Could not open file %s\n", filepath);
    syslog(LOG_ERR, "Failed to open file: %s", filepath);
    closelog();
    return 1;
  }

  if (fprintf(fp, "%s", writestr) < 0) {
    fprintf(stderr, "Error: Failed to write to file: %s\n", filepath);
    syslog(LOG_ERR, "Unexpected error: Failed to write to file: %s", filepath);
    fclose(fp);
    closelog();
    return 1;
  }

  if (fclose(fp) != 0) {
    fprintf(stderr, "Error: Failed to close file: %s\n", filepath);
    syslog(LOG_ERR, "Unexpected error: Failed to close file: %s", filepath);
    closelog();
    return 1;
  }

  syslog(LOG_DEBUG, "Writing %s to %s", writestr, filepath);
  closelog();
  return 0;
}
