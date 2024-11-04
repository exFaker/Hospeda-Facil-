#include "sqlite/sqlite3.c"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <locale.h>
#include <conio.h>
#include <stdbool.h>

#define LOTACAOMAXIMA 5 // definicao total de quartos do hotel
#define PARALOGIN 1

// criacao da struct
typedef struct sistemadereservadehotel{
    char nomes[100], dataCheckin[100], dataCheckout[100], menu[3], pagamento[100], cpf[20], voltar[10];
    int numeroReserva, numero, dias, codigo;
    float menorDez, entreDezeVinte, maiorVinte, total, desconto;
};

char key[50] = "hW7wD1LGZCVF57tmEub7OAlc4b7lSH4YarOPUzc704gn2CuJAE";


// *** INÍCIO DAS FUNÇÕES COM BANCO DE DADOS ***


void criarLogin() {
    sqlite3* db;
    int rc = sqlite3_open("database.db", &db);
    if (rc) {
        fprintf(stderr, "Nao e possivel abrir o banco de dados: %s\n", sqlite3_errmsg(db));
        return 1;
    } else printf("\n\nEntrando no painel de cadastro de login...\n");

    char login[50];
    char senha[50];
    char ch;

    printf("\nLogin novo: ");
    int j = 0;
    while((ch = getch()) != '\r'){  // Loop que lê cada caractere digitado até que o enter seja pressionado
        if(ch == '\b' && j > 0){  // Se o caractere for um backspace e houver caracteres para apagar
            login[--j] = '\0';  // Apaga o último caractere
            printf("\b \b");  // Move o cursor para trás, imprime um espaço para apagar o *, e move o cursor para trás novamente
        } else if(ch != ' ' && ch != '\b'){
            login[j++] = ch;  // Adiciona o caractere ao login
            printf("*");  // Imprime um *
        }
    }
    login[j] = '\0';  // Adiciona o caractere nulo ao final da string login

    printf("\nSenha nova: ");
    j = 0;
    while((ch = getch()) != '\r'){  // Loop que lê cada caractere digitado até que o enter seja pressionado
        if(ch == '\b' && j > 0){  // Se o caractere for um backspace e houver caracteres para apagar
            senha[--j] = '\0';  // Apaga o último caractere
            printf("\b \b");  // Move o cursor para trás, imprime um espaço para apagar o *, e move o cursor para trás novamente
        } else if(ch != ' ' && ch != '\b'){
            senha[j++] = ch;  // Adiciona o caractere à senha
            printf("*");  // Imprime um *
        }
    }

    senha[j] = '\0';

    const char* sqlInsert = "INSERT INTO logins (login, senha) "
                            "VALUES (?, ?);";

    sqlite3_stmt* stmt;
    int rc2 = sqlite3_prepare_v2(db, sqlInsert, -1, &stmt, 0);
    if (rc2 != SQLITE_OK) {
        fprintf(stderr, "Falha ao prepara tratamento: %s\n", sqlite3_errmsg(db));
        return rc2;
    }

    sqlite3_bind_text(stmt, 1, login, -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 2, senha, -1, SQLITE_STATIC);

    rc2 = sqlite3_step(stmt);
    if (rc2 != SQLITE_DONE) fprintf(stderr, "Falha ao adicionar, erro: %s\n", sqlite3_errmsg(db));
    else printf("\n\nUsuario cadastrado com sucesso!\n\n");

    system("pause");
    system("cls");

    // Limpando o statement
    sqlite3_finalize(stmt);
    return rc2;
}

