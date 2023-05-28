#include <iostream>
#include <cstdlib>
#include <string>
#include <sstream>

#include <mysql_connection.h>
#include <mysql_driver.h>
#include <cppconn/driver.h>
#include <cppconn/exception.h>
#include <cppconn/resultset.h>
#include <cppconn/statement.h>

using namespace std;

void create_user(sql::Connection *con, string name, int age)
{
    try
    {
        // prepare a SQL statement to insert the new user into the database
        sql::Statement *stmt = con->createStatement();
        stringstream ss;
        ss << "INSERT INTO users (name, age) VALUES ('" << name << "', " << age << ")";
        stmt->execute(ss.str());
        cout << "User created successfully" << endl;
        delete stmt;
    }
    catch (sql::SQLException &e)
    {
        cout << "SQL Exception: " << e.what() << endl;
    }
}

void delete_user(sql::Connection *con, string name)
{
    try
    {
        sql::Statement *stmt = con->createStatement();
        stringstream ss;
        ss << "DELETE FROM users WHERE name = '" << name << "'";
        stmt->execute(ss.str());
        cout << "User deleted successfully" << endl;
        delete stmt;
    }
    catch (sql::SQLException &e)
    {
        cout << "SQL Exception: " << e.what() << endl;
    }
}

void update_user(sql::Connection *con, string name, string new_name, int new_age)
{
    try
    {
        // prepare a SQL statement to update the user in the database
        sql::Statement *stmt = con->createStatement();
        stringstream ss;

        if (name.empty())
        {
            cout << "Please enter a name to perform the update" << endl;
            return;
        }

        if (new_name.empty() && (new_age != 0))
        {
            ss << "UPDATE users SET age = " << new_age << " WHERE name = '" << name << "'";
        }
        else if (!new_name.empty() && (new_age == 0))
        {
            ss << "UPDATE users SET name = '" << new_name << "' WHERE name = '" << name << "'";
        }
        else if (new_name.empty() && (new_age == 0))
        {
            cout << "Nothing to update" << endl;
            return;
        }
        else if (!new_name.empty() && (new_age != 0))
        {
            ss << "UPDATE users SET name = '" << new_name << "', age = " << new_age << " WHERE name = '" << name << "'";
        }
        stmt->execute(ss.str());
        cout << "User updated successfully" << endl;
        delete stmt;
    }
    catch (sql::SQLException &e)
    {
        cout << "SQL Exception: " << e.what() << endl;
    }
}

void read_users(sql::Connection *con)
{
    try
    {
        // prepare a SQL statement to select all users from the database
        sql::Statement *stmt = con->createStatement();
        sql::ResultSet *res = stmt->executeQuery("SELECT * FROM users");

        // iterate through the result set and display the user information on the console
        while (res->next())
        {
            cout << "-------------------------" << endl;
            cout << "Name: " << res->getString("name") << endl;
            cout << "Age: " << res->getInt("age") << endl;
            cout << "-------------------------" << endl;
        }
        delete res;
        delete stmt;
    }
    catch (sql::SQLException &e)
    {
        cout << "SQL Exception: " << e.what() << endl;
    }
}

int main()
{
    try
    {
        sql::Driver *driver;
        sql::Connection *con;

        // create a MySQL driver instance
        driver = get_driver_instance();

        // create a MySQL connection instance
        con = driver->connect("tcp://localhost:3306", "root", "password");

        // specify the MySQL schema to use
        con->setSchema("mydbtest");

        // Menu interface
        int choice;
        string name;
        string new_name;
        int new_age;

        while (true)
        {
            cout << "-------------------------" << endl;
            cout << "1. Create User" << endl;
            cout << "2. Delete User" << endl;
            cout << "3. Update User" << endl;
            cout << "4. Read Users" << endl;
            cout << "0. Exit" << endl;
            cout << "Enter your choice: ";
            cin >> choice;

            switch (choice)
            {
            case 1:
                cout << "Enter name: ";
                cin >> name;
                cout << "Enter age: ";
                cin >> new_age;
                create_user(con, name, new_age);
                break;
            case 2:
                cout << "Enter name: ";
                cin >> name;
                delete_user(con, name);
                break;
            case 3:
                cout << "Enter name: ";
                cin >> name;
                cout << "Enter new name: ";
                cin >> new_name;
                cout << "Enter new age: ";
                cin >> new_age;
                update_user(con, name, new_name, new_age);
                break;
            case 4:
                read_users(con);
                break;
            case 0:
                delete con;
                exit(0);
            default:
                cout << "Invalid choice. Please try again." << endl;
            }
        }
    }
    catch (sql::SQLException &e)
    {
        cout << "SQL Exception: " << e.what() << endl;
    }

    return 0;
}
