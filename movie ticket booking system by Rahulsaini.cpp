#include <iostream>
#include <fstream>
#include <string>
#include <cstdlib>
using namespace std;

struct MovieTime {
    string time;
    int price;
};

class Movie {
private:
    string name;
    MovieTime show;
    int seats;
    static int totalMovies;

public:
    Movie() {
        name = "";
        show.time = "";
        show.price = 0;
        seats = 0;
        totalMovies++;
    }

    Movie(string n, string t, int p = 150, int s = 0) {
        name = n;
        show.time = t;
        show.price = p;
        seats = s;
        totalMovies++;
    }

    Movie(const Movie &m) {
        name = m.name;
        show = m.show;
        seats = m.seats;
    }

    inline void displayMovie() {
        cout << "Movie: " << name << " | Time: " << show.time
             << " | Price: " << show.price << " | Seats: " << seats << endl;
    }

    void bookSeats(int s);
    void bookSeats(string str);

    string getName() { return name; }
    string getTime() { return show.time; }
    int getSeats() { return seats; }
    int getPrice() { return show.price; }

    void setSeats(int s) { seats = s; }

    void showInfo() const {
        cout << "[Const] Movie: " << name << " at " << show.time << endl;
    }

    friend void showDetails(Movie m);

    static int getTotalMovies() {
        return totalMovies;
    }

    ~Movie() {}
};

int Movie::totalMovies = 0;

void Movie::bookSeats(int s) {
    if (s <= seats) {
        seats -= s;
        cout << "Booked " << s << " seats successfully!\n";
    } else {
        cout << "Not enough seats available.\n";
    }
}

void Movie::bookSeats(string str) {
    int s = atoi(str.c_str());
    bookSeats(s);
}

void showDetails(Movie m) {
    cout << "[Friend] " << m.name << " - " << m.show.time
         << " - Price: " << m.show.price << " - Seats: " << m.seats << endl;
}

class Ticket {
    int *seatCount;
public:
    Ticket(int count) {
        seatCount = new int;
        *seatCount = count;
    }
    void showTicket() {
        cout << "Total tickets: " << *seatCount << endl;
    }
    ~Ticket() {
        delete seatCount;
    }
};

class BookingSystem {
protected:
    Movie movies[10];
    int count;

public:
    BookingSystem() {
        count = 0;
    }

    void addMovie(string n, string t, int p, int s) {
        if (count < 10) {
            movies[count] = Movie(n, t, p, s);
            count++;
        }
    }

    void showMovies() {
        cout << "\n---- Available Movies ----\n";
        for (int i = 0; i < count; i++) {
            cout << i + 1 << ". ";
            movies[i].displayMovie();
        }
    }

    virtual void bookTicket(int index, int num) {
        if (index < 0 || index >= count) {
            cout << "Invalid movie choice!\n";
            return;
        }
        movies[index].bookSeats(num);
    }

    int getCount() {
        return count;
    }

    Movie getMovie(int i) {
        return movies[i];
    }

    void updateMovie(int i, Movie m) {
        movies[i] = m;
    }

    virtual ~BookingSystem() {}
};

class ReportSystem {
public:
    void saveBooking(string customer, Movie m, int seats) {
        ofstream fout("bookings.txt", ios::app);
        fout << customer << "," << m.getName() << "," << m.getTime()
             << "," << seats << "," << m.getPrice() * seats << "\n";
        fout.close();
    }

    void showBookings() {
        ifstream fin("bookings.txt");
        string line;
        cout << "\n---- All Bookings ----\n";
        while (getline(fin, line)) {
            cout << line << "\n";
        }
        fin.close();
    }
};

class AdvancedSystem : public BookingSystem, public ReportSystem {
public:
    void bookTicket(int index, int num) {
        if (index < 0 || index >= count) {
            cout << "Invalid selection!\n";
            return;
        }
        if (movies[index].getSeats() < num) {
            cout << "Not enough seats available!\n";
            return;
        }

        string cust;
        cout << "Enter your name: ";
        cin.ignore();
        getline(cin, cust);

        movies[index].bookSeats(num);
        saveBooking(cust, movies[index], num);
        cout << "Booking Successful!\n";
    }
};

class TicketCounter {
    int tickets;
public:
    TicketCounter(int t = 0) { tickets = t; }
    TicketCounter operator+(TicketCounter t2) {
        return TicketCounter(tickets + t2.tickets);
    }
    int getTickets() { return tickets; }
};

class Payment {
public:
    virtual void payBill(int amount) = 0;
};

class OnlinePayment : public Payment {
public:
    void payBill(int amount) {
        cout << "Paid Rs." << amount << " online successfully!\n";
    }
};

template <typename T>
T calculateTotal(T price, int seats) {
    return price * seats;
}

void checkInput(int index, int total) {
    if (index < 0 || index >= total) {
        throw "Invalid movie number!";
    }
}

int main() {
    AdvancedSystem system;

    system.addMovie("Pushpa 2", "10:00 AM", 180, 50);
    system.addMovie("Dabang", "1:00 PM", 200, 40);
    system.addMovie("Kantara", "4:00 PM", 250, 30);
    system.addMovie("RRR", "7:00 PM", 300, 25);

    int choice;

    do {
        cout << "\n===== MOVIE BOOKING SYSTEM =====\n";
        cout << "1. Show Movies\n";
        cout << "2. Book Ticket\n";
        cout << "3. Show All Bookings\n";
        cout << "4. Exit\n";
        cout << "Enter choice: ";
        cin >> choice;

        try {
            switch (choice) {
            case 1:
                system.showMovies();
                cout << "Total Movies (Static): " << Movie::getTotalMovies() << endl;
                break;

            case 2: {
                system.showMovies();
                int index, seats;
                cout << "Enter movie number: ";
                cin >> index;
                checkInput(index - 1, system.getCount());
                cout << "Enter number of seats: ";
                cin >> seats;
                system.bookTicket(index - 1, seats);

                int total = calculateTotal(system.getMovie(index - 1).getPrice(), seats);
                OnlinePayment pay;
                pay.payBill(total);
                break;
            }

            case 3:
                system.showBookings();
                break;

            case 4:
                cout << "Thank you for using Movie Booking System!\n";
                break;

            default:
                cout << "Invalid choice.\n";
            }
        } catch (const char *msg) {
            cout << "Exception caught: " << msg << endl;
        }

    } while (choice != 4);

    return 0;
}

