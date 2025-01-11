#include <iostream> // Standard input/output stream
#include <vector>  // Vector container
#include <string> 
#include <iomanip> // Input/output manipulators
#include <fstream> // File stream
#include <regex>
#include <limits> // Numeric limits
#include <sstream> // String stream

using namespace std;   

// Class to hold interaction data
class Interaction {
public:
    string type;
    string date;

    Interaction(string type, string date) : type(type), date(date) {}
};

// Class to hold customer data
class Customer {
public:
    string firstName;
    string lastName;
    string email;
    string phone;
    int customerId;
    vector<Interaction> interactions;

    Customer(string firstName, string lastName, string email, string phone, int customerId) 
        : firstName(firstName), lastName(lastName), email(email), phone(phone), customerId(customerId) {
    interactions.push_back(Interaction("No Interaction", "N/A")); // Default interaction
}
    
    // Overload the equality operator to compare customers
    bool operator==(const Customer& other) const {
        return this->customerId == other.customerId;  // Compare by customer ID
    }
};

// CRM class to manage customers and interactions
class CRM {
private:
    vector<Customer> customers;
    int nextCustomerId;

    // Function to validate first name using regex
    bool isValidFirstName(const string& firstName) {
        // allow letters, apostrophes, periods, hyphens, and underscores between words
        regex firstnamePattern("^[a-zA-Z]+([',.\\-_][a-zA-Z]+)*$");
        return regex_match(firstName, firstnamePattern);
    }

    // Function to validate last name using regex
    bool isValidLastName(const string& lastName) {
        // allow letters, apostrophes, periods, hyphens, and underscores between words
        regex lastnamePattern("^[a-zA-Z]+([',.\\-_][a-zA-Z]+)*$");
        return regex_match(lastName, lastnamePattern);
    }

    // Function to validate email using regex
    bool isValidEmail(const string& email) {
        regex emailPattern("[a-zA-Z0-9_+&*-]+(?:\\.[a-zA-Z0-9_+&*-]+)*@(?:[a-zA-Z0-9-]+\\.)+[a-zA-Z]{2,7}");
        return regex_match(email, emailPattern);
    }

    // Function to validate phone number using regex
    bool isValidPhone(const string& phone) {
        regex phonePattern("^\\+?[0-9]{10,15}$");
        return regex_match(phone, phonePattern);
    }

    // Function to check if the customer exists
    Customer* findCustomerById(int id) {
        for (auto& customer : customers) {
            if (customer.customerId == id) {
                return &customer;
            }
        }
        return nullptr;
    }

public:
    CRM() : nextCustomerId(1) {
        loadFromFile();  // Load data from file on start
    }

    // Function to load customers from a CSV file
    void loadFromFile() {
        ifstream file("customers.csv");
        string line;

        // ignore the header
        if (getline(file, line)) {
            cout << "Header detected: " << line << endl;  // Debug: remove if not needed
        }

        while (getline(file, line)) {
            stringstream ss(line);
            string firstName, lastName, email, phone, interactionsData;
            int customerId;

            // read customer data
            getline(ss, firstName, ',');
            getline(ss, lastName, ',');
            getline(ss, email, ',');
            getline(ss, phone, ',');
            ss >> customerId;

            // read the rest of the line as interactions data
            getline(ss, interactionsData);
            interactionsData = interactionsData.substr(interactionsData.find_first_not_of(", ")); // remove leading spaces and commas

            // Create a new customer object
            Customer customer(firstName, lastName, email, phone, customerId);

            // Parse interactions data
            stringstream interactionStream(interactionsData);
            string interactionItem;

            while (getline(interactionStream, interactionItem, '|')) {
                string type, date;
                size_t typePos = interactionItem.find("Type:");
                size_t datePos = interactionItem.find("Date:");

                if (typePos != string::npos && datePos != string::npos) {
                    type = interactionItem.substr(typePos + 5, datePos - typePos - 6); // Estract the type
                    date = interactionItem.substr(datePos + 5);                        // Estract the date
                    customer.interactions.push_back(Interaction(type, date));
                }
            }

            customers.push_back(customer);
            nextCustomerId = max(nextCustomerId, customerId + 1);
        }
    }

