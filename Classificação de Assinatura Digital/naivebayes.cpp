// Projeto 6 - Modelo de Classificação com Naive Bayes Para Classificação de Assinatura Digital em C++

// Warning suppression
#pragma GCC diagnostic push												
#pragma GCC diagnostic ignored "-Wsign-compare"

// Libs
#include <iostream>
#include <chrono> // Medir o tempo de execução do algoritmo 
#include <fstream>				
#include <vector> // vetores 
#include <string>
#include <numeric>
#include <algorithm> // contém funções para pré-processamento de dados 		
#include <cmath>				
#include <math.h>				

// Namespace
using namespace std;
using namespace std::chrono;

// Índice de início dos dados de teste
const int startTest = 900;	

// Mais variáveis de controle
double calcMean(vector<double> vect);
double calcVariance(vector<double> vect);

// Precisei criar uma constante do valor PI (M_PI) pois estava dando erro ao usar a função da biblioteca cmath...
const double pi = 3.14159265358979323846; 

// Métodos auxiliares
void print2DVector(vector < vector <double> > vect);
vector<vector<double> > priorProb(vector < double > vect); // metodo da propriedade a priori da variavel de saída
vector<vector<double> > countclasse(vector<double> vect); // metodo de contagem de classe 

// metodos para variaveis categoricas
vector<vector<double> > likelihoodtipo_doc (vector<double> classe, vector<double> tipo_doc, vector<vector<double> > count_classe);
vector<vector<double> > likelihoodcertificado_valido (vector<double> classe, vector<double> certificado_valido, vector<vector<double> > count_classe);

// metodos para variaveis numericas 
vector<vector<double> > uso_diasMean (vector<double> classe, vector<double> uso_dias, vector<vector<double> > count_classe);
vector<vector<double> > uso_diasVar (vector<double> classe, vector<double> uso_dias, vector<vector<double> > count_classe);
vector<vector<double> > uso_dias_metrics (vector<vector<double> > uso_diasMean, vector<vector<double> > uso_diasVar);
double calc_uso_dias_lh (double v, double mean_v, double var_v);

// Método de implementação do Teorema de Bayes
vector<vector<double> > calc_raw_prob(double tipo_doc, double certificado_valido, double uso_dias, vector<vector<double> > apriori, vector<vector<double> > lh_tipo_doc, vector<vector<double> > lh_certificado_valido, vector<vector<double> > uso_dias_mean, vector<vector<double> > uso_dias_var);

//  Cálculo de métricas de avaliação do modelo
vector<vector<double> > confusionMatrix(vector<double> matA, vector<double> matB);
double accuracy(vector<double> matA, vector<double> matB);

// Número de previsões que serão mostradas
const int numOfIterations = 5;	

