
# Sistema de Cadastro de Clientes - Hotel

Este projeto é um sistema de cadastramento de clientes para um hotel, desenvolvido em C. Ele permite que os funcionários registrem informações dos hóspedes, como nome, CPF, datas de check-in e check-out, forma de pagamento, e calcula automaticamente o valor total da estadia com descontos aplicáveis. O objetivo é proporcionar uma interface simples e eficiente para o gerenciamento de reservas e registros de clientes.


## Funcionalidades

Cadastro de Hóspedes: Registra dados essenciais dos hóspedes, incluindo:

Nome e CPF

Data de Check-in e Check-out

Quantidade de dias da reserva

Forma de pagamento

Cálculo de Preço: Calcula o valor da estadia de acordo com a quantidade de dias:

Até 10 dias: tarifa base

Entre 10 e 20 dias: desconto automático

Acima de 20 dias: tarifa promocional

Aplicação de Desconto: Um desconto de 10% é aplicado para pagamentos via Pix, débito ou dinheiro.


## Tecnologias Utilizadas

- Linguagem de Programação: C
 - Design de Interface:  [Figma](https://www.figma.com/design/IP9KhQxS9kcxSd4M0NoWn1/Hospeda-Facil?node-id=8002-161&m=dev&t=HwU67qw4QBRDpx5p-1)
- banco - DB.Browser.for.SQLite
## Color Reference

| Color             | Hex                                                                |
| ----------------- | ------------------------------------------------------------------ |
| Example Color | ![#CDD6CF](https://via.placeholder.com/10/CDD6C?text=+) #CDD6CF |
| Example Color | ![#366758](https://via.placeholder.com/10/366758?text=+) #366758 |
| Example Color | ![#FFFFFF](https://via.placeholder.com/10/ffffff?text=+) #FFFFFF |



## Pré-requisitos

Para compilar e executar este projeto, você precisará de um compilador de C, como GCC, e de uma configuração básica do ambiente de desenvolvimento.

## Como Usar

Clonar o Repositório:

```bash
  git clone https://github.com/exFaker/Hospeda-Facil-.git
```

Compilar o Código: Compile o programa usando um compilador C:

```bash
   gcc sistema_hotel.c -o sistema_hotel

```

Executar o Programa:

```bash
  ./sistema_hotel

```


## Author

- [@exFaker](https://github.com/exFaker)