    // Function to save customers to CSV file
    void saveToFile() {
        ofstream file("customers.csv");

        // write the header
        file << "First Name,Last Name,Email,Phone,Customer ID,Interactions" << endl;

        for (auto& customer : customers) {
            file << customer.firstName << "," 
                << customer.lastName << "," 
                << customer.email << "," 
                << customer.phone << "," 
                << customer.customerId << ",";

            // manage interactions
            bool firstInteraction = true;
            for (size_t i = 0; i < customer.interactions.size(); ++i) {
                // ignore the default interaction if there are other interactions
                if (customer.interactions[i].type == "No Interaction" &&
                    customer.interactions[i].date == "N/A" &&
                    customer.interactions.size() > 1) {
                    continue;
                }

                if (!firstInteraction) {
                    file << "|"; // Add separator between interactions
                }
                file << "Type:" << customer.interactions[i].type 
                    << ",Date:" << customer.interactions[i].date;
                firstInteraction = false;
            }

            file << endl;
        }
    }

    // Add a new customer with input validation
    void addCustomer() {
        string firstName, lastName, email, phone;

        // Loop until valid first name is provided
        do {
            cout << "Enter First Name (blanks not allowed, use instead  ',  .,  - or  _ ): ";
            cin >> firstName;
            if (!isValidFirstName(firstName)) {
                cout << "Invalid first name format!" << endl;
            }
        } while (!isValidFirstName(firstName));  // Continue until the first name is valid

        // Loop until valid last name is provided
        do {
            cout << "Enter Last Name (blanks not allowed, use instead  ',  .,  - or  _ ): ";
            cin >> lastName;
            if (!isValidLastName(lastName)) {
                cout << "Invalid last name format!" << endl;
            }
        } while (!isValidLastName(lastName));  // Continue until the last name is valid

        // Loop until valid email is provided
        do {
            cout << "Enter Email: ";
            cin >> email;
            if (!isValidEmail(email)) {
                cout << "Invalid email format!" << endl;
            }
        } while (!isValidEmail(email));  // Continue until the email is valid

        // Loop until valid phone number is provided
        do {
            cout << "Enter Phone: ";
            cin >> phone;
            if (!isValidPhone(phone)) {
                cout << "Invalid phone number format! It should be 10-15 digits." << endl;
            }
        } while (!isValidPhone(phone));  // Continue until the phone number is valid

        // Check for duplicates
        for (auto& customer : customers) {
            if (customer.firstName == firstName && customer.lastName == lastName) {
                cout << "Customer already exists!" << endl;
                return;
            }
        }

        // Add the new customer
        Customer newCustomer(firstName, lastName, email, phone, nextCustomerId++);
        
        // add a default interaction if the customer has no interactions
        if (newCustomer.interactions.empty()) {
            newCustomer.interactions.push_back(Interaction("No Interaction, Date: N/A", "N/A"));
        }

        customers.push_back(newCustomer);
        saveToFile();  // Save the new customer to file
        cout << "Customer added successfully!" << endl;
    }

    // Display all customers with their interactions
    void displayCustomers() {
        cout << setw(10) << "ID" << setw(20) << "First Name" << setw(20) << "Last Name" 
            << setw(30) << "Email" << setw(15) << "Phone" << setw(50) << "Interactions" << endl;

        for (auto& customer : customers) {
            cout << setw(10) << customer.customerId 
                << setw(20) << customer.firstName 
                << setw(20) << customer.lastName 
                << setw(30) << customer.email 
                << setw(15) << customer.phone;

            // Control if there are interactions different from "No Interaction"
            bool hasNonDefaultInteraction = false;
            string interactionsSummary;

            for (auto& interaction : customer.interactions) {
                if (interaction.type != "No Interaction") {
                    if (!interactionsSummary.empty()) {
                        interactionsSummary += " | ";  // separator between interactions
                    }
                    interactionsSummary += "Type: " + interaction.type + ", Date: " + interaction.date;
                    hasNonDefaultInteraction = true;
                }
            }

            // if there are valid interactions, display them
            if (hasNonDefaultInteraction) {
                cout << setw(50) << interactionsSummary << endl;
            } else {
                // else display "No Interaction" only once
                cout << setw(50) << "No Interaction, Date: N/A" << endl;
            }
        }
    }

