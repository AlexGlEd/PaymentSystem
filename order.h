#pragma once
#include "position.h"
#include <vector>
#include <algorithm>

class Order {
public:
	void add_position(std::unique_ptr<Position>&& position) { 
		const std::string position_info = position->get_ptr_product()->get_info(); // запоминаем информацию о продукте
		auto pos_cmp = [position_info](std::unique_ptr<Position>& ptr_pos) -> bool // л€мбда-функци€ в качестве унарного предиката: сравнивает информацию о текущем продукте с position_info
		{ return ptr_pos->get_ptr_product()->get_info() == position_info; };
		auto old_pos = find_if(m_ptr_positions.begin(), m_ptr_positions.end(), pos_cmp);
		if (old_pos == m_ptr_positions.end()) // если в заказе не было указанного продукта
			m_ptr_positions.push_back(std::move(position)); // добавл€ем новую позицию в заказ
		 //а не надо ли тут убить старую позицию?
		else *old_pos = std::move(position); // иначе мен€ем позицию
	}
	double get_cost() {
		double  total_cost = 0;
		for_each(m_ptr_positions.begin(), m_ptr_positions.end(), [&total_cost](std::unique_ptr<Position>& ptr_pos) {total_cost += ptr_pos->get_cost(); }); // складываем стоимости позиций
		return total_cost;
	}
	void get_info() {
		for_each(m_ptr_positions.begin(), m_ptr_positions.end(), [](std::unique_ptr<Position>& ptr_pos)
			{ std::cout << ptr_pos->get_ptr_product()->get_info() << std::endl
			    << "Quantity: " << ptr_pos->get_quantity() << std::endl
			        << "Cost: " << ptr_pos->get_cost() << std::endl; });
		std::cout << "Total cost: " << get_cost() << std::endl;
	}
	bool empty() { return m_ptr_positions.size(); }
	friend class Client; // объ€вл€ем, что класс Client - друг, чтобы Client мог вызвать метод clear()
private:
	std::vector<std::unique_ptr<Position>> m_ptr_positions;
	void clear() { m_ptr_positions.clear(); }
};