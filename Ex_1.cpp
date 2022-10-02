#include <iostream>
#include <fstream>
#include <vector>
#include <unordered_map>
#include <stack>
#include <queue>
#include <string>

class Node
{
public:
	std::string info;
	Node* left;
	Node* right;
	Node* parent;
};

int precedenta(char element)
{
	if (element == '(')
	{
		return 0;
	}
	else if (element == '-' || element == '+')
	{
		return 1;
	}
	else if (element == '*' || element == '/')
	{
		return 2;
	}
	else if (element == '^')
	{
		return 3;
	}
}

void citire_expresie_aritmetica(std::string& sir)
{
	std::ifstream fin;
	fin.open("input.in");
	std::getline(fin, sir);
	fin.close();
}
std::vector<std::string> determinare_forma_poloneza(std::string sir, std::unordered_map<std::string, double>& variabile)
{
	std::vector<std::string> Fp;
	std::stack<char> OP;
	std::string caracter;
	std::unordered_map<std::string, double>::iterator it;
	bool el_prec_este_numar = false;
	bool el_prec_este_operator = false;
	bool el_prec_este_variabila = false;
	for (int i = 0; i < sir.size(); i++)
	{
		if (sir[i] == '(' || sir[i] == ')' || sir[i] == '+' || sir[i] == '-' || sir[i] == '*' || sir[i] == '/' || sir[i] == '^')
		{
			if (sir[i] == '(')
			{
				OP.push(sir[i]);
				el_prec_este_operator = true;
				el_prec_este_variabila = false;
				el_prec_este_numar = false;
			}
			else
			{
				if (sir[i] == ')')
				{
					while (!OP.empty() && OP.top() != '(')
					{
						caracter = OP.top();
						Fp.push_back(caracter);
						OP.pop();
					}
					if (OP.empty())
					{
						std::cout << "Erroare: avem o paranteza in plus" << std::endl;
					}
					else
					{
						OP.pop();
					}
					el_prec_este_operator = false;
				}
				else
				{
					if (el_prec_este_operator == false)
					{
						while (!OP.empty() && precedenta(sir[i]) <= precedenta(OP.top()))
						{
							caracter = OP.top();
							Fp.push_back(caracter);
							OP.pop();
						}
						OP.push(sir[i]);
					}
					else
					{
						std::cout << "Erroare: avem 2 operatori diferiti de ( consecutivi, prin urmare al doilea este ignorat " << std::endl;
					}
					el_prec_este_operator = true;
				}
				el_prec_este_variabila = false;
				el_prec_este_numar = false;
			}
		}
		else if (sir[i] != ' ')
		{
			int val = (int)(sir[i] - '0');
			if (val >= 0 && val <= 9)
			{
				if (el_prec_este_numar == true)
				{
					Fp[Fp.size() - 1] = Fp[Fp.size() - 1] + sir[i];
				}
				else
				{
					caracter = sir[i];
					Fp.push_back(caracter);
				}
				el_prec_este_numar = true;
				el_prec_este_variabila = false;
			}
			else if (sir[i] == '.' && el_prec_este_numar == true)
			{
				caracter = sir[i];
				Fp[Fp.size() - 1] = Fp[Fp.size() - 1] + sir[i];
				el_prec_este_numar = true;
				el_prec_este_variabila = false;
			}
			else
			{
				caracter = sir[i];
				if (el_prec_este_variabila == true)
				{
					caracter = sir[i];
					it = variabile.find(Fp[Fp.size() - 1]);
					variabile.erase(it);
					variabile[Fp[Fp.size() - 1] + caracter] = 0;
					Fp.push_back(Fp[Fp.size() - 1] + caracter);
				}
				else
				{
					variabile[caracter] = 0;
					Fp.push_back(caracter);
				}
				el_prec_este_variabila = true;
				el_prec_este_numar = false;
			}
			el_prec_este_operator = false;
		}
	}
	while (!OP.empty())
	{
		caracter = OP.top();
		if (caracter == "(")
		{
			std::cout << "Erroare: Elementul ( este introdus suplimentar" << std::endl;
		}
		else
		{
			Fp.push_back(caracter);
		}
		OP.pop();
	}
	return Fp;
}

void afisare_forma_poloneza(std::vector<std::string>Fp)
{
	std::cout << "Forma poloneza este:\n";
	for (int i = 0; i < Fp.size(); i++)
	{
		std::cout << Fp[i] << " ";
	}
}


void construire_arbore(std::vector<std::string> Fp, std::stack<Node*>& stiva_de_noduri)
{
	Node* nod1;
	Node* nod2;
	for (int i = 0; i < Fp.size(); i++)
	{
		Node* nou_nod = new Node;
		if (Fp[i] == "+" || Fp[i] == "-" || Fp[i] == "*" || Fp[i] == "/" || Fp[i] == "^")
		{
			nod2 = stiva_de_noduri.top();
			stiva_de_noduri.pop();
			nod1 = stiva_de_noduri.top();
			stiva_de_noduri.pop();
			nou_nod->info = Fp[i];
			nou_nod->left = nod1;
			nou_nod->right = nod2;
			nod1->parent = nou_nod;
			nod2->parent = nou_nod;
			stiva_de_noduri.push(nou_nod);
		}
		else
		{
			nou_nod->info = Fp[i];
			nou_nod->parent = nullptr;
			nou_nod->left = nullptr;
			nou_nod->right = nullptr;
			stiva_de_noduri.push(nou_nod);
		}
	}

}

