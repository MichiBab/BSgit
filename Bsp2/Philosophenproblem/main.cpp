#include "generic.h"
#include "gewichte.h"
#include "philothread.h"

static void gewichtstest(){
    
    int myweights[gewichteAnzahl] = {0,0,0};
    int myweights2[gewichteAnzahl] = {0,0,0};
    int toomuch[gewichteAnzahl] = {0,0,0};

    int kiloneeded = 23;
    init_Gewichte();
    
    cout<<"MEINE GEWICHTE: " << myweights[0] << myweights[1] << myweights[2] <<endl;
    
    display_Gewichtsstatus();

    get_weights(kiloneeded,myweights);
    
    cout<<"MEINE GEWICHTE NACH GETWEIGHT MIT " << kiloneeded << "kg: " << myweights[0] << myweights[1] << myweights[2] <<endl;
    
    display_Gewichtsstatus();

    get_weights(18,myweights2);
    
    cout<<"MEINE GEWICHTE2 NACH GETWEIGHT2 MIT " << 18 << "kg: " << myweights2[0] << myweights2[1] << myweights2[2] <<endl;

    display_Gewichtsstatus();

    get_weights(40,toomuch);

    cout<<"MEINE GEWICHTEtm NACH TOOMUCH " << 40 << "kg: " << toomuch[0] << toomuch[1] << toomuch[2] <<endl;

    display_Gewichtsstatus();

    put_weights(myweights);

    cout<<"MEINE GEWICHTE NACH PUTWEIGHT: " << myweights[0] << myweights[1] << myweights[2] <<endl;
    
    display_Gewichtsstatus();
    
    put_weights(myweights2);

    cout<<"MEINE GEWICHTE2 NACH PUTWEIGHT2: " << myweights2[0] << myweights2[1] << myweights2[2] <<endl;

    display_Gewichtsstatus();
    }

int main(){
    //gewichtstest();
    init_Gewichte();
    
    philothread* philo = new philothread(26,0);
    philo->start();
    philo->p_thread.join();
    return 0;
}
