
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <grp.h>
#include <pthread.h>
#include <ctype.h>


// mutex lock
pthread_mutex_t lock;

void * handle_connection (void* cs);

#define PORT 4444

int assignment_count=0;
struct students_info{
	char name[50];
	char reg_no[50];
	char id[50];
	char password[50];
	char marks[10][50];
	struct students_info *next;
};
struct students_info *head=NULL,*tail=NULL,*p,*q,*temp;


struct faculty_pass{
	char name[50];
	char id[50];
	char password[50];
	struct faculty_pass *next;
};
struct faculty_pass *fa_head,*fa_tail,*fa_temp,*fa_p,*fa_q;

struct ta_pass{
	char name[50];
	char id[50];
	char password[50];
	struct ta_pass *next;
};
struct ta_pass *ta_head,*ta_tail,*ta_temp,*ta_p,*ta_q;


int main(){

	int sockfd, ret;
	struct sockaddr_in serverAddr;

	int newSocket;
	struct sockaddr_in newAddr;

	socklen_t addr_size;

	char buffer[1024];
	pid_t childpid;

	sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if(sockfd < 0){
		printf("[-]Error in connection.\n");
		exit(1);
	}
	printf("[+]Server Socket is created.\n");

	memset(&serverAddr, '\0', sizeof(serverAddr));
	serverAddr.sin_family = AF_INET;
	serverAddr.sin_port = htons(PORT);
	serverAddr.sin_addr.s_addr = inet_addr("127.0.0.1");

	ret = bind(sockfd, (struct sockaddr*)&serverAddr, sizeof(serverAddr));
	if(ret < 0){
		printf("[-]Error in binding.\n");
		exit(1);
	}
	printf("[+]Bind to port %d\n", 4444);

	if(listen(sockfd, 10) == 0){
		printf("[+]Listening....\n");
	}else{
		printf("[-]Error in binding.\n");
	}


	while(1){
		newSocket = accept(sockfd, (struct sockaddr*)&newAddr, &addr_size);
		if(newSocket < 0){
			exit(1);
		}
		printf("Connection accepted from %s:%d\n", inet_ntoa(newAddr.sin_addr), ntohs(newAddr.sin_port));

		pthread_t t;
		int *pclient = malloc(sizeof(int));
		*pclient = newSocket;
			
			// create thread for client handling
		pthread_create(&t, NULL, handle_connection, pclient);

	}

	close(newSocket);


	return 0;
}





