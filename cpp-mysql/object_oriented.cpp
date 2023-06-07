#include <mysql.h>
#include <iostream>
#include <string>
#include <limits>

const char *DB_HOST = "localhost";
const char *DB_USER = "root";
const char *DB_PASS = "password";
const char *DB_NAME = "hospital";

class Medico
{
private:
    int id;
    std::string nome;
    std::string email;
    std::string cpf;
    std::string telefone;
    std::string hospital;
    std::string especializacao;

public:
    Medico() {}

    Medico(const std::string &nome, const std::string &email, const std::string &cpf, const std::string &telefone, const std::string &hospital, const std::string &especializacao)
        : nome(nome), email(email), cpf(cpf), telefone(telefone), hospital(hospital), especializacao(especializacao) {}

    void setId(int id)
    {
        this->id = id;
    }

    int getId() const
    {
        return id;
    }

    void setNome(const std::string &nome)
    {
        this->nome = nome;
    }

    std::string getNome() const
    {
        return nome;
    }

    void setEmail(const std::string &email)
    {
        this->email = email;
    }

    std::string getEmail() const
    {
        return email;
    }

    void setCpf(const std::string &cpf)
    {
        this->cpf = cpf;
    }

    std::string getCpf() const
    {
        return cpf;
    }

    void setTelefone(const std::string &telefone)
    {
        this->telefone = telefone;
    }

    std::string getTelefone() const
    {
        return telefone;
    }

    void setHospital(const std::string &hospital)
    {
        this->hospital = hospital;
    }

    std::string getHospital() const
    {
        return hospital;
    }

    void setEspecializacao(const std::string &especializacao)
    {
        this->especializacao = especializacao;
    }

    std::string getEspecializacao() const
    {
        return especializacao;
    }
};

class HospitalManager
{
private:
    MYSQL *connection;

public:
    HospitalManager()
        : connection(nullptr) {}

    ~HospitalManager()
    {
        if (connection)
        {
            mysql_close(connection);
        }
    }

    bool connect()
    {
        connection = mysql_init(NULL);
        if (!mysql_real_connect(connection, DB_HOST, DB_USER, DB_PASS, DB_NAME, 0, NULL, 0))
        {
            std::cout << "Erro ao conectar ao banco de dados: " << mysql_error(connection) << std::endl;
            return false;
        }
        return true;
    }

    bool createMedico(const Medico &medico)
    {
        std::string insertQuery = "INSERT INTO medicos (nome, email, cpf, telefone, hospital, especializacao) VALUES ('" + medico.getNome() + "', '" + medico.getEmail() + "', '" + medico.getCpf() + "', '" + medico.getTelefone() + "', '" + medico.getHospital() + "', '" + medico.getEspecializacao() + "')";
        if (mysql_query(connection, insertQuery.c_str()) != 0)
        {
            std::cout << "Erro ao inserir dados: " << mysql_error(connection) << std::endl;
            return false;
        }
        return true;
    }

    void readMedicos()
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

    bool updateMedico(const Medico &medico)
    {
        std::string updateQuery = "UPDATE medicos SET nome = '" + medico.getNome() + "', email = '" + medico.getEmail() + "', cpf = '" + medico.getCpf() + "', telefone = '" + medico.getTelefone() + "', hospital = '" + medico.getHospital() + "', especializacao = '" + medico.getEspecializacao() + "' WHERE id = " + std::to_string(medico.getId());
        if (mysql_query(connection, updateQuery.c_str()) != 0)
        {
            std::cout << "Erro ao atualizar dados: " << mysql_error(connection) << std::endl;
            return false;
        }
        return true;
    }

    bool deleteMedico(int id)
    {
        std::string deleteQuery = "DELETE FROM medicos WHERE id = " + std::to_string(id);
        if (mysql_query(connection, deleteQuery.c_str()) != 0)
        {
            std::cout << "Erro ao excluir dados: " << mysql_error(connection) << std::endl;
            return false;
        }
        return true;
    }
};

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

int main()
{
    HospitalManager manager;

    if (!manager.connect())
    {
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
        {
            std::string nome, email, cpf, telefone, hospital, especializacao;

            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            std::cout << "Digite o nome: ";
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

            Medico medico(nome, email, cpf, telefone, hospital, especializacao);
            if (manager.createMedico(medico))
            {
                std::cout << "Dados inseridos com sucesso!\n";
            }
            break;
        }
        case 2:
            manager.readMedicos();
            break;
        case 3:
        {
            int id;

            std::cout << "Digite o ID para atualizar: ";
            std::cin >> id;

            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            std::string nome, email, cpf, telefone, hospital, especializacao;

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

            Medico updatedMedico(nome, email, cpf, telefone, hospital, especializacao);
            updatedMedico.setId(id);
            if (manager.updateMedico(updatedMedico))
            {
                std::cout << "Dados atualizados com sucesso!\n";
            }
            break;
        }
        case 4:
        {
            int id;

            std::cout << "Digite o ID para excluir: ";
            std::cin >> id;

            if (manager.deleteMedico(id))
            {
                std::cout << "Dados excluídos com sucesso!\n";
            }
            break;
        }
        case 5:
            return 0;
        default:
            std::cout << "Opção inválida!\n";
            break;
        }
    }

    return 0;
}
