#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <cctype>
#include <sstream>
#include <set>
#include <windows.h>
#include <map>
#include <iomanip>
using namespace std;

int originalPrice = 0; 

typedef struct person {
	string names;
	string DoB;
	string email;
	string phoneNumber;
	string gender;
} person;

typedef struct ticket {
	person customer; // thong tin khach hang co trong ve
	string flightCode; 
	string departure;
	string destination;
	string time;
	string date;
	int price;
	string seatCode; 
} ticket; 

typedef struct node {
	ticket data;
	struct node *next;
} node;

node *makeNode(ticket data) {
	node *newNode = new node();
	newNode->data = data;
	newNode->next = NULL;
	return newNode;
}

void SetColor(int text_color) {
    HANDLE hStdout = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(hStdout, text_color);
}

int numberOfTickets(node* head) {
	int cnt = 0;
	while (head != NULL) {
		cnt++; 
		head = head->next; 
	} 
	return cnt;
}

person importPersonalInformation() {
	person person;
	cout << endl;
	cout << "\t\t\t\t\t  === NHAP THONG TIN KHACH HANG ===" << endl;
	cout << "\t\t\t\t\t************************************" << endl; 
	cout << "\t\t\t\t\t  Nhap ho va ten: ";
	getline(cin, person.names);
	cout << "\t\t\t\t\t  Nhap ngay sinh: ";
	getline(cin, person.DoB);
	cout << "\t\t\t\t\t  Nhap email: ";
	getline(cin, person.email);
	cout << "\t\t\t\t\t  Nhap so dien thoai: ";
	getline(cin, person.phoneNumber);
	cout << "\t\t\t\t\t  Gioi tinh (Nam/Nu/Khac): ";
	getline(cin, person.gender); 
	cout << "\t\t\t\t\t************************************" << endl;
	return person;
}

void savePersonalInformation(person temp) {
	ofstream WriteFile("PersonalInformation.txt", ios::app);
	if (!WriteFile) {
		cout << "\t\t\t\t\t Da xay ra loi"; 
	}
	WriteFile << temp.names << "," << temp.DoB << "," << temp.email << "," << temp.phoneNumber << "," << temp.gender << endl;
	WriteFile.close(); 
	cout << "\t\t\t\tThong tin da duoc luu vao" << " \"PersonalInformation.txt\"" << endl; 
}

void resetPersonalInformation() {
	ofstream reset("PersonalInformation.txt", ios::out | ios::trunc); 
	if (!reset) {
		cout << "\t\t\t\t\t Co loi xay ra khi reset"; 
	}
	reset.close();
}

void displayFlightInformationList(vector<ticket>& flightList) {
	ifstream ReadFile("FlightInformation.txt");  
	if (!ReadFile) {
		cout << "Da xay ra loi"; 
		return;
	}
	while (true) {
		ticket temp; 
		if (!getline(ReadFile, temp.flightCode)) break; // neu khong con chuyen bay nao thi dung lai 
		getline(ReadFile, temp.departure); 
		getline(ReadFile, temp.destination); 
		getline(ReadFile, temp.date); 
		getline(ReadFile, temp.time); 
		ReadFile >> temp.price; originalPrice = temp.price;
		ReadFile.ignore(2); 
		flightList.push_back(temp); 
	}
	cout << endl; 
	cout << "\t\t\t\t\t   === DANH SACH CAC CHUYEN BAY ===" << endl;
	cout << "\t\t\t\t\t        Co tat ca " << flightList.size() << " chuyen bay" << endl; 
	cout << "\t\t\t\t\t************************************" << endl; 
	for (int i = 0; i<flightList.size(); i++) {
		cout << "\t\t\t\t\tChuyen bay thu " << i + 1 << ": " 
			 << endl
			 << "\t\t\t\t\tMa so chuyen bay: " << flightList[i].flightCode
			 << endl
			 << "\t\t\t\t\tNoi xuat phat: " << flightList[i].departure 
			 << endl
			 << "\t\t\t\t\tNoi den: " << flightList[i].destination
			 << endl
			 << "\t\t\t\t\tNgay xuat phat: " << flightList[i].date
			 << endl
			 << "\t\t\t\t\tThoi gian: " << flightList[i].time
			 << endl
			 << "\t\t\t\t\tGia ve: " << flightList[i].price; 
		cout << endl;
		if (i != flightList.size() - 1) {
			cout << "\t\t\t\t\t------------------------------------" << endl;
		}
	}
	cout << "\t\t\t\t\t************************************" << endl; 
	ReadFile.close();
}

