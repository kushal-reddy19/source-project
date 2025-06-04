#include <iostream>
#include <fstream>
#include <iomanip>
#include <vector>
#include <string>
using namespace std;

class Room {
public:
    int roomNumber;
    string roomType;
    double rentPerDay;
    bool isBooked;

    Room(int rn = 0, string rt = "", double rent = 0.0, bool booked = false)
        : roomNumber(rn), roomType(rt), rentPerDay(rent), isBooked(booked) {}

    void displayRoom() {
        cout << setw(10) << roomNumber << setw(15) << roomType << setw(15)
             << rentPerDay << setw(15) << (isBooked ? "Booked" : "Available") << endl;
    }

    void saveRoom(ofstream &fout) {
        fout << roomNumber << "," << roomType << "," << rentPerDay << "," << isBooked << endl;
    }

    void loadRoom(ifstream &fin) {
        char comma;
        fin >> roomNumber >> comma;
        getline(fin, roomType, ',');
        fin >> rentPerDay >> comma >> isBooked;
    }
};

class Booking {
public:
    string customerName;
    string customerID;
    int roomNumber;
    string checkInDate;
    string checkOutDate;
    int daysStayed;
    double rentPerDay;

    Booking(string name = "", string id = "", int rNum = 0, string in = "", string out = "",
            int days = 0, double rent = 0.0)
        : customerName(name), customerID(id), roomNumber(rNum),
          checkInDate(in), checkOutDate(out), daysStayed(days), rentPerDay(rent) {}

    void displayBooking() {
        cout << "Customer: " << customerName << ", ID: " << customerID
             << ", Room: " << roomNumber << ", Stay: " << daysStayed
             << " days, Total: " << rentPerDay * daysStayed << endl;
    }

    void saveBooking(ofstream &fout) {
        fout << customerName << "," << customerID << "," << roomNumber << ","
             << checkInDate << "," << checkOutDate << "," << daysStayed << "," << rentPerDay << endl;
    }

    void loadBooking(ifstream &fin) {
        char comma;
        getline(fin, customerName, ',');
        getline(fin, customerID, ',');
        fin >> roomNumber >> comma;
        getline(fin, checkInDate, ',');
        getline(fin, checkOutDate, ',');
        fin >> daysStayed >> comma >> rentPerDay;
    }

    void generateReceipt() {
        ofstream rec("receipt_" + customerID + ".txt");
        rec << "================= Hotel Booking Receipt =================\n";
        rec << "Customer Name   : " << customerName << "\n";
        rec << "Customer ID     : " << customerID << "\n";
        rec << "Room Number     : " << roomNumber << "\n";
        rec << "Check-in Date   : " << checkInDate << "\n";
        rec << "Check-out Date  : " << checkOutDate << "\n";
        rec << "Room Rent/Day   : " << rentPerDay << "\n";
        rec << "Total Days      : " << daysStayed << "\n";
        rec << "---------------------------------------------------------\n";
        rec << "Total Bill      : " << rentPerDay * daysStayed << "\n";
        rec << "=========================================================\n";
        rec.close();
        cout << "Receipt saved as: receipt_" << customerID << ".txt\n";
    }
};

// Global vectors
vector<Room> rooms;
vector<Booking> bookings;

// File operations
void loadRooms() {
    rooms.clear();
    ifstream fin("rooms.txt");
    while (fin.peek() != EOF) {
        Room r;
        r.loadRoom(fin);
        if (fin) rooms.push_back(r);
    }
    fin.close();
}

void saveRooms() {
    ofstream fout("rooms.txt");
    for (Room r : rooms)
        r.saveRoom(fout);
    fout.close();
}

void loadBookings() {
    bookings.clear();
    ifstream fin("bookings.txt");
    while (fin.peek() != EOF) {
        Booking b;
        b.loadBooking(fin);
        if (fin) bookings.push_back(b);
    }
    fin.close();
}

void saveBookings() {
    ofstream fout("bookings.txt");
    for (Booking b : bookings)
        b.saveBooking(fout);
    fout.close();
}

// Room operations
void addRoom() {
    int rn;
    string type;
    double rent;
    cout << "Enter room number: ";
    cin >> rn;
    cout << "Enter room type (Single/Double/Deluxe): ";
    cin >> type;
    cout << "Enter rent per day: ";
    cin >> rent;
    rooms.push_back(Room(rn, type, rent, false));
    saveRooms();
    cout << "Room added successfully.\n";
}

void displayAllRooms() {
    cout << setw(10) << "Room No" << setw(15) << "Type" << setw(15)
         << "Rent" << setw(15) << "Status" << endl;
    for (Room r : rooms)
        r.displayRoom();
}