bool checarLogin(char login[50], char senha[50]) {
    sqlite3* db;
    int rc = sqlite3_open("database.db", &db);
    if (rc) {
        fprintf(stderr, "Nao e possivel abrir o banco de dados: %s\n", sqlite3_errmsg(db));
        return 1;
    } else printf("Validando login...\n");

    /*
    *   Depois de aberto, começa a executar o código que faz as pesquisa dos dados de logins 
    *   com base no login e senha.
    */
    const char* sqlSelect = "SELECT * FROM logins WHERE login = ? AND senha = ?";

    sqlite3_stmt* stmt;
    int rc2 = sqlite3_prepare_v2(db, sqlSelect, -1, &stmt, 0);
    if (rc2 != SQLITE_OK) {
        fprintf(stderr, "Falha ao preparar declaração: %s\n", sqlite3_errmsg(db));
        return rc2;
    }

    sqlite3_bind_text(stmt, 1, login, -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 2, senha, -1, SQLITE_STATIC);

    rc2 = sqlite3_step(stmt);

    // Limpa a declaração e fecha o banco de dados
    sqlite3_finalize(stmt);
    sqlite3_close(db);

    if (rc2 == SQLITE_ROW) return true;
    else return false;
}

// Faz a consulta de uma reserva no banco de dados com base no número da reserva
void pesquisarReserva(int num_reserva) {

    // Abre o banco de dados
    sqlite3* db;
    int rc = sqlite3_open("database.db", &db);
    if (rc) {
        fprintf(stderr, "Nao e possivel abrir o banco de dados: %s\n", sqlite3_errmsg(db));
        return 1;
    } else printf("Banco de dados aberto com sucesso!\n");

    /*
    *   Depois de aberto, começa a executar o código que faz as pesquisa dos dados da reserva 
    *   com base no número da reserva.
    */
    const char* sqlSelect = "SELECT nome, cpf, num_reserva, dias_hospedagem, data_checkin, data_checkout, "
                            "pagamento, valor_total, desconto, valor_final "
                            "FROM reservas WHERE num_reserva = ?;";

    sqlite3_stmt* stmt;
    int rc2 = sqlite3_prepare_v2(db, sqlSelect, -1, &stmt, 0);
    if (rc2 != SQLITE_OK) {
        fprintf(stderr, "Falha ao preparar declaração: %s\n", sqlite3_errmsg(db));
        return rc2;
    }

    // Vinculando o parâmetro
    sqlite3_bind_int(stmt, 1, num_reserva);

    // Executando a consulta
    rc2 = sqlite3_step(stmt);
    if (rc2 == SQLITE_ROW) {
        printf("Reserva encontrada:\n");
        printf("Nome: %s\n", sqlite3_column_text(stmt, 0));
        printf("CPF: %s\n", sqlite3_column_text(stmt, 1));
        printf("Número da Reserva: %d\n", sqlite3_column_int(stmt, 2));
        printf("Dias de Hospedagem: %d\n", sqlite3_column_int(stmt, 3));
        printf("Data de Check-in: %s\n", sqlite3_column_text(stmt, 4));
        printf("Data de Check-out: %s\n", sqlite3_column_text(stmt, 5));
        printf("Pagamento: %s\n", sqlite3_column_text(stmt, 6));
        printf("Valor Total: %.2f\n", sqlite3_column_double(stmt, 7));
        printf("Desconto: %s\n", sqlite3_column_text(stmt, 8));
        printf("Valor Final: %.2f\n", sqlite3_column_double(stmt, 9));
    } else if (rc == SQLITE_DONE) {
        printf("Nenhuma reserva encontrada com o número %d.\n", num_reserva);
    } else {
        fprintf(stderr, "Falha ao executar chave: %s\n", sqlite3_errmsg(db));
    }

    // Limpando o statement
    sqlite3_finalize(stmt);
    return rc2;
}