// Função main
int main() {

    // Define o nome do arquivo como string
	string fileName = "dados/dataset.csv";

    // Objeto para receber o conteúdo do arquivo		
	ifstream inputFile;					
	
    // Abre o arquivo
	inputFile.open(fileName);	
	
	// Verifica se há algum erro
	if(!inputFile.is_open()) {
		cout << "Falha ao abrir o arquivo." << endl;
		return 0;
	}

    // Declaração das variáveis

    // Variáveis escalares do tipo double para tratar os valores de cada coluna
	double idVal; // atribuimos idVal para a primeira coluna que está vazia no dataset
	double tipo_docVal;
	double classeVal;
	double certificado_validoVal;
	double uso_diasVal;
    
    // Variáveis do tipo vetor para todos os elementos de cada coluna do dataset
	vector<double> id;
	vector<double> tipo_doc;			
	vector<double> classe;		
	vector<double> certificado_valido;		
	vector<double> uso_dias;		
	
    // Variável para armazenar o cabeçalho do arquivo
	string header;		

    // Variável para armazenar cada célula do arquivo csv		
	string cell; 	
	
	// Recupera o cabeçalho para desconsiderar a linha
	getline(inputFile, header);
	
    // Loop de carga e limpeza inicial dos dados
    while(inputFile.good()) {
				
        // Leitura da coluna de id
		getline(inputFile, cell, ','); 			

        // Remove aspas		
		cell.erase(remove(cell.begin(), cell.end(), '\"' ),cell.end());	
	
        // Agora seguimos fazendo a leitura somente das células com valores
		if(!cell.empty()) {								

            // Converte o id de string para double	
			idVal = stod(cell);	

            // Append do valor de x no vetor	
			id.push_back(idVal);					
							
            // Leitura da coluna tipo_doc
			getline(inputFile, cell, ','); 	

            // Conversão de string para double				
			tipo_docVal = stod(cell);	

            // Append no vetor		
			tipo_doc.push_back(tipo_docVal);		
				
            // Leitura da coluna classe	
			getline(inputFile, cell, ',');	

            // Converte para double			
			classeVal = stod(cell);	

            // Append no vetor		
			classe.push_back(classeVal);	
			
            // Leitura da coluna certificado_valido 
			getline(inputFile, cell, ',');		

            // Converte para double		
			certificado_validoVal = stod(cell);	

            // Append no vetor				
			certificado_valido.push_back(certificado_validoVal);				
				
            // Leitura da coluna uso_dias
			getline(inputFile, cell);	

            // Conversão para double			
			uso_diasVal = stod(cell);		

            // Append no vetor				
			uso_dias.push_back(uso_diasVal);					
		}
		else {

            // Se linha vazia, finaliza o loop											
			break;
		}	
	}

	// Inicia a medição do tempo de execução
	auto start = high_resolution_clock::now();  

	cout << "Iniciando a Execução do Algoritmo" << endl << endl;
	
	// Fazemos a divisão dos dados criando os vetores de treinamento

	// Vetor com dados de treino para tipo_doc
	vector<double> tipo_doctrain_data;

	// Carrega o vetor
	for(int i = 0; i < startTest; i++) {
		tipo_doctrain_data.push_back(tipo_doc.at(i));
	}
	
	// Vetor com dados de treino para classe
	vector<double> classetrain_data;

	// Carrega o vetor
	for(int i = 0; i < startTest; i++) {
		classetrain_data.push_back(classe.at(i));
	}
	
	// Vetor com dados de treino para certificado_valido
	vector<double> certificado_validotrain_data;

	// Carrega o vetor
	for(int i = 0; i < startTest; i++) {
		certificado_validotrain_data.push_back(certificado_valido.at(i));
	}	
	
	// Vetor com dados de treino para uso_dias
	vector<double> uso_diastrain_data;
	
	// Carrega o vetor
	for(int i = 0; i < startTest; i++) {
		uso_diastrain_data.push_back(uso_dias.at(i));
	}

	// Fazemos a divisão dos dados criando os vetores de teste

	// Vetor com dados de teste para tipo_doc
	vector<double> tipo_doctest_data;
	
	// Carrega o vetor
	for(int i = startTest; i < id.size(); i++) {
		tipo_doctest_data.push_back(tipo_doc.at(i));
	}
	
	// Vetor com dados de teste para classe
	vector<double> classetest_data;
	
	// Carrega o vetor
	for(int i = startTest; i < id.size(); i++) {
		classetest_data.push_back(classe.at(i));
	}
	
	// Vetor com dados de teste para certificado_valido
	vector<double> certificado_validotest_data;
	
	// Carrega o vetor
	for(int i = startTest; i < id.size(); i++) {
		certificado_validotest_data.push_back(certificado_valido.at(i));
	}	
	
	// Vetor com dados de teste para uso_dias
	vector<double> uso_diastest_data;
	
	// Carrega o vetor
	for(int i = startTest; i < id.size(); i++) {
		uso_diastest_data.push_back(uso_dias.at(i));
	}

	// Começo do Algoritmo Naive Bayes

	cout << "Probabilidade Apriori:" << endl;

	// Probabilidades apriori
	// Matriz 1x2 
	vector<vector<double> > apriori = priorProb(classetrain_data);					
	cout << "Probabilidades Apriori: " << endl;
	print2DVector(apriori);
	cout << endl;
	
	// Vetor com a contagem da variável classe
	// Matriz 1x2 
	vector<vector<double> > count_classe = countclasse(classetrain_data);		
	
	cout << "Probabilidade Condicional:" << endl;
	
	// Likelihood (Probabilidade) para variável tipo_doc
	// Matriz 2x3
	vector<vector<double> > lh_tipo_doc = likelihoodtipo_doc(classetrain_data, tipo_doctrain_data, count_classe); 
	cout << "\ttipo_doc " << endl;
	print2DVector(lh_tipo_doc);
	cout << endl;
	
	// Likelihood (Probabilidade) para variável certificado_valido
	// Matriz 2x2
	vector<vector<double> > lh_certificado_valido = likelihoodcertificado_valido(classetrain_data, certificado_validotrain_data, count_classe); 
	cout << "\tcertificado_valido: " << endl;
	print2DVector(lh_certificado_valido);
	cout << endl;
	
	// Média e variância da variável uso_dias 
	// Matriz 1x2
	vector<vector<double> > uso_dias_mean = uso_diasMean(classetrain_data, uso_diastrain_data, count_classe);	
	vector<vector<double> > uso_dias_var = uso_diasVar(classetrain_data, uso_diastrain_data, count_classe);     

	// Métricas da variável uso_dias 
	cout << "\tuso_dias: " << endl;
	vector<vector<double> > uso_diasMetrics = uso_dias_metrics(uso_dias_mean, uso_dias_var);
	print2DVector(uso_diasMetrics);
	cout << endl << endl;

	// Média da variável uso_dias
	cout << "uso_dias Mean: " << endl;
	print2DVector(uso_dias_mean);
	cout << endl;
	
	// Variância da variável uso_dias
	cout << "uso_dias Variance: " << endl;
	print2DVector(uso_dias_var);
	cout << endl << endl;
	
	// Final do Algoritmo Naive Bayes
	
	auto stop = high_resolution_clock::now();	

	// Vetor para as probabilidades após o treinamento
	// Matriz 1x2 preenchida com zeros 
	vector<vector<double> > raw(1, vector<double> (2, 0)); 
		
	cout << "Prevendo as Probabilidades em Dados de Teste:" << endl;
	
	// Vamos imprimir as 5 primeiras previsões
	for(int i = startTest; i < (startTest + numOfIterations); i++) {	

		// Matriz 1x2 	
		raw = calc_raw_prob(tipo_doc.at(i), certificado_valido.at(i), uso_dias.at(i), apriori, lh_tipo_doc, lh_certificado_valido, uso_dias_mean, uso_dias_var);   
		print2DVector(raw);
	}
	cout << endl << endl;
	
	// Registra a conclusão do algoritmo
	// Biblioteca chrono em c++ serve para medir o tempo de execução de um trecho de código.
	std::chrono::duration<double> elapsed_sec = stop-start; // Calcula a diferença de dentro entre stop e start, onde stop = momento em que a execução do código foi interrompida e start = momento em que a execução do código foi iniciada. A diferença é atribuida a variavel elapsed_sec
	cout << "Tempo de Execução: " << elapsed_sec.count() << endl << endl;	// A função count() é chamada para obter o valor da duração em segundos

	// Normaliza as probabilidades
	vector<double> p1(146); // vetor p1 com 146 elementos, inicializandos com o valor padrão de 0.
	for(int i = 0; i < tipo_doctest_data.size(); i++) { // Esse loop itera sobre os elementos do vetor "tipo_doctest_data"

		// Matriz 1x2
		raw = calc_raw_prob(tipo_doctest_data.at(i), certificado_validotest_data.at(i), uso_diastest_data.at(i), apriori, lh_tipo_doc, lh_certificado_valido, uso_dias_mean, uso_dias_var);   
		if((raw.at(0).at(0)) > 0.5 ) { // verifica se na posição [0][0] da matriz 'raw' é maior do que 0.5. 
			p1.at(i) = 0; // Se for atribui o valor de 0 ao elemento correspondente no vetor 'p1' na posição 'i'
		}
		else if((raw.at(0).at(1)) > 0.5) { //verifica se o valor na posição [0][1] da matriz "raw" é maior que 0.5
			p1.at(i) = 1; // Se for verdadeiro, atribui o valor 1 ao elemento correspondente no vetor "p1" na posição "i".
		}
		else {}
	} // No final do loop o vetor 'p1' conterá valores 0 ou 1.

	// Matriz de Confusão
	cout << "Confusion Matrix: " << endl;
	vector<vector<double> > table = confusionMatrix(p1, classetest_data);
	print2DVector(table); 
	cout << endl;
	
	double acc = accuracy(p1, classetest_data);
	cout << "Accuracy: " << acc << endl;
	
	// Sensitivity = TP / (TP + FN)
	double sensitivity = (table.at(0).at(0) / ( table.at(0).at(0) + table.at(1).at(0)));
	cout << "Sensitivity: " << sensitivity << endl;
	
	// Specificity = TN / (TN + FP)
	double specificity = (table.at(1).at(1) / ( table.at(1).at(1) + table.at(0).at(1)));
	cout << "Specificity: " << specificity << endl << endl;
	
	return 0;


} // Final da Função Main (ava, jura?)

