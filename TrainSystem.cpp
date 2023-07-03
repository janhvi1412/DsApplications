#include <iostream>
#include <vector>
#include <string>

using namespace std;

class Train {
public:
    string name;
    int totalSeats;
    int availableSeats;

    Train(string trainName, int seats) : name(trainName), totalSeats(seats), availableSeats(seats) {}

    bool bookTicket(int numSeats) {
        if (numSeats <= availableSeats) {
            availableSeats -= numSeats;
            return true;
        }
        return false;
    }

    void cancelTicket(int numSeats) {
        availableSeats += numSeats;
    }
};

class ReservationSystem {
public:
    vector<Train> trains;

    void addTrain(const Train& train) {
        trains.push_back(train);
    }

    void displayTrains() {
        cout << "Available Trains:" << endl;
        for (const Train& train : trains) {
            cout << "Train Name: " << train.name << endl;
            cout << "Available Seats: " << train.availableSeats << endl;
            cout << endl;
        }
    }

    void bookTicket(int trainIndex, int numSeats) {
        if (trainIndex >= 0 && trainIndex < trains.size()) {
            Train& train = trains[trainIndex];
            if (train.bookTicket(numSeats)) {
                cout << "Ticket booked successfully!" << endl;
            } else {
                cout << "Sorry, there are not enough seats available." << endl;
            }
        } else {
            cout << "Invalid train index." << endl;
        }
    }

    void cancelTicket(int trainIndex, int numSeats) {
        if (trainIndex >= 0 && trainIndex < trains.size()) {
            Train& train = trains[trainIndex];
            train.cancelTicket(numSeats);
            cout << "Ticket canceled successfully!" << endl;
        } else {
            cout << "Invalid train index." << endl;
        }
    }
};

int main() {
    ReservationSystem reservationSystem;

    Train train1("Express", 100);
    Train train2("Rajdhani", 150);
    Train train3("Shatabdi", 200);

    reservationSystem.addTrain(train1);
    reservationSystem.addTrain(train2);
    reservationSystem.addTrain(train3);

    int choice;
    do {
        cout << "Railway Reservation System" << endl;
        cout << "1. Display Available Trains" << endl;
        cout << "2. Book Ticket" << endl;
        cout << "3. Cancel Ticket" << endl;
        cout << "4. Exit" << endl;
        cout << "Enter your choice: ";
        cin >> choice;

        switch (choice) {
            case 1:
                reservationSystem.displayTrains();
                break;
            case 2:
                int trainIndex, numSeats;
                cout << "Enter the train index: ";
                cin >> trainIndex;
                cout << "Enter the number of seats: ";
                cin >> numSeats;
                reservationSystem.bookTicket(trainIndex, numSeats);
                break;
            case 3:
                cout << "Enter the train index: ";
                cin >> trainIndex;
                cout << "Enter the number of seats: ";
                cin >> numSeats;
                reservationSystem.cancelTicket(trainIndex, numSeats);
                break;
            case 4:
                cout << "Exiting... Thank you!" << endl;
                break;
            default:
                cout << "Invalid choice. Please try again." << endl;
                break;
        }
        cout << endl;

    } while (choice != 4);

    return 0;
}