// Insere uma reserva no banco de dados
int criarReserva(const char* nome, const char* cpf, int num_reserva, int dias_hospedagem,
                const char* data_checkin, const char* data_checkout, const char* pagamento,
                float valor_total, float desconto, float valor_final) {

    // Abre o banco de dados
    sqlite3* db;
    int rc = sqlite3_open("database.db", &db);
    if (rc) {
        fprintf(stderr, "Nao e possivel abrir o banco de dados: %s\n", sqlite3_errmsg(db));
        return 1;
    } else printf("\n\nEntrando no sistema de criar reservas!\n");

    // Depois de aberto, começa a executar o código que insere os dados da reserva
    const char* sqlInsert = "INSERT INTO reservas (nome, cpf, num_reserva, dias_hospedagem, data_checkin, "
                            "data_checkout, pagamento, valor_total, desconto, valor_final) "
                            "VALUES (?, ?, ?, ?, ?, ?, ?, ?, ?, ?);";

    sqlite3_stmt* stmt;
    int rc2 = sqlite3_prepare_v2(db, sqlInsert, -1, &stmt, 0);
    if (rc2 != SQLITE_OK) {
        fprintf(stderr, "Failed to prepare statement: %s\n", sqlite3_errmsg(db));
        return rc2;
    }

    // Vinculando os parâmetros
    sqlite3_bind_text(stmt, 1, nome, -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 2, cpf, -1, SQLITE_STATIC);
    sqlite3_bind_int(stmt, 3, num_reserva);
    sqlite3_bind_int(stmt, 4, dias_hospedagem);
    sqlite3_bind_text(stmt, 5, data_checkin, -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 6, data_checkout, -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 7, pagamento, -1, SQLITE_STATIC);
    sqlite3_bind_double(stmt, 8, (double) valor_total);
    sqlite3_bind_double(stmt, 9, (double) desconto);
    sqlite3_bind_double(stmt, 10, (double) valor_final);

    // Executando a inserção
    rc2 = sqlite3_step(stmt);
    if (rc2 != SQLITE_DONE) fprintf(stderr, "Falha ao adicionar, erro: %s\n", sqlite3_errmsg(db));
    else printf("Reserva adicionada com sucesso!\n\n");

    // Limpando o statement
    sqlite3_finalize(stmt);
    return rc2;
}

// Delete todas as reservas do banco de dados
int destruirReservas() {

    // Abre o banco de dados
    sqlite3* db;
    int rc = sqlite3_open("database.db", &db);
    if (rc) {
        fprintf(stderr, "Nao e possivel abrir o banco de dados: %s\n", sqlite3_errmsg(db));
        return 1;
    } else printf("Banco de dados aberto com sucesso!\n");


    /*
    *   Depois de aberto, começa a executar o código que deleta os dados da reserva com 
    *   base no número da reserva
    */ 
    const char* sqlDelete = "DELETE FROM reservas";
    sqlite3_stmt* stmt;
    int rc2 = sqlite3_prepare_v2(db, sqlDelete, -1, &stmt, 0);
    if (rc2 != SQLITE_OK) {
        fprintf(stderr, "Failed to prepare statement: %s\n", sqlite3_errmsg(db));
        return rc2;
    }

    // Executando a deleção
    rc2 = sqlite3_step(stmt);
    if (rc2 != SQLITE_DONE) fprintf(stderr, "Execution failed: %s\n", sqlite3_errmsg(db)); 
    else printf("Record deleted successfully\n");

    // Limpando o statement
    sqlite3_finalize(stmt);
    return rc2;
}

