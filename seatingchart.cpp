#include <iostream>
#include <string>
#include <string.h>
#include <vector>
#include <map>
#include <fstream>
#include <cctype>
#include <sstream>
#include <set>
#include <windows.h>
#include <iomanip>
#include <filesystem>
#include "seatingchart.h"
#include "global.h"
using namespace std;
using namespace std::filesystem;

int number_of_bookedSeats = 0;

void importSeatingChartData()
{
    path directory = flightCode;
    if (!exists(directory))
    {
        cout << "   Chuyen bay khong ton tai!" << endl;
        return;
    }
    path bookedSeatingPath = directory / BOOKED_CHART;
    path originalSeatingPath = directory / ORIGINAL_CHART;

    ifstream inputFile(bookedSeatingPath);
    if (!inputFile)
    {
        inputFile.open(originalSeatingPath);
        if (!inputFile)
        {
            cout << "Da xay ra loi khi mo file" << endl;
            return;
        }
    }
    string line;
    bool isFirstClass = false, isEconomyClass = false;
    while (getline(inputFile, line))
    {
        if (line == "First Class:")
        {
            isFirstClass = true;
            isEconomyClass = false;
            continue;
        }
        if (line == "Economy Class:")
        {
            isFirstClass = false;
            isEconomyClass = true;
            continue;
        }
        if (!line.empty())
        {
            stringstream ss(line);
            vector<string> row;
            string token;
            while (ss >> token)
            {
                if (token == "--" || token == "---")
                {
                    ++number_of_bookedSeats;
                }
                row.push_back(token);
            }
            if (isFirstClass)
            {
                firstClass.push_back(row);
            }
            else if (isEconomyClass)
            {
                economyClass.push_back(row);
            }
        }
    }
    inputFile.close();
}

void displaySeatingChart()
{
    system("cls");

    cout << endl;
    cout << "╔════════════════════╦═══════════════════════════════════════════════════════════════════════════════════╗" << endl;
    cout << "║    First Class     ║                                     Economy Class                                 ║" << endl;
    cout << "╠════════════════════╬═══════════════════════════════════════════════════════════════════════════════════╣" << endl;

    cout << "║                    ║ ";
    for (const auto &seat : economyClass[0])
    {
        cout << seat << " ";
    }
    cout << "   ║" << endl;

    cout << "║ " << firstClass[0][0] << " " << firstClass[1][0] << " " << firstClass[2][0] << " " << firstClass[3][0] << "        ║ ";
    for (const auto &seat : economyClass[1])
    {
        cout << seat << " ";
    }
    cout << "   ║" << endl;

    cout << "║ " << firstClass[0][1] << " " << firstClass[1][1] << " " << firstClass[2][1] << " " << firstClass[3][1] << "        ║ ";
    for (const auto &seat : economyClass[2])
    {
        cout << seat << " ";
    }
    cout << "   ║" << endl;

    cout << "╠════════════════════╬═══════════════════════════════════════════════════════════════════════════════════╣" << endl;
    cout << "║ ------Loi di------ ║ ------------------------------------Loi di--------------------------------------- ║" << endl;
    cout << "╠════════════════════╬═══════════════════════════════════════════════════════════════════════════════════╣" << endl;

    cout << "║ " << firstClass[0][2] << " " << firstClass[1][2] << " " << firstClass[2][2] << " " << firstClass[3][2] << "        ║ ";
    for (const auto &seat : economyClass[3])
    {
        cout << seat << " ";
    }
    cout << "   ║" << endl;

    cout << "║ " << firstClass[0][3] << " " << firstClass[1][3] << " " << firstClass[2][3] << " " << firstClass[3][3] << "        ║ ";
    for (const auto &seat : economyClass[4])
    {
        cout << seat << " ";
    }
    cout << "   ║" << endl;

    cout << "║                    ║ ";
    for (const auto &seat : economyClass[5])
    {
        cout << seat << " ";
    }
    cout << "   ║" << endl;

    cout << "╚════════════════════╩═══════════════════════════════════════════════════════════════════════════════════╝" << endl;

    cout << endl
         << "                  [--] hay [---]: Ghe dat dat                 1A: Ghe chua dat " << endl;
}