int selectFlight(vector<ticket>& flightList) {
    cout << "\t\t\t\t\tHay chon chuyen bay phu hop voi lich trinh cua ban" << endl; 
    cout << "\t\t\t\t\tBan se chon chuyen bay nao: "; 
    int choice, index = 0; 
    while (true) {
        if (!(cin >> choice)) { // kiem tra ki tu 
            cout << "\t\t\t\t\t------------------------------------" << endl;
            cout << "\t\t\t\t\t      LUA CHON KHONG HOP LE!" << endl; 
            cout << "\t\t\t\t\t      Vui long chon lai!" << endl; 
            cout << "\t\t\t\t\t------------------------------------" << endl;
            continue;
        }
        if (choice < 1 || choice > flightList.size()) {
            cout << endl;
            cout << "\t\t\t\t\t------------------------------------" << endl;
            cout << "\t\t\t\t\t      LUA CHON KHONG HOP LE!" << endl; 
            cout << "\t\t\t\t\t      Vui long chon lai!" << endl; 
            cout << "\t\t\t\t\t------------------------------------" << endl;
            continue; 
        }
        cin.ignore();
        index = choice - 1; 
        break; 
    }
    return index;
}

void importSeatingChartData(vector<vector<string>> &takenSeatingChart) {
    ifstream readFile("BookedSeatingChart.txt");
    if (!readFile) {
        readFile.open("SeatingChart.txt");
        if (!readFile) {
            cout << "Da xay ra loi khi mo file seating chart!";
            return;
        }
    }
    string temp;
    while (getline(readFile, temp)) {
        stringstream ss(temp);
        string token;
        vector<string> row;
        while (ss >> token) {
            row.push_back(token);
        }
        takenSeatingChart.push_back(row);
    }
    readFile.close();
}

void displaySeatingChart(vector<vector<string>> takenSeatingChart) {
	cout << endl << "\t\t\t\t\t   === SO DO GHE NGOI MAY BAY ===" << endl; 
	cout << "\t\t\t\t\t************************************" << endl;
	for (int i = 0; i<takenSeatingChart.size(); i++) {
		cout << "\t\t\t\t\t      ";
		for (int j = 0; j<takenSeatingChart[i].size(); j++) {
			cout << takenSeatingChart[i][j] << " "; 
		}
		cout << endl; 
	}
	cout << "\t\t\t\t\t************************************" << endl;
}