// Método para imprimir o vetor
void print2DVector(vector<vector<double> > vect) {
	for(int i = 0; i < vect.size(); i++) {
		for(int j = 0; j < vect[i].size(); j++) {
			cout << vect[i][j] << " ";
		}
		cout << endl;
	}
}

// Calcula as probabilidade apriori nos dados de treino
vector<vector<double> > priorProb(vector<double> vect) {

	// Matriz 1x2
	vector<vector<double> > prior(1, vector<double> (2, 0)); 
	
	for(int i = 0; i < vect.size(); i++) {
		if(vect.at(i) == 0) {
			prior.at(0).at(0)++;
		}
		else {
			prior.at(0).at(1)++;
		}
	}

	prior.at(0).at(0) = prior.at(0).at(0) / vect.size();
	prior.at(0).at(1) = prior.at(0).at(1) / vect.size();
		
	return prior;
}

// Calcula a contagem de classe (será usada para calcular as probabilidades das variáveis de entrada)
vector<vector<double> > countclasse(vector<double> vect) {

	// Matriz 1x2 = 1 linha e 2 colunas preenchidas com 0 
	vector<vector<double> > prior(1, vector<double> (2, 0)); // 1° elemento é usado para contar quantos são iguais a 0, 2° elemento da matriz é usado para calcular quantos são diferentes de 0
	
	for(int i = 0; i < vect.size(); i++) { // percorre todos elementos do vect 
		if(vect.at(i) == 0) { // verifica se é = 0
			prior.at(0).at(0)++; // se for, a contagem na primeira coluna de prior é incrementada em 1
		}
		else {
			prior.at(0).at(1)++; // caso contrário, a contagem na segunda coluna é incrementada em 1
		}
	}
	return prior;
}

