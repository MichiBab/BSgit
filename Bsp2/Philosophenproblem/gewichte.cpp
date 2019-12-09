#include "generic.h"
#include "gewichte.h"



static int gewichtsArray[gewichteAnzahl][anzahlUndKilogram];
static int addiertesGewicht;
static int genommen[gewichteAnzahl];


/*
 * Temporaere Werte werden gecleared
 * */
static int clearGenommenUndAddiert(){
     addiertesGewicht=0;
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
     clearGenommenUndAddiert();
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
static int rekursive(int neededWeights, int rek_gewichtGroesseIndex, int startindexGEWICHTE, int maxIndex){
    if(maxIndex<0){ //Wenn Boundary verkleinen nichts gebracht hat
        return 0;
        }
    if(startindexGEWICHTE<0){ //Wenn nichts geklappt hat: Boundary verkleinern
        clearGenommenUndAddiert();
        return rekursive(neededWeights,maxIndex-1,maxIndex-1,maxIndex-1);
        }
    if(addiertesGewicht == neededWeights){//Erfolgreiches Ergebnis
        return 0;
        }
    
    int anzahl = gewichtsArray[rek_gewichtGroesseIndex][0] - genommen[rek_gewichtGroesseIndex];
    int gewicht = gewichtsArray[rek_gewichtGroesseIndex][1];

    //check if its available
    if(anzahl >= 1){
        if(addiertesGewicht + gewicht <= neededWeights){ // wenn wir damit noch nicht das maximum überschreiten
            addiertesGewicht += gewicht;
            genommen[rek_gewichtGroesseIndex] +=1;
            return rekursive(neededWeights,maxIndex,startindexGEWICHTE, maxIndex); // try again with max index
            } else{
                if(rek_gewichtGroesseIndex > 0) {
                    return rekursive(neededWeights,(rek_gewichtGroesseIndex-1), startindexGEWICHTE,maxIndex);
                    }// try again with max index different starting index
                else{//try again with different starting index
                    clearGenommenUndAddiert();
                    return rekursive(neededWeights, startindexGEWICHTE-1, startindexGEWICHTE-1,maxIndex);
                    }
                }
        }
        else{
            if(rek_gewichtGroesseIndex > 0){ 
                return rekursive(neededWeights,(rek_gewichtGroesseIndex-1), startindexGEWICHTE,maxIndex);
                }// try again with max index different starting index
            else{//try again with different starting index
                clearGenommenUndAddiert();
                return rekursive(neededWeights, startindexGEWICHTE-1, startindexGEWICHTE-1,maxIndex);
                }
                }
    
    }
    
/*
 * Beim  erfolgreichen Ergebnis werden die Gewichte endgültig rausgenommen. Dazu dient das Temporäre
 * Array "genommen". Es werden dann seine Werte in myTakenWeights übertragen
 * */
static int take_weights(int myTakenWeights[gewichteAnzahl]){
    gewichtsArray[2][0] -= genommen[2];
    gewichtsArray[1][0] -= genommen[1];
    gewichtsArray[0][0] -= genommen[0];
    
    for(int i = 0; i<gewichteAnzahl;i++){
        myTakenWeights[i] = genommen[i];
        }
    
    return 0;
    }


int get_weights(int neededWeights, int myTakenWeights[gewichteAnzahl]){
    //lock mutex
    clearGenommenUndAddiert();
    rekursive(neededWeights, gewichtsIndexing, gewichtsIndexing, gewichtsIndexing);
    if (addiertesGewicht != neededWeights){
        clearGenommenUndAddiert();
        //SLEEP ZUM TESTEN NUR
        //std::this_thread::sleep_for(std::chrono::milliseconds(1000));
        cout<<"didnt work for me"<<endl;
        //unlock mutex
        }
    else{
    take_weights(myTakenWeights);
    //cout<<addiertesGewicht<<endl;
    //unlock mutex
    }
    return 0;
    }
    

int put_weights(int myweights[gewichteAnzahl]){
    clearGenommenUndAddiert();
    for(int i = 0; i<gewichteAnzahl;i++){
        gewichtsArray[i][anzahlindex] += myweights[i];
        }
    for(int i = 0; i<gewichteAnzahl;i++){
        myweights[i] = 0;
        }
    return 0;
    
    }

int display_Gewichtsstatus(){
    
    cout<<"Gebliebene Gewichte:             " <<gewichtsArray[0][0]  <<" mal 2kg; " << gewichtsArray[1][0] <<" mal 3kg; " << gewichtsArray[2][0] <<" mal 5kg; "  <<endl;
    

    return 0;
    }


int routineTEST(philothread* p){
    display_Gewichtsstatus();
    get_weights(p->myNeededWeights,p->myTakenWeights);
    display_Gewichtsstatus();
    put_weights(p->myTakenWeights);
    display_Gewichtsstatus();
    return 0;
    }