void modifyRoom() {
    int rn;
    cout << "Enter room number to modify: ";
    cin >> rn;
    bool found = false;
    for (auto &r : rooms) {
        if (r.roomNumber == rn) {
            found = true;
            cout << "Current Room:\n";
            r.displayRoom();
            cout << "Enter new type: ";
            cin >> r.roomType;
            cout << "Enter new rent: ";
            cin >> r.rentPerDay;
            saveRooms();
            cout << "Room updated.\n";
            break;
        }
    }
    if (!found) cout << "Room not found.\n";
}

void deleteRoom() {
    int rn;
    cout << "Enter room number to delete: ";
    cin >> rn;
    for (auto it = rooms.begin(); it != rooms.end(); ++it) {
        if (it->roomNumber == rn) {
            rooms.erase(it);
            saveRooms();
            cout << "Room deleted.\n";
            return;
        }
    }
    cout << "Room not found.\n";
}

void displayAvailableRooms() {
    cout << "\n=== Available Rooms ===\n";
    for (Room r : rooms)
        if (!r.isBooked) r.displayRoom();
}

void displayBookedRooms() {
    cout << "\n=== Booked Rooms ===\n";
    for (Room r : rooms)
        if (r.isBooked) r.displayRoom();
}

// Booking operations
void bookRoom() {
    string cname, cid, checkin, checkout;
    int rnum, days;
    displayAvailableRooms();
    cout << "Enter room number to book: ";
    cin >> rnum;

    Room *roomPtr = nullptr;
    for (auto &r : rooms) {
        if (r.roomNumber == rnum && !r.isBooked) {
            roomPtr = &r;
            break;
        }
    }

    if (!roomPtr) {
        cout << "Room not available.\n";
        return;
    }

    cout << "Enter customer name: ";
    cin >> cname;
    cout << "Enter customer ID: ";
    cin >> cid;
    cout << "Enter check-in date (YYYY-MM-DD): ";
    cin >> checkin;
    cout << "Enter check-out date (YYYY-MM-DD): ";
    cin >> checkout;
    cout << "Enter number of days staying: ";
    cin >> days;

    bookings.push_back(Booking(cname, cid, rnum, checkin, checkout, days, roomPtr->rentPerDay));
    roomPtr->isBooked = true;

    saveRooms();
    saveBookings();
    cout << "Room booked successfully.\n";
}

void checkOutRoom() {
    string cid;
    cout << "Enter customer ID for checkout: ";
    cin >> cid;

    for (auto it = bookings.begin(); it != bookings.end(); ++it) {
        if (it->customerID == cid) {
            for (auto &r : rooms)
                if (r.roomNumber == it->roomNumber)
                    r.isBooked = false;
            it->generateReceipt();
            bookings.erase(it);
            saveRooms();
            saveBookings();
            cout << "Check-out complete.\n";
            return;
        }
    }
    cout << "Booking not found.\n";
}

void searchBooking() {
    int option;
    cout << "\nSearch by:\n1. Customer Name\n2. Room Number\nChoice: ";
    cin >> option;
    if (option == 1) {
        string name;
        cout << "Enter customer name: ";
        cin >> name;
        bool found = false;
        for (Booking b : bookings)
            if (b.customerName == name) {
                b.displayBooking();
                found = true;
            }
        if (!found) cout << "No booking found.\n";
    } else if (option == 2) {
        int roomNum;
        cout << "Enter room number: ";
        cin >> roomNum;
        bool found = false;
        for (Booking b : bookings)
            if (b.roomNumber == roomNum) {
                b.displayBooking();
                found = true;
            }
        if (!found) cout << "No booking found.\n";
    } else {
        cout << "Invalid option.\n";
    }
}

void displayAllBookings() {
    if (bookings.empty()) {
        cout << "No bookings found.\n";
        return;
    }
    for (const Booking &b : bookings)
        b.displayBooking();
}

void displayAllReceipts() {
    for (const Booking &b : bookings)
        b.generateReceipt();
}

// Menu
void showMenu() {
    int choice;
    do {
        cout << "\n=== Hotel Management Menu ===\n";
        cout << "1. Add Room\n2. Display All Rooms\n3. Modify Room\n4. Delete Room\n";
        cout << "5. Show Available Rooms\n6. Show Booked Rooms\n";
        cout << "7. Book Room\n8. Check-Out\n9. Search Booking\n10. View All Bookings\n";
        cout << "11. Generate All Receipts\n12. Exit\nChoice: ";
        cin >> choice;

        switch (choice) {
            case 1: addRoom(); break;
            case 2: displayAllRooms(); break;
            case 3: modifyRoom(); break;
            case 4: deleteRoom(); break;
            case 5: displayAvailableRooms(); break;
            case 6: displayBookedRooms(); break;
            case 7: bookRoom(); break;
            case 8: checkOutRoom(); break;
            case 9: searchBooking(); break;
            case 10: displayAllBookings(); break;
            case 11: displayAllReceipts(); break;
            case 12: cout << "Exiting...\n"; break;
            default: cout << "Invalid choice.\n"; break;
        }
    } while (choice != 12);
}

// Main
int main() {
    loadRooms();
    loadBookings();
    showMenu();
    return 0;
}

