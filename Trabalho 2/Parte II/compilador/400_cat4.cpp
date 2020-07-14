#include <iostream>
#include <string>
#include <ostream>
#include <cassert>
#include <stack>
#include <vector>
#include <algorithm>

using namespace std;

//Excessao usada pela classe MyVec
class MyVecException {
private:
	std::string msg;
public:
	MyVecException (const std::string &msg0): msg(msg0) { }
	const std::string & what() const { return msg; }
};


template<class T>
class MyVec {
public:
	//decidimos utilizar o typedef com o objetivo de "abstrair" o conceito de iterador (a utilidade disso sera vista posteriormente)
	typedef T * iterator; //define um iterador, um tipo utilizado para percorrer uma estrutura de dados e "apontar" para os items armazenados nela

	template<class T2>
	friend std::ostream& operator<<(std::ostream &, const MyVec<T2> &);

	//construtores/destrutures/etc
	MyVec();
	MyVec(int n, const T&init=T());

	//destrutor
	~MyVec() { destroy(); };

	//Construtor de copia
	MyVec(const MyVec &);
	MyVec & operator=(const MyVec &);

	//funcoes de acesso
	T &operator[](int pos) {return data[pos];}
	const T &operator[](int pos) const {return data[pos];}
	void push_back(const T&);
	void pop_back() { assert(dataSize!=0); dataSize--; }
	void resize(int newSize);
	void insert(const T&,int pos);
	void clear();

	bool empty() const {return size() == 0;};
	int size() const {return dataSize;};

	iterator begin() {return data;} //Exercicio: e se tivermos uma lista constante, como itera-la para, por exemplo, imprimir os elementos?
	iterator end() {return data+dataSize;} //retorna um apontador para um elemento que estaria APOS o final da lista

	//versoes constantes dos 4 metodos acima
	const iterator begin() const {return data;} //Exercicio: e se tivermos uma lista constante, como itera-la para, por exemplo, imprimir os elementos?
	const iterator end() const {return data+dataSize;} //retorna um apontador para um elemento que estaria APOS o final da lista
	
private:
	T *data;
	int dataSize; //quantos elementos ha na lista?
	int dataCapacity; //quantos elementos atualmente cabem na lista? 

	void create();
	void destroy();
	void resizeCapacity(int newCapacity);
};

template<class T>
void MyVec<T>::push_back(const T&elem) {
	if(dataSize + 1 >= dataCapacity) { //preciso redimensionar?
		if(dataCapacity ==0) //Por que e' preciso testar isso?
			resizeCapacity(1);
		else
			resizeCapacity(dataCapacity*2);
	}
	data[dataSize] = elem;
	dataSize++;
}

template<class T>
void MyVec<T>::resize(int newSize) {
	if(newSize >= dataCapacity) { //primeiro temos que realocar o vector...
		resizeCapacity(newSize); //para o resize vamos usar o tamanho exato caso seja necessario realocar o vector..
	}
	for(int i=dataSize;i<newSize;i++) { //preencha a parte "extra" do vector com valores "em branco"
		data[i] = T();
	}
	dataSize = newSize;
}

//insere o elemento na posicao pos e move os elementos ja presentes no vetor
// pos pode ser:
//   uma posicao apos o final do vetor (para inserir na ultima posicao) 
//   ou uma posicao ja existente (nesse caso os elementos nessa posicao e apos ela serao movidos "para a frente")
template<class T>
void MyVec<T>::insert(const T&elem,int pos) {
	if(pos > dataSize || pos < 0) {
		throw MyVecException("Erro na funcao insert: posicao invalida");
	}

	if(dataSize + 1 >= dataCapacity) { //preciso redimensionar?
		if(dataCapacity ==0) //Exercicio: Por que e' preciso testar isso?
			resizeCapacity(1);
		else
			resizeCapacity(dataCapacity*2);
	}	

	for(int i=dataSize;i>pos;i--)
		data[i] = data[i-1];
	data[pos] = elem;
	dataSize++;
}


template<class T>
void MyVec<T>::clear() {
	destroy();
	create();
}

template<class T>
void MyVec<T>::resizeCapacity(int newCapacity) {
	if(newCapacity<=dataCapacity)
		return; //a principio, nunca vamos reduzir o tamanho do vetor

	T* oldData = data;
	data = new T[newCapacity];

	for(int i=0;i<dataSize;i++)
		data[i] = oldData[i];

	delete []oldData;

	dataCapacity = newCapacity;
}

template<class T>
void MyVec<T>::destroy() {
	delete []data;
	dataSize = dataCapacity = 0;
}

template<class T>
void MyVec<T>::create() {
	data = NULL;
	dataSize = dataCapacity = 0;
}

template<class T>
MyVec<T>::MyVec() {
	create();
}

template<class T>
MyVec<T>::MyVec(int n, const T&init) {
	dataSize = dataCapacity = n;
	data = new T[n];
	for(int i=0;i<n;i++) data[i] = init;
}

//Construtor de copia
template<class T>
MyVec<T>::MyVec(const MyVec &other) {
	create(); //Crie um vetor vazio
	*this = other; 
}

