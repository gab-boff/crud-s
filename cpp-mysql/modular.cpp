#include <mysql.h>
#include <iostream>
#include <string>
#include <limits>

const char *DB_HOST = "localhost";
const char *DB_USER = "root";
const char *DB_PASS = "password";
const char *DB_NAME = "hospital";

void showMenu()
{
    std::cout << "Operações CRUD\n";
    std::cout << "1. Criar\n";
    std::cout << "2. Ler\n";
    std::cout << "3. Atualizar\n";
    std::cout << "4. Excluir\n";
    std::cout << "5. Sair\n";
    std::cout << "Escolha uma opção: ";
}

void createData(MYSQL *connection)
{
    std::string nome, email, cpf, telefone, hospital, especializacao;

    std::cout << "Digite o nome: ";
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    std::getline(std::cin, nome);
    std::cout << "Digite o email: ";
    std::getline(std::cin, email);
    std::cout << "Digite o CPF: ";
    std::getline(std::cin, cpf);
    std::cout << "Digite o número de telefone: ";
    std::getline(std::cin, telefone);
    std::cout << "Digite o nome do hospital: ";
    std::getline(std::cin, hospital);
    std::cout << "Digite a especialização: ";
    std::getline(std::cin, especializacao);

    std::string insertQuery = "INSERT INTO medicos (nome, email, cpf, telefone, hospital, especializacao) VALUES ('" + nome + "', '" + email + "', '" + cpf + "', '" + telefone + "', '" + hospital + "', '" + especializacao + "')";
    if (mysql_query(connection, insertQuery.c_str()) != 0)
    {
        std::cout << "Erro ao inserir dados: " << mysql_error(connection) << std::endl;
        return;
    }

    std::cout << "Dados inseridos com sucesso!\n";
}

void readData(MYSQL *connection)
{
    const char *selectQuery = "SELECT * FROM medicos";
    if (mysql_query(connection, selectQuery) != 0)
    {
        std::cout << "Erro ao selecionar dados: " << mysql_error(connection) << std::endl;
        return;
    }

    MYSQL_RES *result = mysql_store_result(connection);
    if (result)
    {
        MYSQL_ROW row;
        while ((row = mysql_fetch_row(result)) != NULL)
        {
            std::cout << "ID: " << row[0] << ", Nome: " << row[1] << ", Email: " << row[2] << ", CPF: " << row[3] << ", Telefone: " << row[4] << ", Hospital: " << row[5] << ", Especialização: " << row[6] << std::endl;
        }
        mysql_free_result(result);
    }
}

void updateData(MYSQL *connection)
{
    int id;
    std::string nome, email, cpf, telefone, hospital, especializacao;

    std::cout << "Digite o ID para atualizar: ";
    std::cin >> id;
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    std::cout << "Digite o novo nome: ";
    std::getline(std::cin, nome);
    std::cout << "Digite o novo email: ";
    std::getline(std::cin, email);
    std::cout << "Digite o novo CPF: ";
    std::getline(std::cin, cpf);
    std::cout << "Digite o novo número de telefone: ";
    std::getline(std::cin, telefone);
    std::cout << "Digite o novo nome do hospital: ";
    std::getline(std::cin, hospital);
    std::cout << "Digite a nova especialização: ";
    std::getline(std::cin, especializacao);

    std::string updateQuery = "UPDATE medicos SET nome = '" + nome + "', email = '" + email + "', cpf = '" + cpf + "', telefone = '" + telefone + "', hospital = '" + hospital + "', especializacao = '" + especializacao + "' WHERE id = " + std::to_string(id);
    if (mysql_query(connection, updateQuery.c_str()) != 0)
    {
        std::cout << "Erro ao atualizar dados: " << mysql_error(connection) << std::endl;
        return;
    }

    std::cout << "Dados atualizados com sucesso!\n";
}

void deleteData(MYSQL *connection)
{
    int id;

    std::cout << "Digite o ID para excluir: ";
    std::cin >> id;

    std::string deleteQuery = "DELETE FROM medicos WHERE id = " + std::to_string(id);
    if (mysql_query(connection, deleteQuery.c_str()) != 0)
    {
        std::cout << "Erro ao excluir dados: " << mysql_error(connection) << std::endl;
        return;
    }

    std::cout << "Dados excluídos com sucesso!\n";
}

int main()
{
    MYSQL *connection = mysql_init(NULL);

    if (!mysql_real_connect(connection, DB_HOST, DB_USER, DB_PASS, DB_NAME, 0, NULL, 0))
    {
        std::cout << "Erro ao conectar ao banco de dados: " << mysql_error(connection) << std::endl;
        return 1;
    }

    int option;
    while (true)
    {
        showMenu();
        std::cin >> option;

        switch (option)
        {
        case 1:
            createData(connection);
            break;
        case 2:
            readData(connection);
            break;
        case 3:
            updateData(connection);
            break;
        case 4:
            deleteData(connection);
            break;
        case 5:
            mysql_close(connection);
            return 0;
        default:
            std::cout << "Opção inválida!\n";
            break;
        }
    }

    return 0;
}
