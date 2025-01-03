#include <iostream>
#include <vector>
#include <string>
#include <algorithm> // For std::transform and std::tolower

using namespace std;

struct Customer {
    string name;
    string phone;
    string source;
    string destination;
    int seat_number = -1; // Default seat number
    char reserved_for = 'N'; // Default reserved for 'None'
};

class SeatHeap {
public:
    vector<Customer> customers;
    vector<bool> is_booked;
    int capacity;

    SeatHeap(int capacity) : capacity(capacity) {
        customers.resize(capacity);
        is_booked.resize(capacity, false);
    }

    int book_seat(char gender, const string& name, const string& phone, const string& source, const string& destination) {
        for (int i = 0; i < capacity; i++) {
            // Check if seat is available and matches gender requirement or is unreserved
            if (!is_booked[i] && (customers[i].reserved_for == gender || customers[i].reserved_for == 'N')) {
                is_booked[i] = true;
                customers[i].seat_number = i + 1;
                customers[i].name = name;
                customers[i].phone = phone;
                customers[i].source = source;
                customers[i].destination = destination;
                customers[i].reserved_for = gender; // Mark the seat as reserved for the given gender

                cout << "Seat " << i + 1 << " booked successfully for " << name << "." << endl;
                return i + 1;
            }
        }
        cout << "No available seat for the specified requirements." << endl;
        return -1; // No available seats
    }
};

// Helper function to convert a string to lowercase
string to_lowercase(const string& str) {
    string lower_str = str;
    transform(lower_str.begin(), lower_str.end(), lower_str.begin(), ::tolower);
    return lower_str;
}

// Function to build the prefix table for the KMP algorithm
void build_prefix_table(const string& pattern, vector<int>& prefix_table) {
    int m = pattern.size();
    int j = 0;
    prefix_table[0] = 0;

    for (int i = 1; i < m; i++) {
        while (j > 0 && pattern[i] != pattern[j]) {
            j = prefix_table[j - 1];
        }
        if (pattern[i] == pattern[j]) {
            j++;
        }
        prefix_table[i] = j;
    }
}

// Function to search for a booked ticket by name using KMP algorithm
void search_booked_ticket(SeatHeap& heap, const string& name_to_search) {
    string search_name = to_lowercase(name_to_search);
    int n = search_name.size();
    vector<int> prefix_table(n);
    build_prefix_table(search_name, prefix_table);

    for (int i = 0; i < heap.capacity; i++) {
        if (heap.is_booked[i]) { // Search only in booked tickets
            const string& ticket_name = to_lowercase(heap.customers[i].name); // Convert stored name to lowercase
            int j = 0; // Index for the pattern

            // Perform KMP search
            for (int k = 0; k < ticket_name.size(); k++) {
                while (j > 0 && ticket_name[k] != search_name[j]) {
                    j = prefix_table[j - 1];
                }
                if (ticket_name[k] == search_name[j]) {
                    j++;
                }
                if (j == n) { // Full match found
                    cout << "Ticket found! Seat Number: " << heap.customers[i].seat_number << endl;
                    return;
                }
            }
        }
    }

    cout << "No ticket found for the name: " << name_to_search << endl;
}

int main() {
    SeatHeap heap(20);

    // Booking some seats
    heap.book_seat('M', "John", "1234567890", "CityA", "CityB");
    heap.book_seat('F', "Alice", "9876543210", "CityC", "CityD");
    heap.book_seat('M', "Bob", "1122334455", "CityE", "CityF");

    // Search for a ticket
    string name_to_search;
    cout << "\nEnter the name to search for the ticket: ";
    cin >> name_to_search;

    search_booked_ticket(heap, name_to_search);

    return 0;
}