// Calcula a probabilidade da variável tipo_doc nos dados de treino
vector<vector<double> > likelihoodtipo_doc (vector<double> classe, vector<double> tipo_doc, vector<vector<double> > count_classe) {
	
	// Variável 2x3
	vector<vector<double> > lh_tipo_doc (2, vector<double>(3,0)); 				
	
	for(int i = 0; i < classe.size(); i++) {
		if(classe.at(i) == 0) {
			if(tipo_doc.at(i) == 1) {
				lh_tipo_doc.at(0).at(0)++;
			}
			else if(tipo_doc.at(i) == 2) {
				lh_tipo_doc.at(0).at(1)++;
			}
			else if (tipo_doc.at(i) == 3) {
				lh_tipo_doc.at(0).at(2)++;
			}
			else {}
		}
		else if(classe.at(i) == 1) {
			if(tipo_doc.at(i) == 1) {
				lh_tipo_doc.at(1).at(0)++;
			}
			else if(tipo_doc.at(i) == 2) {
				lh_tipo_doc.at(1).at(1)++;
			}
			else if (tipo_doc.at(i) == 3) {
				lh_tipo_doc.at(1).at(2)++;
			}
			else {}
		}
		else{}
	}
	
	for(int i = 0; i < lh_tipo_doc.size(); i++) {
		for(int j = 0; j < lh_tipo_doc[i].size(); j++) {
			if(i == 0) {
				lh_tipo_doc.at(i).at(j) = lh_tipo_doc.at(i).at(j) / count_classe.at(0).at(0);
			}
			else if(i == 1) {
				lh_tipo_doc.at(i).at(j) = lh_tipo_doc.at(i).at(j) / count_classe.at(0).at(1);
			}
		}
	}
	
	return lh_tipo_doc;
}

// Calcula a probabilidade da variável certificado_valido nos dados de treino
// Calcula a probabilidade condicional de um certificado ser valido dada a classe de um objeto 
vector<vector<double> > likelihoodcertificado_valido (vector<double> classe, vector<double> certificado_valido, vector<vector<double> > count_classe) {
	
	// Variável 2x2
	vector<vector<double> > lh_certificado_valido (2, vector<double>(2,0)); // criando um vetor 2x2 preenchida com zeros 	
	
	for(int i = 0; i < classe.size(); i++) {
		if(classe.at(i) == 0) {
			if(certificado_valido.at(i) == 0) {
				lh_certificado_valido.at(0).at(0)++;
			}
			else if(certificado_valido.at(i) == 1) {
				lh_certificado_valido.at(0).at(1)++;
			}
			else {}
		}
		else if(classe.at(i) == 1) {
			if(certificado_valido.at(i) == 0) {
				lh_certificado_valido.at(1).at(0)++;
			}
			else if(certificado_valido.at(i) == 1) {
				lh_certificado_valido.at(1).at(1)++;
			}
			else {}
		}
		else{}
	}

	for(int i = 0; i < lh_certificado_valido.size(); i++) {
		for(int j = 0; j < lh_certificado_valido[i].size(); j++) {
			if(i == 0) {
				lh_certificado_valido.at(i).at(j) = lh_certificado_valido.at(i).at(j) / count_classe.at(0).at(0);
			}
			else if(i == 1) {
				lh_certificado_valido.at(i).at(j) = lh_certificado_valido.at(i).at(j) / count_classe.at(0).at(1);
			}
		}
	}
	
	return lh_certificado_valido;
}

