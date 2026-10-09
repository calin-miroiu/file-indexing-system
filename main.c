#include <stdlib.h>
#include <stdio.h>
#include <string.h>

typedef struct node{
	char *cuv;
	struct node *next;
}TNod;

typedef struct lista{
	char *id;
	int scor;
	TNod *cuvinte;
	struct lista *next;
	struct lista *prev;
}TList;

typedef struct fisier{
	TList *fisier;
	struct fisier *next;
}TFisier;

typedef struct Tree{
	struct Tree* noduri[26];
	TFisier* fisier;
}ANod;

typedef struct heap
{
	int maxheapsize;
	int size;
	TFisier **elem;
}PriQueue;

int getleftchild(int i){
	return 2 * i + 1;
}

int getrightchild(int i){
	return 2 * i + 2;
}

int getparent(int i){
	return (i - 1) / 2;
}

void inserare_arbore(ANod **root, char *cuvant, TList *fisier){
	if(!*root){
		*root = calloc(1, sizeof(ANod));
	}
	ANod *idx = *root;
	for(unsigned int i = 0; i < strlen(cuvant); i++){ //unsigned pentru ca primeam warning de la comparatie
		int j = cuvant[i] - 'a';
		if(!idx->noduri[j]){
			idx->noduri[j] = calloc(1, sizeof(ANod));
		}
		idx = idx->noduri[j];
	}
	TFisier *nou = malloc(sizeof(TFisier));
	nou->fisier = fisier;
	nou->next = idx->fisier;
	idx->fisier = nou;
}

TFisier* initfisier(char *id1, int scor1, TNod *cuv1, TFisier *head){
	if(!head){
		TFisier *nod = malloc(sizeof(TFisier));
		nod->fisier = malloc(sizeof(TList));
		nod->next = NULL;
		head = nod;
	}
	head->fisier->next = head->fisier->prev = NULL;
	head->fisier->scor = scor1;
	head->fisier->id = malloc((strlen(id1) + 1) * sizeof(char));
	strcpy(head->fisier->id, id1);
	head->fisier->cuvinte = cuv1;
	return head;
}

TFisier *gaseste_fisier(TFisier *head, char *id){
	while(head){
		if(strcmp(head->fisier->id, id) == 0){
			return head;
		}
		head = head->next;
	}
	return NULL;
}

void free_trie(ANod *root) {
	if (!root) return;
	for (int i = 0; i < 26; i++) {
		if (root->noduri[i]) {
			free_trie(root->noduri[i]);
		}
	}
	free(root);
}

TFisier* ADD(char *id1, int scor1, TNod *cuv1, TFisier *head, ANod **root){
	if(head){
		if(gaseste_fisier(head, id1)){//daca fisierul deja exista nu il mai adaug
			printf("EXISTS\n");
			return head;
		}
		TFisier *tmp = malloc(sizeof(TFisier));
		tmp->fisier = calloc(1, sizeof(TList));
		tmp->fisier->id = malloc(strlen(id1) + 1);
		tmp->fisier->scor = scor1;
		tmp->fisier->next = head->fisier;
		tmp->fisier->prev = NULL;
		head->fisier->prev = tmp->fisier;
		tmp->next = head;
		strcpy(tmp->fisier->id, id1);
		tmp->fisier->cuvinte = cuv1;
		head = tmp;
	}else head = initfisier(id1, scor1, cuv1, head);//daca nu este niciun fisier in lista
	TNod *idx = cuv1;
	while (idx){
		inserare_arbore(root, idx->cuv, head->fisier);
		idx = idx->next;
	}
	printf("OK\n");
	return head;
}

//scurtarea codului in functiile siftup si siftdown
int comparatie(TFisier *f1, TFisier *f2){
	if(f1->fisier->scor > f2->fisier->scor){
		return 1;
	}else{ if(f1->fisier->scor == f2->fisier->scor && 
			strcmp(f1->fisier->id, f2->fisier->id) < 0){
				return 1;
			}
		}
	return 0;
}

void sortare(char **v, int n){
	for(int i = 0; i < n - 1; i++){
		for(int j = i + 1; j < n; j++){
			if(strcmp(v[i],v[j]) > 0){
				char *aux = v[i];
				v[i] = v[j];
				v[j] = aux;
			}
		}
	}
}