// Imprime todas as reservas no banco de dados
void imprimirTodasReservas() {
    // Abre o banco de dados
    sqlite3* db;
    int rc = sqlite3_open("database.db", &db);
    if (rc) {
        fprintf(stderr, "Nao e possivel abrir o banco de dados: %s\n", sqlite3_errmsg(db));
        return 1;
    } else printf("Banco de dados aberto com sucesso!\n");


    // Depois de aberto, começa a executar o código que mostra todos as reservas feitas
    const char* sqlSelectAll = "SELECT nome, cpf, num_reserva, dias_hospedagem, data_checkin, data_checkout, "
                               "pagamento, valor_total, desconto, valor_final FROM reservas;";
    sqlite3_stmt* stmt;
    int rc2 = sqlite3_prepare_v2(db, sqlSelectAll, -1, &stmt, 0);
    if (rc2 != SQLITE_OK) {
        fprintf(stderr, "Failed to prepare statement: %s\n", sqlite3_errmsg(db));
        return rc2;
    }

    printf("Lista de todas as reservas:\n");

    // Executando a consulta e iterando sobre os resultados
    while ((rc2 = sqlite3_step(stmt)) == SQLITE_ROW) {
        printf("Nome: %s\n", sqlite3_column_text(stmt, 0));
        printf("CPF: %s\n", sqlite3_column_text(stmt, 1));
        printf("Número da Reserva: %d\n", sqlite3_column_int(stmt, 2));
        printf("Dias de Hospedagem: %d\n", sqlite3_column_int(stmt, 3));
        printf("Data de Check-in: %s\n", sqlite3_column_text(stmt, 4));
        printf("Data de Check-out: %s\n", sqlite3_column_text(stmt, 5));
        printf("Pagamento: %s\n", sqlite3_column_text(stmt, 6));
        printf("Valor Total: %.2f\n", sqlite3_column_double(stmt, 7));
        printf("Desconto: %s\n", sqlite3_column_text(stmt, 8));
        printf("Valor Final: %.2f\n", sqlite3_column_double(stmt, 9));
        printf("---------------------------------------\n");
    }

    if (rc2 != SQLITE_DONE) {
        fprintf(stderr, "Failed to execute query: %s\n", sqlite3_errmsg(db));
    }

    // Limpando a declaração
    sqlite3_finalize(stmt);
}


// *** FIM DAS FUNÇÕES COM BANCO DE DADOS ***



// inicializacao da struct
struct sistemadereservadehotel reserva[100];

