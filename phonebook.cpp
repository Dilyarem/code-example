#include <iostream>
#include <map>
#include <set>
#include <string>
#include <cstring>
#include <cctype>
#include <exception>
#include <algorithm>


typedef std::string name_type;
typedef std::string phone_type;
typedef std::string email_type;

struct PersonInfo {
	std::set<phone_type> phones;
	email_type email;
};

class PhoneBook {
	std::map<name_type, PersonInfo> persons;
	std::map<phone_type, std::set<name_type>> phones;
public:
	void add_person(const name_type& name, const phone_type& phone, const email_type& email) {
		if (persons.find(name) != persons.end()) {
			throw std::out_of_range("such a person already exists");
		}

		if (phone.size() != 0) {
			persons[name].phones.insert(phone);
			phones[phone].insert(name);
		}

		persons[name].email = email;
	}

	void add_phone(const name_type& name, const phone_type& phone) {
		if (persons.at(name).phones.find(phone) != persons.at(name).phones.end()) {
			throw std::out_of_range("this person already has this phone");
		}
		persons[name].phones.insert(phone);
		phones[phone].insert(name);
	}

	void replace_email(const name_type& name, const email_type& email) {
		persons.at(name).email = email;
	}

	void replace_phone(const name_type& name, const phone_type& old_phone, const phone_type& new_phone) {
		if (persons.at(name).phones.find(new_phone) != persons.at(name).phones.end()) {
			throw std::out_of_range("this person already has such a new phone");
		}
		if (persons.at(name).phones.erase(old_phone) == 0) {
			throw std::out_of_range("this person didn`t have such a old phone");
		}
		phones.at(old_phone).erase(name);
		if (phones.at(old_phone).size() == 0) {
			phones.erase(old_phone);
		}
		phones[new_phone].insert(name);
		persons.at(name).phones.insert(new_phone);
	}

	void delete_person(const name_type& name) {
		for (auto phone_iter = persons.at(name).phones.begin(); phone_iter != persons.at(name).phones.end(); ++phone_iter) {
			phones[*phone_iter].erase(name);
			if (phones.at(*phone_iter).size() == 0) {
				phones.erase(*phone_iter);
			}
		}
		persons.erase(name);
	}

	void delete_phone(const name_type& name, const phone_type& phone) {
		if (persons.at(name).phones.erase(phone) == 0) {
			throw std::out_of_range("this person didn`t have such a phone");
		}
		phones.at(phone).erase(name);
		if (phones.at(phone).size() == 0) {
			phones.erase(phone);
		}
	}

	void print_person(const name_type& name) const {
		persons.at(name);
		std::cout << "Name: " << name;
		if (persons.at(name).phones.size() != 0) {
			std::cout << "; Phones: ";
			for (auto phone_iter = persons.at(name).phones.begin(); phone_iter != persons.at(name).phones.end(); ++phone_iter) {
				std::cout << *phone_iter;
				if (phone_iter != (--persons.at(name).phones.end())) {
					std::cout << ", ";
				}
			}
		}
		if (persons.at(name).email.size() != 0) {
			std::cout << "; Email: " << persons.at(name).email;
		}
		std::cout << '\n';
	}

	void print_phone(const phone_type& phone) const {
		phones.at(phone);
		std::cout << "Names: ";
		for (auto name_iter = phones.at(phone).begin(); name_iter != phones.at(phone).end(); ++name_iter) {
			std::cout << *name_iter;
			if (name_iter != --phones.at(phone).end()) {
				std::cout << ", ";
			}
		}
		std::cout << '\n';
	}
};

