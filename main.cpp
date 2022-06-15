#include <iostream>
#include "priceBase.h"

int main()
{
	ProductInfo Apples_inf("Green sweet apple");
	WeightProduct Apples(Apples_inf, 11.3);
	std::cout << Apples.get_info() << std::endl;

	ProductInfo Pencil_inf("Red pensile with rubber");
	AmountProduct Pencil(Pencil_inf, 12);
	std::cout << Pencil.get_info() << std::endl;

	WeightPosition apples(std::make_unique<WeightProduct>(ProductInfo("Green sweet apple"), 7), 8);

	Order order;
	std::cout << order.empty() << std::endl;
	auto pos1 = WeightPosition(std::make_unique<WeightProduct>(Apples), 0.5);
	order.add_position(std::make_unique<WeightPosition>(std::move(pos1)));
	order.get_info();
	std::cout << std::endl;

	auto pos2 = AmountPosition(std::make_unique<AmountProduct>(Pencil),2);
	order.add_position(std::make_unique<AmountPosition>(std::move(pos2)));
	order.get_info();
	std::cout << std::endl;

	PriceBase& priceBase = PriceBase::Instance();
	auto pos4 = WeightPosition(std::make_unique<WeightProduct>(priceBase.get_product_weight_price("Apple")), 2);
	order.add_position(std::make_unique<WeightPosition>(std::move(pos4)));
	order.get_info();
	std::cout << std::endl;

	Client Ivan;
	Ivan.pay_and_receive_order(order);
	Ivan.earn_money(500);
	Ivan.pay_and_receive_order(order);
	system("pause");
	return 0;
}