// função para criar as reservas dos hóspedes.
void reservaHospedes(){
    int i=0;
    char menu[3];
    system("cls");

    for(i=0; i<LOTACAOMAXIMA; i++){
        system("cls");
        printf("(-1) Para finalizar o cadastro. \n");
        printf("Crie um codigo de reserva para o hóspede: \n");
        scanf("%d", &reserva[i].numeroReserva);
        if(reserva[i].numeroReserva==-1){
            break;
        }
        for (int j=0; j<i; j++){
            if (reserva[j].numeroReserva==reserva[i].numeroReserva){
                printf("Número de reserva já existe, insira outro número: \n");
                scanf("%d", &reserva[i].numeroReserva);
                j=-1; // Reinicia a verificação com o novo número
            }
        }
        getchar();
        printf("Informe o nome do hóspede: \n");
        fgets(reserva[i].nomes, 100, stdin);
        reserva[i].nomes[strcspn(reserva[i].nomes, "\n")]=0; // Remove o '\n' no final da string lida por fgets
        printf("Informe o CPF do hóspede: \n");
        fgets(reserva[i].cpf, 15, stdin);
        reserva[i].cpf[strcspn(reserva[i].cpf, "\n")]=0; // Remove o '\n' no final da string lida por fgets 
        // comando para conferir se o cpf é valido.
        for(int j=0; j<1; j++){ 
            if(strcmp(reserva[i].cpf, "00000000000")==0){
                printf("CPF inválido. Insira um CPF válido: \n");
                fgets(reserva[i].cpf, 15, stdin);
                reserva[i].cpf[strcspn(reserva[i].cpf, "\n")]=0; // Remove o '\n' no final da string lida por fgets
                j=-1;
            }
        } 
        printf("Quantos dias o hóspede estará hospedado com a gente? \n");
        scanf("%d", &reserva[i].dias);
        getchar();
        printf("Qual a data que o hóspede fará checkin? \n");
        fgets(reserva[i].dataCheckin, 100, stdin);

        // Remove a quebra de linha que fica no final da string
        if (reserva[i].dataCheckin[strlen(reserva[i].dataCheckin) - 1] == '\n')
        reserva[i].dataCheckin[strlen(reserva[i].dataCheckin) - 1] = '\0';

        printf("Qual a data que o hóspede fará checkout? \n");
        fgets(reserva[i].dataCheckout, 100, stdin);

        // Remove a quebra de linha que fica no final da string
        if (reserva[i].dataCheckout[strlen(reserva[i].dataCheckout) - 1] == '\n')
        reserva[i].dataCheckout[strlen(reserva[i].dataCheckout) - 1] = '\0';

        printf("Qual a forma de pagamento? \n");
        fgets(reserva[i].pagamento, 100, stdin);
        reserva[i].pagamento[strcspn(reserva[i].pagamento, "\n")]=0; // Remove o '\n' no final da string lida por fgets

        if(reserva[i].dias<10){
            reserva[i].menorDez=reserva[i].dias*160;
            reserva[i].total=reserva[i].menorDez;
        }else if(reserva[i].dias>10 && reserva[i].dias<20){
            reserva[i].entreDezeVinte=reserva[i].dias*130;
            reserva[i].total=reserva[i].entreDezeVinte;
        }else if(reserva[i].dias>20){
            reserva[i].maiorVinte=reserva[i].dias*110;
            reserva[i].total=reserva[i].maiorVinte;
        }
        if(strcmp(reserva[i].pagamento, "Pix")==0||strcmp(reserva[i].pagamento, "Debito")==0||strcmp(reserva[i].pagamento, "Dinheiro")==0||strcmp(reserva[i].pagamento, "pix")==0||strcmp(reserva[i].pagamento, "debito")==0||strcmp(reserva[i].pagamento, "dinheiro")==0||strcmp(reserva[i].pagamento, "PIX")==0||strcmp(reserva[i].pagamento, "DEBITO")==0||strcmp(reserva[i].pagamento, "DINHEIRO")==0){
            reserva[i].desconto=reserva[i].total*0.1;
        }else{
            printf("Você não tem direito a desconto.\n");
        }

        // comando para imprimir para o usuario o cadastro e pedir a confirmacao do mesmo.
        system("cls");
        printf("Nome: %s\n", reserva[i].nomes);
        printf("Cpf: %s\n", reserva[i].cpf);
        printf("Número de Reserva: %d\n", reserva[i].numeroReserva);
        printf("Dias de Hospedagem: %d\n", reserva[i].dias);
        printf("Data de Check-in: %s\n", reserva[i].dataCheckin);
        printf("Data de Check-out: %s\n", reserva[i].dataCheckout);
        printf("Forma de Pagamento: %s\n", reserva[i].pagamento);
        printf("Valor cheio: %.2f\n", reserva[i].total);
        printf("Desconto: %.2f\n", reserva[i].desconto);
        printf("Valor final: %.2f\n", reserva[i].total-reserva[i].desconto);
        printf("Esse cadastro esta correto? ");
        scanf("%s", &menu);
        for(int j=0; j<1; j++){
            if(strcmp(menu, "N")==0||strcmp(menu, "n")==0||strcmp(menu, "nao")==0||strcmp(menu, "NAO")==0||strcmp(menu, "Nao")==0){
                continue;
        }

        /*FILE *arquivo=fopen("reservaHospedes.txt", "a"); // Abre o arquivo para anexação
        if(arquivo==NULL){
        printf("Erro ao abrir o arquivo.\n");
        return;
        }

        fprintf(arquivo, "*  ######   #######   #####   #######  ######   ##   ##    ##      ##### *\n");
        fprintf(arquivo, "*   ##  ##   ##   #  ##   ##   ##   #   ##  ##  ##   ##   ####    ##   ## *\n");
        fprintf(arquivo, "*   ##  ##   ## #    #         ## #     ##  ##   ## ##   ##  ##   # *\n");
        fprintf(arquivo, "*   #####    ####     #####    ####     #####    ## ##   ##  ##    ##### *\n");
        fprintf(arquivo, "*   ## ##    ## #         ##   ## #     ## ##     ###    ######        ## *\n");
        fprintf(arquivo, "*   ##  ##   ##   #  ##   ##   ##   #   ##  ##    ###    ##  ##   ##   ## *\n");
        fprintf(arquivo, "*  #### ##  #######   #####   #######  #### ##     #     ##  ##    ##### *\n");
      
        fprintf(arquivo, "\n\t\t*  𝐑𝐄𝐒𝐄𝐑𝐕𝐀 %d *                \n", i+1);
        fprintf(arquivo, "###########################################\n");
        fprintf(arquivo, "Nome: %s\n", reserva[i].nomes);
        fprintf(arquivo, "Cpf: %s\n", reserva[i].cpf);
        fprintf(arquivo, "Número de Reserva: %d\n", reserva[i].numeroReserva);
        fprintf(arquivo, "Dias de Hospedagem: %d\n", reserva[i].dias);
        fprintf(arquivo, "Data de Check-in: %s", reserva[i].dataCheckin);
        fprintf(arquivo, "Data de Check-out: %s", reserva[i].dataCheckout);
        fprintf(arquivo, "Forma de Pagamento: %s\n", reserva[i].pagamento);
        fprintf(arquivo, "Valor cheio: %.2f\n", reserva[i].total);
        fprintf(arquivo, "Desconto: %.2f\n", reserva[i].desconto);
        fprintf(arquivo, "Valor final: %.2f\n", (reserva[i].total - reserva[i].desconto));
        fprintf(arquivo, "###########################################\n");
        fprintf(arquivo, "\n"); // Linha em branco para separar registros

        fclose(arquivo); // Fecha o arquivo*/

        criarReserva(reserva[i].nomes, reserva[i].cpf, reserva[i].numeroReserva, 
                    reserva[i].dias, reserva[i].dataCheckin, reserva[i].dataCheckout,
                    reserva[i].pagamento, reserva[i].total, reserva[i].desconto, 
                    (reserva[i].total - reserva[i].desconto)
        );
    }

    // comando para limitar a quantidade de reserva.
    if(i>=LOTACAOMAXIMA){
        system("cls");
        printf("O cadastro está lotado, por favor pare de cadastrar.\n");
        printf("Pressione Enter para voltar ao menu inicial.\n");
        getchar(); // Aguarda o usuário pressionar Enter
        return; // Retorna para o menu inicial
        }

    }
}

