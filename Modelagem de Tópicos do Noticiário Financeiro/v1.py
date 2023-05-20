# Modelagem de Tópicos do Noticiário Financeiro 

# Versão 1 - Voting 

# Imports 
import numpy as np
from nltk.corpus import stopwords # stopwords são palavras comuns em um idioma que não são tão importantes e geralmente são removidas de textos em tarefas de PLN, ex de stopwords em inglês: 'a', 'and', 'the', 'in', 'on'
from sklearn.datasets import load_files # É usado para carregar arquivos de texto armazenados em uma estrutura de diretório
from sklearn.feature_extraction.text import TfidfVectorizer # usada para converter uma coleção de documentos brutos em uma matriz de recursos Tf-idf. Tf-idf é uma medida da importância de uma palavra em um documento em relação a uma coleção de documentos.
from sklearn.model_selection import train_test_split # é usado para dividir um dataset em dois subconjuntos: um conjunto de treinamento e um conjunto de teste
from sklearn.linear_model import LogisticRegression # algoritmo de ml que é usado para prever um resultado binário
from sklearn.naive_bayes import MultinomialNB # algoritmo de ml, baseado no teorema de bayes, usa probabilidades para prever a classe de uma amostra.
from sklearn.ensemble import RandomForestClassifier, VotingClassifier # RandomForest = algoritmo de classificação baseado em arvores de decisão / votingClassifier permite que vc use mais de um unico modelo.
from sklearn.metrics import accuracy_score # usada para calcular a acurácia de um modelo de classificação.

# Carregando os dados
noticias = load_files('dados', encoding = 'utf-8', decode_error = 'replace')
# O primeiro argumento é o caminho para o diretório que contém os arquivos
# O segundo argumento é o encoding dos arquivos, que é o formato de codificação dos caracteres
# Terceiro argumento especifica como lidar com erros de decodificação dos arquivos

# Separando variaveis de entrada e saida 
X = noticias.data
y = noticias.target

# Lista de stop words 
my_stop_words = set(stopwords.words('english'))

# Divisão em treino e teste (70/30)
X_treino, X_teste, y_treino, y_teste = train_test_split(X, y, test_size = 0.30, random_state = 75)
# random_state =  determina a semente do gerador de números aleatórios usado para dividir os dados. Isso garante que a divisão seja reproduzível.

# Vetorização 
vectorizer = TfidfVectorizer(norm = None, stop_words = my_stop_words, max_features = 1000, decode_error = "ignore")
# norm = significa que o TfidfVectorizer NÃO irá normalizar os valores de entrada
# stop_words = significa que o TfidfVectorizer irá remover as palavras contidas na lista "my_stop_words"
# max_features = significa que o TfidfVectorizer irá selecionar apenas as 1000 palavras mais importantes para incluir no vetor de características resultante.
# decode_error = significa que o TfidfVectorizer irá ignorar qualquer erro de decodificação encontrado ao processar o texto.

# Aplicamos a vetorização. 
# Observe que treinamos e aplicamos em treino e apenas aplicamos em teste 
X_treino_vectors = vectorizer.fit_transform(X_treino)
X_teste_vectors = vectorizer.transform(X_teste)

# Explicação código linha 42: 
# O método "fit_transform" é usado para ajustar o vetorizador ao conjunto de dados de treinamento fornecido (X_treino) e, em seguida, transformá-lo em um conjunto de vetores.


# Criando 3 modelos com 3 algoritmoos diferentes
# https://scikit-learn.org/stable/modules/generated/sklearn.linear_model.LogisticRegression.html
modelo1 = LogisticRegression(multi_class = 'multinomial', solver = 'lbfgs', random_state = 30, max_iter = 1000)

# https://scikit-learn.org/stable/modules/generated/sklearn.ensemble.RandomForestClassifier.html
modelo2 = RandomForestClassifier(n_estimators = 1000, max_depth = 100, random_state = 1)

# https://scikit-learn.org/stable/modules/generated/sklearn.naive_bayes.MultinomialNB.html
modelo3 = MultinomialNB()


# Lista para o resultado 
resultado = []

# Iniciando o modelo de votação 
# https://scikit-learn.org/stable/modules/generated/sklearn.ensemble.VotingClassifier.html
# https://scikit-learn.org/stable/modules/ensemble.html#voting-classifier
voting_model = VotingClassifier(estimators = [ ('lg', modelo1), ('rf', modelo2), ('nb', modelo3) ], voting = 'soft')
print("\nModelo de Votação:\n")
print(voting_model)

# Treinamento
voting_model = voting_model.fit(X_treino_vectors, y_treino)
    
# Previsões com dados de teste
previsoes =  voting_model.predict(X_teste_vectors)

# Grava o resultado
resultado.append(accuracy_score(y_teste, previsoes))
    
# Print
print('\nAcurácia do Modelo:', accuracy_score(y_teste, previsoes),'\n')
print('\n')