void OperCompilator(const std::string& operation, PhoneBook& phone_book, const name_type& name, const phone_type& old_phone, const phone_type& new_phone, const email_type& email) {
	if (!operation.compare("add person")) {
		try {
			phone_book.add_person(name, old_phone, email);
			std::cout << "DONE\n";
		}
		catch (const std::out_of_range&) {
			std::cout << "FAIL\n";
		}
		return;
	}

	if (!operation.compare("add phone")) {
		try {
			phone_book.add_phone(name, old_phone);
			std::cout << "DONE\n";
		}
		catch (const std::out_of_range&) {
			std::cout << "FAIL\n";
		}
		return;
	}

	if (!operation.compare("replace email")) {
		try {
			phone_book.replace_email(name, email);
			std::cout << "DONE\n";
		}
		catch (const std::out_of_range&) {
			std::cout << "FAIL\n";
		}
		return;
	}

	if (!operation.compare("replace phone")) {
		try {
			phone_book.replace_phone(name, old_phone, new_phone);
			std::cout << "DONE\n";
		}
		catch (const std::out_of_range&) {
			std::cout << "FAIL\n";
		}
		return;
	}

	if (!operation.compare("delete person")) {
		try {
			phone_book.delete_person(name);
			std::cout << "DONE\n";
		}
		catch (const std::out_of_range&) {
			std::cout << "FAIL\n";
		}
		return;
	}

	if (!operation.compare("delete phone")) {
		try {
			phone_book.delete_phone(name, old_phone);
			std::cout << "DONE\n";
		}
		catch (const std::out_of_range&) {
			std::cout << "FAIL\n";
		}
		return;
	}

	if (!operation.compare("print person")) {
		try {
			phone_book.print_person(name);
		}
		catch (const std::out_of_range&) {
			std::cout << "FAIL\n";
		}
		return;
	}

	if (!operation.compare("print phone")) {
		try {
			phone_book.print_phone(old_phone);
		}
		catch (const std::out_of_range&) {
			std::cout << "FAIL\n";
		}
		return;
	}
}

bool IsEmail(const email_type& email) {
	return email.find('@') != std::string::npos;
}

bool Has_Digits(const phone_type& phone) {
	for (auto iter = phone.begin(); iter != phone.end(); ++iter) {
		if (std::isdigit(*iter)) {
			return true;
		}
	}
	return false;
}

void InputParse(std::string& input, name_type& name, phone_type& old_phone, phone_type& new_phone, email_type& email) {
	bool has_old_phone = false;
	std::string word;
	while (input.size() > 0) {
		auto pos = input.find_first_of(' ');
		if (pos != std::string::npos) {
			word = input.substr(0, pos);
			input = input.substr(pos + 1);
		} else {
			word = input;
			input = "";
		}

		if (!word.compare("Email:") || !word.compare("Phones:"))
			break;

		if (!IsEmail(word) && !Has_Digits(word)) {
			if (name.size() == 0) {
				name += word;
			} else {
				name += ' ' + word;
			}
			continue;
		}

		if (IsEmail(word)) {
			email = word;
			continue;
		}

		if (!has_old_phone) {
			old_phone = word;
			has_old_phone = true;
		} else {
			new_phone = word;
		}
	}
}

phone_type FormatePhone(const phone_type& phone) {
	phone_type formated_phone;
	std::remove_copy_if(phone.begin(), phone.end(), std::back_inserter(formated_phone),
		[](char symb) {
		return !isdigit(symb);
	});
	return formated_phone;
}

int main() {
	PhoneBook phone_book;

	std::string operation;
	std::string oper_detail;
	while (true) {
		std::cin >> operation;
		
		if (!operation.compare("finish"))
			break;

		std::cin >> oper_detail;
		operation += ' ' + oper_detail;

		std::string  input;
		std::getline(std::cin, input);
		input = input.substr(1);

		name_type name;
		phone_type old_phone;
		phone_type new_phone;
		email_type email;

		InputParse(input, name, old_phone, new_phone, email);

		old_phone = FormatePhone(old_phone);
		new_phone = FormatePhone(new_phone);

		OperCompilator(operation, phone_book, name, old_phone, new_phone, email);
	}

	return 0;
}