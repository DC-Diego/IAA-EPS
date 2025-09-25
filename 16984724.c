#include<stdio.h>
#include<stdlib.h>

int stringToInt(char * num){
  int i = 0;
  int newNum = 0;
  while(num[i]!='\0'){
    newNum = newNum*10+(num[i]-'0');
    i++;
  }
  return newNum;
}


int* preencherMochila(int*profundidades, int qtdIlhas, int profundidadeContainer, int * combinacoes){
  int * ordenadas = (int*) malloc(sizeof(int)*(qtdIlhas));  
  for (int j = 0; j < qtdIlhas; j++)
    ordenadas[j] = profundidades[j];
  
  for(int i=0; i < qtdIlhas;i++){
    int min = ordenadas[i];
    int j = i - 1;
    while (j >= 0 && ordenadas[j] > min) {
      ordenadas[j + 1] = ordenadas[j];
      j = j - 1;
    }
    ordenadas[j + 1] = min;
  }


  int ** backpack = (int**) malloc(sizeof(int*)*(profundidadeContainer+1));
  
  for (int i = 0; i < profundidadeContainer+1; i++)
  {
    backpack[i] = (int*) malloc(sizeof(int)*(qtdIlhas+1));

  }
  backpack[0][0] = 1;
  for (int i = 1; i < profundidadeContainer+1; i++)
  backpack[i][0] = 0;


  for (int i = 0; i < profundidadeContainer+1; i++)
  {
    for (int j = 1; j <= qtdIlhas; j++)
    {
      int target = i-ordenadas[j-1];
      if(backpack[i][j-1] != 0 || target == 0 || (target > 0 && backpack[target][j-1] != 0) ){
        backpack[i][j] = 1;
        }else{
        backpack[i][j]=0;
      }

    }
  }
  
  if(backpack[profundidadeContainer][qtdIlhas]==0){
    for (int i = 0; i < profundidadeContainer+1; i++)
    {
      free(backpack[i]);

    }
    free(backpack);
    free(ordenadas);

    return NULL;
  }

  int * solutionIds =(int*) malloc(sizeof(int)*(qtdIlhas));

  int tamanhoBusca = profundidadeContainer;
  int sizeSolution =0;
 
  for (int i = qtdIlhas; i > 0; i--)
  {
      if(backpack[tamanhoBusca][i]!=0 && tamanhoBusca-ordenadas[i-1] >= 0){
        solutionIds[sizeSolution] = i-1;
        tamanhoBusca = tamanhoBusca-ordenadas[i-1];
        sizeSolution+=1;
      }
  }
 
  int * solution = (int*) malloc(sizeof(int)*(sizeSolution));
  for (int i = 0; i < sizeSolution; i++){
    solution[i] = ordenadas[solutionIds[i]];
  }

  for (int i = 0; i < profundidadeContainer+1; i++)
  {
    free(backpack[i]);

  }
  free(backpack);
  free(ordenadas);
  free(solutionIds);

  *combinacoes = sizeSolution;
  return solution;

}




int * montarMatriz(char * nomeArquivo, int m, int n){
  FILE * arquivo = fopen(nomeArquivo, "r");
  if(arquivo == NULL){
    printf("\nErro ao abrir o arquivo: %s\n", nomeArquivo);
    return NULL;
  }
  int * matriz = (int*) malloc(sizeof(int)*m*n);
  matriz[0] = 0;
  int i = 0;
  int c;
  while((c=fgetc(arquivo))!=EOF){


    if((char) c == ' ' || (char) c == '\n'){
      i++;
      matriz[i] = 0;
    }else{
      matriz[i] = matriz[i]*10+ (c-'0');
    }
  }
 
  fclose(arquivo);
  return matriz;
}

int floodFill(int * matriz,int linhas, int colunas, int x, int y, int * posVisitada){
  int profundidade;
  int posicao = x+y*colunas; 

  if(x >=0 && y>=0 && x < colunas&& y<linhas && posVisitada[posicao] != 0 && 0 != matriz[posicao]){
    profundidade = matriz[posicao];
    posVisitada[posicao] = 0;
  }else{
    return 0;
  }
  
  profundidade += floodFill(matriz, linhas, colunas, x+1,y, posVisitada); // x+1
  profundidade += floodFill(matriz, linhas, colunas, x-1,y, posVisitada); // x-1
  profundidade += floodFill(matriz, linhas, colunas, x,y+1, posVisitada); // y+1
  profundidade += floodFill(matriz, linhas, colunas, x,y-1, posVisitada); // y-1

  return profundidade;
}


int* buscarIlhas(int * matriz, int linhas, int colunas, int*qtdIlhas){
  *qtdIlhas = 0;
  int* ilhasProfundidade = (int*) malloc(sizeof(int)*linhas*colunas/2+1); 
  int* posVisitada = (int*) malloc(sizeof(int)*linhas*colunas); 
  for (int i = 0; i < linhas*colunas; i++)
    posVisitada[i] = 1;


  for(int i = 0; i < linhas*colunas;i++){
    if(matriz[i]!=0 &&posVisitada[i]!=0){
      int aux = floodFill(matriz, linhas, colunas, i%colunas,i/colunas, posVisitada); 
      ilhasProfundidade[(*qtdIlhas)++] = aux <6?1:aux/6;
    }
  }
  free(posVisitada);
  return ilhasProfundidade;
}

int main(int argc, char *argv[]){
  int profundidade_container, linhas, colunas;
  char * fileName;
  int * matriz;

  int qtdIlhas;
  int *profundidades;

  if(argc != 5){
    printf("Erro!, insira 5 parametros para funcionar:\n./EP1 <profundidade_container> <linhas_da_matriz> <colunas_da_matriz> exemplo.txt ");
    return -1;
  }else{
    profundidade_container = stringToInt(argv[1]);
    linhas = stringToInt(argv[2]);
    colunas= stringToInt(argv[3]);
    fileName = argv[4];

    matriz =  montarMatriz(fileName, linhas, colunas);
    if(matriz ==NULL)return-1;

    profundidades = buscarIlhas(matriz, linhas, colunas, &qtdIlhas);  
  }

  printf("%d\n", qtdIlhas);
  for(int e = 0; e < qtdIlhas;e++)
    printf("%d ", profundidades[e]);

  printf("\n");
  int qtdCombinacoes;
  int * combinacao = preencherMochila(profundidades, qtdIlhas, profundidade_container, &qtdCombinacoes);
  if(combinacao == NULL)
    printf("Nao ha resposta valida!");
  else  {
    for(int i = 0; i < qtdCombinacoes;i++){
      printf("%d ", combinacao[i]);
    }

  } 


  return 0;
}
