// Previsão doo Custo de Vida em Áreas Urbanas com Regressão Linear em C++

#include<stdc++.h>
#include<iostream>
#include<fstream>
#include<sstream>
#include<string>
#include<numeric>
#include<vector> 


using namespace std; 


// Classe para carga e manipulação de dados 
class CarregaCsv { 

    protected: 

        // Vetores 
        vector<pair<string,vector<float> > >data; // Nesse código, estamos declarando uma variável chamada data como um vetor de pares (pair) de tipos <string, vector<float>>.
        vector <float> x, y, yp, y_gra, x_test, y_test; 

        // Nome do arquivo csv
        string file_name; 

        // demais variaveis 
        float x_factor, y_factor; 
        int n;

    public: 

        // Método para leitura dos dados 
        void read(string file){ // função read é definida com um parametro file, que é uma string contendo o nome do arquivo a ser lido
            file_name = file; // var atualizada com o valor do parâmetro file
            cout << "Arquivo csv: " << file_name << endl; // mostrando o nome do arquivo csv

            // Stream 
            fstream fo; // declarando uma variavel do tipo fstream

            // abre o arquivo 
            fo.open(file_name.c_str(), ios::in); // abre o arquivo especificado pelo nome contido em file_name no modo leitura

            // Variaveis de controle 
            string line, colname; // declaração de variaveis 

            // Leitura do arquivo 
            if(fo.good()) { // verifica se o arquivo foi aberto corretamente, se foi executará esse código:

                // Obtem uma linha 
                getline(fo, line); // Lê a primeira linha do arquivo e o resultado é armazenado na var 'line'

                // Stream de string 
                stringstream s(line); // É usado para analisar a primeira linha e criar uma lista de nomes de colunas

                // Obtém os nomes das colunas 
                while(getline(s, colname, ',')){                            
                    data.push_back({colname, vector<float> {}});
                } // Aqui, um objeto stringstream é usado para processar a linha lida anteriormente. 
                  // O laço while é executado para extrair os nomes das colunas separados por vírgulas. 
                  // Cada nome de coluna é armazenado em um par de valores, onde o primeiro valor é o nome da coluna e o segundo valor é um vetor vazio. 
                  // Esse par é adicionado ao vetor data.

                // variavel de controle 
                int temp1; // declaração da variavel 

                // Leitura de cada linha de dados 
                while(getline(fo, line)){ // Cada linha de dados subsequente é lida usando a função 'getline' e é armazenado na var 'line'
                    // Indice da coluna 
                    int col = 0; 

                    // stream (Esse objeto stringstream será usado para extrair os valores individuais da linha.)
                    stringstream s1(line); 

                    // Loop para leitura das linhas (extrair cada valor numérico da linha de dados)
                    while(s1 >> temp1) { //extrai um valor stringstream s1 e o armazena em temp1
                        // carrega o vetor data 
                        data.at(col).second.push_back(temp1); // O valor numérico extraido é adicionado ao vetor correspondente
                        // data.at(col) acessa o par (coluna) de índice col no vetor data. Nesse caso, col inicial é igual a 0,
                        // data.at(col).second acessa o segundo elemento desse par, que é o vetor de floats associado à coluna. Inicialmente esse vetor está vazio

                        col++; // indice col é incrementado

                        if(s1.peek() == ','){ // se o proximo caractere no objeto stringstream s1 for uma virgula
                            s1.ignore(); // ele é ignorado 
                        }
                    }
                }

                // A primeira variavel (coluna) e´x , técnicamente armazena o segundo elemento do primeiro indice (Salario_Ano_Anterior) na variavel x e fds
                for(int i = 0; i < data[0].second.size(); i++){ // irá armazenar até o i ser congruente com o .size(), ou seja, todos os valores serão armazenados
                    x.push_back(data[0].second.at(i));
                }

                // A segunda variavel (coluna) é y, faz a mesma bosta que o código anterior porém pega o segundo elemento do SEGUNDO indice (Salario_Ano_Atual) e armazena em y
                for(int i = 0; i < data[1].second.size(); i++){ 
                    y.push_back(data[1].second.at(i));
                }

            }

            else { // caso o arquivo for BÃO não ele dará um erro ( ͡° ͜ʖ ͡°)
                cout << "Erro no arquivo"; 
            }

            // fecha o stream do arquivo ( ͡° ͜ʖ ͡°)
            fo.close();

            // chama os demais métodos 
            check(); 
            data_normalization(); 
            split_data(); 

        }

