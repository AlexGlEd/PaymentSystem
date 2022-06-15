#pragma once
#include <iostream>
#include <map>
#include "client.h"


class PriceBase {
public:
	PriceBase(const PriceBase&) = delete;
	PriceBase& operator= (const PriceBase&) = delete;
	static PriceBase& Instance() {
		static PriceBase instance;
		return instance;
	}
	WeightProduct& get_product_weight_price(const std::string& name) { 
		auto it = m_products_weight_price.find(name);
		if (it == m_products_weight_price.end())
			throw std::out_of_range("There are no products with this name in the database!");
		return it->second;
	}
	AmountProduct& get_product_ammount_price(const std::string& name) { 
		auto it = m_products_ammount_price.find(name);
		if (it == m_products_ammount_price.end())
			throw std::out_of_range("There are no products with this name in the database!");
		return it->second;
	}
protected:
	PriceBase();
private:
	std::map<std::string, ProductInfo> m_products_info;
	std::map<std::string, WeightProduct> m_products_weight_price;
	std::map<std::string, AmountProduct> m_products_ammount_price;
};

PriceBase::PriceBase() {
	m_products_info = {
		{ "Apple", ProductInfo("Green sweet apple") },
        { "Nuts", ProductInfo("Macadamia nut") },
        { "Pencil", ProductInfo("Red pencil with rubber") },
		{ "Chair", ProductInfo("Wooden chair with armrests") },
	};
	m_products_weight_price = {
		{ "Apple", WeightProduct(m_products_info.at("Apple"), 11.3) },
	    { "Nuts", WeightProduct(m_products_info.at("Nuts"), 34.2) },
	};
	m_products_ammount_price = {
	    { "Penicil", AmountProduct(m_products_info.at("Pencil"), 5.7) },
	    { "Chair", AmountProduct(m_products_info.at("Chair"), 75) },
	};
}