// Hotel Booking and Room Management System 

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <iomanip>

using namespace std;

// Structure to store date
struct Date {
    int day, month, year;
};

// Class representing a room
class Room {
public:
    int roomNumber;
    string roomType;
    float pricePerNight;
    bool isBooked;

    Room() {}
    Room(int number, string type, float price, bool booked = false) {
        roomNumber = number;
        roomType = type;
        pricePerNight = price;
        isBooked = booked;
    }

    void displayRoom() {
        cout << left << setw(10) << roomNumber
             << setw(15) << roomType
             << setw(10) << pricePerNight
             << (isBooked ? "Booked" : "Available") << endl;
    }
};

// Class for customer details
class Customer {
public:
    string name;
    string idProof;
    string contact;
    Date checkIn;
    Date checkOut;
    int roomNumber;

    void displayBooking() {
        cout << left << setw(15) << name
             << setw(15) << idProof
             << setw(15) << contact
             << "Room: " << roomNumber << "\tCheck-in: "
             << checkIn.day << "/" << checkIn.month << "/" << checkIn.year
             << "\tCheck-out: " << checkOut.day << "/" << checkOut.month << "/" << checkOut.year
             << endl;
    }
};

// Manager class to control the hotel system
class HotelManager {
    vector<Room> rooms;
    vector<Customer> bookings;

public:
    HotelManager() {
        loadRooms();
        loadBookings();
    }

    void loadRooms();
    void saveRooms();
    void loadBookings();
    void saveBookings();

    void addRoom();
    void viewRooms();
    void deleteRoom();
    void editRoom();

    void bookRoom();
    void viewBookings();
    void searchBooking();
    void checkoutCustomer();
};

// Load rooms from file
void HotelManager::loadRooms() {
    ifstream fin("rooms.txt");
    Room r;
    while (fin >> r.roomNumber >> r.roomType >> r.pricePerNight >> r.isBooked) {
        rooms.push_back(r);
    }
    fin.close();
}

// Save rooms to file
void HotelManager::saveRooms() {
    ofstream fout("rooms.txt");
    for (Room r : rooms) {
        fout << r.roomNumber << " " << r.roomType << " "
             << r.pricePerNight << " " << r.isBooked << endl;
    }
    fout.close();
}

// Load bookings from file
void HotelManager::loadBookings() {
    ifstream fin("bookings.txt");
    Customer c;
    while (fin >> c.name >> c.idProof >> c.contact
               >> c.roomNumber
               >> c.checkIn.day >> c.checkIn.month >> c.checkIn.year
               >> c.checkOut.day >> c.checkOut.month >> c.checkOut.year) {
        bookings.push_back(c);
    }
    fin.close();
}

// Save bookings to file
void HotelManager::saveBookings() {
    ofstream fout("bookings.txt");
    for (Customer c : bookings) {
        fout << c.name << " " << c.idProof << " " << c.contact << " "
             << c.roomNumber << " "
             << c.checkIn.day << " " << c.checkIn.month << " " << c.checkIn.year << " "
             << c.checkOut.day << " " << c.checkOut.month << " " << c.checkOut.year << endl;
    }
    fout.close();
}

// Add a new room
void HotelManager::addRoom() {
    int number;
    string type;
    float price;
    cout << "Enter Room Number: ";
    cin >> number;
    cout << "Enter Room Type (Single/Double/Deluxe): ";
    cin >> type;
    cout << "Enter Price Per Night: ";
    cin >> price;
    Room r(number, type, price);
    rooms.push_back(r);
    saveRooms();
    cout << "Room added successfully.\n";
}

// View all rooms
void HotelManager::viewRooms() {
    cout << left << setw(10) << "Room No"
         << setw(15) << "Type"
         << setw(10) << "Price"
         << "Status" << endl;
    for (Room r : rooms) {
        r.displayRoom();
    }
}