        // Método para checar os dados 
        void check(){ 

            int i = 0;
			
            // Verifica se x foi carregado corretamente (comparando)
            while(i < data[0].second.size())
            {
				if(x.at(i) != data[0].second.at(i))
                {
					cout << "Erro em x" << endl;
					break;
				}
			
            i++;
			
            }
			
			i = 0;
			
            // Verifica se y foi carregado corretamente (comparando)
			while(i < data[1].second.size()) {
				if(y.at(i) != data[1].second.at(i))
                {
					cout << "Erro em y"  << endl;
					break;
				}
			
            i++;
			
            }
        }

        // Método de normalização dos dados
        // Porque normalizar? Para ajustar os valores de x e y para uma escala comparável. 
        void data_normalization(int normal_level = 7, int type = 0) //normal_level = especifica o nivel de normalização // type especifica o tipo
        {
            // Variáveis de controle
    	    long double x_sum = 0, y_sum = 0; // Serão usadas para armazenar a soma dos valores de x e y
    	
            // Loop pelos valores de x
    	    for(int i = 0; i < x.size();i++) 
    	    { 
    		    x_sum += x[i]; // x_sum armazena a soma total dos valores de x
    		    y_sum += y[i]; // y_sum armazena a soma total dos valores de y
		    }
		
            // Cria fatores para a normalização
		    x_factor = x_sum / (normal_level * x.size()); // O fator de Normalização é calculado dividindo a soma total dos valor de x(x_sum) pelo produto do nivel de normalização(normal_level) e o tamanho do vetor de x(x_.size())
		    y_factor = y_sum / (normal_level * y.size()); // mesma bosta so que para o y


		
            // Implementando a famosa dona Normalização ( ಠ ͜ʖಠ)
		    for(int i = 0; i < x.size();i++)
    	    {
    		    x[i] = x[i] / x_factor; // divide cada elemento x[i] pelo fator de normalização x_factor 
    		    y[i] = y[i] / y_factor; // Mema bosta só que para y
                // Dessa forma, o loop percorre totodos os elementos nos vetores x e ye normaliza cada valor dividindo-o pelo seu respectivo fator de normalização
		    }
		
	    }