void * handle_connection (void* p_cs) {

	// read in client thread pointer and free after
	int newSocket = *((int*)p_cs);
	free(p_cs);
	char buffer[1024];

	char operation[50];							//Array for signup or login cheaking
			
	recv(newSocket, buffer, 1024, 0);

	if (strcmp(buffer,"Student")==0)
	{
		//Code for Getting the Type of user
		memset(buffer, 0, sizeof(buffer));
		strcat(buffer,"Hello Students.");
		send(newSocket, buffer, strlen(buffer), 0);
		memset(buffer, 0, sizeof(buffer));
		recv(newSocket, buffer, 1024, 0);

		int i=0;
		while(buffer[i]!=' '){
		operation[i]=buffer[i];
		i++;
		}
		operation[i]='\0';
		i++;



		if (strcmp(operation,"SignIn")==0)			//Server code for SignIn
					{
					char userid[50];
					char password[50];

					int k=0;
					while(buffer[i]!=' '){
						userid[k]=buffer[i];
						i++;
						k++;
					}
					userid[k]='\0';
					k=0;
					i++;
					while(buffer[i]!='\0'){
						password[k]=buffer[i];
						k++;
						i++;
					}
					password[k]='\0';
					int found=0;
					p=head;
					while(p!=NULL && found==0){
						if (strcmp(p->id,userid)==0)
						{
							if (strcmp(p->password,password)==0)
							{
								found=1;
								q=p;
								break;
							}
						}
						p=p->next;
					}

					if (found)
					{
						memset(buffer, 0, sizeof(buffer));
						strcat(buffer,"Welcome ");
						strcat(buffer,q->name);
						strcat(buffer,"\n");
						strcat(buffer,"What are you willing to do.\n Press 1 for getting your marks\n2 to see the Assignment\n3 do Upload Assignment.");
						send(newSocket, buffer, strlen(buffer), 0);
						memset(buffer, 0, sizeof(buffer));
						recv(newSocket, buffer, 1024, 0);
						if (strcmp(buffer,"1")==0)
						{
							int assignment_no=0;
							memset(buffer, 0, sizeof(buffer));
							strcat(buffer,"For which Assignment you wish to see the marks.");
							send(newSocket, buffer, strlen(buffer), 0);
							memset(buffer, 0, sizeof(buffer));

							recv(newSocket, buffer, 1024, 0);

							int len;
   							 len = strlen(buffer);

							for(k=0; k<len; k++){
									assignment_no = assignment_no * 10 + ( buffer[k] - '0' );
							}
    						//printf("%d",n);
							//sprintf(buffer,"%d",assignment_no);
							memset(buffer, 0, sizeof(buffer));
							strcat(buffer,q->marks[assignment_no]);
							send(newSocket, buffer, strlen(buffer), 0);

						}else if (strcmp(buffer,"2")==0)
						{
							//Code for sending file over the connectio 
						}else if (strcmp(buffer,"3")==0)
						{
							//Code for students file upload


						char stu_temp_file_name[50],ta_temp_ass[50],ta_temp_marks[50];
						int assignment_no=0;
						memset(buffer, 0, sizeof(buffer));
						strcat(buffer,"Which Assignment you want to upload?");
						send(newSocket, buffer, strlen(buffer), 0);
						memset(buffer, 0, sizeof(buffer));
						recv(newSocket, buffer, 1024, 0);
						strcpy(stu_temp_file_name,buffer);

						int n;
  						FILE *fp;
  						char filename[50];
  						memset(filename, 0, sizeof(filename));
  						memset(buffer, 0, sizeof(buffer));
  						//strcat(filename,"\"");
  						strcat(filename,"/students/");
  						strcat(filename,q->reg_no);
  						strcat(filename,"_");
  						strcat(filename,stu_temp_file_name);
  						strcat(filename,".txt");
  						//strcat(filename,"\"");
  						//char buffer[SIZE];
 
  						fp = fopen(filename, "w");
  						if(fp==NULL){
  							printf("Error opening file.");
  						}
  						while (1) {
    					n = recv(newSocket, buffer, 1024, 0);
    					printf("%s",buffer);
    					if (strcmp(buffer,"EOF")){
      					break;
      					//return;
    					}
    					fputs(buffer,fp);
    					bzero(buffer, 1024);
  						}








						}
					}else{
						memset(buffer, 0, sizeof(buffer));
						strcat(buffer,"Sorry. Invalid credentials.");
						send(newSocket, buffer, strlen(buffer), 0);
					}
					/*
					memset(buffer, 0, sizeof(buffer));
					strcat(buffer,"Username is: ");
					strcat(buffer,userid);
					strcat(buffer,"and password is: ");
					strcat(buffer,password);
					send(newSocket, buffer, strlen(buffer), 0);
					*/
				
			}else if (strcmp(operation,"SignUp")==0)				//Server code for SignUp
			{
				int k=0;
				char name[50];
				char reg_no[50];
				char id[50];
				char password[50];

				while(buffer[i]!='|'){
					name[k]=buffer[i];
					i++;
					k++;
				}
				name[k]='\0';
				i++;
				k=0;
				while(buffer[i]!=' '){
					reg_no[k]=buffer[i];
					i++;
					k++;
				}
				reg_no[k]='\0';
				i++;
				k=0;
				while(buffer[i]!=' '){
					id[k]=buffer[i];
					i++;
					k++;
				}
				id[k]='\0';
				i++;
				k=0;

				while(buffer[i]!='\0'){
					password[k]=buffer[i];
					i++;
					k++;
				}
				password[k]='\0';
				
				temp=(struct students_info *)malloc(sizeof(struct students_info));
				strcpy(temp->name,name);
				strcpy(temp->reg_no,reg_no);
				strcpy(temp->id,id);
				strcpy(temp->password,password);
				for (i = 0; i < 10; ++i)
				{
					sprintf(temp->marks[i],"Marks Unavailable.");
				}
				temp->next=NULL;
				if (head==NULL)
				{
					head=temp;
					tail=temp;
				}else{
					tail->next=temp;
					tail=temp;
				}
				printf("Record in database are: \n");
				p=head;
				while(p){
					printf("Name: %s\n",p->name);
					printf("Reg No: %s\n",p->reg_no);
					printf("Username: %s\n",p->id);
					printf("Password: %s\n",p->password);
					printf("----------------------------------------------------------------\n");
					p=p->next;

				}
				memset(buffer, 0, sizeof(buffer));
				strcat(buffer,"Registration Successfull!\n");
				send(newSocket, buffer, strlen(buffer), 0);
			}

	}else if (strcmp(buffer,"TA")==0)									//Code for the operation of the Teaching Assistanat
	{
		
			memset(buffer, 0, sizeof(buffer));
			strcat(buffer,"Hello Teaching Assistant.");
			send(newSocket, buffer, strlen(buffer), 0);
			memset(buffer, 0, sizeof(buffer));
			recv(newSocket, buffer, 1024, 0);

			int i=0;
			while(buffer[i]!=' '){
				operation[i]=buffer[i];
				i++;
			}
			operation[i]='\0';
			i++;

				if (strcmp(operation,"SignIn")==0)			//Server code for SignIn
				{
					char userid[50];
					char password[50];

					int k=0;
					while(buffer[i]!=' '){
						userid[k]=buffer[i];
						i++;
						k++;
					}
					userid[k]='\0';
					k=0;
					i++;
					while(buffer[i]!='\0'){
						password[k]=buffer[i];
						k++;
						i++;
					}
					password[k]='\0';
					int found=0;
					ta_p=ta_head;
					while(ta_p){
						if (strcmp(ta_p->id,userid)==0)
						{
							if (strcmp(ta_p->password,password)==0)
							{
								found=1;
								ta_q=ta_p;
								break;
							}
						}
						ta_p=ta_p->next;
					}

					if (found)
					{
						char ta_temp_reg[50],ta_temp_ass[50],ta_temp_marks[50];
						int assignment_no=0;
						memset(buffer, 0, sizeof(buffer));
						strcat(buffer,"Welcome ");
						strcat(buffer,ta_q->name);
						strcat(buffer,"\n");
						strcat(buffer,"Please enter the Registration No of the sudent whose marks you want to update.");
						send(newSocket, buffer, strlen(buffer), 0);
						memset(buffer, 0, sizeof(buffer));
						recv(newSocket, buffer, 1024, 0);
						strcpy(ta_temp_reg,buffer);

						int found=0;
						p=head;
						while(p){
							if (strcmp(p->reg_no,ta_temp_reg)==0)
							{
								found=1;
								q=p;
								break;
							}
							p=p->next;
						}

						if (found)
						{
							//Code for getting Assignment number
						memset(buffer, 0, sizeof(buffer));
						strcat(buffer,"Which Assignment you want to update the marks.");
						send(newSocket, buffer, strlen(buffer), 0);
						memset(buffer, 0, sizeof(buffer));
						recv(newSocket, buffer, 1024, 0);
						int len;
   							 len = strlen(buffer);

							for(k=0; k<len; k++){
									assignment_no = assignment_no * 10 + ( buffer[k] - '0' );
							}
						//sprintf(buffer,"%d",assignment_no);

						//Code for getting the marks from the client 
						memset(buffer, 0, sizeof(buffer));
						strcat(buffer,"Please enter the marks you want to update for the student");
						send(newSocket, buffer, strlen(buffer), 0);
						memset(buffer, 0, sizeof(buffer));
						recv(newSocket, buffer, 1024, 0);
						strcpy(q->marks[assignment_no],buffer);
						memset(buffer, 0, sizeof(buffer));
						strcat(buffer,"Marks are Successfully UPDATED.");
						send(newSocket, buffer, strlen(buffer), 0);
						}else{
							memset(buffer, 0, sizeof(buffer));
							strcat(buffer,"No Student found.");
							send(newSocket, buffer, strlen(buffer), 0);
						}
						

					}else{
						memset(buffer, 0, sizeof(buffer));
						strcat(buffer,"Sorry. Invalid credentials.");
						send(newSocket, buffer, strlen(buffer), 0);
					}
					/*
					memset(buffer, 0, sizeof(buffer));
					strcat(buffer,"Username is: ");
					strcat(buffer,userid);
					strcat(buffer,"and password is: ");
					strcat(buffer,password);
					send(newSocket, buffer, strlen(buffer), 0);
					*/
				
			}else if (strcmp(operation,"SignUp")==0)				//Server code for SignUp
			{
				int k=0;
				char name[50];
				//char reg_no[50];
				char id[50];
				char password[50];

				while(buffer[i]!='|'){
					name[k]=buffer[i];
					i++;
					k++;
				}
				name[k]='\0';
				i++;
				k=0;
				while(buffer[i]!=' '){
					id[k]=buffer[i];
					i++;
					k++;
				}
				id[k]='\0';
				i++;
				k=0;

				while(buffer[i]!='\0'){
					password[k]=buffer[i];
					i++;
					k++;
				}
				password[k]='\0';
				
				ta_temp=(struct ta_pass *)malloc(sizeof(struct ta_pass));
				strcpy(ta_temp->name,name);
				//strcpy(ta_temp->reg_no,reg_no);
				strcpy(ta_temp->id,id);
				strcpy(ta_temp->password,password);
				ta_temp->next=NULL;
				if (ta_head==NULL)
				{
					ta_head=ta_temp;
					ta_tail=ta_temp;
				}else{
					ta_tail->next=ta_temp;
					ta_tail=ta_temp;
				}
				printf("TA Records in database are: \n");
				ta_p=ta_head;
				while(ta_p){
					printf("Name: %s\n",ta_p->name);
					//printf("Reg No: %s\n",ta_p->reg_no);
					printf("Username: %s\n",ta_p->id);
					printf("Password: %s\n",ta_p->password);
					printf("----------------------------------------------------------------\n");
					ta_p=ta_p->next;

				}
				memset(buffer, 0, sizeof(buffer));
				strcat(buffer,"Registration Successfull!\n");
				send(newSocket, buffer, strlen(buffer), 0);
			}


	}else if (strcmp(buffer,"Faculty")==0)
	{
			memset(buffer, 0, sizeof(buffer));
			strcat(buffer,"Hello Faculty Member.");
			send(newSocket, buffer, strlen(buffer), 0);
			memset(buffer, 0, sizeof(buffer));
			recv(newSocket, buffer, 1024, 0);

			int i=0;
			while(buffer[i]!=' '){
				operation[i]=buffer[i];
				i++;
			}
			operation[i]='\0';
			i++;

			//Code for the Sign In opetion 
			if (strcmp(operation,"SignIn")==0)			//Server code for SignIn
				{
					char userid[50];
					char password[50];

					int k=0;
					while(buffer[i]!=' '){
						userid[k]=buffer[i];
						i++;
						k++;
					}
					userid[k]='\0';
					k=0;
					i++;
					while(buffer[i]!='\0'){
						password[k]=buffer[i];
						k++;
						i++;
					}
					password[k]='\0';
					int found=0;
					fa_p=fa_head;
					while(fa_p){
						if (strcmp(fa_p->id,userid)==0)
						{
							if (strcmp(fa_p->password,password)==0)
							{
								found=1;
								fa_q=fa_p;
								break;
							}
						}
						fa_p=fa_p->next;
					}

					if (found)
					{
						char fa_temp_file_name[50],ta_temp_ass[50],ta_temp_marks[50];
						int assignment_no=0;
						memset(buffer, 0, sizeof(buffer));
						strcat(buffer,"Welcome ");
						strcat(buffer,fa_q->name);
						strcat(buffer,"\n");
						strcat(buffer,"Please enter the name of the file you want to upload.");
						send(newSocket, buffer, strlen(buffer), 0);
						memset(buffer, 0, sizeof(buffer));
						recv(newSocket, buffer, 1024, 0);
						strcpy(fa_temp_file_name,buffer);

						int n;
  						FILE *fp;
  						char filename[50];
  						memset(filename, 0, sizeof(filename));
  						memset(buffer, 0, sizeof(buffer));
  						//strcat(filename,"\"");
  						strcat(filename,fa_temp_file_name);
  						strcat(filename,".txt");
  						//strcat(filename,"\"");
  						//char buffer[SIZE];
 
  						fp = fopen(filename, "w");
  						if(fp==NULL){
  							printf("Error opening file.");
  						}
  						while (1) {
    					n = recv(newSocket, buffer, 1024, 0);
    					printf("%s",buffer);
    					if (n <= 0){
      					break;
      					//return;
    					}
    					fputs(buffer,fp);
    					bzero(buffer, 1024);
  						}

												

					
					/*
					memset(buffer, 0, sizeof(buffer));
					strcat(buffer,"Username is: ");
					strcat(buffer,userid);
					strcat(buffer,"and password is: ");
					strcat(buffer,password);
					send(newSocket, buffer, strlen(buffer), 0);
					*/
				
			}else{
				memset(buffer, 0, sizeof(buffer));
						strcat(buffer,"Invalid credentials. ");
						send(newSocket, buffer, strlen(buffer), 0);
			}


	}else if (strcmp(operation,"SignUp")==0)				//Server code for SignUp
			{
				int k=0;
				char name[50];
				//char reg_no[50];
				char id[50];
				char password[50];

				while(buffer[i]!='|'){
					name[k]=buffer[i];
					i++;
					k++;
				}
				name[k]='\0';
				i++;
				k=0;
				while(buffer[i]!=' '){
					id[k]=buffer[i];
					i++;
					k++;
				}
				id[k]='\0';
				i++;
				k=0;

				while(buffer[i]!='\0'){
					password[k]=buffer[i];
					i++;
					k++;
				}
				password[k]='\0';
				
				fa_temp=(struct faculty_pass *)malloc(sizeof(struct faculty_pass));
				strcpy(fa_temp->name,name);
				//strcpy(ta_temp->reg_no,reg_no);
				strcpy(fa_temp->id,id);
				strcpy(fa_temp->password,password);
				fa_temp->next=NULL;
				if (fa_head==NULL)
				{
					fa_head=fa_temp;
					fa_tail=fa_temp;
				}else{
					fa_tail->next=fa_temp;
					fa_tail=fa_temp;
				}
				printf("Faculty Records in database are: \n");
				fa_p=fa_head;
				while(fa_p){
					printf("Name: %s\n",fa_p->name);
					//printf("Reg No: %s\n",ta_p->reg_no);
					printf("Username: %s\n",fa_p->id);
					printf("Password: %s\n",fa_p->password);
					printf("----------------------------------------------------------------\n");
					fa_p=fa_p->next;

				}
				memset(buffer, 0, sizeof(buffer));
				strcat(buffer,"Registration Successfull!\n");
				send(newSocket, buffer, strlen(buffer), 0);
			}
	




			/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
}

}
