# Calculadora de IMC e Criadora de dados csv
### O que ele faz?
Um programa feito em C puro que tem a capacidade de ler arquivos .csv com informações sobre uma pessoa e retornar uma tabela completa com todas suas informações como peso, altura e IMC.
O programa é capaz de criar um arquivo .csv, se ele não existir, ler o arquivo e escrever informações dentro desse arquivo. Essas informações são salvas quando o programa termina, portanto não há preocupação quanto a dados temporários. Toda vez que o programa é executado ele sempre abrirá o arquivo do mesmo nome, portanto não modifique o nome do arquivo e nem sua extensão. É possível alterar o nome do arquivo a ser salvo, para isso veja alterações internas logo em seguida.

### Acessando arquivos .csv

Para acessar os arquivos .csv, abra a pasta entitulada de `output`, lá dentro estará os arquivos `pessoa.csv` e `peso.csv`. Esses são os nomes padrões de salvamento do arquivo.

### Alterações internas

Para alterações internas do programa, abra o arquivo e C e altere as opções no início, logo após as bibliotecas usadas.

### Como abrir o programa?

Para abrir o programa, é necessário um compilador C, como o `GCC`. Instale-o e abra o arquivo entitulado de `main.c`