        // Método para divisão dos dados em treino e teste
        void split_data(int debug = 0) // parametro debug é opcional, serve para exibir informações adicionais durante a execução (0 = não exibe, para exibir altere para 1)
        {
            // Fator de split (70/30)
    	    int total = 0.3 * x.size(); // calcula o tamanho do dataset como 30% do tamanho total dos dados
    	    srand(time(0)); //semente de gerador de numeros aleatorios, garante que a sequencia de numeros aleatórios gerada seja diferente a cada execução do programa rsrs
    	
    	    if(debug) // se o parametro for verdadeiro executa a parada ai, como padrão ele é falso então nem irá executar sabosta ( ͠° ͟ʖ ͡°)
    	    {
    		    cout << "Índices" << endl;
		    }
    	
            for (int i = 0; i < total; i++)
    	    {
                // Tamanho de x e y
    		    int x_mod = x.size(); // tamanho de x
	    	    int y_mod = y.size(); // tamanho de y
	    	 
                // Índice de separação dos dados
	    	    int temp = rand(); // Um numero aleatório é gerado 
	    	    int x_index = temp % x_mod; // Os indices de separação são gerados através do resto da divisão do numero aleatório pelo tamanho do vetor x
	    	    int y_index = temp % y_mod; // msm coisa só que com vetor y 
                // esses indices indicam quais elementos dos vetores serão selecionados para o conjunto de teste
	    	    
                if(debug) // verifica o modo de depuração rs
			    {
				    cout << x_index << endl;
			    }

                // Carrega os dados de teste
	    	    x_test.push_back(x[x_index]); // Os elementos selecionados a partir do x_index são adicionados ao vetor x_test || o elemento que será adicionado é o x[x_index] ou seja, o elemento de x correspondente ao indice x_index
			    y_test.push_back(y[y_index]); // Os elementos selecionador a partir do y_index são adicionados ao vetor y_test
                // No final do processo, os vetores x_test e y_test conerão os elementos selecionados de x e y que formam o conjunto de teste para o modelo. 

                // Remove os dados de teste dos dados originais que então serão usados para treino
			    x.erase(x.begin() + x_index); //erase() = remover um elemento de um vetor, begin() aponta para o primeiro elemento, begin() + x_ index desloca o iterador para a posição do elemento que deve ser removido com abse no valor de x_index
			    y.erase(y.begin() + y_index); // msm coisa só que com y
                // se usassemos x.erase(x[x_index]) estariamos passando diretamente o valor do elemento para a função. Isso não é correto e resultaria em um erro de compilação, pois a função erase() espera receber um iterador, não um valor.
		    }
		
            if(debug) // debug novamente 
		    {
			    cout << "X_test" << endl;
		    }
		
		    if(debug) // again 
		    {
			    for (int i = 0; i < x_test.size(); i++)
	    	    {
	    		    cout << x_test[i] << endl;
	    	    }
		    }
    	
    	    cout << endl; // linha em branco padrão 
            // Resumo da função: 
            // O método divide os dados em conjunto de treinamento e teste (70/30) de forma aleatória e removendo os dados de teste dos dados originais.
            // o parametro debug é usado para ativar a impressão de informaçoes adicionais
	    }

        // Método para definir o tamanho da amostra de dados de entrada
        void set_size()
        {
            n = x.size();
        }
};

// Classe para o algoritmo de regressão linear 
class LinearRegression:public CarregaCsv
{ 
    protected: 
        float m, c, xbar, ybar; 
        float slope, intercept; 

    public: 

        // Construtor 
        LinearRegression(){ 
            m = 0; 
            c = 0; 
            xbar = 0; 
            ybar = 0;
        }

        // Método para os cálculos da Regressão Linear 
        void calculate()
    	{
            // Variáveis
       		float z, q, s = 0, d = 0;
	   		float siz = y.size(); // siz é inicializada com o tamanho do vetor y
       		
            // Cálculo acumulado
            xbar = accumulate(x.begin(), x.end(),0) / siz; // accumulate é utilizado para somar todos os elementos do vetor x, começando do primeiro elemento(x.begin()) até o ultimo (x.end()), 0 é o valor inicial da soma, ou seja, começa a soma a partir de zero. Em seguida é dividido pelo tamanho do vetor siz para calcular a média dos elementos do vetor x
       		ybar = accumulate(y.begin(), y.end(),0) / siz; // mesma coisa só que pra Y 
       		
            // Loop de cálculo
			for(int i = 0;i < n;i++) // n = numero de elementos nos vetores x e y
       		{
           		z = (x[i] - xbar); // calcula a diferença entre o valor atual de x[i] e a média xbar
           		q = (y[i] - ybar); // calcula a diferença entre o valor atual de y[i] e a média ybar
           		s = s + (z * q); // atualiza a variavel 's' somando o produto entre z e q ao valor acumulado anteriormente. ps: Essa variavel é usada para armazenar o numerador do calculo do coeficiente angular (m) da regressão linear.
           		d = d + z * z; // atualiza a variavrel d somando o quadrado de x ao valor acumulado anteriormente. Essa variável d é usada para armazenar o denominador do cálculo do coeficiente angular (m) da regressão linear.
                // Essencialmente, esse loop percorre os elementos dos vetores "x" e "y" e calcula os valores intermediários necessários para o cálculo dos coeficientes da regressão linear, como o numerador e o denominador
       		}
       
            // Calcula os coeficientes
        	m = (s/d); // ao dividir s por d, obtemos o coeficiente angular (slope) da regressão linear. 
        	c = ybar - m * xbar; // ybar = media do valores no vetor y; m = slope; xbar = media dos valores no vetor x; juntos realizanm o cálculo do coeficiente linear(intercept)
            // Essencialmente, essas linhas de código concluem o cálculo dos coeficientes da regressão linear. 
            // O coeficiente angular (m) representa a inclinação da reta de regressão, 
            // enquanto o coeficiente linear (c) representa o ponto de interseção da reta com o eixo y.
            // Esses coeficientes são essenciais para descrever a relação linear entre as variáveis "x" e "y" na regressão linear.
    	}