bool isValidSeat(const string &seat)
{
    bool check = false;

    if (!isalpha(seat[seat.length() - 1]))
    {
        return check;
    }

    string number = "";
    for (char c : seat)
    {
        if (isdigit(c))
        {
            number += c;
        }
    }

    int num = stoi(number);
    if (!(num < 1 || num > 25))
        check = true;
    return check;
}

string selectSeat(int index)
{
    string choice;
    while (true)
    {
        cout << " → Hay chon ghe: ";
        getline(cin, choice);

        if (!isValidSeat(choice) && seatStatus.find(choice) != seatStatus.end())
        {
            cout << endl;
            cout << "   ════ Lua chon khong phu hop hoac ghe da dat ═════";
            cout << endl;
            continue;
        }

        bool foundFirstClass = false, foundEconomyClass = false;
        if (passengers[index].classLabel == "First Class")
        {
            for (int i = 0; i < firstClass.size(); i++)
            {
                for (int j = 0; j < firstClass[i].size(); j++)
                {
                    if (firstClass[i][j] == choice)
                    {
                        if (choice.length() == 2)
                            firstClass[i][j] = "--";
                        else if (choice.length() == 3)
                            firstClass[i][j] = "---";
                        foundFirstClass = true;
                        break;
                    }
                }
                if (foundFirstClass)
                    break;
            }
            if (!foundFirstClass)
            {
                cout << "   Ghe chon khong phu hop voi First Class!" << endl;
                continue;
            }
        }

        if (passengers[index].classLabel == "Economy Class")
        {
            for (int i = 0; i < economyClass.size(); i++)
            {
                for (int j = 0; j < economyClass[i].size(); j++)
                {
                    if (economyClass[i][j] == choice)
                    {
                        if (choice.length() == 2)
                            economyClass[i][j] = "--";
                        else if (choice.length() == 3)
                            economyClass[i][j] = "---";
                        foundEconomyClass = true;
                        break;
                    }
                }
                if (foundEconomyClass)
                    break;
            }
            if (!foundEconomyClass)
            {
                cout << "   Ghe chon khong phu hop voi Economy Class!" << endl;
                continue;
            }
        }

        if (foundFirstClass == true || foundEconomyClass == true)
        {
            cout << "   Chac chan muon chon " << choice << " (Y/N): ";
            string okay;
            getline(cin, okay);
            if (okay[0] == 'Y' || okay[0] == 'y')
            {
                seatStatus.insert({choice, "X"});
                return choice;
            }
            else
            {
                seatStatus.erase(choice);
                continue;
            }
        }
        else
        {
            cout << endl;
            cout << "   ════ Lua chon khong phu hop hoac ghe da dat ═════";
            cout << endl;
            continue;
        }
    }
}

void updateSeatingChartFile()
{
    path directorypath = flightCode;
    path bookedSeatingPath = directorypath / BOOKED_CHART;
    ofstream WriteFile(bookedSeatingPath);
    if (!WriteFile)
    {
        cout << "   Loi khi mo file BookedSeatingChart" << endl;
        return;
    }
    WriteFile << "First Class:" << endl;
    for (int i = 0; i < firstClass.size(); i++)
    {
        for (int j = 0; j < firstClass[i].size(); j++)
        {
            WriteFile << firstClass[i][j] << " ";
        }
        WriteFile << endl;
    }
    WriteFile << endl;
    WriteFile << "Economy Class:" << endl;
    for (int i = 0; i < economyClass.size(); i++)
    {
        for (int j = 0; j < economyClass[i].size(); j++)
        {
            WriteFile << economyClass[i][j] << " ";
        }
        WriteFile << endl;
    }
    WriteFile.close();
}

