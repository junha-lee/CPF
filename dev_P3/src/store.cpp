#include "store.h"

Store::Store(){
    Ingredient i_water("water", 5, 1000);
    Ingredient i_bean("beans", 3, 2000);
	Ingredient i_orange("orange", 2, 1000);
	Ingredient i_apple("apple", 2, 1000);
    _ingredient_v.push_back(i_water);
    _ingredient_v.push_back(i_bean);
	_ingredient_v.push_back(i_orange);
	_ingredient_v.push_back(i_apple);
    
    req_t req;
    

    Coffee c_water("Water", 2000);
    req.ingredient = i_water;
    req.amount = 1;
    c_water.setRequirement(req);

    Coffee c_americano("Americano", 5000);
    req.ingredient = i_water;
    req.amount = 1;
    c_americano.setRequirement(req);
    req.ingredient = i_bean;
    req.amount = 1;
    c_americano.setRequirement(req);

	Coffee c_orangejuice("Orangejuice", 4000);
	req.ingredient = i_water;
	req.amount = 1;
	c_orangejuice.setRequirement(req);
	req.ingredient = i_orange;
	req.amount = 1;
	c_orangejuice.setRequirement(req);

	Coffee c_applejuice("Applejuice", 4000);
	req.ingredient = i_water;
	req.amount = 1;
	c_applejuice.setRequirement(req);
	req.ingredient = i_apple;
	req.amount = 1;
	c_applejuice.setRequirement(req);

	Coffee c_fruitjuice("Fruitjuice", 5000);
	req.ingredient = i_water;
	req.amount = 1;
	c_fruitjuice.setRequirement(req);
	req.ingredient = i_apple;
	req.amount = 1;
	c_fruitjuice.setRequirement(req);
	req.ingredient = i_orange;
	req.amount = 1;
	c_fruitjuice.setRequirement(req);
	_coffee_v.push_back(c_water);
    _coffee_v.push_back(c_americano);
	_coffee_v.push_back(c_orangejuice);
	_coffee_v.push_back(c_applejuice);
	_coffee_v.push_back(c_fruitjuice);
	

	_sales = 0;
    _money = 4000;
}

std::string Store::makeMenu(){
    std::string menu("***menu***\n");
    int cnt = 0;
    for (int i=0;i<_coffee_v.size();i++){
        Coffee c = _coffee_v[i];
        if (isAvailable(c)){
            menu += std::to_string(++cnt);
            menu += (". " + c.getName());
            menu += (":\t\tprice: " + std::to_string(c.getPrice()));
            menu += "\n";
        }
    }
    
    if(_ingredient_v[0].getAmount()>0){
    menu+="?. Random:\t\tprice: menu-500";
    }
    menu += "\n";
    menu+="****************";

    return menu;
}

bool Store::isAvailable(const Coffee& c){
    for (int i=0;i<c._required_ingredient_v.size();i++){
        req_t req = c._required_ingredient_v[i];
        for (int j=0;j<_ingredient_v.size();j++){
            Ingredient ingre = _ingredient_v[j];
            if (req.ingredient.getName()==ingre.getName()){
                if (req.amount > ingre.getAmount()){
                    return 0;
                }
            }
        
        }
    }
    

    return 1;
}



int Store::take_order(std::string &coffee){
    int flag=0;
     if("Random"==coffee){
        int menuchose= 0;
        int max=_ingredient_v[0].getAmount();
          for(int i=1;i<_ingredient_v.size();i++){
              if(_ingredient_v[i].getAmount()>=max){
                  menuchose=i;
				  max = _ingredient_v[i].getAmount();
              }
          }
          if((_ingredient_v[2].getAmount()==_ingredient_v[3].getAmount())&&menuchose==3){
              menuchose=4;
          }
            switch(menuchose){
            case  0:
            coffee="Water";
            flag=1;
            break;
            case 1:
            coffee="Americano";
            flag=1;
            break;
            case 2:
            coffee="Orangejuice";
            flag=1;
            break;
            case 3:
            coffee="Applejuice";
            flag=1;
            break;
            case 4:
            coffee="Fruitjuice";
            flag=1;
            }
        
        }
    for (int i=0;i<_coffee_v.size();i++){
        Coffee c = _coffee_v[i];
        if (coffee==c.getName()){
            _sales += c.getPrice();
            _money += c.getPrice();
            if(flag==1){
                _sales -=500;
                _money -=500;
            }
            for (int j=0;j<c._required_ingredient_v.size();j++){
                req_t req_i = c._required_ingredient_v[j];
                for (int k=0;k<_ingredient_v.size();k++){
                    Ingredient ingredient = _ingredient_v[k];
                    if (req_i.ingredient.getName()==ingredient.getName()){
                        _ingredient_v[k].dec(req_i.amount);
                    }
                }
            }
            
            break;
        }
    }
   
}

void Store::printAllIngredients(){
    std::cout<<"Remaining Ingredients"<<std::endl;
    for (int i=0;i<_ingredient_v.size();i++){
        std::cout<<i+1<<"."<<_ingredient_v[i].getName()<<": "<<_ingredient_v[i].getAmount()<<std::endl;
    }
}
void Store::buyIngredients(){
    
    int Buy;
			std::cin >> Buy;
            int numbuy = 0;
			int sumBuy = 0;
			
            std::cout << "How many would you like to buy?";
			std::cin >> numbuy;
			
            sumBuy = _ingredient_v[Buy-1].increaseAmount(numbuy);

            if (getmoney() > sumBuy){
                _money-=sumBuy;
					_ingredient_v[Buy-1].increase(numbuy);
					std::cout << "Price:" << sumBuy << std::endl;
				}
                else {
					std::cout << " There is not enough money. " << std::endl;
				}
            }

