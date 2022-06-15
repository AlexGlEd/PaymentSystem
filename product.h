#pragma once
#include <iostream>
#include <string> // для std::to_string


class ProductInfo { // класс хранит информацию о продукте
public:
	ProductInfo(const std::string& info) : m_info(info) {};
	const std::string get_info() const { return m_info; }
private:
	const std::string m_info;
};


class Product { //абстрактный класс
public:
	Product(const ProductInfo& product_info) :m_product_info(product_info) {}; // получаем по ссылке объект с информацией о продукте
	virtual const std::string get_info() const = 0;
	virtual double get_cost() const = 0;
protected:
	const ProductInfo& m_product_info;
};


class WeightProduct : public Product {
public:
	WeightProduct(const ProductInfo& product_info, double cost) : Product(product_info), m_cost_per_kg(cost) {};
	const std::string get_info() const override { return (m_product_info.get_info() + " : " + std::to_string(m_cost_per_kg)); }
	double get_cost() const override { return m_cost_per_kg; }
private:
	double m_cost_per_kg;
};


class AmountProduct : public Product {
public:
	AmountProduct(const ProductInfo& product_info, double cost) : Product(product_info), m_cost_per_one(cost) {};
	const std::string get_info() const override { return (m_product_info.get_info() + " : " + std::to_string(m_cost_per_one)); }
	double get_cost() const override { return m_cost_per_one; }
private:
	double m_cost_per_one;
};