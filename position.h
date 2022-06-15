#pragma once
#include "product.h"


class Position { //абстрактный класс
public:
	Position(std::unique_ptr<Product>&& product) : m_ptr_product(std::move(product)) {};
	std::unique_ptr<Product>& get_ptr_product() { return m_ptr_product; }
	virtual double get_cost() const = 0;
	virtual double get_quantity() const = 0;
protected:
	std::unique_ptr<Product> m_ptr_product;
};


class WeightPosition : public Position {
public:
	WeightPosition(std::unique_ptr<WeightProduct>&& product, double weight) : Position(std::move(product)), m_weight(weight) {};
	double get_cost() const override { return m_ptr_product->get_cost() * m_weight; }
	double get_quantity() const override { return m_weight; }
private:
	double m_weight;
};


class AmountPosition : public Position {
public:
	AmountPosition(std::unique_ptr<AmountProduct>&& product, size_t amount) : Position(std::move(product)), m_amount(amount) {};
	double get_cost() const override { return m_ptr_product->get_cost() * m_amount; }
	double get_quantity() const override  { return m_amount; }
private:
	double m_amount;
};