template<class T>
MyVec<T> & MyVec<T>::operator=(const MyVec &other) {
	if(this==&other) return *this; 
	destroy(); //Exercicio: por que precisamos disso?
	dataCapacity = other.dataCapacity;
	dataSize = other.dataSize;
	//data = other.data; //por que nao podemos fazer isso?
	data = new T[dataCapacity];
	for(int i=0;i<dataSize;i++) data[i] = other.data[i];
	return *this;
}

template<class T2>
std::ostream& operator<<(std::ostream &out, const MyVec<T2> &v) {
	out << "Size: " << v.size() << "\n";
	out << "Capacity: " << v.dataCapacity << "\n";
	for(int i=0;i<v.size();i++) out << v.data[i] << " ";
	out << "\n";
	return out;
}

bool balanced(const string &str, stack<int> &erro) {
    stack<char> s;
    
    for(int i=0;i<str.size();i++) {
        char c = str[i];
        
        switch(c) {
            case '(':
				erro.push(i+1);
				if(i<str.size()-1 && str[i+1]=='*') {
					s.push(':');
					i++;
				}
				else s.push(c);
				break;
            case '[':
            case '{':
			case '<':
                s.push(c);
				erro.push(i+1);
                break;
            case ')':
                if(s.empty() || s.top()!='(') {
					erro.push(i+1);
					return false;
				}
                s.pop();
				erro.pop();
                break;
            case ']':
                if(s.empty() || s.top()!='[')  {
					erro.push(i+1);
					return false;
				}
                s.pop();
				erro.pop();
                break;
            case '}': 
                if(s.empty() || s.top()!='{')  {
					erro.push(i+1);
					return false;
				}
                s.pop();
				erro.pop();
                break;
			case '>': 
                if(s.empty() || s.top()!='<') {
					erro.push(i+1);
					return false;
				}
                s.pop();
				erro.pop();
                break;
			case '*':
				if(s.empty()) return false;
				if(i==str.size()-1 && s.top()==':')  {
					erro.push(i+1);
					return false;
				}
				if(i<str.size()-1 && str[i+1]==')') {
					if(s.top()!=':')  {
						erro.push(i+1);
						return false;
					}
					s.pop();
					erro.pop(); 
					i++;
				}
				break;
        }
    }

    if(!s.empty()) return false;
    return true;
}

long long int global_target;
string global_set;
string global_result;
bool stop = false;

bool equation(char v, char w, char x, char y, char z, long long int &target) {
	long long int resp;
	resp = (int(v)-64) - (int(w)-64)*(int(w)-64) + (int(x)-64)*(int(x)-64)*(int(x)-64) - (int(y)-64)*(int(y)-64)*(int(y)-64)*(int(y)-64) + (int(z)-64)*(int(z)-64)*(int(z)-64)*(int(z)-64)*(int(z)-64);

	if(resp==target) 
		return true;
	
	return false;
}

bool equation2(string str, long long int &target) {
	return equation(str[0], str[1], str[2], str[3], str[4], global_target);
}

void permuting(vector<vector<int> > &result, vector<int> nums, vector<int> perm) {
	if (!stop) {
	    if (perm.size() == 5) {
					string substring;
					for (int i = 0 ; i < perm.size(); i++) {
						substring.push_back(global_set[perm[i]]);
					}
					if (equation2(substring, global_target)) {
						global_result = substring;
						stop = true;
					}
	        result.push_back(perm);
	    } else {
	        for (int i = 0; i < nums.size(); i++) {
	                int aux = nums[i];
	                perm.push_back(nums[i]);
	                nums.erase(nums.begin() + i);
	                permuting(result, nums, perm);
	                nums.insert(nums.begin() + i, aux);
	                perm.pop_back();
	        }
  		}
	}
}

vector<vector<int> > permute(vector<int>& nums) {
    vector<vector<int> > result;
    vector<int> perm;
    permuting(result, nums, perm);

    return result;
}

void combs(long long int &target, string set) {
	vector<int> nums;
	stop = false;
	for (int i = set.size()-1; i >= 0; i--) {
		nums.push_back(i);
	}
	vector< vector<int> > combination_indexes = permute(nums);

	return;
}

int main () {

	long long int target;
	string set;
	cin >> target >> set;
	global_target = target;
	global_set = set;
	sort(global_set.begin(), global_set.end());


	while(set!="END") {
		combs(target, set);
		if (!stop) {
			cout << "no solution" << endl;
		} else {
			cout << global_result << endl;
		}
		cin >> target >> set;
		global_target = target;
		global_set = set;
		sort(global_set.begin(), global_set.end());

	}

	int x;

	for(int i=0;i<100;i++) {
		if(i&1) {
			cout << i << endl;
			x = 4;
		}
		else {
			cout << 3*i << endl;
			x = 9;
		}
	}

	string s;
	while(getline(cin,s)) {
		stack<int> erro;
		bool ok = balanced(s,erro);
		if(ok) cout << "YES\n";
		else { 
			cout << "NO ";
			if(erro.empty()) cout << "0\n";
			else cout << erro.top() << endl;
		}	
	}

	return 0;
}