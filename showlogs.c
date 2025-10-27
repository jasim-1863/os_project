#include "types.h"
#include "user.h"

int
main(int argc, char *argv[])
{
  struct filelog logs[10];  // Smaller buffer
  int count, i;
  
  count = getlogs(logs, 10);  // Request only 10 entries
  
  if(count < 0) {
    printf(2, "Error: failed to get logs\n");
    exit();
  }
  
  if(count == 0) {
    printf(1, "No file access logs found.\n");
    exit();
  }
  
  printf(1, "File Access Log (%d entries):\n", count);
  printf(1, "PID\tFilename\n");
  printf(1, "---\t--------\n");
  
  for(i = 0; i < count; i++) {
    printf(1, "%d\t%s\n", logs[i].pid, logs[i].filename);
  }
  
  exit();
}
