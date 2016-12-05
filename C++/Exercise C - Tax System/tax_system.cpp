#include <iostream>
#include <stdlib.h>
#include <ctime>

using namespace std;


class data{
    int zones_num;
    float *building_values, *plot_values;
    float *building_taxes, *plot_taxes;
    float warehouse_tax, tree_fields_tax, year_fields_tax;
public:
    data();
    float get_plot_values(int i);
    float get_plot_taxes(int i);
    float get_building_values(int i);
    float get_building_taxes(int i);
    float get_tree_fields_tax();
    float get_year_fields_tax();
    float get_warehouse_tax();
}d;


data::data(){
    int i;

    cout<<"Give the existing Zones: ";
    cin>>zones_num;

    if ((building_values=(float*)malloc(zones_num*sizeof(float)))==NULL){
        cout<<"allocation problem..";
        exit(0);
    }

    if ((plot_values=(float*)malloc(zones_num*sizeof(float)))==NULL){
        cout<<"allocation problem..";
        exit(1);
    }

    if ((building_taxes=(float*)malloc(zones_num*sizeof(float)))==NULL){
        cout<<"allocation problem..";
        exit(2);
    }

    if ((plot_taxes=(float*)malloc(zones_num*sizeof(float)))==NULL){
        cout<<"allocation problem..";
        exit(3);
    }

    for (i=0; i<zones_num; i++){
        cout<<"--ZONE "<<i<<"--"<<endl;
        cout<<"Give Building Values: ";
        cin>>building_values[i];
        cout<<"Give Buiding tax factor: ";
        cin>>building_taxes[i];
        cout<<"Give Plot Values: ";
        cin>>plot_values[i];
        cout<<"Give Plot tax factor: ";
        cin>>plot_taxes[i];
    }

    cout<<"----------\n";
    cout<<"Give Werehouse tax reduction: ";
    cin>>warehouse_tax;
    cout<<"Give tree fields tax: ";
    cin>>tree_fields_tax;
    cout<<"Give year fields tax: ";
    cin>>year_fields_tax;
}

float data::get_plot_values(int i){
    return plot_values[i];
}

float data::get_plot_taxes(int i){
    return plot_taxes[i];
}

float data::get_building_values(int i){
    return building_values[i];
}

float data::get_building_taxes(int i){
    return building_taxes[i];
}

float data::get_tree_fields_tax(){
    return tree_fields_tax;
}

float data::get_year_fields_tax(){
    return year_fields_tax;
}

float data::get_warehouse_tax(){
    return warehouse_tax;
}

/*****************************************/
class ground{
    int zone;
    float E, plot_values, plot_taxes, fields_taxes;
public:
    ground(int z, float e);
    ground(float e, int option);
    float field_tax();
    float plot_tax();
};


ground::ground(int z, float e){
    zone=z;
    E=e;
    plot_values=d.get_plot_values(z);
    plot_taxes=d.get_plot_taxes(z);
}

ground::ground(float e, int option){
    zone=0; //by default
    E=e;
    if (option==0)
        fields_taxes=d.get_tree_fields_tax();
    else if (option)
        fields_taxes=d.get_year_fields_tax();
}

float ground::field_tax(){
    int tax;
    tax=E*fields_taxes;
    return tax;
}

float ground::plot_tax(){
    int tax;
    tax=E*plot_values*plot_taxes;
    return tax;
}

/*****************************************/
class building{
    float E, building_values, building_taxes;
    int zone, floors;
public:
    building(int z, float e, int flrs);
    building(int z, float e);
    float house_taxes();
    float warehouse_taxes();
};


building::building(int z, float e, int flrs){
    zone=z;
    E=e;
    floors=flrs;
    building_values=d.get_building_values(z);
    building_taxes=d.get_building_taxes(z);

}

building::building(int z, float e){
    zone=z;
    E=e;
    building_values=d.get_building_values(z);
    building_taxes=d.get_building_taxes(z);
}

float building::house_taxes(){
    float tax;
    tax=E*building_values*building_taxes*floors;
    return tax;
}