void FIND(char *cuv, ANod *root){
	if(!root){
		printf("EMPTY\n");
		return;
	}
	for(unsigned int i = 0; i < strlen(cuv); i++){//unsigned pentru comparatia cu strlen
		int k = cuv[i] - 'a';
		if(!root->noduri[k]){
			printf("EMPTY\n");
			return;
		}else{
			root = root->noduri[k];
		}
	}
	if(!root->fisier){
		printf("EMPTY\n");
		return;
	}
	TFisier *copie = root->fisier;
	int nr = 0;
	while(copie){
		nr++;
		copie = copie->next;
	}
	copie = root->fisier;
	char **vector = calloc(nr, sizeof(char*));
	for(int i = 0; i < nr; i++){
		vector[i] = copie->fisier->id;
		copie = copie->next;
	}
	sortare(vector, nr);
	printf("%d", nr);
	for(int i = 0; i < nr; i++){
		printf(" %s", vector[i]);
	}
	printf("\n");
	free(vector);
}

void siftDown(PriQueue *h, int idx){
	int left = getleftchild(idx);
	int right = getrightchild(idx), i = idx;
	if(left < h->size && comparatie(h->elem[left], h->elem[i])){
		i = left;
	}
	if(right < h->size && comparatie(h->elem[right], h->elem[i])){
		i = right;
	}
	if(i != idx){
		TFisier *aux = h->elem[idx];
		h->elem[idx] = h->elem[i];
		h->elem[i] = aux;
		siftDown(h, i);
	}
}

TFisier* eliminare(PriQueue *h){
	if(!h->size) return NULL;
	TFisier *max = h->elem[0];
	h->elem[0] = h->elem[h->size - 1];
	h->size--;
	if(h->size > 0){
		siftDown(h, 0);
	}
	return max;
}

void siftUP(PriQueue *h, int idx){
	if(!idx) return;
	int parent = getparent(idx);
	if(comparatie(h->elem[parent], h->elem[idx]) ||(
		!comparatie(h->elem[parent], h->elem[idx]) &&
		!comparatie(h->elem[idx], h->elem[parent]))){
			return;
		}
	TFisier *aux = h->elem[parent];
	h->elem[parent] = h->elem[idx];
	h->elem[idx] = aux;
	siftUP(h, parent);
}

void insertheap(PriQueue *h, TFisier *x){
	if(h->size == h->maxheapsize){
		h->elem = realloc(h->elem, (2 * h->maxheapsize) * sizeof(TFisier*));
		h->maxheapsize *= 2;
	}
	h->elem[h->size] = x;
	siftUP(h, h->size);
	h->size++;
}

void PRINT(ANod *root, char *cuvant, int nivel, int *ok){
	int nr = 0;
	TFisier *copie = root->fisier;
	if(root->fisier){
		*ok = 1;//sa stiu daca am afisat ceva sau trebuie afisat empty
	}
	if(root->fisier != NULL){
		cuvant[nivel] = '\0';
		while (copie){
			nr++;
			copie = copie->next;
		}
		char **vector = calloc(nr, sizeof(char*));
		copie = root->fisier;
		for(int i = 0; i < nr; i++){
			vector[i] = copie->fisier->id;
			copie = copie->next;
		}
		sortare(vector, nr);
		printf("%s %d", cuvant, nr);
		for(int i = 0; i < nr; i++){
			printf(" %s", vector[i]);
		}
		printf("\n");
		free(vector);
	}
	for(int i = 0; i < 26; i++){
		if(root->noduri[i]){
			cuvant[nivel] = i + 'a';
			PRINT(root->noduri[i], cuvant, nivel + 1, ok);
		}
	}
}

void apelPRINT(ANod *root){
	if(!root){
		printf("EMPTY\n");
		return;
	}
	char cuvant[101];
	int ok = 0;
	PRINT(root, cuvant, 0, &ok);
	if(!ok){
		printf("EMPTY\n");
		return;
	}
}

