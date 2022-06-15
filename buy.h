#include <iostream>
#include "payment.h"

void buy() {

	try {
		Order order;
		std::cout << order.empty() << std::endl;

		PriceBase& priceBase = PriceBase::Instance();

		auto pos1 = WeightPosition(std::make_unique<WeightProduct>(priceBase.get_product_weight_price("Apple")), 0.5); // добавляем яблоки в заказ
		order.add_position(std::make_unique<WeightPosition>(std::move(pos1)));
		order.get_info(); // выводим текущий заказ
		std::cout << std::endl;

		auto pos2 = AmountPosition(std::make_unique<AmountProduct>(priceBase.get_product_amount_price("Pencil")), 2); // добавляем карандаши в заказ
		order.add_position(std::make_unique<AmountPosition>(std::move(pos2)));
		order.get_info();
		std::cout << std::endl;

		auto pos3 = WeightPosition(std::make_unique<WeightProduct>(priceBase.get_product_weight_price("Apple")), 1.5); // меняем количество яблок
		order.add_position(std::make_unique<WeightPosition>(std::move(pos3)));
		order.get_info();
		std::cout << std::endl;

		Client Ivan;
		Ivan.pay_and_receive_order(order); // пытаемся оплатить заказ(но недостаточно средств)
		order.get_info();
		std::cout << std::endl;

		Ivan.earn_money(500);
		Ivan.pay_and_receive_order(order); // 2-я попытка
		order.get_info();
		std::cout << std::endl;
	}
	catch (std::exception& error) {
		std::cout << error.what() << std::endl;
	}
}