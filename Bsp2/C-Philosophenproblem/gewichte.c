#include "gewichte.h"
#include "err.h"
static int gewichtsArray[gewichteAnzahl][anzahlUndKilogram];


pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t condVar = PTHREAD_COND_INITIALIZER;
int condsignal = 0;

static void cleanUpMutex(void* arg){
    pthread_mutex_unlock(&mutex);
    }

int destroy_gewichte(){
    err(pthread_mutex_destroy(&mutex), "fehler bei mutexDestroyGewichte1\n",false);
    err(pthread_cond_destroy(&condVar), "fehler bei mutexDestroyGewichte2\n",false);
    return 0;
    }

/*
 * Temporaere Werte werden gecleared
 * */
static int clearGenommenUndAddiert(int* addiertesGewicht, int genommen[gewichteAnzahl]){
     *addiertesGewicht = 0;
     for(int i = 0; i<gewichteAnzahl;i++){
         genommen[i] = 0;
         }
     return 0;
     }

 /*
 * gewichtsArray
 * 0 0 = 4  x
 * 0 1 = 2  kg
 * 
 * 1 0 = 4  x
 * 1 1 = 3  kg
 * 
 * 2 0 = 5  x
 * 2 1 = 5  kg
 * */
int init_Gewichte(){

     gewichtsArray[0][0] = 4;
     gewichtsArray[0][1] = 2;
     
     gewichtsArray[1][0] = 4;
     gewichtsArray[1][1] = 3;
     
     gewichtsArray[2][0] = 5;
     gewichtsArray[2][1] = 5;
     return 0;
    }
 
 
 
 
/*
 * Es wird das letzte Element genommen. Wenn es kleinergleich needed ist, wird es erneut aufgerufen. Wenn etwas nicht additativ passt, 
 * um unter needed zu bleiben, wird ein kleineres Gewicht probiert, bis es kleiner nicht geht. Wenn es fehlschlägt, wird es erneut anfangen,
 * alles zurücksetzen und probieren mit dem kleinern zu starten. Dann wird wieder das größere genommen bis zum kleinsten. Klappt auch 
 * das nicht, wird die boundary verkleinert, das ganze wird wiederholt, aber das größte gewicht wird abgeschnitten. Es schlägt fehl, wenn
 * alle Gewichte abgeschnitten wurden.
 * */ 
static int rekursive(int neededWeights, int rek_gewichtGroesseIndex, int startindexGEWICHTE, int maxIndex,
int* addiertesGewicht, int genommen[gewichteAnzahl]){
  
    if(maxIndex<0){ //Wenn Boundary verkleinen nichts gebracht hat
        return 0;
        }
    if(startindexGEWICHTE<0){ //Wenn nichts geklappt hat: Boundary verkleinern
        clearGenommenUndAddiert(addiertesGewicht,genommen);
        return rekursive(neededWeights,maxIndex-1,maxIndex-1,maxIndex-1,addiertesGewicht,genommen);
        }
    if(*addiertesGewicht == neededWeights){//Erfolgreiches Ergebnis
        return 0;
        }
    
    int anzahl = gewichtsArray[rek_gewichtGroesseIndex][0] - genommen[rek_gewichtGroesseIndex];
    int gewicht = gewichtsArray[rek_gewichtGroesseIndex][1];

    //check if its available
    if(anzahl >= 1){
        if(*addiertesGewicht + gewicht <= neededWeights){ // wenn wir damit noch nicht das maximum überschreiten
            *addiertesGewicht += gewicht;
            genommen[rek_gewichtGroesseIndex] +=1;
            return rekursive(neededWeights,maxIndex,startindexGEWICHTE, maxIndex,addiertesGewicht,genommen); // try again with max index
            } else{
                if(rek_gewichtGroesseIndex > 0) {
                    return rekursive(neededWeights,(rek_gewichtGroesseIndex-1), startindexGEWICHTE,maxIndex,addiertesGewicht,genommen);
                    }// try again with max index different starting index
                else{//try again with different starting index
                    clearGenommenUndAddiert(addiertesGewicht,genommen);
                    return rekursive(neededWeights, startindexGEWICHTE-1, startindexGEWICHTE-1,maxIndex,addiertesGewicht,genommen);
                    }
                }
        }
        else{
            if(rek_gewichtGroesseIndex > 0){ 
                return rekursive(neededWeights,(rek_gewichtGroesseIndex-1), startindexGEWICHTE,maxIndex,addiertesGewicht,genommen);
                }// try again with max index different starting index
            else{//try again with different starting index
                clearGenommenUndAddiert(addiertesGewicht,genommen);
                return rekursive(neededWeights, startindexGEWICHTE-1, startindexGEWICHTE-1,maxIndex,addiertesGewicht,genommen);
                }
                }
   
    }
    
