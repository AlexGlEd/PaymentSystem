#pragma once
#include <iostream>
#include <string> // дл€ std::to_string
#include <memory>
#include <vector>
#include <algorithm>
#include <map>


//------------------------------------------------------------PRODUCT--------------------------------------------------------------

class ProductInfo { // класс хранит информацию о продукте
public:
	ProductInfo(const std::string& info) : m_info(info) {};
	const std::string& get_info() const { return m_info; }
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


//------------------------------------------------------------POSITION------------------------------------------------------------


class Position { //абстрактный класс
public:
	Position(std::unique_ptr<Product>&& ptr_product) : m_ptr_product(std::move(ptr_product)) {};
	std::unique_ptr<Product>& get_ptr_product() { return m_ptr_product; }
	virtual double get_cost() const = 0;
	virtual double get_quantity() const = 0;
protected:
	std::unique_ptr<Product> m_ptr_product;
};


class WeightPosition : public Position {
public:
	WeightPosition(std::unique_ptr<WeightProduct>&& ptr_product, double weight) : Position(std::move(ptr_product)), m_weight(weight) {};
	double get_cost() const override { return m_ptr_product->get_cost() * m_weight; }
	double get_quantity() const override { return m_weight; }
private:
	double m_weight;
};


class AmountPosition : public Position {
public:
	AmountPosition(std::unique_ptr<AmountProduct>&& ptr_product, size_t amount) : Position(std::move(ptr_product)), m_amount(amount) {};
	double get_cost() const override { return m_ptr_product->get_cost() * m_amount; }
	double get_quantity() const override { return m_amount; }
private:
	size_t m_amount;
};


//------------------------------------------------------------ORDER------------------------------------------------------------------


class Order {
public:
	void add_position(std::unique_ptr<Position>&& position) {
		//const std::string& position_info = position->get_ptr_product()->get_info(); // запоминаем информацию о продукте

		auto pos_cmp = [&position_info = position->get_ptr_product()->get_info()](std::unique_ptr<Position>& ptr_pos) -> bool // л€мбда-функци€ в качестве унарного предиката: сравнивает информацию о текущем продукте с position_info
		{ return ptr_pos->get_ptr_product()->get_info() == position_info; };

		auto old_pos = find_if(m_ptr_positions.begin(), m_ptr_positions.end(), pos_cmp);
		if (old_pos == m_ptr_positions.end()) // если в заказе не было указанного продукта
			m_ptr_positions.push_back(std::move(position)); // добавл€ем новую позицию в заказ
		else *old_pos = std::move(position); // иначе мен€ем позицию
	}
	double get_cost() const {
		double  total_cost = 0;
		for_each(m_ptr_positions.begin(), m_ptr_positions.end(), [&total_cost](std::unique_ptr<Position>& ptr_pos) {total_cost += ptr_pos->get_cost(); }); // складываем стоимости позиций
		return total_cost;
	}
	void get_info() const {
		for_each(m_ptr_positions.begin(), m_ptr_positions.end(), [](std::unique_ptr<Position>& ptr_pos)
			{ std::cout << ptr_pos->get_ptr_product()->get_info() << std::endl
			<< "Quantity: " << ptr_pos->get_quantity() << std::endl
			<< "Cost: " << ptr_pos->get_cost() << std::endl; });
		std::cout << "Total cost: " << get_cost() << std::endl;
	}
	bool empty() const { return m_ptr_positions.empty(); }
	friend class Client; // объ€вл€ем, что класс Client - друг, чтобы Client мог вызвать метод clear()
private:
	std::vector<std::unique_ptr<Position>> m_ptr_positions;
	void clear() { m_ptr_positions.clear(); }
};


//------------------------------------------------------------CLIENT------------------------------------------------------------------


class Client {
public:
	void earn_money(double money) { m_balance.add_money(money); } // добавили в баланс з/п
	bool pay_and_receive_order(Order& order) {
		if (m_balance.waste_money(order.get_cost()) == false) { // провер€ем наличие средств на балансе
			std::cout << "Not enough money to pay for the order!" << std::endl;
			return false;
		}
		else {
			order.clear();
			std::cout << "The order is recieved!" << std::endl;
			return true;
		}
	}
private:
	class Balance {
	public:
		void add_money(double money) { m_money += money; }
		bool waste_money(double money) {
			if (money > m_money) return false;
			else {
				m_money -= money;
				return true;
			}
		}
	private:
		double m_money = 0;
	};
	Balance m_balance;
};


//-----------------------------------------------------------PRICE_BASE---------------------------------------------------------------


class PriceBase { // singleton
public:
	PriceBase(const PriceBase&) = delete;
	PriceBase& operator= (const PriceBase&) = delete;
	static PriceBase& Instance() {
		static PriceBase instance;
		return instance;
	}
	WeightProduct& get_product_weight_price(const std::string& name) { // находим информацию о весовом продукте с нужным наименованием
		auto it = m_products_weight_price.find(name);
		if (it == m_products_weight_price.end())
			throw std::out_of_range("There are no products with this name in the database!");
		return it->second;
	}
	AmountProduct& get_product_amount_price(const std::string& name) { // находим информацию о количественном продукте с нужным наименованием
		auto it = m_products_amount_price.find(name);
		if (it == m_products_amount_price.end())
			throw std::out_of_range("There are no products with this name in the database!");
		return it->second;
	}
protected:
	PriceBase();
private:
	std::map<std::string, ProductInfo> m_products_info;
	std::map<std::string, WeightProduct> m_products_weight_price;
	std::map<std::string, AmountProduct> m_products_amount_price;
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
	m_products_amount_price = {
		{ "Pencil", AmountProduct(m_products_info.at("Pencil"), 5.7) },
		{ "Chair", AmountProduct(m_products_info.at("Chair"), 75) },
	};
}