// Delete room
void HotelManager::deleteRoom() {
    int number;
    cout << "Enter room number to delete: ";
    cin >> number;
    bool found = false;
    for (auto it = rooms.begin(); it != rooms.end(); ++it) {
        if (it->roomNumber == number) {
            rooms.erase(it);
            found = true;
            break;
        }
    }
    saveRooms();
    if (found)
        cout << "Room deleted successfully.\n";
    else
        cout << "Room not found.\n";
}

// Edit room details
void HotelManager::editRoom() {
    int number;
    cout << "Enter room number to edit: ";
    cin >> number;
    for (Room &r : rooms) {
        if (r.roomNumber == number) {
            cout << "Enter new type: ";
            cin >> r.roomType;
            cout << "Enter new price: ";
            cin >> r.pricePerNight;
            saveRooms();
            cout << "Room updated.\n";
            return;
        }
    }
    cout << "Room not found.\n";
}

// Book a room
void HotelManager::bookRoom() {
    Customer c;
    cout << "Enter customer name: ";
    cin >> c.name;
    cout << "Enter ID proof: ";
    cin >> c.idProof;
    cout << "Enter contact number: ";
    cin >> c.contact;
    viewRooms();
    cout << "Enter room number to book: ";
    cin >> c.roomNumber;
    for (Room &r : rooms) {
        if (r.roomNumber == c.roomNumber && !r.isBooked) {
            cout << "Enter check-in date (dd mm yyyy): ";
            cin >> c.checkIn.day >> c.checkIn.month >> c.checkIn.year;
            cout << "Enter check-out date (dd mm yyyy): ";
            cin >> c.checkOut.day >> c.checkOut.month >> c.checkOut.year;
            r.isBooked = true;
            bookings.push_back(c);
            saveRooms();
            saveBookings();
            cout << "Room booked successfully.\n";
            return;
        }
    }
    cout << "Room not available.\n";
}

// View all bookings
void HotelManager::viewBookings() {
    for (Customer c : bookings) {
        c.displayBooking();
    }
}

// Search booking by ID proof
void HotelManager::searchBooking() {
    string id;
    cout << "Enter ID proof to search: ";
    cin >> id;
    for (Customer c : bookings) {
        if (c.idProof == id) {
            c.displayBooking();
            return;
        }
    }
    cout << "Booking not found.\n";
}

// Checkout a customer
void HotelManager::checkoutCustomer() {
    string id;
    cout << "Enter ID proof to checkout: ";
    cin >> id;
    for (auto it = bookings.begin(); it != bookings.end(); ++it) {
        if (it->idProof == id) {
            for (Room &r : rooms) {
                if (r.roomNumber == it->roomNumber) {
                    r.isBooked = false;
                    break;
                }
            }
            bookings.erase(it);
            saveRooms();
            saveBookings();
            cout << "Checkout complete. Room is now available.\n";
            return;
        }
    }
    cout << "Booking not found.\n";
}

// Main menu
int main() {
    HotelManager hm;
    int choice;
    do {
        cout << "\n--- Hotel Booking Management System ---\n";
        cout << "1. Add Room\n";
        cout << "2. View Rooms\n";
        cout << "3. Edit Room\n";
        cout << "4. Delete Room\n";
        cout << "5. Book Room\n";
        cout << "6. View Bookings\n";
        cout << "7. Search Booking\n";
        cout << "8. Checkout Customer\n";
        cout << "0. Exit\n";
        cout << "Enter your choice: ";
        cin >> choice;

        switch (choice) {
            case 1: hm.addRoom(); break;
            case 2: hm.viewRooms(); break;
            case 3: hm.editRoom(); break;
            case 4: hm.deleteRoom(); break;
            case 5: hm.bookRoom(); break;
            case 6: hm.viewBookings(); break;
            case 7: hm.searchBooking(); break;
            case 8: hm.checkoutCustomer(); break;
            case 0: cout << "Exiting...\n"; break;
            default: cout << "Invalid choice. Try again.\n";
        }
    } while (choice != 0);

    return 0;
}