/*
 * Beim  erfolgreichen Ergebnis werden die Gewichte endgültig rausgenommen. Dazu dient das Temporäre
 * Array "genommen". Es werden dann seine Werte in myTakenWeights übertragen
 * */
static int take_weights(int myTakenWeights[gewichteAnzahl], int tempWeights[gewichteAnzahl]){
    gewichtsArray[2][0] -= tempWeights[2];
    gewichtsArray[1][0] -= tempWeights[1];
    gewichtsArray[0][0] -= tempWeights[0];
    for(int i = 0; i<gewichteAnzahl;i++){
        myTakenWeights[i] = tempWeights[i];
        tempWeights[i]=0;
        }
    return 0;
    }




int get_weights(int neededWeights, int myTakenWeights[gewichteAnzahl], int* addiertesGewicht, int tempWeights[gewichteAnzahl]){
    clearGenommenUndAddiert(addiertesGewicht,myTakenWeights); // cleanup method, first as init

    
    pthread_mutex_lock(&mutex); //Lock, da rekursive den Speicher von den Gewichten nutzt
    pthread_cleanup_push(cleanUpMutex,NULL);
    pthread_testcancel();
    rekursive(neededWeights, gewichtsIndexing, gewichtsIndexing, gewichtsIndexing, addiertesGewicht,tempWeights);
    if(*addiertesGewicht == neededWeights){
        take_weights(myTakenWeights,tempWeights);
    }
    pthread_cleanup_pop(1);//remove cleanup handler
    //pthread_mutex_unlock(&mutex);
    
    while (*addiertesGewicht != neededWeights){
        clearGenommenUndAddiert(addiertesGewicht,myTakenWeights); //cleanup 
        //HIER CONDITION VAR WAIT
        
        pthread_mutex_lock(&mutex);
        pthread_cleanup_push(cleanUpMutex,NULL);
        pthread_testcancel();
        
        while(condsignal == 0){
            pthread_cond_wait(&condVar,&mutex);
        }
        condsignal=0;
        rekursive(neededWeights, gewichtsIndexing, gewichtsIndexing, gewichtsIndexing, addiertesGewicht,tempWeights);
        if(*addiertesGewicht == neededWeights){
            take_weights(myTakenWeights,tempWeights);
        }
        else{
            clearGenommenUndAddiert(addiertesGewicht,myTakenWeights);
        }
        pthread_cleanup_pop(1);//remove cleanup handler
        //pthread_mutex_unlock(&mutex);
        }

    return 0;
    }
    

int put_weights(int myweights[gewichteAnzahl]){
    
    pthread_mutex_lock(&mutex);
    pthread_cleanup_push(cleanUpMutex,NULL);
    pthread_testcancel();
    
    for(int i = 0; i<gewichteAnzahl;i++){
        gewichtsArray[i][anzahlindex] += myweights[i];
        myweights[i] = 0;
        }
    condsignal=1;
    pthread_cond_signal(&condVar);
    pthread_cleanup_pop(1);//remove cleanup handler
    //pthread_mutex_unlock(&mutex);
    return 0;
    
    }

int display_leftover(){
    int erg = (gewichtsArray[0][0]*2)+(gewichtsArray[1][0]*3)+(gewichtsArray[2][0]*5);
    printf("{%d %d %d :%d}\n",gewichtsArray[0][0],gewichtsArray[1][0],
                                gewichtsArray[2][0], erg);
    return erg;
    }

int display_person(int myweights[gewichteAnzahl]){
    int erg = (myweights[0]*2)+(myweights[1]*3)+(myweights[2]*5);
    printf("[%d %d %d :%d]",myweights[0],myweights[1],myweights[2],erg);
    return erg;

    }



