//added search function 

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
        ofstream rec;
        string filename = "receipt_" + customerID + ".txt";
        rec.open(filename.c_str());
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
        cout << "Receipt generated and saved as: " << filename << endl;
    }
};

// Global Vectors
vector<Room> rooms;
vector<Booking> bookings;

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

void bookRoom() {
    string cname, cid, checkin, checkout;
    int rnum, days;
    displayAllRooms();
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
        cout << "Room not available or already booked.\n";
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
            for (auto &r : rooms) {
                if (r.roomNumber == it->roomNumber)
                    r.isBooked = false;
            }
            it->generateReceipt();
            bookings.erase(it);
            saveRooms();
            saveBookings();
            cout << "Check-out complete and room marked available.\n";
            return;
        }
    }
    cout << "No booking found with that customer ID.\n";
}

void searchBooking() {
    int option;
    cout << "\nSearch Booking by:\n";
    cout << "1. Customer Name\n";
    cout << "2. Room Number\n";
    cout << "Enter your choice: ";
    cin >> option;

    if (option == 1) {
        string name;
        cout << "Enter customer name to search: ";
        cin >> name;
        bool found = false;
        for (Booking b : bookings) {
            if (b.customerName == name) {
                b.displayBooking();
                found = true;
            }
        }
        if (!found) cout << "No booking found for customer: " << name << endl;
    } else if (option == 2) {
        int roomNum;
        cout << "Enter room number to search: ";
        cin >> roomNum;
        bool found = false;
        for (Booking b : bookings) {
            if (b.roomNumber == roomNum) {
                b.displayBooking();
                found = true;
            }
        }
        if (!found) cout << "No booking found for room number: " << roomNum << endl;
    } else {
        cout << "Invalid option.\n";
    }
}

void showMenu() {
    int choice;
    do {
        cout << "\n=== Hotel Booking and Room Management ===\n";
        cout << "1. Add Room\n";
        cout << "2. Display All Rooms\n";
        cout << "3. Book Room\n";
        cout << "4. Check-Out Room\n";
        cout << "5. Search Booking\n";
        cout << "6. Exit\n";
        cout << "Enter choice: ";
        cin >> choice;

        switch (choice) {
            case 1: addRoom(); break;
            case 2: displayAllRooms(); break;
            case 3: bookRoom(); break;
            case 4: checkOutRoom(); break;
            case 5: searchBooking(); break;
            case 6: cout << "Exiting program...\n"; break;
            default: cout << "Invalid choice!\n"; break;
        }
    } while (choice != 6);
}

int main() {
    loadRooms();
    loadBookings();
    showMenu();
    return 0;
}