        // Método para mostrar os resultados 
        void show() 
        {
            cout << "Slope da Linha da Regressão: " << m << endl; 
            cout << "Intercept da Linha de Regressão: "<< c << endl; 
        }

        // Método para a previsão do modelo 
        void predicted()
    	{
        	int i;
        	
            for(i = 0;i < y_test.size();i++) // itera sobre as amostras de teste usando a função size para obter o tamanho do vetor y_test
    		{ // O loop continuará enquanto i  for menor que o tamanho do vector 
          		float z; // será usada para armazenar o resultado da predição para cada amostra de teste
                
                // Fórmula de regressão
          		z = m * x_test[i] + c; // calcula a predição para a amostra de teste atual

                // Armazena os resultados
          		yp.push_back(z); // armazena o valor predito 'z' no vetor 'yp'
       		}

            // Essa função predicted é responsável por calcular as predições para 
            // as amostras de teste com base nos coeficientes da regressão linear previamente calculados.
    	}

        // Algoritmo de aprendizado
        void gradient_descent()
    	{	
			// Array para armazenar os valores dos erros
        	vector<float>error;            
        	float err = 0;

			// Inicializa b0
        	float b0 = 0;     

			// Inicializa b1              
        	float b1 = 0;          

			// Inicializa a taxa de aprendizagem       
        	float alpha = 0.00019;       

            // Total de loops de treinamento      
        	int max_learning_loops = 4500;
	    
	    	for (int j = 0; j < max_learning_loops; j++)
	    	{
	    		for (int i = 0; i < n; i ++)
		    	{
                    // Fórmula de regressão
			    	float p = b0 + b1 * x[i];

                    // Erro da previsão
			    	err += (p - y[i]) * (p - y[i]);
		    	}
		    
                // Armazena o erro
		    	error.push_back(err);
			
                // Altera o valor do coeficiente
		    	b0 = b0 - (alpha * err);

                // A partir daqui faremos um ajuste fino

                // Para armazenar o erro
		    	float err1 = 0;
			
                // Nova rodada de treinamento
				for (int i = 0; i < n; i ++)
		    	{
                    // Fórmula de regressão
			    	float p = b0 + b1 * x[i];

                    // Erro da previsão
			    	err1 += (p - y[i]) * (p - y[i]);
		    	}
		    
                // Altera o valor do coeficiente
		    	b0 = b0 + (2 * alpha * err);

                // Para armazenar o erro
		    	float err2 = 0;
		    
                // Nova rodada de treinamento
		    	for (int i = 0; i < n; i ++)
		    	{
                    // Fórmula de regressão
			    	float p = b0 + b1 * x[i];

                    // Erro da previsão
			    	err2 += (p - y[i])* (p - y[i]);
		    	}
		    
                // Verifica os erros e aumenta a taxa de aprendizado
		    	if(err1 < err2)
		    	{
		    		b0 = b0 - (2 * alpha * err);
				}
			
                // Atualiza b1
				b1 = b1 - (alpha * err);

                // Armazena o erro
		    	err1 = 0;
		    
                // Nova rodada de treinamento
		    	for (int i = 0; i < n; i ++)
		    	{
                    // Fórmula de regressão
			    	float p = b0 + b1 * x[i];

                    // Erro da previsão
			    	err1 += (p - y[i]) * (p - y[i]);
		    	}
		    
                // Atualiza b1
		    	b1 = b1 + (2 * alpha * err);

                // Armazena o erro
		    	err2 = 0;
		    
                // Nova rodada de treinamento
		    	for (int i = 0; i < n; i ++)
		    	{
                    // Fórmula de regressão
			    	float p = b0 + b1 * x[i];

                    // Erro da previsão
			    	err2 += (p - y[i]) * (p - y[i]);
		    	}
		    
                // Verifica os erros e aumenta a taxa de aprendizado
		    	if(err1 < err2)
		    	{
		    		b1 = b1 - (2 * alpha * err);
				}
			
				err = 0;
			
			}
		
	    	cout << "Valores Finais do Gradiente Descente: " << "c (intercepto) = " << b0 << " " << "m (slope) = " << b1 << " " << endl;
	    	slope = b1;
	    	intercept = b0;
    	}