void afisare_arbore_pe_niveluri(Node* top)
{
	Node* parinte;
	std::queue<Node*>coada_de_noduri;
	coada_de_noduri.push(top);
	while (coada_de_noduri.size()!=0)
	{
		std::cout << coada_de_noduri.front()->info << " ";
		parinte = coada_de_noduri.front();
		coada_de_noduri.pop();
		if (parinte->left != nullptr)
		{
			coada_de_noduri.push(parinte->left);
		}
		if (parinte->right != nullptr)
		{
			coada_de_noduri.push(parinte->right);
		}
	}
}

void print2DUtil(Node* root, int space, int COUNT = 10)
{
	if (root == NULL)
		return;

	space += COUNT;

	print2DUtil(root->right, space);

	std::cout << std::endl;
	for (int i = COUNT; i < space; i++)
		std::cout << " ";
	std::cout << root->info << "\n";

	print2DUtil(root->left, space);
}

void print2D(Node* top)
{
	print2DUtil(top, 0);
}

void citire_variabile(std::unordered_map<std::string, double>& variabile)
{
	if (variabile.size() != 0)
	{
		double valoare;
		std::unordered_map<std::string, double>::iterator it;
		std::cout << "Adaugare valori variabilelor: \n";
		for (it = variabile.begin(); it != variabile.end(); it++)
		{
			std::cout << "Citeste valoare pentru variabila " << it->first << "= ";
			std::cin >> valoare;
			it->second = valoare;
		}
	}
}

double efectueaza_operatia(std::string operator_, double x, double y)
{
	if (operator_ == "+")
	{
		return x + y;
	}
	if (operator_ == "-")
	{
		return x - y;
	}
	if (operator_ == "*")
	{
		return x * y;
	}
	if (operator_ == "/")
	{
		return x / y;
	}
	if (operator_ == "^")
	{
		return pow(x, y);
	}
}

void evaluare_expresie(Node* node, std::unordered_map<std::string, double>variabile, double& rezultat)
{
	std::unordered_map<std::string, double>::iterator it1;
	std::unordered_map<std::string, double>::iterator it2;
	if (node == nullptr)
	{
		delete node;
		return;
	}
	if (node->left->info == "+" || node->left->info == "-" || node->left->info == "*" || node->left->info == "/" || node->left->info == "^")
	{
		evaluare_expresie(node->left, variabile, rezultat);
	}
	if (node->right->info == "+" || node->right->info == "-" || node->right->info == "*" || node->right->info == "/" || node->right->info == "^")
	{
		evaluare_expresie(node->right, variabile, rezultat);
	}

	if (node->left != nullptr && node->right != nullptr)
	{
		double valoare1, valoare2;
		it1 = variabile.find(node->left->info);
		it2 = variabile.find(node->right->info);
		if (it1 != variabile.end())
		{
			valoare1 = it1->second;
		}
		if (it2 != variabile.end())
		{
			valoare2 = it2->second;
		}
		if ((int)node->left->info.front() - '0' >= 0 && (int)node->left->info.front() - '0' <= 9)
		{
			valoare1 = std::stod(node->left->info);
		}
		if ((int)node->right->info.front() - '0' >= 0 && (int)node->right->info.front() - '0' <= 9)
		{
			valoare2 = std::stod(node->right->info);
		}
		rezultat = efectueaza_operatia(node->info, valoare1, valoare2);
		node->info = std::to_string(rezultat);
		node->left = nullptr;
		node->right = nullptr;
	}
}

void copiere_node(Node*& nod1, Node* nod2)
{
	if (nod2 != nullptr)
	{
		nod1->left = nod2->left;
		nod1->right = nod2->right;
		nod1->parent = nod2->parent;
		nod1->info = nod2->info;
	}
}

Node* copiere_arbore(Node* initial_tree)
{
	if (initial_tree == nullptr) 
	{
		return nullptr;
	}

	Node* copie_nod = new Node;
	copie_nod->info = initial_tree->info;

	copie_nod->left = copiere_arbore(initial_tree->left);
	copie_nod->right = copiere_arbore(initial_tree->right);

	return copie_nod;
}

void determinare_rezultat_in_functie_de_valoarea_mai_multor_variabile(Node* top, std::unordered_map<std::string, double>variabile)
{
	double rezultat = 0;
	std::string optiune;
	std::cout << "Apasa 0 daca doresti sa intrerupi executia programului, sau altceva daca doresti sa citesti valorile variabilelor si sa afli rezultatul:\n ";
	std::cin >> optiune;
	while (optiune != "0")
	{
		citire_variabile(variabile);
		Node* nod_copiat = new Node;
		nod_copiat=copiere_arbore(top);
		evaluare_expresie(nod_copiat, variabile, rezultat);
		std::cout <<"Rezultatul este: "<< rezultat << std::endl<<std::endl;
		std::cout << "Apasa 0 daca doresti sa intrerupi executia programului, sau altceva daca doresti sa citesti valorile variabilelor si sa afli rezultatul:\n ";
		std::cin >> optiune;
	}
}

int main()
{
	std::string sir;
	std::unordered_map<std::string, double> variabile;
	std::vector<std::string>Fp;
	std::stack<Node*>stiva_de_noduri;
	citire_expresie_aritmetica(sir);
	Fp = determinare_forma_poloneza(sir, variabile);
	afisare_forma_poloneza(Fp);
	construire_arbore(Fp, stiva_de_noduri);
	std::cout << std::endl<<std::endl<<"Afisare arbore pe niveluri:\n";
	afisare_arbore_pe_niveluri(stiva_de_noduri.top());
	std::cout << std::endl<<std::endl;
	determinare_rezultat_in_functie_de_valoarea_mai_multor_variabile(stiva_de_noduri.top(), variabile);

	return 0;
}