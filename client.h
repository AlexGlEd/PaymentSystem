#pragma once
#include "order.h"


class Client {
public:
	void earn_money(double money) { m_balance.add_money(money); }
	bool pay_and_receive_order(Order& order) {
		if (m_balance.waste_money(order.get_cost()) == false) {
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