float building::warehouse_taxes(){
    float tax;
    tax=E*building_values*building_taxes*d.get_warehouse_tax();
    return tax;
}

/*****************************************/
class property:private ground, private building{
public:
    property(int a,float b,int c,float d,int e);
    property(float a, int b, int c, float d);
    property(int a,float b,int c,float d);
    property(float a,int b,int c,float d,int e);
    float get_house_taxes();
    float get_warehouse_taxes();
    float get_plot_taxes();
    float get_field_taxes();
};


property::property(int a,float b,int c,float d,int e):ground(a,b),building(c,d,e) {}

property::property(float a,int b,int c,float d):ground(a,b),building(c,d) {}

property::property(int a,float b,int c,float d):ground(a,b),building(c,d) {}

property::property(float a,int b,int c,float d,int e):ground(a,b),building(c,d,e) {}

float property::get_house_taxes(){
    return house_taxes();
}

float property::get_warehouse_taxes(){
    return warehouse_taxes();
}

float property::get_plot_taxes(){
    return plot_tax();
}

float property::get_field_taxes(){
    return field_tax();
}


/**************************************/
main()
{
    int field_type, option, zone, floors;
    float total_tax=0, house_E, plot_E, field_E, warehouse_E;

    for (;;){

        cout<<"------------"<<endl;
        cout<<"0. EXIT"<<endl;
        cout<<"1.Plot - House"<<endl;
        cout<<"2.Field - House"<<endl;
        cout<<"3.Plot - Warehouse"<<endl;
        cout<<"4.Field - Warehouse"<<endl;

        cin>>option;
        if (option==0)
            break;

        switch(option){
            case 1:{
                cout<<"Give Zone: ";
                cin>>zone;
                cout<<"Give house's square meters(E): ";
                cin>>house_E;
                cout<<"Give plot's square meters(E): ";
                cin>>plot_E;
                cout<<"Give house's floors: ";
                cin>>floors;

                property p(zone,plot_E,zone,house_E,floors);
                total_tax+=p.get_plot_taxes()+p.get_house_taxes();

                cout<<"TAXES: "<<p.get_plot_taxes()+p.get_house_taxes()<<endl<<endl;
                break;
            }
            case 2:{
                cout<<"Give field's square meters(E): ";
                cin>>field_E;
                cout<<"Give house's square meters(E): ";
                cin>>house_E;
                cout<<"Give field type"<<endl;
                cout<<"0.Trees field\n1. Year field\n ";
                cin>>field_type;
                cout<<"Give house's floors: ";
                cin>>floors;

                property p(field_E,field_type,0,house_E,floors);
                total_tax+=p.get_field_taxes()+p.get_house_taxes();

                cout<<"TAXES: "<<p.get_field_taxes()+p.get_house_taxes()<<endl<<endl;
                break;
            }
            case 3:{
                cout<<"Give zone: ";
                cin>>zone;
                cout<<"Give plot's square meters(E): ";
                cin>>plot_E;
                cout<<"Give Warehouse's square meters(E): ";
                cin>>warehouse_E;

                property p(zone,plot_E,zone,warehouse_E);
                total_tax+=p.get_plot_taxes()+p.get_warehouse_taxes();

                cout<<"TAXES: "<<p.get_plot_taxes()+p.get_warehouse_taxes()<<endl<<endl;
                break;
            }
            case 4:{
                cout<<"Give field's square meters(E): ";
                cin>>field_E;
                cout<<"Give Warehouse's square meters(E): ";
                cin>>warehouse_E;
                cout<<"Give field type"<<endl;
                cout<<"0.Trees field\n1. Year field\n ";
                cin>>field_type;

                property p(field_E,field_type,0,warehouse_E);
                total_tax+=p.get_field_taxes()+p.get_warehouse_taxes();

                cout<<"TAXES: "<<p.get_field_taxes()+p.get_warehouse_taxes()<<endl<<endl;
                break;
            }
        }
    }


    cout<<endl<<"TOTAL TAXES: "<<total_tax<<endl;
    cout<<"Program will now exit.."<<endl<<endl;
}