// Calcula a média da variável uso_dias nos dados de treino
vector<vector<double> > uso_diasMean (vector<double> classe, vector<double> uso_dias, vector<vector<double> > count_classe) {

	// Matriz 1x2
	vector<vector<double> > mean(1, vector<double> (2, 0));

	// contabilizar a ocorrencia de valores com base na variável de saída 	
	for(int i = 0; i < classe.size(); i++) {
		if(classe.at(i) == 0) {
			mean.at(0).at(0) += uso_dias.at(i);
		}
		else if(classe.at(i) == 1) {
			mean.at(0).at(1) += uso_dias.at(i);
		}
		else{}
	}
	// calculo da média
	for(int i = 0; i < mean.size(); i++) {
		for(int j = 0; j < mean[i].size(); j++) {
			if(j == 0) {
				mean.at(i).at(j) = mean.at(i).at(j) / count_classe.at(0).at(0);
			}
			else if(j == 1) {
				mean.at(i).at(j) = mean.at(i).at(j) / count_classe.at(0).at(1);
			}
		}
	}
	return mean;
}

// Calcula a variância da variável uso_dias nos dados de treino
vector<vector<double> > uso_diasVar (vector<double> classe, vector<double> uso_dias, vector<vector<double> > count_classe) {

	// Matriz 1x2
	vector<vector<double> > var(1, vector<double> (2, 0)); // essa matriz será usada para armazenar os valor de variância calculados

	// Matriz 1x2
	vector<vector<double> > mean = uso_diasMean(classe, uso_dias, count_classe); // calcular a matriz de média 'mean' para cada classe 

	// percorrer todos os elementos da amostra e para cada elemento calcular a variancia de cada um deles
   // a variancia para cada classe é armazenada na matriz var.  
	for(int i = 0; i < classe.size(); i++) {
		if(classe.at(i) == 0) {
			var.at(0).at(0) += pow( ( uso_dias.at(i) - mean.at(0).at(0) ), 2);
		}
		else if(classe.at(i) == 1) {
			var.at(0).at(1) += pow( ( uso_dias.at(i) - mean.at(0).at(1) ), 2);
		}
		else{}
	}

	// percorrer a matriz var e dividir cada elemento pelo numero de graus de liberdade para obter a variancia média para cada classe
   // O numero de graus de liberdade é igual ao número de elementos da classe menos 1
	for(int i = 0; i < var.size(); i++) {
		for(int j = 0; j < var[i].size(); j++) {
			if(j == 0) {
				var.at(i).at(j) = var.at(i).at(j) / ( count_classe.at(0).at(0) - 1 ) ;
			}
			else if (j == 1) {
				var.at(i).at(j) = var.at(i).at(j) / ( count_classe.at(0).at(1) - 1 ) ;
			}
			else {}
		}
	}	
	return var;
}

// Formata as métricas (média e variância) da variável uso_dias em uma matriz 2x2
vector<vector<double> > uso_dias_metrics (vector<vector<double> > uso_diasMean, vector<vector<double> > uso_diasVar) {

	// Matriz 2x2
	vector<vector<double> > metrics(2, vector<double>(2, 0));  // cria uma matriz 2x2 preenchida com zeros 
	
	metrics.at(0).at(0) = uso_diasMean.at(0).at(0); // é definido como o valor da matriz "uso_diasMean" na posição (0,0)
	metrics.at(0).at(1) = sqrt(uso_diasVar.at(0).at(0)); // é definido como a raiz quadrada do valor da matriz "uso_diasVar" na posição (0,0)
	metrics.at(1).at(0) = uso_diasMean.at(0).at(1); // é definido como o valor da matriz "uso_diasMean" na posição (0,1)
	metrics.at(1).at(1) = sqrt(uso_diasVar.at(0).at(1)); //  é definido como a raiz quadrada do valor da matriz "uso_diasVar" na posição (0,1).
	
	return metrics;
}