        // Previsão do gradiente
    	void predict_gradient()
    	{
        	for(int i = 0;i < x_test.size();i++)
        	{
            	y_gra.push_back((x_test[i] * slope) + intercept);
        	}
    	}

};

// Classe para avaliação do modelo
class Acuracia:public LinearRegression
{
	protected:
    
		float r2f, r2g, r;

	public:

        // Construtor
    	Acuracia()
    	{
            r = 0;
        	r2f = 0;
        	r2g = 0;
    	}
    
        // Calcula a correlação
		void correlation()
    	{
       		float z, q, s = 0, d = 0, siz = n, b = 0, sq;

            // Calcula variância e co-variância
       		for(int i = 0;i < y.size();i++)
       		{
           		z = (x[i] - xbar);
           		q = (y[i] - ybar);
           		s = s + (z * q);
           		d = d + z * z;
           		b += (q * q);
       		}
       
	   		sq = sqrtf(d * b);

            // Calcula o coeficiente de correlação
       		r = (s / sq);

       		cout << "Correlação = " << r << endl;
       
	   		if(r > -0.5 && r < -1.0)
       		{
           		cout << "Forte Relacionamento Negativo." << endl;
           		cout << "Slope é negativo'." << endl;
       		}
       		else if(r >= -0.5 && r <= 0.5)
       		{
           		cout << "Não há relacionamento entre as variáveis." << endl;
           		cout << "Esses dados não devem ser usados para Regressão Linear." << endl;
       		}
       		else if(r > 0.5 && r <= 1.0)
       		{
           		cout << "Forte Relacionamento Positivo." << endl;
           		cout << "Slope é positivo." << endl;
       		}
       		
			cout << endl;
    	}

        // Coeficiente R2 do modelo
    	void rsquare_for_formula()
    	{
    		float ytbar = accumulate(y_test.begin(), y_test.end(),0) / y_test.size();
        	float z = 0, s = 0;

        	for(int i = 0;i < y_test.size();i++)
        	{
              	z += ((yp[i] - y_test[i]) * (yp[i] - y_test[i]));
              	s += ((y[i] - ytbar) * (y[i] - ytbar));
        	}
        	
            r2f = 1 - (z / s);
        	
            cout << "Coeficiente R2 do Modelo:" << r2f << endl;
    	}
    
        // Coeficiente R2 do gradiente
		void rsquare_for_gradient()
    	{
        	float numg, deng = 0;
        	float y_t_bar = accumulate(y_test.begin(), y_test.end(),0) / y_test.size();
        	
            for(int i = 0; i < x_test.size(); i++)
        	{
            	float temp = (x_test[i] * slope) + intercept;
            	numg += ((temp - y_test[i]) * (temp - y_test[i]));
            	deng += ((y_test[i] - y_t_bar) * (y_test[i] - y_t_bar));
        	}
        	cout << "Coeficiente R2 do Gradiente: " << 1 - (numg/deng) << endl << endl;
    	}
};

// Bloco main
int main()
{
    Acuracia acc;
    acc.read("dados/dataset.csv");
    acc.set_size();
    acc.gradient_descent();
    acc.predict_gradient();
    acc.rsquare_for_gradient();
    acc.correlation();
    acc.calculate();
    acc.show();
    acc.predicted();
    acc.rsquare_for_formula();
}

// Fim
