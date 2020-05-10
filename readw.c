    #include<stdio.h>
    #include<pthread.h>
    #include<semaphore.h>
    #include <unistd.h> //sleep
    #include <stdlib.h> //malloc
     
    sem_t mutex,writeblock;
    int data = 0,rcount = 0;
     
    void *reader(void *arg){
    	
    	int f;
    	f = ((int)arg);
    	
    	sem_wait(&mutex);
    	rcount = rcount + 1;
    	if(rcount==1)
    		sem_wait(&writeblock);
    	sem_post(&mutex);
    	
    	//Critical section
    	printf("Data read by the reader%d is %d\n",f,data);
    	sleep(5);
     
    	sem_wait(&mutex);
    	rcount = rcount - 1;
    	if(rcount==0)
    		sem_post(&writeblock);
    	sem_post(&mutex);
    }
     
    void *writer(void *arg){
    	
    	int f = ((int)arg);
    	
    	sem_wait(&writeblock);
     
    	//Critical section
    	data++;
    	printf("Data writen by the writer%d is %d\n",f,data);
    	sleep(5);
      	
      	sem_post(&writeblock);
    }
     
     
    int main()
    {
    	sem_init(&mutex,0,1);
    	sem_init(&writeblock,0,1);
     
        int n;
        printf("Enter number of readers and writers\n");
        scanf("%d",&n);
        //printf("number of readers and writers is %d\n",n);
        char c;
        scanf("%c",&c);
     
        char *str = malloc(sizeof(char)*(n+1));
        int rcount = 0, wcount = 0;
        for(int i=0; i<n; i++){
        	scanf("%c",&str[i]);
        	if(str[i]=='r' || str[i] == 'R')
        		rcount++;
        	else if(str[i]=='w' || str[i] == 'W')
        		wcount++;
        	else{
        		printf("Plox enter valid stuff\n");
        		return 0;
        	}
        }
     
        pthread_t *r = malloc(sizeof(pthread_t)*(rcount));
        pthread_t *w = malloc(sizeof(pthread_t)*(wcount));
        //pthread_t r[3],w[2];
     
        rcount = 0, wcount = 0;
     
        for(int i=0; i<n; i++){
        	if(str[i]=='r' || str[i]=='R'){
        		pthread_create(&r[rcount],NULL,(void *)reader,(void *)rcount);
        		rcount++;
        	}
        	else{
        		pthread_create(&w[wcount],NULL,(void *)writer,(void *)wcount);
        		wcount++;
        	}
        }
     
        rcount = 0, wcount = 0;
     
        for(int i=0; i<n; i++){
        	if(str[i]=='r' || str[i]=='R'){
        		pthread_join(r[rcount],NULL);
        		rcount++;
        	}
        	else{
        		pthread_join(w[wcount],NULL);
        		wcount++;
        	}
        }
     
        return 0;
    }    