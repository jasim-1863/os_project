typedef unsigned int   uint;
typedef unsigned short ushort;
typedef unsigned char  uchar;
typedef uint pde_t;

// File access log entry
struct filelog {
  int pid;                    // Process ID
  char filename[256];         // Filename (with null terminator)
};