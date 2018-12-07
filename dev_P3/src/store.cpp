#include "store.h"

Store::Store(){
    Ingredient i_water("water", 2, 1000);
    Ingredient i_bean("beans", 1, 2000);
    _ingredient_v.push_back(i_water);
    _ingredient_v.push_back(i_bean);
    
    req_t req;
    Coffee c_americano("Americano", 5000);
    req.ingredient = i_water;
    req.amount = 1;
    c_americano.setRequirement(req);
    req.ingredient = i_bean;
    req.amount = 1;
    c_americano.setRequirement(req);

    Coffee c_water("Water", 2000);
    req.ingredient = i_water;
    req.amount = 1;
    c_water.setRequirement(req);

    _coffee_v.push_back(c_americano);
    _coffee_v.push_back(c_water);

    _sales = 0;
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

int Store::take_order(std::string coffee){
    for (int i=0;i<_coffee_v.size();i++){
        Coffee c = _coffee_v[i];
        if (coffee==c.getName()){
            _sales += c.getPrice();

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
        std::cout<<_ingredient_v[i].getName()<<": "<<_ingredient_v[i].getAmount()<<std::endl;
    }
}