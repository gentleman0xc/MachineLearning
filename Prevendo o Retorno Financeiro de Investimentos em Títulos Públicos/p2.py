# Parte 2 - Desenvolvimento do Modelo Sem Uso de Framework

# Imports 

from os import getcwd 
from math import sqrt 
from csv import reader, writer 

# Obtém o caminho do diretório atual 
path = getcwd()

# Função para o cálculo da média 
def mean(values): # values = lista de valores como argumento 
	return sum(values) / float(len(values)) # return indica o valor que a função deve retornar quando ela for chamada

# Explicação da função da linha 13:
# Neste caso a função retorna o resultado da sivisão da soma dos valores 
# da lista pelo seu comprimento, usando as funções ´sum´ e ´len´ do python.
# Por exemplo, se a função for chamada com uma lista `[1,2,3,4,5]` a expressão
# `sum(values) / float(len(values))` será avaliada como `15 / 5`, que é igual
# a `3.0`. 


# Função para o cálculo da covariância
# Em teoria da probabilidade e na estatística, a covariância, ou variância conjunta, é uma medida do grau de interdependência (ou inter-relação) 
# numérica entre duas variáveis aleatórias. Assim, variáveis independentes têm covariância zero.
# A covariância ou variância conjunta é um momento conjunto de primeira ordem das variáveis aleatórias X e Y, centrados nas respectivas médias. 
# É a média do grau de interdependência ou inter-relação numérica linear entre elas.
# A covariância é por vezes chamada de medida de dependência linear entre as duas variáveis aleatórias.
# A covariância entre duas variáveis pode ser obtida de dados de variância.
def covariance(x, mean_x, y, mean_y): 
	covar = 0.0 
	for i in range(len(x)): 
		covar += (x[i] - mean_x) * (y[i] - mean_y) # o operador "+=" é usado para atualizar o valor de uma variável de forma compacta, sem precisar escrever o mesmo valor duas vezes.
	return covar 


# Função para o cálculo da variância
# Na teoria da probabilidade e na estatística, a variância de uma variável aleatória ou processo estocástico é uma medida da sua dispersão estatística, indicando 
# "o quão longe" em geral os seus valores se encontram do valor esperado.
def variance(list, mean):
	return sum([(x - mean)**2 for x in list]) # se lê: para cada x na lista atribua a operação (x-mean)**2

# Explicação da função da linha 41: 
# Por exemplo, se a lista for [1, 2, 3, 4] e a média for 2.5, a list comprehension calculará 
# [(1-2.5)**2, (2-2.5)**2, (3-2.5)**2, (4-2.5)**2] = [2.25, 0.25, 0.25, 1.25], 
# e a função "sum" retornará o valor 4.0, que é a soma dos quadrados das diferenças.


# Função para o cálculo dos coeficientes
def coefficient(covar, var, mean_x, mean_y):
	b1 = covar / var 
	b0 = mean_y - (b1 * mean_x)
	return b1, b0

# Função para carregar o dataset
def carrega_dados(dataset):
	init = 0
	x = list()
	y = list()
	with open(dataset) as file:
		content = reader(file)
		for row in content:
			if init == 0:
				init = 1
			else:
				x.append(row[0])
				y.append(row[1])
	return x, y

# Explicação da função da linha 57: 
# Este código define uma função chamada "carrega_dados" que lê um arquivo de dados e retorna duas listas, uma com os valores da primeira coluna do arquivo
# e outra com os valores da segunda coluna. A função recebe como entrada o nome do arquivo de dados e usa a função 'open' para abri-lo. Em seguida, a função 'reader'
# é usada para ler o conteúdo do arquivo linha a linha. O código verifica se esta é a primeira linha do arquivo (que geralmente contém os rótulos das colunas) e, se for,
# a linha é ignorada. Se não for a primeira linha, o valor da primeira coluna é adicionado à lista "x" e o valor da segunda coluna é adicionado à lista "y". 
#Isso é feito para todas as linhas do arquivo, exceto a primeira. Quando o arquivo é lido por completo, a função retorna as duas listas com os valores das colunas. 
# É importante notar que este código supõe que o arquivo de dados tem uma estrutura específica e que as duas colunas contêm valores numéricos.

# O comando "with" garante que o arquivo será fechado automaticamente quando o contexto de execução terminar, sem que seja necessário chamar uma função de fechamento explícita.