    // Search customers by name and display their details and interactions
    void searchCustomers(string name) {
        bool found = false;

        // format the output
        cout << setw(10) << "ID" << setw(20) << "First Name" << setw(20) << "Last Name" 
            << setw(30) << "Email" << setw(15) << "Phone" << setw(50) << "Interactions" << endl;

        for (auto& customer : customers) {
            if (customer.firstName == name || customer.lastName == name) {
                cout << setw(10) << customer.customerId 
                    << setw(20) << customer.firstName 
                    << setw(20) << customer.lastName 
                    << setw(30) << customer.email 
                    << setw(15) << customer.phone;

                bool hasValidInteraction = false;
                string interactionsSummary;

                // Verify if there are interactions different from "No Interaction"
                for (auto& interaction : customer.interactions) {
                    if (interaction.type != "No Interaction") {
                        if (!interactionsSummary.empty()) {
                            interactionsSummary += " | ";  // separator between interactions
                        }
                        interactionsSummary += "Type: " + interaction.type + ", Date: " + interaction.date;
                        hasValidInteraction = true;
                    }
                }

                // if there are valid interactions, display them
                if (hasValidInteraction) {
                    cout << setw(50) << interactionsSummary << endl;
                } else {
                    // else display "No Interaction" only once
                    cout << setw(50) << "Type: No Interaction, Date: N/A" << endl;
                }

                found = true;
            }
        }

        if (!found) {
            cout << "No customers found!" << endl;
        }
    }

    // Modify a customer's details
    void modifyCustomer() {
    string name;
    cout << "Enter first name or last name of the customer to modify (blanks not allowed, use instead  ',  .,  - or  _ ): ";
    cin >> name;  // user input for the name to search ( first or last name)

    vector<Customer*> matchingCustomers;
    for (auto& customer : customers) {
        if (customer.firstName == name || customer.lastName == name) {
            matchingCustomers.push_back(&customer);
        }
    }

    if (matchingCustomers.empty()) {
        cout << "No customer found with that name." << endl;
        return;
    }

    cout << "Found the following customers:\n";
    for (int i = 0; i < matchingCustomers.size(); ++i) {
        cout << i + 1 << ". " << matchingCustomers[i]->firstName << " " << matchingCustomers[i]->lastName << endl;
    }

    int choice;
    cout << "Choose the correct index to modify the customer (1 of " << matchingCustomers.size() << "): ";
    cin >> choice;

    if (choice < 1 || choice > matchingCustomers.size()) {
        cout << "Invalid choice!" << endl;
        return;
    }

    Customer* customer = matchingCustomers[choice - 1];

    // Ask if the user wants to modify all or just specific fields
    string modifyAll;
    cout << "Do you want to modify all fields (First Name, Last Name, Email, Phone)? (yes/no): ";
    cin >> modifyAll;

    if (modifyAll == "yes") {
        // Modify all details
        string firstName, lastName, email, phone;

        cout << "Enter new First Name (blanks not allowed, use instead  ',  .,  - or  _ ): ";
        cin >> firstName;
        cout << "Enter new Last Name (blanks not allowed, use instead  ',  .,  - or  _ ): ";
        cin >> lastName;
        cout << "Enter new Email: ";
        cin >> email;
        cout << "Enter new Phone: ";
        cin >> phone;

        customer->firstName = firstName;
        customer->lastName = lastName;
        customer->email = email;
        customer->phone = phone;
    } else {
        // Modify only specific fields
        char choice;
        cout << "Do you want to modify First Name? (y/n): ";
        cin >> choice;
        if (choice == 'y') {
            string newFirstName;
            cout << "Enter new First Name (blanks not allowed, use instead  ',  .,  - or  _ ): ";
            cin >> newFirstName;
            customer->firstName = newFirstName;
        }

        cout << "Do you want to modify Last Name? (y/n): ";
        cin >> choice;
        if (choice == 'y') {
            string newLastName;
            cout << "Enter new Last Name (blanks not allowed, use instead  ',  .,  - or  _ ): ";
            cin >> newLastName;
            customer->lastName = newLastName;
        }

        cout << "Do you want to modify Email? (y/n): ";
        cin >> choice;
        if (choice == 'y') {
            string newEmail;
            cout << "Enter new Email: ";
            cin >> newEmail;
            customer->email = newEmail;
        }

        cout << "Do you want to modify Phone? (y/n): ";
        cin >> choice;
        if (choice == 'y') {
            string newPhone;
            cout << "Enter new Phone: ";
            cin >> newPhone;
            customer->phone = newPhone;
            }
        }

        saveToFile();
        cout << "Customer details updated!" << endl;
    } 

    
    // Delete a customer
    void deleteCustomer() {
        string firstName, lastName;
        cout << "Enter first name or last name of the customer to delete: ";
        cin >> firstName;

        vector<Customer*> matchingCustomers;
        for (auto& customer : customers) {
            if (customer.firstName == firstName || customer.lastName == firstName) {
                matchingCustomers.push_back(&customer);
            }
        }

        if (matchingCustomers.empty()) {
            cout << "No customer found with that name." << endl;
            return;
        }

        cout << "Found the following customers:\n";
        for (int i = 0; i < matchingCustomers.size(); ++i) {
            cout << i + 1 << ". " << matchingCustomers[i]->firstName << " " << matchingCustomers[i]->lastName << endl;
        }

        int choice;
        cout << "Choose the index of the customer to delete (1 of " << matchingCustomers.size() << "): ";
        cin >> choice;

        if (choice < 1 || choice > matchingCustomers.size()) {
            cout << "Invalid choice!" << endl;
            return;
        }

        // Remove the customer
        customers.erase(remove(customers.begin(), customers.end(), *matchingCustomers[choice - 1]), customers.end());
        saveToFile();
        cout << "Customer deleted!" << endl;
    }

