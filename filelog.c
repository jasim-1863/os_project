#include "types.h"
#include "defs.h"
#include "param.h"
#include "spinlock.h"

#define MAX_LOG_ENTRIES 100

struct filelog file_logs[MAX_LOG_ENTRIES];
int log_count = 0;
int log_index = 0;
struct spinlock log_lock;

void
filelog_init(void)
{
  initlock(&log_lock, "filelog");
  log_count = 0;
  log_index = 0;
}

void
filelog_add(int pid, char* filename)
{
  acquire(&log_lock);
  
  // Copy filename safely (truncate if too long)
  int len = strlen(filename);
  if(len >= 256) len = 255;
  
  strncpy(file_logs[log_index].filename, filename, len);
  file_logs[log_index].filename[len] = '\0';
  file_logs[log_index].pid = pid;
  
  log_index = (log_index + 1) % MAX_LOG_ENTRIES;
  if(log_count < MAX_LOG_ENTRIES)
    log_count++;
    
  release(&log_lock);
}

int
filelog_get(struct filelog* logs, int max_entries)
{
  int i, count;
  
  acquire(&log_lock);
  
  count = log_count;
  if(count > max_entries)
    count = max_entries;
    
  // Simple sequential copy - just copy the most recent entries
  for(i = 0; i < count; i++) {
    int src_index = (log_index - count + i + MAX_LOG_ENTRIES) % MAX_LOG_ENTRIES;
    logs[i] = file_logs[src_index];
  }
  
  release(&log_lock);
  return count;
}
