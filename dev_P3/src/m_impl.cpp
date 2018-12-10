#include "manager.h"
#include <iostream>
#include <string>
#include <ctime>


static bool admin_mode = false;


int i = 0;

std::time_t t = std::time(0);
std::tm* now = std::localtime(&t);
int day = now->tm_mday;

static void sig_handler(int sig){
    std::cout<<"\nAdmin Mode On"<<std::endl;
    admin_mode = true;
}

static void sig_handler2(int sig){  //^C
    std::cout<<"\nBYE"<<std::endl;
    unlink(FIFO_MANAGER_PATH);
    exit(0);
}

int Manager::start(){
    signal(SIGTSTP, sig_handler);    //^Z
    signal(SIGINT, sig_handler2);    //^C
   
    std::string input;
    log("[Manager] Waiting customers...");
    while(1){
        if (true == admin_mode){
            admin_handler();
            admin_mode = false;
            log("[Manager] Waiting customers...");
            continue;
        }
        
        customer_handler();
    }

    return 0;
}


int Manager::customer_handler(){
    msg_t msg;
    int read_bytes;

    pid_t c_pid;
    
    while(1){
        if (noCustomers() && admin_mode==1){
            break;
        }
        read_bytes = receive_msg(_rfd, msg);    //waiting a msg from c.
        if (read_bytes<=0) continue;
        log("received", msg);
        process(msg);
        
    }

    return 0;
}

int Manager::process(const msg_t& msg){
    char customer_fifo[256];
    sprintf(customer_fifo, FIFO_CUSTOMER_PATH, msg.pid);
    _wfd = open(customer_fifo, O_WRONLY);
    
    msg_t new_msg;
    
    if (strcmp(msg.cmd, CMD_HELLO)==0){
        new_msg = make_msg(CMD_MANAGER, "");
        
        send_msg(_wfd, new_msg);
        log("[Manager] new customer");
        log("[Manager] now waiting commands");
        
        _remainingCustomers++;

        close(_wfd);
    }
    else if (strcmp(msg.cmd, CMD_SHOW_MENU)==0){
	new_msg = make_msg(CMD_MANAGER, _store.makeMenu().c_str());
        send_msg(_wfd, new_msg);
        log("[Manager] sent menu");
        close(_wfd);
    }
    else if (strcmp(msg.cmd, CMD_MAKE_ORDER)==0){
        std::string c(msg.data);
        take_order(c);
        new_msg = make_msg(CMD_MANAGER, "");
        send_msg(_wfd, new_msg);
        log("[Manager] ");
        std::cout<<c;
        log(" here");
        close(_wfd);
    }
    else if (strcmp(msg.cmd, CMD_BYE)==0){
	i++;
        new_msg = make_msg(CMD_MANAGER, "");
        send_msg(_wfd, new_msg);
        log("[Manager] done");
        close(_wfd);

        _remainingCustomers--;
    }
    
    return 0;
}

int Manager::admin_handler(){



    std::string input;
    while(1){

        std::time_t t = std::time(0);
        std::tm* now = std::localtime(&t);
        int year = now->tm_year + 1900;
        int month = now->tm_mon + 1;
        int dayOfMonth = now->tm_mday;
        int hour = now->tm_hour;
        int minute = now->tm_min;
        if(day!=dayOfMonth){
        i=0;
        day=dayOfMonth;
    }

        
        std::cout<<std::endl<<"Time:"<<year<<"-"<<month<<"-"<<dayOfMonth<<std::endl<<hour<<"-"<<minute<<std::endl;
        std::cout<<"\nPlease give an input"<<std::endl;
        std::cout<<"1. check sales (type 1)"<<std::endl;
        std::cout<<"2. check remaining ingredients (type 2)"<<std::endl;
        std::cout<<"3. buy ingredients(type 3)"<<std::endl;
        std::cout<<"4. today's visitor number(type 4)"<<std::endl;
	    std::cout<<"5. go back to the default mode (type q)"<<std::endl;


        std::cin>>input;

        if (input=="1"){
            std::cout<<"Sales: "<<_store.getSales()<<std::endl;
	  std::cout<<"Money: "<<_store.getmoney()<<std::endl;
        }
        else if (input=="2"){
            _store.printAllIngredients();

        }else if(input == "3"){
            _store.printAllIngredients();
            _store.buyIngredients();
            
            
            
            
	}
	    else if(input == "4"){
		std::cout<<"today's visitor number = "<< i <<std::endl;
	}
        else if (input=="q"){
            break;
        }
    }
    return 0;
}
