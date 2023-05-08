# Projeto 1 - Classificação de Texto com aprendizagem supervisionada

#Pacotes 
import re # pacote expressão regular (usado para filtrar o texto)
import praw
import config
import numpy as np #É uma biblioteca Python que fornece um objeto array multidimensional
from sklearn.model_selection import train_test_split # Usado para fazer a divisão de dados de treino e de teste
from sklearn.feature_extraction.text import TfidfVectorizer # Usado para preparar a matriz com os dados de texto
from sklearn.decomposition import TruncatedSVD # usado para redução de dimensionalidade
from sklearn.neighbors import KNeighborsClassifier # algoritmo de ml
from sklearn.ensemble import RandomForestClassifier # algoritmo de ml
from sklearn.linear_model import LogisticRegressionCV # algoritmo de ml 
from sklearn.metrics import classification_report # imprimir as métricas de avaliação dos modelos
from sklearn.pipeline import Pipeline # ajuda criar códigos que possuam um padrão que possa ser facilmente entendido.
from sklearn.metrics import confusion_matrix # imprimir o resultado final do modelo e avaliar a performance
import matplotlib.pyplot as plt # usado para criar  gráficos 
import seaborn as sns # usado para criar gráficos 

## Carregando os dados 

# Lista de temas que usaremos para buscas no Reddit. 
# Essas serão as classes que usaremos como variavel target 
assuntos = ['datascience', 'machinelearning', 'physics', 'astrology', 'conspiracy']

# função para carregar os dados 
def carrega_dados(): 

	# primeiro extraimos os dados do Reddit acessando via API 
	api_reddit = praw.Reddit(
	    client_id="ZZZZZZZZZZZZZZZZZZZZZZZZZZZZ",
	    client_secret="ZZZZZZZZZZZZZZZZ",
	    password="ZZZZZZZZZZZZZZZZZZZZ",
	    user_agent="ZZZZZZZZZZZZZZZZZZ",
	    username="ZZZZZZZZZZZZZ",)

	# Contamos o numero de caracteres usando expressões regulares 
	char_count = lambda post: len(re.sub('\W|\d', '', post.selftext)) # \W = Caractere \d = digito / 
	# Essa é uma função anonima que verifica se eu tenho Caracteres e digitos, se tiver, eu irei contar a quantidade usando a função len()

	# Definimos a condição para filtrar os posts (retornaremos somente posts com 100 ou mais caracteres)
	mask = lambda post: char_count(post) >= 100 

	# Listas para os resultados 
	data = []
	labels = []

	# Loop 
	for i, assunto in enumerate(assuntos): 

		# Extrai os posts 
		subreddit_data = api.reddit.subreddit(assunto).new(limit = 1000)

		# Filtra os posts que não satisfazem nossa condição 
		posts = [post.selftext for post in filter(mask, subreddit_data)]

		# adiciona posts e labels ás listas 
		data.extend(posts)
		labels.extend([i] * len(posts))

		# Print
		print(f"Número de posts do assunto r/{assunto}: {len(posts)}",
              f"\nUm dos posts extraídos: {posts[0][:600]}...\n",
              "_" * 80 + '\n')

	return data, labels


## Divisão em Dados de Treino e Teste

# Variáveis de controle
TEST_SIZE = .2
RANDOM_STATE = 0


# Função para split dos dados
def split_data():

    print(f"Split {100 * TEST_SIZE}% dos dados para teste e avaliação do modelo...")
    
    # Split dos dados
    X_treino, X_teste, y_treino, y_teste = train_test_split(data, 
                                                            labels, 
                                                            test_size = TEST_SIZE, 
                                                            random_state = RANDOM_STATE)

    print(f"{len(y_teste)} amostras de teste.")
    
    return X_treino, X_teste, y_treino, y_teste

## Pré-Processamento de Dados e Extração de Atributos

# - Remove símbolos, números e strings semelhantes a url com pré-processador personalizado
# - Vetoriza texto usando o termo frequência inversa de frequência de documento
# - Reduz para valores principais usando decomposição de valor singular
# - Particiona dados e rótulos em conjuntos de treinamento / validação

# Variáveis de controle
MIN_DOC_FREQ = 1
N_COMPONENTS = 1000
N_ITER = 30

# Função para o pipeline de pré-processamento
def preprocessing_pipeline():
    
    # Remove caracteres não "alfabéticos"
    pattern = r'\W|\d|http.*\s+|www.*\s+'
    preprocessor = lambda text: re.sub(pattern, ' ', text)

    # Vetorização TF-IDF
    vectorizer = TfidfVectorizer(preprocessor = preprocessor, stop_words = 'english', min_df = MIN_DOC_FREQ)

    # Reduzindo a dimensionalidade da matriz TF-IDF 
    decomposition = TruncatedSVD(n_components = N_COMPONENTS, n_iter = N_ITER)
    
    # Pipeline
    pipeline = [('tfidf', vectorizer), ('svd', decomposition)]

    return pipeline
