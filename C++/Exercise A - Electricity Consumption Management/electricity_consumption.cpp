#include <iostream>
#include <stdlib.h>
#include <string.h>
#define admin_pass "1111"
using namespace std;

class clients{
    long int code, password;
    float kwcost, maxrest, debts;
    char name[30], loginname[5];
    int account;
public:
    void set_all();
    char* get_loginname();
    long int get_password();
    float get_kwcost();
    void set_debts(float a);
    float get_debts();
    char* get_name();
    long int get_code();
    float get_maxrest();
};
/*******************************************/
void clients::set_all()
{
    cout<<"Client Code: ";
    cin>>code;
    cout<<"Client Name: ";
    cin>>name;
    cout<<"Client login Name: ";
    cin>>loginname;
    cout<<"Client Password: ";
    cin>>password;
    cout<<"Client's Kw Cost(euro): ";
    cin>>kwcost;
    cout<<"Client's max Rest Cost(euro): ";
    cin>>maxrest;

};
/*******************************************/
char* clients::get_loginname()
{
    return loginname;
}
/******************************************/
long int clients::get_password()
{
    return password;
}
/******************************************/
float clients::get_kwcost()
{
    return kwcost;
}

/******************************************/
void clients::set_debts(float a)
{
    debts=a;
}
/******************************************/
float clients::get_debts()
{
    return debts;
}
/******************************************/
char* clients::get_name()
{
    return name;
}
/******************************************/
long int clients::get_code()
{
    return code;
}
/******************************************/
float clients::get_maxrest()
{
    return maxrest;
}
/******************************************/
void add_client(clients *cldata, int *num)
{
    float debts;
    if ((cldata=(class clients*)realloc(cldata,(*num+1)*sizeof(class clients)))==NULL)
    {
        cout<<"allocation prolbem";
        exit(0);
    }
    cout<<"Dwse stoixeia tou pelati " <<*num+1 <<": \n";
    cldata[*num].set_all();
    cout<<"Client's Debts(euro): ";
    cin>>debts;
    cldata[*num].set_debts(debts);
    cout<<"client successfully added"<<endl;
    *num=*num+1;
}
/******************************************/
void delete_client(clients *cldata, int *num)
{
    int i;
    long int code;
    char name[30];

    cout<<"Dwse to onoma tou pelati gia diagrafi: ";
    cin>>name;
    cout<<"Dwse ton kwdiko tou pelati gia diagrafi: ";
    cin>>code;
    for (i=0; i<*num; i++)
    {
        if ((strcmp(name,cldata[i].get_name())==0) && (code=cldata[i].get_code()))
        {
            cldata[i]=cldata[*num-1];
            if ((cldata=(class clients*)realloc(cldata,(*num-1)*sizeof(class clients)))==NULL)
            {
                cout<<"allocation problem";
                exit(0);
            }
            cout<<"Client Successfully deleted"<<endl;
            *num=*num-1;
        };
    }
}
/******************************************/
void print_all(clients *cldata, int num)
{
    int i;
    for (i=0; i<num; i++)
    {
        cout<<"Client Name: "<<cldata[i].get_name()<<endl;
        cout<<"Client Code: "<<cldata[i].get_code()<<endl;
        cout<<"Client Debts: "<<cldata[i].get_debts()<<endl<<endl;
    }
}
/******************************************/
void print_stop(clients *cldata,int num)
{
    int i;

    cout<<"Pelates pou tha diakopi i ilektrodotisi tous: "<<endl;
    for (i=0; i<num; i++)
    {
        if (cldata[i].get_debts()>cldata[i].get_maxrest())
        {
                cout<< cldata[i].get_name()<<endl;
                cout<< cldata[i].get_code()<<endl;
                cout<<"***********"<<endl;
        }
    }
}
/******************************************/

int main()
{
    long int password;
    float debts, payment, kwatts;
    int account, option, i, num;
    char username[5];
    clients *cldata;


    cout<<"Dwse aritho pelatwn: ";
    cin>>num;

    if ((cldata=(class clients*)malloc(num*sizeof(class clients)))==NULL){
        cout<<"allocation problem";
        exit(0);
    }
    for (i=0; i<num; i++)
    {
        cout<<"Dwse stoixeia tou pelati " <<i+1 <<": \n";
        cldata[i].set_all();
        cout<<"Client's Debts(euro): ";
        cin>>debts;
        cldata[i].set_debts(debts);
    }


    for(;;){

            cout<<"0.EXIT\n1.PELATIS\n2.DIAXEIRISTHS\n---------------\n";
            cin>>option;

            if (option==0) break;

            if (option==1){
                cout<<"Dwse Username kai Password \n";
                cout<<"Username: "; cin>>username;
                cout<<"Password: "; cin>>password;
                for (i=0; i<num; i++)
                    if (strcmp(username,cldata[i].get_loginname())==0 && password==cldata[i].get_password())
                    {
                        cout<<"Correct Password\n";
                        cout<<"Dwse trapeziko logariasmo: ";
                        cin>>account;
                        cout<<"Dwse poso Kwatts: ";
                        cin>>kwatts;
                        cout<<"Dwse poso plirwmis: ";
                        cin>>payment;
                        cldata[i].set_debts( (cldata[i].get_kwcost()*kwatts) + cldata[i].get_debts() - payment );
                        cout<<"To upoloipo tou logariasmou "<<account<<" einai "<<cldata[i].get_debts()<<" euro\n";
                    }
            }

            if (option==2){
                cout<<"Dwse Kwdiko Diaxeiristi(0:exit):\n";
                cin>>password;
                if (password==0) break;
                while (password != 1111){
                    cout<<"Lathos Kwdikos\n";
                    cout<<"Dwse Kwdiko Diaxeiristi(0:exit):\n";
                    cin>>password;
                    if (password==0) break;
                }
                do{
                    cout<<"---------------\n";
                    cout<<"EPILOGES\n";
                    cout<<"0.Exit\n";
                    cout<<"1.Eisagwgi neou Pelati\n";
                    cout<<"2.Diagrafi Pelati\n";
                    cout<<"3.Ektupwsi stoixeiwn pelatwn pou tha ginei diakopi iletrodotisi\n";
                    cout<<"4.Ektupwsi stoixeiwn olwn twn pelatwn\n";
                    cin>>option;

                    switch(option){
                    case 1:
                        add_client(cldata,&num);
                        break;
                    case 2:
                        delete_client(cldata,&num);
                        continue;
                    case 3:
                        print_stop(cldata, num);
                        break;
                    case 4:
                        print_all(cldata, num);
                        break;
                    };

                }while ((option!=0) && (option!=1) && (option!=2) && (option!=3));
            }
    }
}