void resetSeatingChart()
{
    path directorypath = flightCode;
    if (!exist(directorypath)) {
        cout << "   Chuyen bay khong ton tai" << endl; 
        return; 
    }
    path originalSeatingPath = directorypath / ORIGINAL_CHART;
    path bookedSeatingPath = directorypath / BOOKED_CHART;
    ifstream ReadFile(originalSeatingPath);
    if (!ReadFile)
    {
        cout << "  Khong the mo file SeatingChart.txt" << endl;
        return;
    }
    ofstream WriteFile(bookedSeatingPath, ios::trunc);
    if (!WriteFile)
    {
        cout << "  Khong the mo file BookedSeatingChart.txt" << endl;
        return;
    }
    string line;
    while (getline(ReadFile, line))
    {
        WriteFile << line << endl;
    }
    ReadFile.close();
    WriteFile.close();
    cout << "   SO DO GHE DA DUOC RESET" << endl;
}

bool isNumber(const string& str) {
    if (str.empty()) return false;
    for (char c : str) {
        if (!isdigit(c)) return false;
    }
    return true;
}

void importSeatCode()
{
    displaySeatingChart();

    cout << endl
         << " → So ve may bay can dat: ";

    string num;
    int number_of_tickets = 0; 
    while (true) 
    {
        cin >> num;
        if (!isNumber(num)) 
        {
            cout << "    Nhap sai, vui long nhap lai: " << endl;
            continue;
        } 

        int number_of_tickets = stoi(num);
        
        if (number_of_tickets > 1 && number_of_tickets < 166 && number_of_tickets < MaxSeat - number_of_bookedSeats - ticketCount) // tong so ve - so ve dat trong he thong - so ve da dat dem duoc hien tai
        {
            break;
        }
        else {
            continue; 
        }
    }
    cin.ignore();

    while (number_of_tickets > 0)
    {
        if (ticketCount > MaxSeat)
        {
            cout << "   Het cho!" << endl;
            return;
        }
        cout << endl
             << "   STT ve may bay: " << ticketCount << endl;
        string ticketCode;
        int index;
        while (true)
        {
            cout << " → Nhap ma so ve may bay: ";
            getline(cin, ticketCode);
            if (bookedStore.find(code) != bookedStore.end())
            {
                cout << "   Ma so nay da dat ghe" << endl;
                cout << "   Nhap lai!" << endl;
                continue;
            }
            index = findPassengers(ticketCode);
            if (index == -1)
            {
                cout << "   Ma so ve may bay khong ton tai" << endl;
                cout << "   Nhap lai!" << endl;
            }
            else
                bookedStore.insert(ticketCode); 
                break;
        }

        store.insert(ticketCode);

        string seat = selectSeat(index);
        updateSeatingChartFile();

        passengers[index].seatCode = seat;

        number_of_tickets--;
        ticketCount++;
    }
}

int findPassengers(const string &code)
{
    for (int i = 0; i < passengers.size(); i++)
    {
        if (passengers[i].ticketCode == code)
        {
            return i;
        }
    }
    return -1;
}

void statistics()
{
    system("cls");

    cout << "╔════════════════════════════════════════════╗" << endl;
    cout << "║                  THONG KE GHE              ║" << endl;
    cout << "╠════════════════════════════════════════════╣" << endl;
    cout << "║ Tong so ghe      : " << setw(4) << MaxSeat << "                    ║" << endl;
    cout << "║                                            ║" << endl;
    cout << "║ So ghe da dat    : " << setw(4) << number_of_bookedSeats << "                    ║" << endl;
    cout << "║                                            ║" << endl;
    cout << "║ So ghe con trong : " << setw(4) << MaxSeat - number_of_bookedSeats << "                    ║" << endl;
    cout << "╚════════════════════════════════════════════╝" << endl;
}