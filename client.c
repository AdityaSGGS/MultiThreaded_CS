#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#define PORT 4444

int main(){

	int clientSocket, ret;
	struct sockaddr_in serverAddr;
	char buffer[1024]="";


	clientSocket = socket(AF_INET, SOCK_STREAM, 0);
	if(clientSocket < 0){
		printf("[-]Error in connection.\n");
		exit(1);
	}
	printf("[+]Client Socket is created.\n");

	memset(&serverAddr, '\0', sizeof(serverAddr));
	serverAddr.sin_family = AF_INET;
	serverAddr.sin_port = htons(PORT);
	serverAddr.sin_addr.s_addr = inet_addr("127.0.0.1");

	ret = connect(clientSocket, (struct sockaddr*)&serverAddr, sizeof(serverAddr));
	if(ret < 0){
		printf("[-]Error in connection.\n");
		exit(1);
	}
	printf("[+]Connected to Server.\n");
	int faculty=0,student=0,ta=0,n;
	printf("Please state your designation. Kindly enter 1 for faculty and 2 for student and 3 for Teaching assistant.\n");
	scanf("%d",&n);
	if (n==2)
	{
		student=1;
		printf("||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||\n");
		printf("||		Welcome to students section of the moodle.		 |||\n");
		printf("||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||\n");
	}else if (n==1)
	{
		faculty=1;
		printf("||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||\n");
		printf("||		Welcome to Faculty section of the moodle.		 |||\n");
		printf("||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||\n");
	}else if (n==3)
	{
		ta=1;
		printf("||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||\n");
		printf("||		Welcome to Teaching Assistant section of the moodle.		 |||\n");
		printf("||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||\n");
	}else{
		printf("You have given incorrect designation. Sorry.\n");
		exit(0);
	}


	///Seperate code for each person that means Students, Faculty, and Teaching Assistanat

	if (student==1)
	{
		//Sending the server that you are student
		memset(buffer, 0, sizeof(buffer));
		strcat(buffer,"Student");
		send(clientSocket, buffer, strlen(buffer), 0);
		

		//Receiving from the server if it has succesfully recognised you as the student
		memset(buffer, 0, sizeof(buffer));
		recv(clientSocket, buffer, 1024, 0);
		printf("Server: %s\n",buffer);


		//Code for the studnet side client containing the login, signup,and marks cheacking and the Submission, Assisngment cheacking

		int choice;
		printf("If you are new user kindly enter 1 for registration.\n");
		printf("If already registered press 2 for Login.\n");
		scanf("%d",&choice);
		if (choice==2)
		{
			char user_id[50];
			char password[50];
			printf("Kindly enter your user id: ");
			scanf("%s",&user_id);
			printf("\nKindly enter your password: ");
			scanf("%s",&password);
			memset(buffer, 0, sizeof(buffer));
			strcat(buffer,"SignIn ");
			strcat(buffer,user_id);
			strcat(buffer," ");
			strcat(buffer,password);

			//sending user_id and password to the server
			send(clientSocket, buffer, strlen(buffer), 0);
			memset(buffer, 0, sizeof(buffer));
			if(recv(clientSocket, buffer, 1024, 0) < 0){
				printf("[-]Error in receiving data.\n");
			}else{
				printf("Server: \t%s\n", buffer);
				char ch[100];
				if (strcmp(buffer,"Sorry. Invalid credentials.")!=0)
				{
					//for choice selection
					scanf("%s",ch);

					if (strcmp(ch,"1")==0)
					{
						memset(buffer, 0, sizeof(buffer));
						strcat(buffer,ch);
						send(clientSocket, buffer, strlen(buffer), 0);
						memset(buffer, 0, sizeof(buffer));
						recv(clientSocket, buffer, 1024, 0);
						printf("%s\n",buffer );


					//For assignment number
						memset(buffer, 0, sizeof(buffer));
						scanf("%s",ch);
						strcat(buffer,ch);
						send(clientSocket, buffer, strlen(buffer), 0);
						memset(buffer, 0, sizeof(buffer));
						recv(clientSocket, buffer, 1024, 0);
						printf("%s\n",buffer );
					}else if (strcmp(ch,"2"))
					{
						
					}else if (strcmp(ch,"3")==0)
					{
						//Code for uploading the assignment in the students section

						memset(buffer, 0, sizeof(buffer));
						strcat(buffer,ch);
						send(clientSocket, buffer, strlen(buffer), 0);
						memset(buffer, 0, sizeof(buffer));
						recv(clientSocket, buffer, 1024, 0);
						printf("%s\n",buffer );

						scanf("%s",ch);
						memset(buffer, 0, sizeof(buffer));
						strcat(buffer,ch);
						send(clientSocket, buffer, strlen(buffer), 0);

						//Code for sending the file data.
						FILE *fp;
  						char filename[100] ="";
  						printf("Please enter the path of the file to upload.\n");
  						scanf("%s",filename);
  						fp = fopen(filename, "r");
  						if (fp == NULL) {
   						 	perror("[-]Error in reading file.");
    						exit(1);
  						}

  						int n;
  						char data[1024] = {0};
 
  						while(fgets(data, 1024, fp) != NULL) {
  							printf("%s",data);
    						if (send(clientSocket, data, sizeof(data), 0) == -1) {
      							perror("[-]Error in Uploading the file.");
      							exit(1);
    						}
    						bzero(data, 1024);
 						}
 						bzero(data, 1024);
 						strcpy(buffer,"EOF");
 						send(clientSocket, data, sizeof(data), 0)
 						close(fp);

						}


					

				}
				
			}
		}else if (choice==1)
		{
			char name[50];
			char reg_no[50];
			char user_id[50];
			char password[50];
			char c;
			int i=0;
			printf("Please enter your name: \n");
			scanf("%c",&c);
			fgets(name, 50, stdin);
			while(name[i]!='\n'){
			    i++;
			}
			name[i]='\0';
			printf("Please enter your registration number.\n");
			scanf("%s",reg_no);
			printf("Kindly enter userid with which you wnat to continue. It should not contain space.\n");
			scanf("%s",user_id);
			printf("Please enter the password for your account.");
			scanf("%s",password);
			memset(buffer, 0, sizeof(buffer));
			strcat(buffer,"SignUp ");
			strcat(buffer,name);
			strcat(buffer,"|");
			strcat(buffer,reg_no);
			strcat(buffer," ");
			strcat(buffer,user_id);
			strcat(buffer," ");
			strcat(buffer,password);
			send(clientSocket, buffer, strlen(buffer), 0);
			if(recv(clientSocket, buffer, 1024, 0) < 0){
				printf("[-]Error in receiving data.\n");
			}else{
			printf("Server: \t%s\n", buffer);
			}

		}else{
			printf("Sorry you have entered invalid choice.\n");
			exit(0);
		}












		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////





	}else if (faculty==1)
	{
		memset(buffer, 0, sizeof(buffer));
		strcat(buffer,"Faculty");
		send(clientSocket, buffer, strlen(buffer), 0);

		//Taking the response from the sevrer if it clasiifies you as the TA
		memset(buffer, 0, sizeof(buffer));
		recv(clientSocket, buffer, 1024, 0);
		printf("Server: %s\n",buffer );

		int choice;
		printf("If you are new Faculty Member kindly enter 1 for registration.\n");
		printf("If already registered press 2 for Login.\n");
		scanf("%d",&choice);

		if (choice==1)				//Code for the new faculty registration
		{
			char name[50];
			//char reg_no[50];
			char user_id[50];
			char password[50];
			char c;
			int i=0;

			printf("Please enter your name: \n");
			scanf("%c",&c);
			fgets(name, 50, stdin);
			while(name[i]!='\n'){
			    i++;
			}
			name[i]='\0';
			printf("Kindly enter userid with which you wnat to continue. It should not contain space.\n");
			scanf("%s",user_id);
			printf("Please enter the password for your account.");
			scanf("%s",password);

			//Code for sending the data to the server of the registration
			memset(buffer, 0, sizeof(buffer));
			strcat(buffer,"SignUp ");
			strcat(buffer,name);
			strcat(buffer,"|");
			strcat(buffer,user_id);
			strcat(buffer," ");
			strcat(buffer,password);
			send(clientSocket, buffer, strlen(buffer), 0);
			memset(buffer, 0, sizeof(buffer));
			if(recv(clientSocket, buffer, 1024, 0) < 0){
				printf("[-]Error in receiving data.\n");
			}else{
			printf("Server: \t%s\n", buffer);
			}
		}else if (choice==2)		//Code for the Already registered faculty member
		{
			char user_id[50];
			char password[50];
			printf("Kindly enter your user id: ");
			scanf("%s",&user_id);
			printf("\nKindly enter your password: ");
			scanf("%s",&password);
			memset(buffer, 0, sizeof(buffer));
			strcat(buffer,"SignIn ");
			strcat(buffer,user_id);
			strcat(buffer," ");
			strcat(buffer,password);

			//sending user_id and password to the server
			send(clientSocket, buffer, strlen(buffer), 0);
			memset(buffer, 0, sizeof(buffer));
			if(recv(clientSocket, buffer, 1024, 0) < 0){
				printf("[-]Error in receiving data.\n");
			}else{
				printf("Server: \t%s\n", buffer);
				char ch[50];
				if (strcmp(buffer,"Sorry. Invalid credentials.")!=0)
				{
					//for sending filename 
					scanf("%s",ch);
					memset(buffer, 0, sizeof(buffer));
					strcat(buffer,ch);
					send(clientSocket, buffer, strlen(buffer), 0);

					//Code for sending the file data.
					FILE *fp;
  					char filename[100] ="";
  					printf("Please enter the path of the file to upload.\n");
  					scanf("%s",filename);
  					fp = fopen(filename, "r");
  					if (fp == NULL) {
   						 perror("[-]Error in reading file.");
    					exit(1);
  					}

  					int n;
  					char data[1024] = {0};
 
  					while(fgets(data, 1024, fp) != NULL) {
  					printf("%s",data);
    					if (send(clientSocket, data, sizeof(data), 0) == -1) {
      							perror("[-]Error in Uploading the file.");
      							exit(1);
    				}
    				bzero(data, 1024);
 					}
					

				}
				
			}

		}else{						//Invalid choice

		}



		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	}else if (ta==1)
	{
		memset(buffer, 0, sizeof(buffer));
		strcat(buffer,"TA");
		send(clientSocket, buffer, strlen(buffer), 0);

		//Taking the response from the sevrer if it clasiifies you as the TA
		memset(buffer, 0, sizeof(buffer));
		recv(clientSocket, buffer, 1024, 0);
		printf("Server: %s\n",buffer );

		//Code for the functions for the Teaching Assistant
		int choice;
		printf("If you are new Teaching Assistant kindly enter 1 for registration.\n");
		printf("If already registered press 2 for Login.\n");
		scanf("%d",&choice);

		if (choice==1)		//Code for the new Registration
		{


			//Code for scanning the Name and userid and password from the Teaching Assistance

			char name[50];
			//char reg_no[50];
			char user_id[50];
			char password[50];
			char c;
			int i=0;

			printf("Please enter your name: \n");
			scanf("%c",&c);
			fgets(name, 50, stdin);
			while(name[i]!='\n'){
			    i++;
			}
			name[i]='\0';
			printf("Kindly enter userid with which you wnat to continue. It should not contain space.\n");
			scanf("%s",user_id);
			printf("Please enter the password for your account.");
			scanf("%s",password);

			//Code for sending the data to the server of the registration
			memset(buffer, 0, sizeof(buffer));
			strcat(buffer,"SignUp ");
			strcat(buffer,name);
			strcat(buffer,"|");
			strcat(buffer,user_id);
			strcat(buffer," ");
			strcat(buffer,password);
			send(clientSocket, buffer, strlen(buffer), 0);
			if(recv(clientSocket, buffer, 1024, 0) < 0){
				printf("[-]Error in receiving data.\n");
			}else{
			printf("Server: \t%s\n", buffer);
			}

		}else if (choice==2)		//Code for the login 
		{
			char user_id[50];
			char password[50];
			printf("Kindly enter your user id: ");
			scanf("%s",&user_id);
			printf("\nKindly enter your password: ");
			scanf("%s",&password);
			memset(buffer, 0, sizeof(buffer));
			strcat(buffer,"SignIn ");
			strcat(buffer,user_id);
			strcat(buffer," ");
			strcat(buffer,password);

			//sending user_id and password to the server
			send(clientSocket, buffer, strlen(buffer), 0);
			memset(buffer, 0, sizeof(buffer));
			if(recv(clientSocket, buffer, 1024, 0) < 0){
				printf("[-]Error in receiving data.\n");
			}else{
				printf("Server: \t%s\n", buffer);
				char ch[50];
				if (strcmp(buffer,"Sorry. Invalid credentials.")!=0)
				{
					//for registration number input of  the student to update the marks
					scanf("%s",ch);
					memset(buffer, 0, sizeof(buffer));
					strcat(buffer,ch);
					send(clientSocket, buffer, strlen(buffer), 0);
					memset(buffer, 0, sizeof(buffer));
					recv(clientSocket, buffer, 1024, 0);
					printf("Server: %s\n",buffer);

					if (strcmp(buffer,"No Student found.")!=0)
					{
						//for Assignment number 
					scanf("%s",ch);
					memset(buffer, 0, sizeof(buffer));
					strcat(buffer,ch);
					send(clientSocket, buffer, strlen(buffer), 0);
					memset(buffer, 0, sizeof(buffer));
					recv(clientSocket, buffer, 1024, 0);
					printf("Server: %s\n",buffer);

					//For Marks Input


					scanf("%s",ch);
					memset(buffer, 0, sizeof(buffer));
					strcat(buffer,ch);
					send(clientSocket, buffer, strlen(buffer), 0);
					memset(buffer, 0, sizeof(buffer));
					recv(clientSocket, buffer, 1024, 0);
					printf("Server: %s\n",buffer);
					}else{

					}

					

				}
				
			}



		}

	}


	//Client side code
	
	/*
	while(1){
		printf("Client: \t");
		scanf("%s", &buffer[0]);
		send(clientSocket, buffer, strlen(buffer), 0);

		if(strcmp(buffer, ":exit") == 0){
			close(clientSocket);
			printf("[-]Disconnected from server.\n");
			exit(1);
		}

		if(recv(clientSocket, buffer, 1024, 0) < 0){
			printf("[-]Error in receiving data.\n");
		}else{
			printf("Server: \t%s\n", buffer);
		}
	}
	*/
	return 0;
}