string selectSeat(vector<vector<string>> &takenSeatingChart, map<string, string>& check) {
    string choice;
    while (true) {
		cout << "\t\t\t\t\t Vi tri ghe ngoi khac nhau se co gia ve khac nhau" << endl;
		cout << "\t\t\t\t\t Vi tri tu 01 -> 10: Gia ve goc * 300%" << endl;
		cout << "\t\t\t\t\t Vi tri tu 11 -> 20: Gia ve goc * 150%" << endl; 
		cout << "\t\t\t\t\t Vi tri tu 21 -> 30: Gia ve goc * 100%" << endl;
        cout << "\t\t\t\t\t      Hay chon ghe: "; 
        getline(cin, choice);
        
        if (choice.length() < 2 || choice.length() > 3) {
        	cout << endl;
            cout << "\t\t\t\t\t------------------------------------" << endl;
            cout << "\t\t\t\t\t      LUA CHON KHONG HOP LE HOAC GHE DA BI DAT!" << endl; 
            cout << "\t\t\t\t\t      Hay chon lai!" << endl;
            cout << "\t\t\t\t\t------------------------------------";  
			cout << endl;
            continue; 
        }
        
        if (choice[0] < 'A' || choice[0] > 'Z' || choice[1] < '0' || choice[1] > '9') {
        	cout << endl;
            cout << "\t\t\t\t\t------------------------------------" << endl;
            cout << "\t\t\t\t\t      LUA CHON KHONG HOP LE HOAC GHE DA BI DAT!" << endl; 
            cout << "\t\t\t\t\t      Hay chon lai!" << endl;
            cout << "\t\t\t\t\t------------------------------------"; 
			cout << endl;
            continue; 
        }
        
        if (check.find(choice) != check.end()) {
        	cout << endl;
            cout << "\t\t\t\t\t------------------------------------" << endl;
            cout << "\t\t\t\t\t      LUA CHON KHONG HOP LE HOAC GHE DA BI DAT!" << endl; 
            cout << "\t\t\t\t\t      Hay chon lai!" << endl;
            cout << "\t\t\t\t\t------------------------------------"; 
			cout << endl;
            continue; 
        }
        
        bool found = false; 
        for (int i = 0; i < takenSeatingChart.size(); i++) {
            for (int j = 0; j < takenSeatingChart[i].size(); j++) {
                if (takenSeatingChart[i][j] == choice) {
                    found = true; 
                    takenSeatingChart[i][j] = "X"; 
                    break; 
                }
            }
            if (found) break;
        }
        
        if (found) {
            cout << "\t\t\t\t     Ban co chac chan muon chon ghe nay: (YES/NO) "; 
            string okay; 
            getline(cin, okay); 
            if (okay[0] == 'Y' || okay[0] == 'y') {
            	check.insert({choice, "X"}); 
                return choice; 
            } else {
                continue; 
            }
        }
    
        cout << "\t\t\t\t\t------------------------------------" << endl;
        cout << "\t\t\t\t\t      LUA CHON KHONG HOP LE!" << endl; 
        cout << "\t\t\t\t\t      Hay chon lai!" << endl;
        cout << "\t\t\t\t\t------------------------------------" << endl;
        cout << endl; 
    }
}

void updateSeatingChartFile(vector<vector<string>> takenSeatingChart) {
    ofstream WriteFile("BookedSeatingChart.txt");
    if (!WriteFile) {
        cout << "Loi khi mo file " << "BookedSeatingChart" << " de cap nhat so do ghe!" << endl;
        return;
    }
    for (size_t i = 0; i < takenSeatingChart.size(); i++) {
        for (size_t j = 0; j < takenSeatingChart[i].size(); j++) {
            WriteFile << takenSeatingChart[i][j] << " ";
        }
        WriteFile << endl;
    }
    WriteFile.close();
}

double ticketPrice(string choice) {
	string temp = choice.substr(1,2); 
	int number = stoi(temp); 
	if (number >= 1 && number <= 10) {
		return originalPrice * 3; 
	} else if (number >= 11 && number <= 20) {
		return originalPrice * 1.5; 
	} else {
		return originalPrice; 
	}
}

void addTicket(node **head, vector<vector<string>>& takenSeatingChart) {
    ticket ticket;
    vector<struct ticket> flightList; 
    displayFlightInformationList(flightList); 
    ticket = flightList[selectFlight(flightList)];
    ticket.customer = importPersonalInformation();
    savePersonalInformation(ticket.customer);
   
    importSeatingChartData(takenSeatingChart);
    displaySeatingChart(takenSeatingChart);
    
    map<string, string> check; // kiem tra ghe da dat hay chua
    ticket.seatCode = selectSeat(takenSeatingChart, check);
    ticket.price = ticketPrice(ticket.seatCode); 
    updateSeatingChartFile(takenSeatingChart);
    
    node *newNode = makeNode(ticket);
    if (*head == NULL)
        *head = newNode;
    else {
        node *temp = *head;
        while (temp->next != NULL)
            temp = temp->next;
        temp->next = newNode;
    }
    cout << endl;
    cout << "\t\t\t\t\t------------------------------------" << endl;
    cout << "\t\t\t\t\t      DA THEM VE THANH CONG!" << endl;
    cout << "\t\t\t\t\t------------------------------------" << endl;
    cout << endl;
}