TFisier* apelADD(TFisier *head, ANod **root){
	char *id = strtok(NULL, " \n");
	char *sscor = strtok(NULL, " \n");
	char *tmp = strtok(NULL, " \n");
	int nr = atoi(tmp);
	int scor = atoi(sscor);
	TNod *arr = NULL, *ultimul;
	for(int i = 0; i < nr; i++){
		char *cuv = strtok(NULL, " \n");
		TNod *idx = arr;
		int ok = 1;
		while (idx){
			if(strcmp(idx->cuv, cuv) == 0){
				ok = 0;
				break;
			}
			idx = idx->next;
		}
		if(ok){
			TNod *nou = malloc(sizeof(TNod));
			nou->cuv = malloc(strlen(cuv) + 1);
			strcpy(nou->cuv, cuv);
			nou->next = NULL;
			if(!arr){
				arr = nou;
				ultimul = nou;
			}else {
				ultimul->next = nou;
				ultimul = nou;
			}
		}
	}
	if(gaseste_fisier(head, id)){
		while (arr) {
			TNod *tmp = arr;
			arr = arr->next;
			free(tmp->cuv);
			free(tmp);
		}
		printf("EXISTS\n");
		return head;
	}
	head = ADD(id, scor, arr, head, root);
	return head;
}

void TOPK(char *cuv, int k, ANod *root){
	if(!root){
		printf("EMPTY\n");
		return;
	}
	for(unsigned int i = 0; i < strlen(cuv); i++){
		int poz = cuv[i] - 'a';
		if(!root->noduri[poz]){
			printf("EMPTY\n");
			return;
		}
		root = root->noduri[poz];
	}
	if(!root->fisier){
		printf("EMPTY\n");
		return;
	}
	int nr = 0;
	TFisier	*copie = root->fisier;
	while (copie){
		nr++;
		copie = copie->next;
	}
	PriQueue *heap = calloc(1, sizeof(PriQueue));
	heap->size = 0;
	heap->maxheapsize = nr;
	heap->elem = calloc(nr, sizeof(TFisier*));
	copie = root->fisier;
	while (copie){
		insertheap(heap, copie);
		copie = copie->next;
	}
	int q;
	if(nr > k){
		q = k;
	}else q = nr;
	printf("%d", q);
	for(int i = 0; i < q; i++){
		TFisier* tmp = eliminare(heap);
		printf(" %s", tmp->fisier->id);
	}
	printf("\n");
	free(heap->elem);
	free(heap);
}

int verif_frunza(ANod *root){
	for(int i = 0 ; i < 26; i++){
		if(root->noduri[i]){
			return 1;
		}
	}
	return 0;
}

int arbore(ANod *root, char *cuv, unsigned int idx, char *id){
	if(!root) return 0;
	TFisier *prev = NULL;
	if(idx == strlen(cuv)){
		TFisier *idxx = root->fisier;
		while (idxx){
			if(strcmp(idxx->fisier->id, id) == 0){
				if(prev){
					prev->next = idxx->next;
				}else root->fisier = idxx->next;
				free(idxx);
				break;
			}
			prev = idxx;
			idxx = idxx->next;
		}
		if(!root->fisier && !verif_frunza(root)){
			return 1;
		}
		return 0;
	}
	int i = cuv[idx] - 'a';
	if(root->noduri[i]){
		idx++;
		int j = arbore(root->noduri[i], cuv, idx, id);
		if(j){//daca j e 1, copilul e inutil si trebuie sters
			free(root->noduri[i]);
			root->noduri[i] = NULL;
			if(!root->fisier && !verif_frunza(root)){//verific daca si nodul curent a ramas acum inutil
				return 1;
			}
		}
	}
	return 0;
}

TFisier *DEL(char *id, TFisier *head, ANod **root){
	TFisier *idx = head, *prev = NULL;
	while(idx){
		if(strcmp(idx->fisier->id, id) == 0){
			break;
		}
		prev = idx;
		idx = idx->next;
	}
	if(!idx){
		printf("NOT FOUND\n");
		return head;
	}
	TNod *idxx = idx->fisier->cuvinte;
	while(idxx){
		int ok = arbore(*root, idxx->cuv, 0 , id);//verifica daca exista acel cuvant
		if(ok){
			free_trie(*root);
			*root = NULL;
		}
		idxx = idxx->next;
	}
	if(prev){
		prev->next = idx->next;
	}else head = idx->next;
	if(idx->fisier->next){
		idx->fisier->next->prev = idx->fisier->prev;
	}
	if(idx->fisier->prev){
		idx->fisier->prev->next = idx->fisier->next;
	}
	TNod *tmp = idx->fisier->cuvinte;
	while (tmp)
	{
		TNod *tmp1 = tmp;
		tmp = tmp->next;
		free(tmp1->cuv);
		free(tmp1);
	}
	printf("OK\n");
	free(idx->fisier->id);
	free(idx->fisier);
	free(idx);
	return head;
}