// função para consultar a reserva através do codigo de reserva.
void consultaReserva(){
    float pagamentoD;
    int codigo;
    system("cls");
    while(1){
        system("cls");
        printf("Informe o número da reserva, (-1) para sair: ");
        scanf("%d", &codigo);
        if(codigo==-1){
            break;
        }

        pesquisarReserva(codigo);

        system("pause");
    }
}

// funcao para carregar o historico de reservas
void carregarReservas(){
    imprimirTodasReservas();
    system("pause");
}

// funcao para apagar o historico de reserva no txt
void apagarHistoricodeReservas() {
    destruirReservas();
    printf("O conteúdo do arquivo foi apagado.\n");
}

// função para consultar as promoções disponiveis.
void consultarPrecos(){
    char menu[3];
    system("cls");

    printf("*  ######   ######    #####   ##   ##   #####     ####    #####   #######   ##### *\n");
    printf("*   ##  ##   ##  ##  ##   ##  ### ###  ##   ##   ##  ##  ##   ##   ##   #  ##   ## *\n");
    printf("*   ##  ##   ##  ##  ##   ##  #######  ##   ##  ##       ##   ##   ## #    # *\n");
    printf("*   #####    #####   ##   ##  #######  ##   ##  ##       ##   ##   ####     ##### *\n");
    printf("*   ##       ## ##   ##   ##  ## # ##  ##   ##  ##       ##   ##   ## #         ## *\n");
    printf("*   ##       ##  ##  ##   ##  ##   ##  ##   ##   ##  ##  ##   ##   ##   #  ##   ## *\n");
    printf("*  ####     #### ##   #####   ##   ##   #####     ####    #####   #######   ##### *\n");

    printf("\nTemos algumas promocões!\n");
    printf("Se a estadia for menor que 10 dias cobramos R$ 160 a dária!\n");
    printf("Se a estadia for maior que 10 e menor 20 dias cobramos R$ 130 a dária!\n");
    printf("Se a estadia for acima de 20 dias, a diária sai por R$ 110!\n\n");

    while(1){
        printf("\nDeseja voltar ao menu principal? \n");
        scanf("%s", &menu);
        if(strcmp(menu, "S")==0||strcmp(menu, "s")==0||strcmp(menu, "sim")==0||strcmp(menu, "SIM")==0||strcmp(menu, "Sim")==0){
            break;
        }
    }
}