    // Add interaction to customer
    void addInteraction(int customerId, string type, string date) {
        Customer* customer = findCustomerById(customerId);
        if (customer) {
            customer->interactions.push_back(Interaction(type, date));
            saveToFile();
            cout << "Interaction added!" << endl;
        } else {
            cout << "Customer not found!" << endl;
        }
    }

    // Display interactions of a customer
    void displayInteractions(int customerId) {
        Customer* customer = findCustomerById(customerId);
        if (customer) {
            if (customer->interactions.empty()) {
                cout << "No interactions found for this customer." << endl;
            } else if (customer->interactions.size() == 1 &&
                    customer->interactions[0].type == "No Interaction" &&
                    customer->interactions[0].date == "N/A") {
                // shows only the default interaction if no other interactions are present
                cout << "Type: " << customer->interactions[0].type
                    << ", Date: " << customer->interactions[0].date << endl;
            } else {
                // shows all interactions except the default one
                bool hasDisplayed = false;
                for (auto& interaction : customer->interactions) {
                    if (interaction.type == "No Interaction" && interaction.date == "N/A") {
                        continue; // skip the default interaction
                    }
                    cout << "Type: " << interaction.type << ", Date: " << interaction.date << endl;
                    hasDisplayed = true;
                }
                if (!hasDisplayed) {
                    cout << "No valid interactions found for this customer." << endl;
                }
            }
        } else {
            cout << "Customer not found!" << endl;
        }
    }
};

// Main function to provide user interface
int main() {
    CRM crm;
    int choice;

    do {
        cout << "\n----------------------------------------\n";
        cout << "InsuraPro Solutions - CRM System Menu:";
        cout << "\n----------------------------------------\n";
        cout << "1. Add Customer\n";
        cout << "2. Display All Customers\n";
        cout << "3. Search Customer\n";
        cout << "4. Modify Customer\n";
        cout << "5. Delete Customer\n";
        cout << "6. Add Interaction\n";
        cout << "7. Display Interactions\n";
        cout << "8. Exit\n";
        
        // Loop to ensure that the input is a valid number between 1 and 8
        while (true) {
            cout << "Enter your choice (from 1 to 8): ";
            cin >> choice;

            // Checking whether the input is a valid number
            if (cin.fail() || choice < 1 || choice > 8) {
                cout << "Invalid choice! Please enter a number between 1 and 8." << endl;
                
                // Clean the error status of cin and ignore the rest of the input
                cin.clear();  // Cleans up error status
                cin.ignore(numeric_limits<streamsize>::max(), '\n');  // Ignore remaining characters
            } else {
                break;  // Exit the loop if the input is valid
            }
        }

        switch (choice) {
            case 1:
                crm.addCustomer();
                break;
            case 2:
                crm.displayCustomers();
                break;
            case 3: {
                string name;
                cout << "Enter name to search (first name or last name): ";
                cin >> name;
                crm.searchCustomers(name);
                break;
            }
            case 4: {
                crm.modifyCustomer(); 
                break;
            }
            case 5: {
                crm.deleteCustomer();
                break;
            }
            case 6: {
                int customerId;
                string type, date;
                cout << "Enter Customer ID to add Interaction: ";
                cin >> customerId;
                cout << "Enter Interaction Type (Meeting/Contact/Contract) - no blanks allowed: ";
                cin >> type;
                cout << "Enter Interaction Date (dd/mm/yyyy): ";
                cin >> date;
                crm.addInteraction(customerId, type, date);
                break;
            }
            case 7: {
                int customerId;
                cout << "Enter Customer ID to display interactions: ";
                cin >> customerId;
                crm.displayInteractions(customerId);
                break;
            }
            case 8:
                cout << "Exiting CRM system. Goodbye!" << endl;
                break;
            default:
                cout << "Invalid choice, please try again!" << endl;
                break;
        }
    } while (choice != 8);

    return 0;
}

