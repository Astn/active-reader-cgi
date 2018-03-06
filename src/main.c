#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <string.h>
#include <unistd.h>

int main(int argc, char** argv, char** envp) {

	// print out HTTP headers
	printf("Content-type: text/html\n"); // required as part of HTTP protocol
	printf("\n");

	// print out HTTP content
	// print all variables
	printf("<html>\n"
		"<head><title>Active Tag Configuration</title></head>\n"
		"<body>\n");

	printf("ENVIRONMENT\n");
	printf("<pre>");
	int envArgCount = 0;
	while(envp[envArgCount] != NULL){
	    printf("%s\n",envp[envArgCount++]);
	}

	printf("</pre>\n");

	printf("InputArgs\n");


	printf("<pre>\n");
	for (int i = 0; i < argc; ++i){
		printf("%s\n",argv[i]);
	}
	printf("</pre>\n");


	printf("Parsing http query string");

        // parameters we handle:
	/*
	EventName=????????????????  (ASCII)
	Point Name=???????????????? (ASCII)
	LoopID=0..7
	ActiveChannel=0..7
	ActivePower=0..100
	ActiveTime=YYYYMMDD-HHMMSS
	*/
	char eventName[100];
	char pointName[100];
	int loopId = 0;
	int activeChannel =0;
	int activePower = 0;
	char activeTime[50];

	char* argEventName = "EventName";

	printf("<pre>");
	char * queryString = getenv("QUERY_STRING");
	if(queryString != NULL){
		printf("queryString contents: \n");
		// as tokens
		char * pch =  strtok (queryString, "&");
		while(pch != NULL){
			printf ("Found: %s\n",pch);

			if(strncmp(pch,argEventName,strlen(argEventName))==0 && strlen(pch) > strlen(argEventName)+1){
				strncpy(eventName, pch+strlen(argEventName)+1, sizeof(eventName));
				printf("Captured %s as %s\n",argEventName, eventName);
			}

			pch = strtok (NULL, "&");
		}
	}
	printf("</pre>");

	printf("</body>\n"
		"</html>\n");

}
