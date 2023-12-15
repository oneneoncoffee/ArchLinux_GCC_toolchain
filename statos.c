// File name: statos.c  
// Made for the Orange Pi Zero 3 and/or Orange pi pc plus computers. 

// header liberys for this project. 
#include <sys/sysinfo.h>
#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <grp.h>

int main() {
  struct sysinfo sys_info;
  struct group* grp;

    gid_t gid;
    char** users;

  int days, hours, mins, x = 1;

  system("clear");  
  printf("\033[4;40m            Systems information                   \033[0;m \n");
  if(sysinfo(&sys_info) != 0)
    perror("sysinfo");

  days = sys_info.uptime / 86400;
  hours = (sys_info.uptime / 3600) - (days * 24);
  mins = (sys_info.uptime / 60) - (days * 1440) - (hours * 60);
  printf("╔═════════════════════════════════════════════════\n");
  printf("║ Uptime: %ddays, %dhours, %dminutes, %ldseconds       \n",
                      days, hours, mins, sys_info.uptime % 60);
  printf("║ Load Avgs: 1min(%ld) 5min(%ld) 15min(%ld)            \n",
          sys_info.loads[0], sys_info.loads[1], sys_info.loads[2]);
  printf("║ Total Ram: %ldk \t Free: %ldk                        \n", sys_info.totalram / 1024, sys_info.freeram / 1024);
  printf("║ Shared Ram:  %ldk ", sys_info.sharedram / 1024);
  printf(" Buffered Ram: %ldk \n", sys_info.bufferram / 1024);
  printf("║ Total Swap: %ldk \t Free swap: %ldk \n", sys_info.totalswap / 1024, sys_info.freeswap / 1024);
  printf("║ Total High Memory: %ldk  Free high memory: %ldk \n", sys_info.totalhigh / 1024, sys_info.freehigh / 1024);
  printf("║ \n");
  printf("║ Total Number of processes: %d \n", sys_info.procs);  
  gid = getgid();
    printf("║ Group ID: %d", gid);
    if((grp = getgrgid(gid)) == NULL ) return 1;
    printf(" Group %s ", grp->gr_name );
    printf("\n║  Users in your group ");
    for( users = grp->gr_mem; *users != NULL; users++,++x ); printf( "%d", ++x);     
    if(strcmp(grp->gr_passwd,"x") == 0) printf("  Password is protected by shadow file. \n");
    else printf("║ Password: %s ", grp->gr_passwd);
FILE *file;
char temp[128];
char path[40];
int cpu_count = 0;
printf("║ \n");
while(1) {
sprintf(path, "/sys/class/thermal/thermal_zone%d/temp", cpu_count);
file = fopen(path, "r");
if (file == NULL) { break; }
int temperature;
fscanf(file, "%d", &temperature);
fclose(file);
printf("║CPU/CORE %d Temperature: %.2f C\n", cpu_count, temperature / 1000.0);
cpu_count++;
}
printf("║Total cpus/cores %d\n", cpu_count);

    return 0;
}
