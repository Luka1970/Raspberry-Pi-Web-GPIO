#include <sys/types.h>
#include <sys/stat.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <errno.h>
#include <unistd.h>
#include <syslog.h>
#include <string.h>
#include <sstream>

#include "mysql.cpp"

using namespace std;

#define DAEMON_NAME "gpioserver"

int intRevision;
char* strRevision;

void process(){
	//syslog (LOG_NOTICE, "Writing to my Syslog");
	intRevision = mysql_get_revision();
	sprintf(strRevision, "%d", intRevision);

	mysql_log_insert(strRevision);
}

int main(int argc, char *argv[]) {

	//Set our Logging Mask and open the Log
	setlogmask(LOG_UPTO(LOG_NOTICE));
	openlog(DAEMON_NAME, LOG_CONS | LOG_NDELAY | LOG_PERROR | LOG_PID, LOG_USER);

	syslog(LOG_INFO, "gpioserverd Starting.");

	pid_t pid, sid;

	//Fork the Parent Process
	pid = fork();

	if (pid < 0) { exit(EXIT_FAILURE); }

	//We got a good pid, Close the Parent Process
	if (pid > 0) { exit(EXIT_SUCCESS); }

	pid = getpid();

	// Create PID file.
	FILE *fp = fopen("/var/run/gpioserver.pid", "w");
	if (!fp) {
		syslog(LOG_INFO, "Failed opening PID file.");
		exit(EXIT_FAILURE);
	}
	fprintf(fp, "%d\n", pid);
	fclose(fp);

	//Change File Mask
	umask(0);

	//Create a new Signature Id for our child
	sid = setsid();
	if (sid < 0) { exit(EXIT_FAILURE); }

	//Change Directory
	//If we cant find the directory we exit with failure.
	if ((chdir("/")) < 0) { exit(EXIT_FAILURE); }

	//Close Standard File Descriptors
	close(STDIN_FILENO);
	close(STDOUT_FILENO);
	close(STDERR_FILENO);

	//----------------
	//Main Process
	//----------------
	while(true){
		//Run our Process
		process();
		//Sleep for 1 second
		sleep(1);
	}
	//Close the log
	closelog ();
}