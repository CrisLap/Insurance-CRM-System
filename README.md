# Insurance-CRM-System
### InsuraPro Solution:  CRM System

Insurance CRM System aims to simplify customer relationship management for insurance companies by developing an intuitive and interactive software that optimizes the organization and access to customer information. This project is a console-based application written in C++, leveraging Object-Oriented Programming (OOP) principles to provide a structured and scalable CRM system.

## Project Structure

- `insurance_crm.cpp` : Contains the implementation of the `Interaction`, `Customer`, and `CRM` classes and the main user interface.
- `customers.csv`: The CSV file where customer data and interactions are saved and loaded.

## Requirements

- C++ compiler (e.g., `g++`)
- Development environment like Visual Studio Code

## Compilation and Execution

To compile and run the project, follow these steps:

1. Open the terminal and navigate to the project directory.
2. Compile the file `insurance_crm.cpp` using the command:

    ```
    g++ .vscode/insurance_crm.cpp -o insurance_crm
    ```

3. Run the compiled program:

    ```
    ./insurance_crm
    ```

## Features

1. **Add Customer**: Allows the user to add new customers. Validates the first name, last name, email, and phone number. Checks for duplicates and prompts the user if a duplicate is found.
2. **Display All Customers**: Displays all customers present in the CSV file. If the file is empty, it notifies the user.
3. **Search Customer**: Searches for customers by first name or last name and notifies the user if the customer is not found.
4. **Modify Customer**: Enables modification of existing customer details. Searches for customers by name and notifies the user if no customers are available. Validates the modified data similarly to adding a new customer and checks for duplicates.
5. **Delete Customer**: Removes customers from the CRM by searching for customers by name. Notifies the user if the customer is not found.
6. **Add Interaction**: Allows the user to add interactions (appointment, contact, contract) for a customer.
7. **Display Interactions**: Displays all interactions of a specific customer.

## Getting Started

### Installation

Ensure you have a C++ compiler installed on your machine.

### Running the Program

Run the following command to start the program:

```sh
./insurance_crm
```

## Project Diagram

```plaintext
insurance_crm.cpp
│
├── Interaction
│   ├── Interaction(type, date)
│
├── Customer
│   ├── Customer(firstName, lastName, email, phone, customerId)
│   ├── operator==(other)  // Overloaded equality operator
│
├── CRM
│   ├── CRM()
│   ├── loadFromFile()
│   ├── saveToFile()
│   ├── isValidFirstName(firstName)
│   ├── isValidLastName(lastName)
│   ├── isValidEmail(email)
│   ├── isValidPhone(phone)
│   ├── findCustomerById(id)
│   ├── addCustomer()
│   ├── displayCustomers()
│   ├── searchCustomers(name)
│   ├── modifyCustomer()
│   ├── deleteCustomer()
│   ├── addInteraction(customerId, type, date)
│   ├── displayInteractions(customerId)
│
├── main()
```

## License

This project is distributed under the MIT License. See the `LICENSE` file for more details.