TFisier *apelDEL(TFisier *head, ANod **root){
	char *id = strtok(NULL, " \n");
	if(id){
		head = DEL(id, head, root);
	}
	return head;
}

void delkw(TFisier *fisier, char *cuv, ANod **root){
	TNod *idx = fisier->fisier->cuvinte, *ultimul = NULL;
	int ok = 0;
	while (idx){
		if(strcmp(idx->cuv, cuv) == 0){
			ok = 1;
			if(!ultimul){
				fisier->fisier->cuvinte = idx->next;
			}else {
				ultimul->next = idx->next;
			}
			free(idx->cuv);
			free(idx);
			break;
		}
		ultimul = idx;
		idx = idx->next;
	}
	if(ok){
		int ok1 = arbore(*root, cuv, 0, fisier->fisier->id);
		if(ok1){
			free_trie(*root);
			*root = NULL;
		}
	}
}

TFisier *apeldelkw(TFisier *head, ANod **root){
	char *id = strtok(NULL, " \n");
	TFisier *fisier = gaseste_fisier(head, id);
	if(!fisier){
		printf("NOT FOUND\n");
		return head;
	}
	char *cuv = strtok(NULL, " \n");// am pus terminatorul /n la strtok pentru a separa cuvintele fara enter
	if(!cuv){
		return head;
	}
	delkw(fisier, cuv, root);
	printf("OK\n");
	return head;
}

void addkw(TFisier *fisier, char *cuv, ANod **root){
	TNod *idx = fisier->fisier->cuvinte, *ultimul = NULL;
	while(idx){
		if(strcmp(idx->cuv, cuv) == 0){
			return;
		}
		ultimul = idx;
		idx = idx->next;
	}
	TNod *nou = malloc(sizeof(TNod));
	nou->cuv = malloc(strlen(cuv) + 1);
	strcpy(nou->cuv, cuv);
	nou->next = NULL;
	if(!ultimul){
		fisier->fisier->cuvinte = nou;
	} else {
		ultimul->next = nou;
	}
	inserare_arbore(root, nou->cuv, fisier->fisier);
}

TFisier* apelADDKW(TFisier *head, ANod **root){
	char *id = strtok(NULL, " \n");
	if (!id) return head;
	TFisier *fisier = gaseste_fisier(head, id);
	if(!fisier){
		printf("NOT FOUND\n");
		return head;
	}
	char *cuv = strtok(NULL, " \n");
	if(!cuv){
		return head;
	}
	addkw(fisier, cuv, root);
	printf("OK\n");
	return head;
}

int main(){
	int n;
	ANod *root = NULL;
	scanf("%d", &n);
	char s[10005];
	fgets(s, 10005, stdin);
	TFisier	*head = NULL;
	for(int i = 0; i < n; i++){
		fgets(s, 10005, stdin);
		char *p = strtok(s, " \n");
		if(!p) {
			i--; 
			continue;
		}
		char operatie[10001];
		strcpy(operatie, p);
		if(strcmp(operatie, "ADD") == 0){
			head = apelADD(head, &root);
		}else if(strcmp(operatie, "DEL") == 0){
			head = apelDEL(head, &root);
		}else if(strcmp(operatie, "ADDKW") == 0){
			head = apelADDKW(head, &root);
		}else if(strcmp(operatie, "DELKW") == 0){
			head = apeldelkw(head, &root);
		}else if(strcmp(operatie, "FIND") == 0){
				char *cuv = strtok(NULL, " \n");
				FIND(cuv, root);
		}else if(strcmp(operatie, "PRINT") == 0){
			apelPRINT(root);
		}else if(strcmp(operatie, "TOPK") == 0){
			char *cuv = strtok(NULL, " \n");
			char *s1 = strtok(NULL, " \n");
			TOPK(cuv, atoi(s1), root);
		}
	}
}