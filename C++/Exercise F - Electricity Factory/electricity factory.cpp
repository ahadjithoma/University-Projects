#include <iostream>
#include <stdlib.h>
#include <time.h>
using namespace std;


class source {
    char* type;
    static int types_num;
    static int src_num;
    static source ** src_types;
    static source ** sources;
public:
    source();
    source(char* n);
    char*get_type_name(){return type;};

    static int get_types_num(){return types_num;};
    static int get_src_num(){return src_num;};
    static source ** get_src_types(){return src_types;};
    static source ** get_sources(){return sources;};

    virtual void create_sources(int num)=0;
    virtual float get_damage_p()=0;
    virtual float get_power()=0;
    virtual int get_id()=0;

};

/**static*/
int source::types_num;
int source::src_num;
source ** source::src_types;
source ** source::sources;
/*********/

source::source(){
    src_num++;
    if (src_num==1){
        if ((sources=(source **)malloc(sizeof(source *)))==NULL){
            cout<<"allocation problem..";
            exit(1);
        }
    }
    else {
        if ((sources=(source **)realloc(sources,src_num*sizeof(source *)))==NULL){
            cout<<"allocation problem..";
           exit(2);
        }
    }
    sources[src_num-1]=this;
}

source::source(char *n){
    types_num++;
    type=n;
    if (types_num==1){
        if ((src_types=(source **)malloc(sizeof(source *)))==NULL){
            cout<<"allocation problem..";
            exit(3);
        }
    }
    else {
        if ((src_types=(source **)realloc(src_types, types_num*sizeof(source *)))==NULL){
            cout<<"allocation problem..";
            exit(4);
        }
    }
    src_types[types_num-1]=this;
}

/****************** SOLAR **********************/
class solar: public source{
    float  light_flow, S, damage_p, power;
    float E;
    int id;
public:
    solar();
    solar(char *n);
    void create_sources(int num);
    float get_damage_p(){return damage_p;};
    float get_E(){return E;};
    int get_id(){return id;};
    float get_power();
}s("SOLAR");

solar::solar():source(){
    cout<<"\n-NEW SOLAR SOURCE-\n";
    cout<<"Source ID: ";
    cin>>id;
    cout<<"Light Flow: ";
    cin>>light_flow;
    cout<<"Rate S: ";
    cin>>S;
    cout<<"Damage possibility[0,1]: ";
    cin>>damage_p;
}

solar::solar(char *n):source(n){
    cout<<"Give Solar Source surface: ";
    cin>>E;
    cout<<"<solar source type imported>\n";
}

void solar::create_sources(int num){
    solar *tmp;

    if (!(tmp=new solar[num])){
        cout<<"allocation problem..";
        exit(5);
    }
    cout<<"<"<<num<<" solar sources created>\n";
}

float solar::get_power(){
    float p;
    p=S*light_flow*s.get_E();
    return p;
}

/****************** WIND **********************/
class wind: public source{
    float wind_speed, A, damage_p, power;
    int id;
public:
    wind();
    wind(char *n);
    void create_sources(int num);
    float get_damage_p(){return damage_p;};
    int get_id(){return id;};
    float get_power();
}w("WIND");


wind::wind(){
    cout<<"\n-NEW WIND SOURCE-\n";
    cout<<"Source ID: ";
    cin>>id;
    cout<<"Wind speed: ";
    cin>>wind_speed;
    cout<<"Rate A: ";
    cin>>A;
    cout<<"Damage possibility[0,1]: ";
    cin>>damage_p;
};

wind::wind(char *n):source(n){
    cout<<"<wind source type imported>\n";
};

void wind::create_sources(int num){
    wind *tmp;
    if (!(tmp=new wind[num])){
        cout<<"allocation problem..";
        exit(6);
    }
    cout<<"<"<<num<<" wind sources created>\n";
}

float wind::get_power(){
    float p;
    p=A*wind_speed;
    return p;
}

/****************** CONTROL *******************/
int control(int src_num, source **srcs){
    float n_pow, power=0;
    int i, *fl, cnt0=0, cnt1=0;

    cout<<"Minimal power needed for station: ";
    cin>>n_pow;
    if ((fl=(int *)malloc(src_num*sizeof(int)))==NULL){
        cout<<"allocation problem..";
        exit(7);
    }

    for (i=0; i<src_num; i++){/**elegxos vlavis**/
        if ((float)(rand()%10)/10<srcs[i]->get_damage_p()){
            cnt0++;
            fl[i]=0;
        }
        else {
            cnt1++;
            fl[i]++;
            power+=srcs[i]->get_power();
        }
    }
    cout<<"----\n";
    cout<<"NON Working Sources: "<<cnt0<<endl;
    for (i=0; i<src_num; i++)
        if (fl[i]==0)
            cout<<srcs[i]->get_id()<<endl;
    cout<<"Working Sources: "<<cnt1<<endl;
    cout<<"Total power =  "<<power<<" mwatt"<<endl;
    cout<<"----\n";

    if (power<n_pow)
        return 1;
    else if (power>=n_pow && power<n_pow*110/100)
        return 2;
    else if (power>n_pow)
        return 3;
}

/****************** MAIN **********************/
main(){
    int src_num, types_num, i, num;
    source **sources, **src_types;
    srand(time(NULL));

    types_num=source::get_types_num();
    src_types=source::get_src_types();
    cout<<"----\n";

    for (i=0; i<types_num; i++){
        cout<<endl<<src_types[i]->get_type_name()<<" SOURCE. Dwse arithmo pigwn: ";
        cin>>num;
        src_types[i]->create_sources(num);
    }

    sources=source::get_sources();
    src_num=source::get_src_num();
    cout<<"----\n";
    num=control(src_num, sources);
    if (num==1)
        cout<<"H isxys den eparkei..\n\n";
    else if (num==2)
        cout<<"H isxys einai katw apo to orio asfaleias..\n\n";
    else if (num==3)
        cout<<"H trofodosia ginete omala..\n\n";
}

