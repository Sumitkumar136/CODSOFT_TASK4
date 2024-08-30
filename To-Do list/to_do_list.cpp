#include <iostream>
#include <string>
#include <fstream>

using namespace std;

const int MAX_TASKS = 100;
const string USER_FILE = "user_details.txt";

class Task {
public:
    string description;
    int completed; 
    Task() {
        description = "";
        completed = 0;
    }
};

class TaskManager {
private:
    Task tasks[MAX_TASKS];
    int taskCount;
    string userID;

    void save_tasks() {
        ofstream outFile(userID + ".txt");
        for (int i = 0; i < taskCount; ++i) {
            outFile << tasks[i].description << "|" << tasks[i].completed << endl;
        }
        outFile.close();
    }

    void load_tasks() {
        ifstream inFile(userID + ".txt");
        string line;
        taskCount = 0;
        while (getline(inFile, line)) {
            size_t delimiterPos = line.find("|");
            tasks[taskCount].description = line.substr(0, delimiterPos);
            tasks[taskCount].completed = stoi(line.substr(delimiterPos + 1));
            taskCount++;
        }
        inFile.close();
    }

public:
    TaskManager() {
        taskCount = 0;
        userID = "";
    }

    int create_account() {
        string password;
        cout << "Create your account"<<endl;
        cout << "Enter User ID: ";
        getline(cin, userID);
        cout << "Enter Password: ";
        getline(cin, password);

        ofstream userFile(USER_FILE, ios::app);
        if (userFile.is_open()) {
            userFile << userID << " " << password << endl;
            userFile.close();
            cout << "Account created successfully!"<<endl;
            return 1; 
        } else {
            cout << "Error: Unable to open file."<<endl;
            return 0; 
        }
    }

    int login() {
        string password, stored_userID, stored_password;
        cout << "Log in to your account"<<endl;
        cout << "Enter User ID: ";
        getline(cin, userID);
        cout << "Enter Password: ";
        getline(cin, password);

        ifstream userFile(USER_FILE);
        if (userFile.is_open()) {
            while (userFile >> stored_userID >> stored_password) {
                if (userID == stored_userID && password == stored_password) {
                    cout << "Login successful!"<<endl;
                    userFile.close();
                    load_tasks();
                    return 1; 
                }
            }
            userFile.close();
            cout << "Invalid User ID or Password."<<endl;
            return 0; 
        } else {
            cout << "Error: File do not open."<<endl;
            return 0; 
        }
    }

    void add_task(const string& description) {
        if (taskCount < MAX_TASKS) {
            tasks[taskCount].description = description;
            tasks[taskCount].completed = 0;
            taskCount++;
            cout << "Task added: " << description << endl;
            save_tasks();
        } else {
            cout << "Task list is full. Cannot add more tasks."<<endl;
        }
    }

    void view_tasks() const {
        cout << "\nTo-Do List:"<<endl;
        if (taskCount == 0) {
            cout << "No tasks available."<<endl;
        } else {
            for (int i = 0; i < taskCount; ++i) {
                cout << i + 1 << ". " << tasks[i].description
                     << " [" << (tasks[i].completed ? "Completed" : "Pending") << "]"<<endl;
            }
        }
        cout << endl;
    }

    void mark_task_completed(int taskIndex) {
        if (taskIndex < 1 || taskIndex > taskCount) {
            cout << "Invalid task number."<<endl;
        } else {
            tasks[taskIndex - 1].completed = 1;
            cout << "Task " << taskIndex << " marked as completed."<<endl;
            save_tasks();
        }
    }

    void remove_task(int taskIndex) {
        if (taskIndex < 1 || taskIndex > taskCount) {
            cout << "Invalid task number."<<endl;
        } else {
            for (int i = taskIndex - 1; i < taskCount - 1; ++i) {
                tasks[i] = tasks[i + 1];
            }
            taskCount--;
            cout << "Task " << taskIndex << " removed."<<endl;
            save_tasks();
        }
    }
};

int main() {
    TaskManager manager;
    int choice;
    string description;
    int taskIndex;

    cout << "\t\t\tWelcome to the To-Do List Manager"<<endl;
    cout << "\t\t**************************************************" << endl;
    cout << "Press 1. Create Account" << endl;
    cout << "Press 2. Log In" << endl;
    cout << "Choose an option: ";
    cin >> choice;
    cin.ignore();  

    int loggedIn = 0;

    if (choice == 1) {
        loggedIn = manager.create_account();
    } else if (choice == 2) {
        loggedIn = manager.login();
    } else {
        cout << "Invalid option."<<endl;
        return 1;
    }

    if (loggedIn == 1) {
        do {
            cout << "\nTo-Do List Manager"<<endl;
            cout << "Press 1. Add Task"<<endl;
            cout << "Press 2. View Tasks"<<endl;
            cout << "Press 3. Mark Task as Completed"<<endl;
            cout << "Press 4. Remove Task"<<endl;
            cout << "Press 5. Exit"<<endl;
            cout << "Choose an option: ";
            cin >> choice;
            cin.ignore();

            switch (choice) {
                case 1:
                    cout << "Enter task description: ";
                    getline(cin, description);
                    manager.add_task(description);
                    break;
                case 2:
                    manager.view_tasks();
                    break;
                case 3:
                    cout << "Enter task number to mark as completed: ";
                    cin >> taskIndex;
                    manager.mark_task_completed(taskIndex);
                    break;
                case 4:
                    cout << "Enter task number to remove: ";
                    cin >> taskIndex;
                    manager.remove_task(taskIndex);
                    break;
                case 5:
                    cout << "Exiting..."<<endl;
                    break;
                default:
                    cout << "Invalid option. Please try again."<<endl;
                    break;
            }
        } while (choice != 5);
    } else {
        cout << "Failed to log in or create an account. Exiting..."<<endl;
    }

    return 0;
}