// Calcula a probabilidade da variável uso_dias
double calc_uso_dias_lh (double v, double mean_v, double var_v) {
	double uso_dias_lh = 0;
	
	// Calcula a probabilidade
	uso_dias_lh = (1 / (sqrt(2 * pi * var_v))) * exp( -(pow((v - mean_v), 2)) / (2 * var_v)); // constante de PI declarado no inicio do código. 
	
	return uso_dias_lh;
}

// Implementando a fórmula do Teorema de Bayes
vector<vector<double> > calc_raw_prob(double tipo_doc, double certificado_valido, double uso_dias, vector<vector<double> > apriori, vector<vector<double> > lh_tipo_doc, vector<vector<double> > lh_certificado_valido, vector<vector<double> > uso_dias_mean, vector<vector<double> > uso_dias_var) {
	
	// Matriz 1x2
	vector<vector<double> > raw(1, vector<double> (2, 0)); 
	
	// Probabilidade da variável de saída
	double num_s = lh_tipo_doc.at(1).at(tipo_doc-1) * lh_certificado_valido.at(1).at(certificado_valido) * apriori.at(0).at(1) *
					calc_uso_dias_lh(uso_dias, uso_dias_mean.at(0).at(1), uso_dias_var.at(0).at(1));	
	// num_s é calculada usando as probabilidades de 'tipo_doc', 'certificado_valido', 'apriori' e 'uso_dias' para a variavel de saídas igual a 1	
					
	// Probabilidade das variáveis de entrada
	double num_p = lh_tipo_doc.at(0).at(tipo_doc-1) * lh_certificado_valido.at(0).at(certificado_valido) * apriori.at(0).at(0) *
					calc_uso_dias_lh(uso_dias, uso_dias_mean.at(0).at(0), uso_dias_var.at(0).at(0));	
	// num_p = mesma coisa que num_s porém com a variavel de saída igual a 0			
		
	// Denominador
	double denominator = lh_tipo_doc.at(1).at(tipo_doc-1) * lh_certificado_valido.at(1).at(certificado_valido) *
					calc_uso_dias_lh(uso_dias, uso_dias_mean.at(0).at(1), uso_dias_var.at(0).at(1)) * apriori.at(0).at(1)
					+ lh_tipo_doc.at(0).at(tipo_doc-1) * lh_certificado_valido.at(0).at(certificado_valido) * 
					calc_uso_dias_lh(uso_dias, uso_dias_mean.at(0).at(0), uso_dias_var.at(0).at(0)) * apriori.at(0).at(0);
	// O denominador é calculado usando as mesmas probabilidades, mas sem especificar a variavel de saída
		
	raw.at(0).at(1) = num_s / denominator; // 0
	raw.at(0).at(0) = num_p / denominator; // 1
	
	return raw; // raw = matriz 1x2 
}

// Retorna: TP, FP, FN, TN 
vector<vector<double> > confusionMatrix(vector<double> matA, vector<double> matB) {

	// Matriz 2x2
	vector<vector<double> > table(2, vector<double>(2, 0));  
	
	// matA = predicted
	// matB = test$classe
	
	/*		TP FP
	 * 		FN TN		*/ 
	
	for(int i = 0; i < matA.size(); i++) {

			// true negative
			if( matA.at(i) == 0 && matB.at(i) == 0 ) {			
				table.at(0).at(0)++;
			}

			// true positive
			else if( matA.at(i) == 1 && matB.at(i) == 1 ) {		
				table.at(1).at(1)++;
			}

			// false positive
			else if( matA.at(i) == 1 && matB.at(i) == 0 ) {	
				table.at(1).at(0)++;
			}

			// false negative
			else if( matA.at(i) == 0 && matB.at(i) == 1 ) {		
				table.at(0).at(1)++;
			}
			else {}
		}	
	return table;
}

// Retorna a acurácia
double accuracy(vector<double> matA, vector<double> matB) {
	int matARow = matA.size();
	int matBRow = matB.size();	
	
	if((matARow != matBRow)) {
		cout << "Erro ao calcular a acurácia. As dimensões precisam ser iguais." << endl;
	}
		
	double sum = 0;
	
	for(int i = 0; i < matA.size(); i++) {
		if(matA.at(i) == matB.at(i)) {
				sum++;
		}

	}	
	return sum / matA.size();
}

#pragma GCC diagnostic pop