void printTickets(node *head) {
    if (head == NULL) {
        cout << "\t\t\t\t\t  Chua co ve nao duoc dat!" << endl;
        return;
    }
    cout << "\t\t\t\t\t    === DANH SACH VE DA DAT ===" << endl;
    cout << "\t\t\t\t\t************************************";
    
    node *temp = head;
    while (temp != NULL) {
    	cout << endl << "\t\t\t\t\t  Tong cong so ve da dat: " << numberOfTickets(head) << " ve" << endl;
        cout << "\t\t\t\t\t------------------------------------" << endl;
        cout << "\t\t\t\t\t  Ma so chuyen bay: " << temp->data.flightCode << endl;
        cout << "\t\t\t\t\t  Khach hang: " << temp->data.customer.names << endl;
        cout << "\t\t\t\t\t  Chuyen bay: " << temp->data.departure << " -> " << temp->data.destination << endl;
        cout << "\t\t\t\t\t  Ngay: " << temp->data.date << " - Gio: " << temp->data.time << endl;
        cout << "\t\t\t\t\t  Gia ve: " << temp->data.price << " - Ma ghe: " << temp->data.seatCode << endl;
        cout << "\t\t\t\t\t------------------------------------" << endl;
        temp = temp->next;
    }
    cout << "\t\t\t\t\t************************************" << endl; 
}

void resetSeatingChart() {
    ifstream ReadFile("SeatingChart.txt");
    if (!ReadFile) {
        cout << "\t\t\t\t\t  Khong the mo file SeatingChart.txt de reset." << endl;
        return;
    }
    ofstream WriteFile("BookedSeatingChart.txt", ios::trunc);
    if (!WriteFile) {
        cout << "\t\t\t\t\t  Khong the mo file BookedSeatingChart.txt de ghi lai." << endl;
        return;
    }
    string line;
    while (getline(ReadFile, line)) {
        WriteFile << line << endl;
    }
    ReadFile.close();
    WriteFile.close();
    cout << "\t\t\t\t\t      SO DO GHE DA DUOC RESET" << endl;
}

void end() {
	cout << endl; 
	cout << "\t\t\t\t\t************************************" << endl; 
	cout << "\t\t\t\t Cam on quy khach da su dung dich vu dat ve ITF-Airways" << endl; 
	cout << "\t\t\t\t\t************************************" << endl; 
	cout << endl;
}

void cancelTicket(node** head, string flightCode) {
    if (*head == NULL) {
		cout << "\t\t\t\t\t  Chua co ve nao duoc dat!" << endl;
		return;
    }
    node* temp = *head; 
	if (temp->data.flightCode == flightCode) {
		*head = temp->next;
		delete temp;
		cout << "\t\t\t\t\t  Ve da duoc huy thanh cong!" << endl;
		return;
	}
}

void menu(node* head) { 
    vector<vector<string>> takenSeatingChart;
	while (true) {
    	SetColor(10);	
        cout << "\t\t\t\t\t************************************" << endl;
        cout << "\t\t\t\t\tChao mung den he thong dat ve may bay" << endl;
        cout << "\t\t\t\t\t          ---ITF-Airways---" << endl;
        SetColor(14); 
        cout << "\t\t\t\t\t    1. Dat ve may bay" << endl;
        cout << "\t\t\t\t\t    2. Cancel a ticket" << endl;
		cout << "\t\t\t\t\t    3. Print tickets" << endl;
		cout << "\t\t\t\t\t    4. Print booked seating chart" << endl;
        cout << "\t\t\t\t\t    0. Exit" << endl;
        SetColor(10); 
        cout << "\t\t\t\t\t************************************" << endl;
        cout << "\t\t\t\t\t    Please enter your choice: ";

        int choice; 
        cin >> choice; 
        cin.ignore();
        cout << endl; 
        
       	switch(choice) {
       		case 1: 
				addTicket(&head, takenSeatingChart);
       			break; 
            case 2: {
            	cout << "\t\t\t\t\t Ban muon xoa ma so chuyen bay nao?";
        		string flightCode; getline(cin, flightCode);
                if (flightCode.length() != 6) {
                    cout << "\t\t\t\t\t  Ma so chuyen bay khong hop le!" << endl;
                    break;
                }
				cancelTicket(&head, flightCode);
				break; 
			}
            case 3: 
				printTickets(head);
				break;
			case 4:
				displaySeatingChart(takenSeatingChart);
				break;
			case 0:
				return;
			default:
				cout << "\t\t\t\t\t  Lua chon khong hop le!" << endl;
				break;
	   }
	}
}

// End
int main() {
	node *head = NULL;
	menu(head);
	end();
	resetPersonalInformation(); 
	resetSeatingChart(); 
	return 0;
}