// função para consultar os descontos através do metodo de pagamento.
void descontos(){
    char menu[3];
    system("cls");

    printf("*  #####    #######   #####     ####    #####   ##   ##  ######    #####    ##### *\n");
    printf("*   ## ##    ##   #  ##   ##   ##  ##  ##   ##  ###  ##  # ## #   ##   ##  ##   ## *\n");
    printf("*   ##  ##   ## #    #        ##       ##   ##  #### ##    ##     ##   ##  # *\n");
    printf("*   ##  ##   ####     #####   ##       ##   ##  ## ####    ##     ##   ##   ##### *\n");
    printf("*   ##  ##   ## #         ##  ##       ##   ##  ##  ###    ##     ##   ##       ## *\n");
    printf("*   ## ##    ##   #  ##   ##   ##  ##  ##   ##  ##   ##    ##     ##   ##  ##   ## *\n");
    printf("*  #####    #######   #####     ####    #####   ##   ##   ####     #####    ##### *\n");

    printf("\nSe o pagamento for em dinheiro, PIX ou cartão de débito o cliente tem direito a 10%% de desconto.\n");
    printf("Se o pagamento for em cartão de credito o cliente não tem direito a desconto.\n\n");

    while(1){
        printf("Deseja voltar ao menu principal? \n");
        scanf("%s", &menu);
        if(strcmp(menu, "S")==0||strcmp(menu, "s")==0||strcmp(menu, "sim")==0||strcmp(menu, "SIM")==0||strcmp(menu, "Sim")==0){
            break;
        }
    }
}

