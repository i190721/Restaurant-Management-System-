#include <iostream>
#include <unistd.h>
#include <cstring>
#include<sys/wait.h>
using namespace std;

class foodItem
{
public:
    string name;
    int seconds;
    double price;
    
    foodItem()
    {
        name="\0";
        seconds=0;
        price=0;
    }
    void setName(string a){
        name=a;
    }
    void setSeconds(int a){
        seconds=a;
    }
    void setPrice(double a){
        price=a;
    }
    
    string getName(){
        return name;
    }
    int getSeconds(){
        return seconds;
    }
    double getPrice(){
        return price;
    }
};

class menu{

public:
    foodItem x[5];

    menu(){

        cout<<x[0].getName();
        x[0].setName("Biryani");x[0].setPrice(200);x[0].setSeconds(5);
        x[1].setName("Ice Cream");x[0].setPrice(150);x[0].setSeconds(2);
        x[2].setName("Chips");x[0].setPrice(1500);x[0].setSeconds(10);
        x[3].setName("Pasta");x[0].setPrice(500);x[0].setSeconds(5);
        x[4].setName("Drinks");x[0].setPrice(100);x[0].setSeconds(1);

    }
    void showMenu(){

        cout<<"Receipt"<<endl;
        cout<<"Food  Price  Time"<<endl;
        for(int i =0;i<5;i++){

            cout<<i<<"# "<<x[i].getName()<<"   "<<x[i].getPrice()<<"    "<<x[i].getSeconds()<<endl;
        }
        cout<<endl<<endl;
    }
    void showChoice(){

            cout<<"Press 0 to Buy Biryani"<<endl;
            cout<<"Press 1 to Buy Ice Cream"<<endl;
            cout<<"Press 2 to Buy Chips"<<endl;
            cout<<"Press 3 to Buy Pasta"<<endl;
            cout<<"Press 4 to Buy Drinks"<<endl;
            cout<<"Press 5 to Finsh Order"<<endl;
    }
};

class Customer{
    public:
    string name;
    int cid;
    
    Customer(string a,int b){
        name =a;
        cid=b;
    }
    Customer(){
       name ="\0";
        cid=0;
    }
    void setName(string a){
        name=a;
    }
    void setId(int a){
        cid=a;
    }
    string getName(){
        return name;
    }
    int getId(){
        return cid;
    }
};
menu a;
int b[6][2]={{-1,-1},{-1,-1},{-1,-1},{-1,-1},{-1,-1},{-1,-1}};
int s=0;
class Order{
    public:
    double cost=0;
    int times=0;
    
    void setCost(double a){
        cost=a;
    }
    void setTime(int a){
        times=a;
    }
    double getCost(){
        return cost;
    }
    int getTime(){
        return times;
    }
    void Total(){
        for(int i =0;i<=5;i++){

            
            cost+=(a.x[b[i][0]].price*b[i][1]);
            if(times<a.x[b[i][0]].seconds){

                times=a.x[b[i][0]].seconds;
            }
        }
        cout<<"Thank you for Eating here! Your bill amounts to: "<<cost<<endl;;
        cout<<"You may have to wait "<<times<<" seconds!"<<endl;;
    }
};
pthread_t t[5],ti[5];

void *cook(void*arg){

    int*v=(int*)arg;
    cout<<"In process..."<<a.x[*v].getName()<<endl;
    sleep(a.x[*v].getSeconds());
    cout<<a.x[*v].getName()<<": Ready"<<endl;
}
double wSales=0;
void * waiterTakes(void*arg){

    int*v=(int*)arg;
    cout<<"Waiter Presents: "<<a.x[*v].getName()<<endl;
    wSales+=a.x[*v].price*b[*v][1];

}
void compareSales(double x){

    double comp=wSales-x;
    cout<<"Comparison of Sales: "<<comp;
}

int main(){

    Order total;
    char buffer[30];
    int fd[2],fd1[2],fd2[2];
    int temp[6][2];
    pipe(fd);
    pipe(fd1);
    pipe(fd2);
    int pid=fork();


    if(pid>0){
        close(fd[1]);
        Customer n;
        cout<<"Enter your id: "<<endl;
        wait(NULL);
        read(fd[0],&n,sizeof(Customer));
        cout<<n.getName() << endl<<n.getId()<<endl;

        int pid2=fork();

        if(pid2>0){

            cout<<"Your menu: "<<endl;
            a.showMenu();
            a.showChoice();
            sleep(1);

            wait(NULL);
            read(fd1[0],b,sizeof(b));

            total.Total();
            sleep(1);
            s=0;
            for(int i =0;i<5;i++){
                
                
                if(b[i][0]>=0&&b[i][0]<=5){
                    s++;
                }
            }
            for(int i =0;i<s;i++){
                int *v=new int(1);
                *v=b[i][0];
                pthread_create(&t[i],NULL,cook,v);
            }
            for(int i =0;i<s;i++){

                pthread_join(t[i],NULL);
                int *v=new int(1);
                *v=b[i][0];
                pthread_create(&ti[i],NULL,waiterTakes,v);
                pthread_join(ti[i],NULL);
            }
                char*buffer=new char[100];
                read(fd2[0],buffer,sizeof(buffer));
                cout<<buffer<<endl;
                compareSales(total.cost);
        }
        else if(pid2==0){

            cout<<"I will Order the following things!"<<endl;
            sleep(2);
            for(int i =0;i<5;i++){
                
                cout<<"Item: ";
                cin>>b[i][0];
                if(b[i][0]>=5){
                    break;
                }
                cout<<"Quantity: ";
                cin>>b[i][1];

            }

            write(fd1[1],b,sizeof(b));
            write(fd2[1],"Confirmed",100);
            exit(0);
        }
        
       
    }
    else if (pid==0){
        close(fd[0]);
        cout<<"Name: ";
        string n;
        int idx = 0;
        getline(cin,n);
        cout<<"ID: ";
        cin>>idx;
        Customer guest(n,idx);
        write(fd[1],&guest,sizeof(Customer));

       
        exit(0);
       
    }
    pthread_exit(NULL);
}