// funcao para pedir o login e senha para acessar o programa
void loginSenha(){
    char senha[50], login[50];
    char senhaCheck[7]="adm123", loginCheck[4]="ADM";
    char ch;

    for(int i=0; i<PARALOGIN; i++){  // Loop que permite até PARALOGIN tentativas

        printf("*  ####      #####     ####    ####    ##   ## *\n");
        printf("*   ##      ##   ##   ##  ##    ##     ###  ## *\n");
        printf("*   ##      ##   ##  ##         ##     #### ## *\n");
        printf("*   ##      ##   ##  ##         ##     ## #### *\n");
        printf("*   ##   #  ##   ##  ##  ###    ##     ##  ### *\n");
        printf("*   ##  ##  ##   ##   ##  ##    ##     ##   ## *\n");
        printf("*  #######   #####     #####   ####    ##   ## *\n");

        printf("\nLogin: ");
        int j = 0;
        while((ch = getch()) != '\r'){  // Loop que lê cada caractere digitado até que o enter seja pressionado
            if(ch == '\b' && j > 0){  // Se o caractere for um backspace e houver caracteres para apagar
                login[--j] = '\0';  // Apaga o último caractere
                printf("\b \b");  // Move o cursor para trás, imprime um espaço para apagar o *, e move o cursor para trás novamente
            } else if(ch != ' ' && ch != '\b'){
                login[j++] = ch;  // Adiciona o caractere ao login
                printf("*");  // Imprime um *
            }
        }
        login[j] = '\0';  // Adiciona o caractere nulo ao final da string login


        if(strcmp(login, key) == 0) {
            criarLogin();
            i = -1;
            continue;
        }

        printf("\nSenha: ");
        j = 0;
        while((ch = getch()) != '\r'){  // Loop que lê cada caractere digitado até que o enter seja pressionado
            if(ch == '\b' && j > 0){  // Se o caractere for um backspace e houver caracteres para apagar
                senha[--j] = '\0';  // Apaga o último caractere
                printf("\b \b");  // Move o cursor para trás, imprime um espaço para apagar o *, e move o cursor para trás novamente
            } else if(ch != ' ' && ch != '\b'){
                senha[j++] = ch;  // Adiciona o caractere à senha
                printf("*");  // Imprime um *
            }
        }

        senha[j] = '\0';  // Adiciona o caractere nulo ao final da string senha

        system("cls");  // Limpa a tela
        if(checarLogin(login, senha)){  // Se a senha e o login forem corretos
            printf("Login e senha corretos! \n\n");  // Imprime uma mensagem de sucesso
            break;  // Sai do loop
        }else{  // Se a senha ou o login estiverem incorretos
            printf("Login e senha incorreta! Favor digite o login e senha novamente \n\n");  // Imprime uma mensagem de erro
            i=-1;  // Redefine o contador de tentativas
        }

        system("pause");
        system("cls");
    }
}

// inicializacao do programa
int main(){
    int opcao;
    // cor das letras.
    system("color 0A"); 

    // implementacao do  login e senha.
    loginSenha();

    do{
        // menu.
        system("cls");
        printf("  ##   ##   #####    #####   ######   #######  #####      ##              #######    ##       ####    ####    #### \n");
        printf("  ##   ##  ##   ##  ##   ##   ##  ##   ##   #   ## ##    ####              ##   #   ####     ##  ##    ##      ## \n");
        printf("  ##   ##  ##   ##  #         ##  ##   ## #     ##  ##  ##  ##             ## #    ##  ##   ##         ##      ## \n");
        printf("  #######  ##   ##   #####    #####    ####     ##  ##  ##  ##             ####    ##  ##   ##         ##      ## \n");
        printf("  ##   ##  ##   ##       ##   ##       ## #     ##  ##  ######             ## #    ######   ##         ##      ##   # \n");
        printf("  ##   ##  ##   ##  ##   ##   ##       ##   #   ## ##   ##  ##             ##      ##  ##    ##  ##    ##      ##  ## \n");
        printf("  ##   ##   #####    #####   ####     #######  #####    ##  ##            ####     ##  ##     ####    ####    ####### \n");
        printf("\n[1] - Fazer reserva.\n");
        printf("[2] - Consultar reserva.\n");
        printf("[3] - Consultar preços.\n");
        printf("[4] - Descontos.\n");
        printf("[5] - Historico de reservas.\n");
        printf("[6] - Zerar historico de reservas.\n");
        printf("[-1] - Sair do programa.\n");
        printf("Opção: ");
        scanf("%d", &opcao);

        // forma para ativar as funcões criadas anteriormente.
        switch(opcao){
            case 1:
                reservaHospedes();
                break;
            case 2:
                consultaReserva();
                break;
            case 3:
                consultarPrecos();
                break;
            case 4:
                descontos();
                break;
            case 5:
                system("cls");
                carregarReservas();
                break;
            case 6:
                apagarHistoricodeReservas();
                break;
            case -1:
                printf("\nSaindo do programa...");
                break;
            default:
                printf("Opção inválida!\n");
            }
    }while(opcao!=-1);

    return 0;
}
